#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include <optional>
#include <pre/json/from_json.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

#include <xxhr/xxhr.hpp>

#include <gh/owner.hxx>
#include <gh/auth.hxx>

namespace gh::releases {

  struct asset_t {
    std::string url;
    std::string browser_download_url;

    std::size_t id;

    std::string name;
    std::optional<std::string> label;
    std::string state;
    std::string content_type;
    std::size_t size;
    std::size_t download_count;
    std::string created_at;
    std::string updated_at;
    gh::owner_t uploader;
  };

  using assets_t = std::vector<asset_t>;
}

BOOST_FUSION_ADAPT_STRUCT(gh::releases::asset_t,
    url,
    browser_download_url,
    id,
    name,
    label,
    state,
    content_type,
    size,
    download_count,
    created_at,
    updated_at,
    uploader
);


namespace gh::releases {

  struct release_t {

    std::string url;
    std::string html_url;
    std::string assets_url;
    std::string upload_url;
    std::string tarball_url;
    std::string zipball_url;
    std::size_t id;

    std::string tag_name;
    std::optional<std::string> target_commitish;
    std::string name;
    std::string body;

    bool draft;
    bool prerelease;
    std::string created_at;
    std::string published_at;

    gh::owner_t author;

    assets_t assets;
  };

}

BOOST_FUSION_ADAPT_STRUCT(gh::releases::release_t
  , url
  , html_url
  , assets_url
  , upload_url
  , tarball_url
  , zipball_url
  , id

  , tag_name
  , target_commitish
  , name
  , body


  , prerelease
  , created_at
  , published_at

  , author

  , assets);


namespace gh {

  using namespace std::string_literals;

  /**
   * \brief gets the provided repo and pass it to result_handler otherwise throws.
   * \param auth credentials
   * \param owner
   * \param repository
   * \param result_handler taking a gh::repos::repository_t
   */
  inline void get_latest_release(std::string owner, std::string repository, 
    std::function<void(releases::release_t&&)>&& result_handler,
    std::optional<auth> auth = std::nullopt,
    const std::string& api_endpoint = "https://api.github.com"s ) {

  
    using namespace xxhr;
    auto url = api_endpoint + "/repos/"s + owner + "/" + repository + "/releases/latest";

    auto response_handler = [&](auto&& resp) {
      if ( (!resp.error) && (resp.status_code == 200) ) {
        std::cout << "resp : " << resp.text << std::endl;
        result_handler(pre::json::from_json<releases::release_t>(resp.text));
      } else {
        throw std::runtime_error( "err : "s + std::string(resp.error) + "status: "s 
            + std::to_string(resp.status_code) + " accessing : "s + url );
      }
    };


    if (auth) {
      GET(url,
        Authentication{auth->user, auth->pass},
        on_response = response_handler);
    } else {
      GET(url,
        on_response = response_handler);
    }
  }

}

