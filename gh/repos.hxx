#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>
#include <pre/json/from_json.hpp>
#include <pre/json/to_json.hpp>
#include <pre/json/mapping.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

#include <xxhr/xxhr.hpp>

#include <gh/auth.hxx>
#include <gh/owner.hxx>
#include <gh/branches.hxx>

namespace gh::repos {

  struct permissions_t {
    bool admin;
    bool push;
    bool pull;
  };
}

BOOST_FUSION_ADAPT_STRUCT(gh::repos::permissions_t,
  admin, push, pull);


namespace gh::repos {

  struct license_t { 
    std::string key;
    std::string name;
    std::optional<std::string> spdx_id;
    std::optional<std::string> url;
  };
}

BOOST_FUSION_ADAPT_STRUCT(gh::repos::license_t,
  key, name, spdx_id, url);

namespace gh::repos {

  struct organization_t {
    std::string login;
    size_t id;
    std::string avatar_url;
    std::string gravatar_id;
    std::string url;
    std::string html_url;
    std::string followers_url;
    std::string following_url;
    std::string gists_url;
    std::string starred_url;
    std::string subscriptions_url;
    std::string organizations_url;
    std::string repos_url;
    std::string events_url;
    std::string received_events_url;
    std::string type;
    bool site_admin;
  };

}

BOOST_FUSION_ADAPT_STRUCT(gh::repos::organization_t,
  login, id, avatar_url, gravatar_id, url, html_url, 
  followers_url, following_url, gists_url, starred_url, 
  subscriptions_url, organizations_url, repos_url, events_url,
  received_events_url, type, site_admin);

namespace gh::repos {

  /**
   * \brief repository as found in repos api.
   */ 
  struct basic_repository_t {
    uint64_t id;

    //! name of the form org/repo or owner/repo
    std::string full_name;
    //! name inside the organization/owner
    std::string name;
    //bool private{}; // TODO: support keyword mapping for member names in pre::json because private is a c++ keyword.
    bool fork{};
    owner_t owner;

    //! Whether the repository is private. (Default: false)
    std::optional<bool> is_private;

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

    std::optional<std::string> homepage;
    std::optional<std::string> language;
    size_t forks_count;
    size_t stargazers_count;
    size_t watchers_count;
    size_t size;

    std::string default_branch;

    size_t open_issues_count;
    std::optional<std::vector<std::string>> topics;

    bool has_issues;
    bool has_projects;
    bool has_wiki;
    bool has_pages;
    bool has_downloads;
    std::optional<bool> archived;
    std::string pushed_at;
    std::string created_at;
    std::string updated_at;

    std::optional<permissions_t> permissions;
    std::optional<bool> allow_rebase_merge;
    std::optional<bool> allow_squash_merge;
    std::optional<bool> allow_merge_commit;
    std::optional<size_t> subscribers_count;
    std::optional<size_t> network_count;
    std::optional<license_t> license;

    std::optional<organization_t> organization;

    std::optional<std::string> visibility;

  };

  struct repository_t : public basic_repository_t {
    std::optional<basic_repository_t> parent;
    std::optional<basic_repository_t> source;
  };
}

BOOST_FUSION_ADAPT_STRUCT(gh::repos::basic_repository_t,
  (auto, id) (auto, full_name) (auto, name)
  (auto, is_private) (auto, fork) (auto, owner) 
  (auto, description)

  (auto, archive_url) (auto, assignees_url) (auto, blobs_url)
  (auto, collaborators_url) (auto, comments_url) (auto, commits_url)
  (auto, compare_url) (auto, contents_url) (auto, contributors_url) 
  (auto, deployments_url)
  (auto, downloads_url) (auto, events_url) (auto, forks_url)
  (auto, git_commits_url) (auto, git_refs_url) (auto, git_tags_url)
  (auto, hooks_url) (auto, html_url) (auto, issue_comment_url) 
  (auto, issue_events_url) (auto, issues_url) (auto, keys_url)
  (auto, labels_url) (auto, languages_url) (auto, merges_url) 
  (auto, milestones_url) (auto, notifications_url) (auto, pulls_url) 
  (auto, releases_url) (auto ,stargazers_url) (auto,statuses_url)
  (auto, subscribers_url) (auto, subscription_url) (auto,tags_url) 
  (auto, teams_url) (auto, trees_url) (auto, url)
  
  (auto, homepage) (auto, language) (auto, forks_count) (auto, stargazers_count)
  (auto, watchers_count) (auto, size) (auto, default_branch) (auto, open_issues_count)
  (auto, topics) (auto, has_issues) (auto, has_projects)
  (auto, has_wiki) (auto, has_pages) (auto, has_downloads) 
  (auto, archived) (auto, pushed_at) (auto, created_at) (auto, updated_at)

  (auto, permissions) (auto, allow_rebase_merge) (auto, allow_squash_merge)
  (auto, allow_merge_commit) (auto, subscribers_count) (auto, network_count)
  (auto, license) (auto, organization) (auto, visibility)
);


