#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <optional>
#include <functional>
#include <boost/fusion/include/adapt_struct.hpp>
#include <pre/json/from_json.hpp>

#include <xxhr/xxhr.hpp>

#include <gh/auth.hxx>

namespace gh::git_data {
 
  struct object_t {
    std::string type;
    std::string sha;
    std::string url;
  };

  struct ref_t {
    std::string ref;
    std::string url;
    object_t object;
  };

  using refs = std::vector<ref_t>;
}

BOOST_FUSION_ADAPT_STRUCT(gh::git_data::object_t, type, sha, url);
BOOST_FUSION_ADAPT_STRUCT(gh::git_data::ref_t, ref, url, object);


namespace gh {

  using namespace std::string_literals;

  //! filter references based on their types
  enum class filter_refs {
    //! tags and branches mixed
    ALL,
    //! Only tags
    TAGS,
    //! Only branches
    HEADS
  };

  /**
   * \brief gets the refs that a repo contains (tags and branch heads)
   * \param auth credentials
   * \param owner
   * \param repository
   * \param filter Whether to list all refs or only tags or branch heads
   * \param result_handler taking a gh::git_data::refs collection.
   */
  inline void get_refs(std::string owner, std::string repository,
    filter_refs filter,
    std::function<void(git_data::refs&&)>&& result_handler,
    std::optional<auth> auth = std::nullopt,
    const std::string& api_endpoint = "https://api.github.com"s ) {
  
    using namespace xxhr;
    auto url = api_endpoint + "/repos/"s + owner + "/" + repository + "/git/refs/";

    if (filter == filter_refs::TAGS) {
      url += "tags";
    } else if (filter == filter_refs::HEADS) {
      url += "heads";
    }

    auto response_handler = [&](auto&& resp) {
      if ( (!resp.error) && (resp.status_code == 200) ) {
        result_handler(pre::json::from_json<git_data::refs>(resp.text));
      } else {
        throw std::runtime_error( "err : "s + std::string(resp.error) + "status: "s 
            + std::to_string(resp.status_code) + " accessing : "s + url );
      }
    };

    if (auth) {
      GET(url, Authentication{auth->user, auth->pass},
        on_response = response_handler
      );
    } else {
      GET(url, on_response = response_handler);
    }
  }

  /**
   * \brief get the ref (tag or branch)
   * \param auth credentials
   * \param owner
   * \param repository
   * \param ref Name of the ref, either "heads/<branch>" or "tags/<tag>"
   * \param result_handler taking a gh::git_data::refs collection.
   */
  inline void get_ref(std::string owner, std::string repository,
    const std::string& ref,
    std::function<void(git_data::ref_t&&)>&& result_handler,
    std::optional<auth> auth = std::nullopt,
    const std::string& api_endpoint = "https://api.github.com"s ) {
  
    using namespace xxhr;
    auto url = api_endpoint + "/repos/"s + owner + "/" + repository
      + "/git/refs/" + ref;

    auto response_handler = [&](auto&& resp) {
      if ( (!resp.error) && (resp.status_code == 200) ) {
        result_handler(pre::json::from_json<git_data::ref_t>(resp.text));
      } else {
        throw std::runtime_error( "err : "s + std::string(resp.error) + "status: "s 
            + std::to_string(resp.status_code) + " accessing : "s + url );
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
