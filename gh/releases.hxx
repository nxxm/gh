#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include <optional>
#include <pre/json/from_json.hpp>
#include <pre/json/to_json.hpp>
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

  struct update_asset_t {
    std::string name;
    std::optional<std::string> label;
    std::optional<std::string> state;
  };
}

BOOST_FUSION_ADAPT_STRUCT(gh::releases::update_asset_t, name, label, state);


namespace gh::releases {

  struct release_t {

    std::string url;
    std::string html_url;
    std::string assets_url;
    std::string upload_url;
    std::optional<std::string> tarball_url;
    std::optional<std::string> zipball_url;
    std::size_t id;

    std::string tag_name;
    std::optional<std::string> target_commitish;
    std::string name;
    std::string body;

    bool draft;
    bool prerelease;
    std::string created_at;
    std::optional<std::string> published_at;

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

  , draft
  , prerelease
  , created_at
  , published_at

  , author

  , assets);

namespace gh::releases {
  struct update_release_t {   

    std::string tag_name;
    std::optional<std::string> target_commitish;
    std::string name;
    std::string body;

    bool draft;
    bool prerelease;
    
    std::optional<std::string> discussion_category_name;    
  };
}

BOOST_FUSION_ADAPT_STRUCT(gh::releases::update_release_t
  , tag_name
  , target_commitish
  , name
  , body
  , draft
  , prerelease
  , discussion_category_name);


namespace gh::releases {
  struct create_release_t {   
    std::string tag_name;
    std::optional<std::string> target_commitish;
    std::string name;
    std::string body;

    bool draft;
    bool prerelease;
    bool generate_release_notes;
    
    std::optional<std::string> discussion_category_name;
  };
}

BOOST_FUSION_ADAPT_STRUCT(gh::releases::create_release_t
  , tag_name
  , target_commitish
  , name
  , body
  , draft
  , prerelease
  , generate_release_notes
  , discussion_category_name);

// just a few MIME types...
namespace gh::releases {

  static inline const std::string CONTENT_TYPE_ZIP = "application/zip";
  static inline const std::string CONTENT_TYPE_TXT = "text/plain";
  static inline const std::string CONTENT_TYPE_TAR = "application/x-tar";
  static inline const std::string CONTENT_TYPE_JSON = "application/json";
  static inline const std::string CONTENT_TYPE_GZ = "application/gzip";
  static inline const std::string CONTENT_TYPE_BZ = "application/x-bzip";
  static inline const std::string CONTENT_TYPE_BZ2 = "application/x-bzip2";
  static inline const std::string CONTENT_TYPE_7Z = "application/x-7z-compressed";
}


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


  /**
   * \brief gets the provided repo and pass it to result_handler otherwise throws.
   * \param auth credentials
   * \param owner
   * \param repository
   * \param tag tag of the release you are looking for 
   * \param result_handler taking a gh::repos::repository_t
   */
  inline void get_release_by_tag(std::string owner, std::string repository,std::string tag,
    std::function<void(releases::release_t&&)>&& result_handler,
    std::optional<auth> auth = std::nullopt,
    const std::string& api_endpoint = "https://api.github.com"s ) {
    using namespace xxhr;
    auto url = api_endpoint + "/repos/"s + owner + "/" + repository + "/releases/tags/"+tag;
    auto retries_count = 5;
    std::function<void(xxhr::Response&&)> response_handler;


    auto do_request = [&]() { 
      if (auth) {
        GET(url,
          Authentication{auth->user, auth->pass},
          on_response = response_handler);
      } else {
        GET(url,
          on_response = response_handler);
      }
    };

    response_handler = [&](auto&& resp) {
      if ( resp.error && (retries_count > 0) ) {
        --retries_count;
        do_request();
      } else if ( (!resp.error) && (resp.status_code == 200) ) {
        result_handler(pre::json::from_json<releases::release_t>(resp.text));
      } else {
        throw std::runtime_error( "err : "s + std::string(resp.error) + "status: "s 
            + std::to_string(resp.status_code) + " accessing : "s + url );
      }
    };

    do_request();
  }