BOOST_FUSION_ADAPT_STRUCT(gh::repos::repository_t, 
  (auto, id) (auto, full_name) (auto, name)
  (auto, is_private) (auto, fork) (auto, owner) 
  (auto, description)

  (auto, archive_url) (auto, assignees_url) (auto, blobs_url)
  (auto, collaborators_url) (auto, comments_url) (auto, commits_url)
  (auto, compare_url) (auto, contents_url) (auto, contributors_url) 
  (auto, deployments_url)
  (auto, downloads_url) (auto, events_url) (auto, forks_url)
  (auto, git_commits_url) (auto, git_refs_url) (auto, git_tags_url)
  (auto, hooks_url) (auto, html_url) (auto, issue_comment_url) 
  (auto, issue_events_url) (auto, issues_url) (auto, keys_url)
  (auto, labels_url) (auto, languages_url) (auto, merges_url) 
  (auto, milestones_url) (auto, notifications_url) (auto, pulls_url) 
  (auto, releases_url) (auto ,stargazers_url) (auto,statuses_url)
  (auto, subscribers_url) (auto, subscription_url) (auto,tags_url) 
  (auto, teams_url) (auto, trees_url) (auto, url)
  
  (auto, homepage) (auto, language) (auto, forks_count) (auto, stargazers_count)
  (auto, watchers_count) (auto, size) (auto, default_branch) (auto, open_issues_count)
  (auto, topics) (auto, has_issues) (auto, has_projects)
  (auto, has_wiki) (auto, has_pages) (auto, has_downloads) 
  (auto, archived) (auto, pushed_at) (auto, created_at) (auto, updated_at)

  (auto, permissions) (auto, allow_rebase_merge) (auto, allow_squash_merge)
  (auto, allow_merge_commit) (auto, subscribers_count) (auto, network_count)
  (auto, license) (auto, organization) (auto, visibility)
    
  (auto, parent) (auto, source));


namespace gh::repos {

  /**
   * \brief repository as found in repos api.
   */ 
  struct create_repository_t {

    //! The name of the repository.
    std::string name;

    //! A short description of the repository.
    std::optional<std::string> description;

    //! Whether the repository is private. (Default: false)
    std::optional<bool> is_private; // needs to be mapped

    /**
     * @brief  Only in context of organization repositories
     * Can be public or private. If your organization is associated with an 
     * enterprise account usin visibility can also be internal. 
     * 
     * Can be one of: public, private, internal 
     */
    std::optional<std::string> visibility;

    //! Whether issues are enabled. (Default: true)
    std::optional<bool> has_issues;

    //! Whether projects are enabled. (Default: true)
    std::optional<bool> has_projects;

    //! Whether the wiki is enabled. (Default: true)
    std::optional<bool> has_wiki;

    //! Whether downloads are enabled. (Default: true)
    std::optional<bool> has_downloads;

    //! The id of the team that will be granted access to this repository. This is only valid when creating a repository in an organization.
    std::optional<size_t> team_id;

    //! Whether the repository is initialized with a minimal README. (Default: false)
    std::optional<bool> auto_init;

    //! The desired language or platform to apply to the .gitignore.
    std::optional<std::string> gitignore_template;

    //! The license keyword of the open source license for this repository.
    std::optional<std::string> license_template;

    //! Whether to allow squash merges for pull requests. (Default: true)
    std::optional<bool> allow_squash_merge;

    //! Whether to allow merge commits for pull requests. (Default: true)
    std::optional<bool> allow_merge_commit;

    //! Whether to allow rebase merges for pull requests. (Default: true)
    std::optional<bool> allow_rebase_merge;

    //! Whether to allow Auto-merge to be used on pull requests. (Default: false)
    std::optional<bool> allow_auto_merge;

    //! Whether to delete head branches when pull requests are merged (Default: false)
    std::optional<bool> delete_branch_on_merge;

    //! Whether this repository acts as a template that can be used to generate new repositories. (Default: false)
    std::optional<bool> is_template;
  };
}

BOOST_FUSION_ADAPT_STRUCT(gh::repos::create_repository_t, 
  (auto, name) (auto, description)
  (auto, is_private) (auto, visibility) 
  (auto, has_issues) (auto, has_projects) (auto, has_wiki) (auto, has_downloads) 
  (auto, team_id) (auto, auto_init) 
  (auto, gitignore_template) 
  (auto, license_template) 
  (auto, allow_squash_merge) 
  (auto, allow_merge_commit) 
  (auto, allow_rebase_merge) 
  (auto, allow_auto_merge) 
  (auto, delete_branch_on_merge) 
  (auto, is_template))

