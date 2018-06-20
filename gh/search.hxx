#pragma once

#include <cstdint>
#include <string>
#include <list>
#include <nlohmann/json.hpp>
#include <pre/json/from_json.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

#include <gh/detail/http.hxx>
//TODO: implement returning the json
//TODO: refactor beast backend into xxhr


namespace gh {

  /**
   * \brief Repository owner, typically a github user or organization.
   */ 
  struct owner_t {
    uint64_t id;
    std::string login;
    bool site_admin{};
    std::string type;
    std::string avatar_url;
    std::string events_url;
    std::string followers_url;
    std::string following_url;
    std::string gists_url;
    std::string gravatar_id;
    std::string html_url;
    std::string node_id;
    std::string organizations_url;
    std::string received_events_url;
    std::string repos_url;
    std::string starred_url;
    std::string subscriptions_url;
    std::string url;
  };

}

BOOST_FUSION_ADAPT_STRUCT(gh::owner_t,
  id, login, site_admin, type, avatar_url, events_url,
  followers_url,following_url,gists_url,gravatar_id,html_url,
  node_id,organizations_url,received_events_url,repos_url,
  starred_url,subscriptions_url,url);

namespace gh {
  /**
   * \brief repository as found in result lists.
   */ 
  struct repository_t {
    uint64_t id;
    //! name of the form org/repo or owner/repo
    std::string full_name;
    //! name inside the organization/owner
    std::string name;
    //bool private{}; // TODO: support keyword mapping for member names in pre::json because private is a c++ keyword.
    bool fork{};
    owner_t owner;

    std::string archive_url;
    std::string assignees_url;
    std::string blobs_url;
    std::string branches_url;
    std::string collaborators_url;
    std::string comments_url;
    std::string commits_url;
    std::string compare_url;
    std::string contents_url;
    std::string contributors_url;
    std::string deployments_url;
    std::optional<std::string> description;
    std::string downloads_url;
    std::string events_url;
    std::string forks_url;
    std::string git_commits_url;
    std::string git_refs_url;
    std::string git_tags_url;
    std::string hooks_url;
    std::string html_url;
    std::string issue_comment_url;
    std::string issue_events_url;
    std::string issues_url;
    std::string keys_url;
    std::string labels_url;
    std::string languages_url;
    std::string merges_url;
    std::string milestones_url;
    std::string node_id;
    std::string notifications_url;

    std::string pulls_url;
    std::string releases_url;
    std::string stargazers_url;
    std::string statuses_url;
    std::string subscribers_url;
    std::string subscription_url;
    std::string tags_url;
    std::string teams_url;
    std::string trees_url;
    std::string url;
    
  };
}
BOOST_FUSION_ADAPT_STRUCT(gh::repository_t,
  id, full_name, name, /*private_,*/ fork, owner,
  archive_url, assignees_url, blobs_url, collaborators_url,
  comments_url, commits_url, compare_url, contents_url,
  contributors_url, deployments_url, description,
  downloads_url, events_url, forks_url, git_commits_url,
  git_refs_url, git_tags_url,hooks_url,html_url,issue_comment_url,
  issue_events_url,issues_url,keys_url,labels_url,
  languages_url, merges_url, milestones_url,node_id,
  notifications_url, pulls_url, releases_url,stargazers_url,statuses_url,
  subscribers_url,subscription_url,tags_url,teams_url,trees_url,url);

namespace gh::code_search {

  struct result {
    //! name of the file found
    std::string name;
    //! Full path to file found (including above name)
    std::string path;
    std::string url;
    std::string git_url;
    std::string html_url;
    double score;
    std::string sha;
    gh::repository_t repository;
  };
    
  using results = std::list<result>;
}

BOOST_FUSION_ADAPT_STRUCT(gh::code_search::result, 
  name, path, url, git_url, 
  html_url, score, sha, repository);


namespace gh {

  /**
   * \brief Provides access to the github search API v3
   * \param the results as a JSON object.
   */
  inline code_search::results query_code_search(const std::string& criteria) {

    //TODO: use xxhr::util::urlEncode to encode criteria. 
    auto found = gh::detail::http_get("api.github.com", "443", std::string("/search/code?q=") + criteria );
    auto found_json = nlohmann::json::parse(found)["items"];
    return pre::json::from_json<code_search::results>(found_json);
  }
}