    /**
   * \brief gets the provided repo and pass it to result_handler otherwise throws.
   * \param auth credentials
   * \param owner
   * \param repository
   * \param tag tag of the release you are looking for 
   * \param result_handler taking a gh::repos::repository_t
   */
  inline void get_release_by_id(std::string owner, std::string repository, size_t id,
    std::function<void(releases::release_t&&)>&& result_handler,
    std::optional<auth> auth = std::nullopt,
    const std::string& api_endpoint = "https://api.github.com"s ) {
    using namespace xxhr;
    auto url = api_endpoint + "/repos/"s + owner + "/" + repository + "/releases/" + std::to_string(id);
    auto retries_count = 5;
    std::function<void(xxhr::Response&&)> response_handler;


    auto do_request = [&]() { 
      if (auth) {
        GET(url,
          Authentication{auth->user, auth->pass},
          on_response = response_handler);
      } else {
        GET(url,
          on_response = response_handler);
      }
    };

    response_handler = [&](auto&& resp) {
      if ( resp.error && (retries_count > 0) ) {
        --retries_count;
        do_request();
      } else if ( (!resp.error) && (resp.status_code == 200) ) {
        result_handler(pre::json::from_json<releases::release_t>(resp.text));
      } else {
        throw std::runtime_error( "err : "s + std::string(resp.error) + "status: "s 
            + std::to_string(resp.status_code) + " accessing : "s + url );
      }
    };

    do_request();
  }

  /**
   * @brief List all github releases for a repo
   * 
   * @param owner 
   * @param repository 
   * @param result_handler 
   * @param auth 
   * @param api_endpoint 
   */
  inline void list_releases(const std::string owner, const std::string repository,
    std::function<void(std::vector<releases::release_t>&&)>&& result_handler,
    std::optional<auth> auth = std::nullopt,
    const std::string& api_endpoint = "https://api.github.com"s 
  ) {
    using namespace xxhr;
    auto url = api_endpoint + "/repos/"s + owner + "/" + repository + "/releases";
    auto retries_count = 5;
    std::function<void(xxhr::Response&&)> response_handler;


    auto do_request = [&]() { 
      if (auth) {
        GET(url,
          Authentication{auth->user, auth->pass},
          on_response = response_handler);
      } else {
        GET(url,
          on_response = response_handler);
      }
    };

    response_handler = [&](auto&& resp) {
      if ( resp.error && (retries_count > 0) ) {
        --retries_count;
        do_request();
      } else if ( (!resp.error) && (resp.status_code == 200) ) {
        result_handler(pre::json::from_json<std::vector<releases::release_t>>(resp.text));
      } else {
        throw std::runtime_error( "err : "s + std::string(resp.error) + "status: "s 
            + std::to_string(resp.status_code) + " accessing : "s + url );
      }
    };

    do_request();
  }

  /**
   * @brief Create a github release in a repository
   * 
   * @param owner repo owner
   * @param repository 
   * @param release 
   * @param result_handler taking a gh::repos::repository_t
   * @param auth credentials
   */
  inline void create_release(const std::string owner, const std::string repository, const releases::create_release_t& release, 
    std::function<void(releases::release_t&&)>&& result_handler, 
    std::optional<auth> auth = std::nullopt, 
    const std::string& api_endpoint = "https://api.github.com"s
  ) {
    using namespace xxhr;

    auto url = api_endpoint + "/repos/"s + owner + "/" + repository + "/releases";
    auto retries_count = 5;
    std::function<void(xxhr::Response&&)> response_handler;

    auto do_request = [&]() { 

      auto header = Header{ { "Content-Type", "application/json" }, { "accept", "application/vnd.github+json" } };
      auto body = Body{ pre::json::to_json(release).dump() };      

      if (auth) {
        POST(url,
          Authentication{auth->user, auth->pass},
          header,
          body,
          on_response = response_handler);
      } else {
        POST(url, header, body, on_response = response_handler);
      }
    };

    response_handler = [&](auto&& resp) {
      if ( resp.error && (retries_count > 0) ) {
        --retries_count;
        do_request();
      } else if ( (!resp.error) && (resp.status_code == 200) ) {        
        result_handler(pre::json::from_json<releases::release_t>(resp.text));
      } else {
        throw std::runtime_error( "err : "s + std::string(resp.error) + "status: "s 
            + std::to_string(resp.status_code) + " accessing : "s + url );
      }
    };

    do_request();      
  }

  /**
   * @brief Update a github release
   * 
   * @param owner repo owner
   * @param repository 
   * @param release_id
   * @param release 
   * @param result_handler taking a gh::repos::repository_t
   * @param auth credentials
   */
  inline void update_release(const std::string owner, const std::string repository, const size_t release_id, 
    const releases::update_release_t& release, 
    std::function<void(releases::release_t&&)>&& result_handler, 
    std::optional<auth> auth = std::nullopt, 
    const std::string& api_endpoint = "https://api.github.com"s
  ) {
    using namespace xxhr;

    auto url = api_endpoint + "/repos/"s + owner + "/" + repository + "/releases/" + std::to_string(release_id);
    auto retries_count = 5;
    std::function<void(xxhr::Response&&)> response_handler;

    auto do_request = [&]() { 

      auto header = Header{ { "Content-Type", "application/json" }, { "accept", "application/vnd.github+json" } };
      auto body = Body{ pre::json::to_json(release).dump() };      

      if (auth) {
        PATCH(url,
          Authentication{auth->user, auth->pass},
          header,
          body,
          on_response = response_handler);
      } else {
        PATCH(url, header, body, on_response = response_handler);
      }
    };

    response_handler = [&](auto&& resp) {
      if ( resp.error && (retries_count > 0) ) {
        --retries_count;
        do_request();
      } else if ( (!resp.error) && (resp.status_code == 200) ) {        
        result_handler(pre::json::from_json<releases::release_t>(resp.text));
      } else {
        throw std::runtime_error( "err : "s + std::string(resp.error) + "status: "s 
            + std::to_string(resp.status_code) + " accessing : "s + url );
      }
    };

    do_request();      
  }