namespace gh {

  using namespace std::string_literals;

  /**
   * \brief gets the provided repo and pass it to result_handler otherwise throws.
   * \param auth credentials
   * \param owner
   * \param repository
   * \param result_handler taking a gh::repos::repository_t
   */
  inline void repos_get(std::string owner, std::string repository, 
    std::function<void(repos::repository_t&&)>&& result_handler,
    std::optional<auth> auth = std::nullopt,
    const std::string& api_endpoint = "https://api.github.com"s ) {

  
    using namespace xxhr;
    auto url = api_endpoint + "/repos/"s + owner + "/" + repository;

    auto response_handler = [&](auto&& resp) {
      if ( (!resp.error) && (resp.status_code == 200) ) {
        result_handler(pre::json::from_json<repos::repository_t>(resp.text, [](auto &jdoc) {
          pre::json::remap_property(jdoc, "private", "is_private");
        }));
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

  namespace detail {
    inline void json_remap_private_to_is_private(nlohmann::json& jdoc) {
      pre::json::remap_property(jdoc, "private", "is_private");
    }

    inline void json_remap_is_private_to_private(nlohmann::json& jdoc) {
      pre::json::remap_property(jdoc, "is_private", "private");
    }
  }

  /**
   * @brief Create a repo under the specified organization
   * 
   * @param organization org the authenticated user has to be a member of
   * @param new_repo repo creation info
   * @param result_handler 
   * @param auth 
   * @param api_endpoint 
   */
  inline void create_repo(const std::string organization, const gh::repos::create_repository_t new_repo,
    std::function<void(repos::repository_t&&)>&& result_handler,
    const auth& auth,
    const std::string& api_endpoint = "https://api.github.com"s
  ) {

    using namespace xxhr;
    auto url = api_endpoint + "/orgs/"s + organization + "/repos";
    auto retries_count = 5;
    std::function<void(xxhr::Response&&)> response_handler;

    auto do_request = [&]() { 
  
      POST(url,
        Authentication{auth.user, auth.pass},
        Header({
          { "Accept", "application/vnd.github+json" },
        }),
        Body(pre::json::to_json(new_repo, detail::json_remap_is_private_to_private).dump()),
        on_response = response_handler);

    };

    response_handler = [&](auto&& resp) {
      if ( resp.error && (retries_count > 0) ) {
        --retries_count;
        do_request();
      } else if ( (!resp.error) && (resp.status_code == 200 || resp.status_code == 201) ) {   
        result_handler(pre::json::from_json<repos::repository_t>(resp.text, detail::json_remap_private_to_is_private));
      } else {
        throw std::runtime_error( "err : "s + std::string(resp.error) + "status: "s 
            + std::to_string(resp.status_code) + " accessing : "s + url );
      }
    };

    do_request();     
  }

  /**
   * @brief Create a repo under the provided user
   * 
   * @param new_repo repo creation info
   * @param result_handler 
   * @param auth 
   * @param api_endpoint 
   */
  inline void create_repo(const gh::repos::create_repository_t new_repo,
    std::function<void(repos::repository_t&&)>&& result_handler,
    const auth& auth,
    const std::string& api_endpoint = "https://api.github.com"s
  ) {

    using namespace xxhr;
    auto url = api_endpoint + "/user/repos";
    auto retries_count = 5;
    std::function<void(xxhr::Response&&)> response_handler;

    auto do_request = [&]() { 
  
      POST(url,
        Authentication{auth.user, auth.pass},
        Header({
          { "Accept", "application/vnd.github+json" },
        }),
        Body(pre::json::to_json(new_repo, detail::json_remap_is_private_to_private).dump()),
        on_response = response_handler);

    };

    response_handler = [&](auto&& resp) {
      if ( resp.error && (retries_count > 0) ) {
        --retries_count;
        do_request();
      } else if ( (!resp.error) && (resp.status_code == 200 || resp.status_code == 201) ) {
        std::cout << "Raw body:" << resp.text << std::endl;
        result_handler(pre::json::from_json<repos::repository_t>(resp.text, detail::json_remap_private_to_is_private));
      } else {
        throw std::runtime_error( "err : "s + std::string(resp.error) + "status: "s 
            + std::to_string(resp.status_code) + " accessing : "s + url );
      }
    };

    do_request();     
  }

  inline void delete_repo(const std::string owner, const std::string repository, const auth auth, 
    const std::string& api_endpoint = "https://api.github.com"s
  ) {
    using namespace xxhr;

    auto url = api_endpoint + "/repos/"s + owner + "/" + repository;
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


}
