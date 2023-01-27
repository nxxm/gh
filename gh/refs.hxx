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

  struct verification_t {
    bool verified;
    std::string reason;
    std::optional<std::string> signature;
    std::optional<std::string> payload;
  };

  //! |brief Object type "tag" on github is used only for tags with annotations
  struct tag_t {
    std::string sha;
    std::string tag;
    std::string message;
    std::string url;
    object_t object;
    std::optional<verification_t> verification;
  };

}

BOOST_FUSION_ADAPT_STRUCT(gh::git_data::object_t, type, sha, url);
BOOST_FUSION_ADAPT_STRUCT(gh::git_data::ref_t, ref, url, object);
BOOST_FUSION_ADAPT_STRUCT(gh::git_data::verification_t, verified, reason, signature, payload);
BOOST_FUSION_ADAPT_STRUCT(gh::git_data::tag_t, sha, tag, message, url, object, verification);


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
      // sometimes, on very large repositories like GRPC, github.com seems to struggle,
      // but it's able to respond the query in two slices by looking up refs and then heads separately
      if(resp.status_code == 502 && filter == filter_refs::ALL) {

        git_data::refs manual_aggregation{};

        get_refs(owner, repository, filter_refs::HEADS, [&](auto &&result_heads) {
          std::move(result_heads.begin(), result_heads.end(), std::back_inserter(manual_aggregation));

          get_refs(owner, repository, filter_refs::TAGS, [&](auto &&result_tags) {
            std::move(result_tags.begin(), result_tags.end(), std::back_inserter(manual_aggregation));            

            result_handler(std::move(manual_aggregation));

          }, auth, api_endpoint);

        }, auth, api_endpoint);        

      } else if ( (!resp.error) && (resp.status_code == 200) ) {
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


  /**
   * \brief get an annotated tag details, useful to know which commit the annotated tag refers too.
   * \param auth credentials
   * \param owner
   * \param repository
   * \param annotated_tag_obj_sha The sha of the annotation object that the tags points to. This is the object.sha from an object returned by get_refs, when object.type is "tag".
   * \param result_handler taking a single git::data::tag_t
   */
  inline void get_tag(std::string owner, std::string repository,
    const std::string& annotated_tag_obj_sha,
    std::function<void(git_data::tag_t&&)>&& result_handler,
    std::optional<auth> auth = std::nullopt,
    const std::string& api_endpoint = "https://api.github.com"s ) {
  
    using namespace xxhr;
    auto url = api_endpoint + "/repos/"s + owner + "/" + repository
      + "/git/tags/" + annotated_tag_obj_sha;

    auto response_handler = [&](auto&& resp) {
      if ( (!resp.error) && (resp.status_code == 200) ) {
        result_handler(pre::json::from_json<git_data::tag_t>(resp.text));
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
