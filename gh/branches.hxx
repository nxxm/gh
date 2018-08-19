#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>
#include <pre/json/from_json.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

#include <xxhr/xxhr.hpp>

namespace gh::repo {

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

    //XXX: cannot is a cpp reserved keyword: bool protected; 
    //std::string protection_url;
  };

  //! Branches in a repo
  using branches = std::vector<branch_t>;

}

BOOST_FUSION_ADAPT_STRUCT(gh::repo::branch_t::commit_t, sha, url);
BOOST_FUSION_ADAPT_STRUCT(gh::repo::branch_t, name, commit);

namespace gh {

  /**
   * \brief list github branches, passing them to the handler as std::vector<branch_t>.
   *        See https://developer.github.com/v3/repos/branches/#list-branches 
   * \param owner
   * \param repo 
   * \param handler Callable with signature : `func(std::vector<branch_t>)`
   */
  inline void list_branches(const std::string& owner, const std::string& repo, auto&& handler) {

    using namespace xxhr;
    auto url = "https://api.github.com/repos/"s + owner + "/" + repo + "/branches"s;
    GET(url,
      Authentication{"daminetreg", "5c8bc510c7880fcb0db28410218665d707564b3f"}, 
      on_response = [&](auto&& resp) {
        
        if (!resp.error) {
          handler(pre::json::from_json<repo::branches>(resp.text));
        } else {
          throw std::runtime_error(url + " is not responding");
        }
      }
    );
  }
}
