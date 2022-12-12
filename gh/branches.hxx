#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <optional>
#include <functional>

#include <pre/json/from_json.hpp>
#include <pre/json/mapping.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

#include <xxhr/xxhr.hpp>

#include <gh/auth.hxx>

namespace gh::repos {

  //! Branch details
  struct branch_t {
    struct commit_t {
      std::string sha;
      std::string url;
    };

    //! name
    std::string name;

    //! sha1 of tip branch commit 
    commit_t commit;

    bool is_protected; // has to be mapped
    std::optional<std::string> protection_url;
  };

  //! Branches in a repos
  using branches = std::vector<branch_t>;

}

BOOST_FUSION_ADAPT_STRUCT(gh::repos::branch_t::commit_t, sha, url);
BOOST_FUSION_ADAPT_STRUCT(gh::repos::branch_t, name, commit, protection_url, is_protected);

namespace gh {

  /**
   * \brief list github branches, passing them to the result_handler as std::vector<branch_t>.
   *        See https://developer.github.com/v3/repos/branches/#list-branches 
   *
   * \param auth credentials
   * \param owner
   * \param repos 
   * \param result_handler Callable with signature : `func(std::vector<branch_t>)`
   */
  inline void list_branches(std::string owner, std::string repos, std::function<void(repos::branches&&)>&& result_handler,
    std::optional<auth> auth = std::nullopt) {

    using namespace xxhr;
    auto url = "https://api.github.com/repos/"s + owner + "/" + repos + "/branches"s;
    auto response_handler = [&](auto&& resp) {
      if ( (!resp.error) && (resp.status_code == 200) ) {

        auto mapper = [&](nlohmann::json &jdoc){ 
          // jdoc should be an array
          if(!jdoc.is_array()) {
            throw std::runtime_error("Unexpected format, service answer should have been an array:\n" + resp.text);
          }

          for (auto &it : jdoc)
          {
            pre::json::remap_property(it, "protected", "is_protected");
          }
        };

        result_handler(pre::json::from_json<repos::branches>(resp.text, mapper));
      } else {
        throw std::runtime_error(url + " is not responding");
      }
    };

    if (auth) {
      GET(url, Authentication{auth->user, auth->pass},
        on_response = response_handler);
    } else {
      GET(url, on_response = response_handler);
    }
  }
}
