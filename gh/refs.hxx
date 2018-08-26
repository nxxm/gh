#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <boost/fusion/include/adapt_struct.hpp>
#include <pre/json/from_json.hpp>

#include <xxhr/xxhr.hpp>

namespace gh::git_data {
 
  struct object_t {
    std::string type;
    std::string sha;
    std::string url;
  };

  struct ref_t {
    std::string ref;
    std::string node_id;
    std::string url;
    object_t object;
  };

  using refs = std::vector<ref_t>;
}

BOOST_FUSION_ADAPT_STRUCT(gh::git_data::object_t, type, sha, url);
BOOST_FUSION_ADAPT_STRUCT(gh::git_data::ref_t, ref, node_id, url, object);


namespace gh {

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
   * \param owner
   * \param repository
   * \param filter Whether to list all refs or only tags or branch heads
   * \param result_handler taking a gh::git_data::refs collection.
   */
  inline void get_refs(const std::string& owner, const std::string& repository,
    filter_refs filter,
    std::function<void(git_data::refs&&)>&& result_handler) {
  
    using namespace xxhr;
    auto url = "https://api.github.com/repos/"s + owner + "/" + repository + "/git/refs/";

    if (filter == filter_refs::TAGS) {
      url += "tags";
    } else if (filter == filter_refs::HEADS) {
      url += "heads";
    }

    GET(url,
      Authentication{"daminetreg", "5c8bc510c7880fcb0db28410218665d707564b3f"},
      on_response = [&](auto&& resp) {
        if ( (!resp.error) && (resp.status_code == 200) ) {
          result_handler(pre::json::from_json<git_data::refs>(resp.text));
        } else {
          throw std::runtime_error( "err : "s + std::string(resp.error) + "status: "s 
              + std::to_string(resp.status_code) + " accessing : "s + url );
        }
      }
    );
  }

  /**
   * \brief get the ref (tag or branch)
   * \param owner
   * \param repository
   * \param ref Name of the ref, either "heads/<branch>" or "tags/<tag>"
   * \param result_handler taking a gh::git_data::refs collection.
   */
  inline void get_ref(const std::string& owner, const std::string& repository,
    const std::string& ref,
    std::function<void(git_data::ref_t&&)>&& result_handler) {
  
    using namespace xxhr;
    auto url = "https://api.github.com/repos/"s + owner + "/" + repository
      + "/git/refs/" + ref;

    GET(url,
      Authentication{"daminetreg", "5c8bc510c7880fcb0db28410218665d707564b3f"},
      on_response = [&](auto&& resp) {
        if ( (!resp.error) && (resp.status_code == 200) ) {
          result_handler(pre::json::from_json<git_data::ref_t>(resp.text));
        } else {
          throw std::runtime_error( "err : "s + std::string(resp.error) + "status: "s 
              + std::to_string(resp.status_code) + " accessing : "s + url );
        }
      }
    );
  }


}