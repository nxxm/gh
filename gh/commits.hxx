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

namespace gh::commits {

  struct commit_committer_author_t {
    std::string date;
    std::string name;
    std::string email;
  };

  struct commit_tree_parent_t {
    std::string url;
    std::string sha;
    std::optional<std::string> html_url;
  };

  struct commit_verification_t {
    bool verified;
    std::string reason;
    std::optional<std::string> signature;
    std::optional<std::string> payload;
  };

  struct commit_t {
    std::string sha;
    std::string node_id;
    std::string url;
    std::string html_url;
    commit_committer_author_t author; 
    commit_committer_author_t committer;
    commit_tree_parent_t tree;
    std::vector<commit_tree_parent_t> parents;
    commit_verification_t verification;
  };
}

BOOST_FUSION_ADAPT_STRUCT(gh::commits::commit_committer_author_t, date, name, email);
BOOST_FUSION_ADAPT_STRUCT(gh::commits::commit_tree_parent_t, url, sha, html_url);
BOOST_FUSION_ADAPT_STRUCT(gh::commits::commit_verification_t, verified, reason, signature, payload);
BOOST_FUSION_ADAPT_STRUCT(gh::commits::commit_t, sha, node_id, url, html_url, author, committer, tree, parents, verification);

namespace gh {

  using namespace std::string_literals;

  /**
   * \brief gets the commit info of a repo by commit ID
   * \param auth credentials
   * \param owner
   * \param repository
   * \param commit_id Git commit hash
   * \param result_handler taking a gh::commits::commit_t
   */
  inline void get_commit(std::string owner, std::string repository, std::string commit_id,
    std::function<void(gh::commits::commit_t&&)>&& result_handler,
    std::optional<auth> auth = std::nullopt,
    const std::string& api_endpoint = "https://api.github.com"s ) {
  
    using namespace xxhr;
    auto url = api_endpoint + "/repos/"s + owner + "/"s + repository + "/git/commits/"s +  commit_id;

    auto response_handler = [&](auto&& resp) {
      if ( (!resp.error) && (resp.status_code == 200) ) {
        result_handler(pre::json::from_json<gh::commits::commit_t>(resp.text));
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
}