  /**
   * @brief Update a github release
   * 
   * @param owner repo owner
   * @param repository 
   * @param release_id
   * @param release 
   * @param result_handler taking a gh::repos::repository_t
   * @param auth credentials
   */
  inline void delete_release(const std::string owner, const std::string repository, const size_t release_id,
    auth auth, const std::string& api_endpoint = "https://api.github.com"s
  ) {
    using namespace xxhr;

    auto url = api_endpoint + "/repos/"s + owner + "/" + repository + "/releases/" + std::to_string(release_id);
    auto retries_count = 5;
    std::function<void(xxhr::Response&&)> response_handler;

    auto do_request = [&]() { 

      auto header = Header{ { "accept", "application/vnd.github+json" } };

      DELETE_(url,
        Authentication{auth.user, auth.pass},
        header,
        on_response = response_handler);
    };

    response_handler = [&](auto&& resp) {
      if ( resp.error && (retries_count > 0) ) {
        --retries_count;
        do_request();
      } else if ( (!resp.error) && (resp.status_code == 204) ) {
        // success        
      } else {
        throw std::runtime_error( "err : "s + std::string(resp.error) + "status: "s 
            + std::to_string(resp.status_code) + " accessing : "s + url );
      }
    };

    do_request();      
  }

  /**
   * @brief List all github releases for a repo
   * 
   * @param owner 
   * @param repository 
   * @param asset_id
   * @param result_handler 
   * @param auth 
   * @param api_endpoint 
   */
  inline void get_release_asset_info(const std::string owner, const std::string repository, size_t asset_id,
    std::function<void(releases::asset_t&&)>&& result_handler,
    std::optional<auth> auth = std::nullopt,
    const std::string& api_endpoint = "https://api.github.com"s 
  ) {
    using namespace xxhr;
    auto url = api_endpoint + "/repos/"s + owner + "/" + repository + "/releases/assets/" + std::to_string(asset_id);
    auto retries_count = 5;
    std::function<void(xxhr::Response&&)> response_handler;


    auto do_request = [&]() { 
      if (auth) {
        GET(url,
          Authentication{auth->user, auth->pass},
          on_response = response_handler);
      } else {
        GET(url,
          on_response = response_handler);
      }
    };

    response_handler = [&](auto&& resp) {
      if ( resp.error && (retries_count > 0) ) {
        --retries_count;
        do_request();
      } else if ( (!resp.error) && (resp.status_code == 200) ) {
        result_handler(pre::json::from_json<releases::asset_t>(resp.text));
      } else {
        throw std::runtime_error( "err : "s + std::string(resp.error) + "status: "s 
            + std::to_string(resp.status_code) + " accessing : "s + url );
      }
    };

    do_request();
  }

  /**
   * @brief Update a github release asset
   * 
   * @param owner repo owner
   * @param repository 
   * @param asset_id
   * @param release 
   * @param result_handler taking a gh::repos::repository_t
   * @param auth credentials
   */
  inline void update_release_asset(const std::string owner, const std::string repository, const size_t asset_id, 
    const releases::update_asset_t& release, 
    std::function<void(releases::asset_t&&)>&& result_handler, 
    auth auth, 
    const std::string& api_endpoint = "https://api.github.com"s
  ) {
    using namespace xxhr;

    auto url = api_endpoint + "/repos/"s + owner + "/" + repository + "/releases/assets/" + std::to_string(asset_id);
    auto retries_count = 5;
    std::function<void(xxhr::Response&&)> response_handler;

    auto do_request = [&]() { 

      auto header = Header{ { "Content-Type", "application/json" }, { "accept", "application/vnd.github+json" } };
      auto body = Body{ pre::json::to_json(release).dump() };      

      PATCH(url,
        Authentication{auth.user, auth.pass},
        header,
        body,
        on_response = response_handler);
    };

    response_handler = [&](auto&& resp) {
      if ( resp.error && (retries_count > 0) ) {
        --retries_count;
        do_request();
      } else if ( (!resp.error) && (resp.status_code == 200) ) {        
        result_handler(pre::json::from_json<releases::asset_t>(resp.text));
      } else {
        throw std::runtime_error( "err : "s + std::string(resp.error) + "status: "s 
            + std::to_string(resp.status_code) + " accessing : "s + url );
      }
    };

    do_request();      
  }  

  /**
   * @brief Delete a github release asset
   * 
   * @param owner repo owner
   * @param repository 
   * @param release_id
   * @param release 
   * @param result_handler taking a gh::repos::repository_t
   * @param auth credentials
   */
  inline void delete_release_asset(const std::string owner, const std::string repository, 
    const size_t asset_id, 
    const auth auth, 
    const std::string& api_endpoint = "https://api.github.com"s
  ) {
    using namespace xxhr;

    auto url = api_endpoint + "/repos/"s + owner + "/" + repository + "/releases/assets/" + std::to_string(asset_id);
    auto retries_count = 5;
    std::function<void(xxhr::Response&&)> response_handler;

    auto do_request = [&]() { 
  
      DELETE_(url,
        Authentication{auth.user, auth.pass},
        on_response = response_handler);

    };

    response_handler = [&](auto&& resp) {
      if ( resp.error && (retries_count > 0) ) {
        --retries_count;
        do_request();
      } else if ( (!resp.error) && (resp.status_code == 204) ) {        
        // success
      } else {
        throw std::runtime_error( "err : "s + std::string(resp.error) + "status: "s 
            + std::to_string(resp.status_code) + " accessing : "s + url );
      }
    };

    do_request();      
  }  



  /**
   * @brief Upload an asset to a release
   * 
   * @param owner 
   * @param repository 
   * @param asset_name File name of the asset
   * @param asset_label Additional info / label
   * @param raw_content 
   * @param content_type @see https://www.iana.org/assignments/media-types/media-types.xhtml
   * @param auth 
   * @param api_endpoint 
   */
  inline void create_release_asset(const std::string owner, const std::string repository, const size_t release_id,
    const std::string asset_name, const std::string asset_label, const std::string raw_content, const std::string content_type, 
    std::function<void(releases::asset_t&&)>&& result_handler, 
    const auth auth, const std::string& uploads_api_endpoint = "https://uploads.github.com"s
  ) {
    using namespace xxhr;

    auto url = uploads_api_endpoint + "/repos/"s + owner + "/" + repository + "/releases/" + std::to_string(release_id) + "/assets";
    auto retries_count = 5;
    std::function<void(xxhr::Response&&)> response_handler;

    auto do_request = [&]() { 
  
      POST(url,
        Authentication{auth.user, auth.pass},
        Header({
          { "Accept", "application/vnd.github+json" },
          { "Content-Type", content_type }
        }),
        Parameters( { 
          { "name", asset_name }, 
          { "label", asset_label }
        }),
        Body(raw_content),
        on_response = response_handler);

    };

    response_handler = [&](auto&& resp) {
      if ( resp.error && (retries_count > 0) ) {
        --retries_count;
        do_request();
      } else if ( (!resp.error) && (resp.status_code == 201) ) {        
        result_handler(pre::json::from_json<releases::asset_t>(resp.text));
      } else {
        throw std::runtime_error( "err : "s + std::string(resp.error) + "status: "s 
            + std::to_string(resp.status_code) + " accessing : "s + url );
      }
    };

    do_request();      
  }  

  /**
   * @brief Download release asset
   * 
   * @param owner 
   * @param repository 
   * @param asset_id 
   * @param result_handler 
   * @param auth 
   */
  inline void download_release_asset(const std::string owner, const std::string repository, 
    const size_t asset_id, std::function<void(std::string)>&& result_handler, 
    std::optional<auth> auth = std::nullopt,
    const std::string& api_endpoint = "https://api.github.com"s
  ) {
    using namespace xxhr;
    auto url = api_endpoint + "/repos/"s + owner + "/" + repository + "/releases/assets/" + std::to_string(asset_id);
    auto retries_count = 5;
    std::function<void(xxhr::Response&&)> response_handler;

    auto do_request = [&]() { 

      Header header({
        { "Accept", "application/octet-stream" }
      });
  
      if (auth) {
        GET(url,
          header,
          Authentication{auth->user, auth->pass},
          on_response = response_handler);
      } else {
        GET(url, header, on_response = response_handler);
      }

    };

    response_handler = [&](auto&& resp) {
      if ( resp.error && (retries_count > 0) ) {
        --retries_count;
        do_request();
      } else if ( (!resp.error) && (resp.status_code == 200) ) {        
        result_handler(resp.text);
      } else {
        throw std::runtime_error( "err : "s + std::string(resp.error) + "status: "s 
            + std::to_string(resp.status_code) + " accessing : "s + url );
      }
    };

    do_request();
  } 
}