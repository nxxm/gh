#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>
#include <pre/json/from_json.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

#include <xxhr/xxhr.hpp>

#include <gh/owner.hxx>
#include <gh/auth.hxx>
#include <gh/pagination.hxx>

namespace gh::repos {

  struct label_t {
    uint64_t id;
    std::string url;
    std::string name;
    std::optional<std::string> description;
    std::string color;
    //XXX: impossible is C++ keyword : bool default;
  };
}

BOOST_FUSION_ADAPT_STRUCT(gh::repos::label_t, id, url, name, description, color);

namespace gh::repos {

  struct milestone_t {
    uint64_t id;
    std::string url;
    std::string html_url;
    std::string labels_url;
    uint64_t number;
    std::string state;
    std::string title;
    std::string description;
    owner_t creator;
    uint64_t open_issues;
    uint64_t closed_issues;

    std::optional<std::string> closed_at;
    std::string created_at;
    std::optional<std::string> updated_at;
    std::optional<std::string> due_on;
  };

}

BOOST_FUSION_ADAPT_STRUCT(gh::repos::milestone_t, id, url, html_url, labels_url, number,
 state, title, description, creator, open_issues, closed_issues, closed_at, created_at, 
 updated_at, due_on);

namespace gh::repos {

  struct issue_t {
    uint64_t id;
    std::string url;
    std::string repository_url;
    std::string labels_url;
    std::string comments_url;
    std::string events_url;
    std::string html_url;
    uint64_t number;
    std::string state;
    std::string title;
    std::optional<std::string> body;
    owner_t user;

    std::vector<label_t> labels;

    std::optional<owner_t> assignee;
    std::vector<owner_t> assignees;



    std::optional<milestone_t> milestone;

    bool locked;
    std::optional<std::string> active_lock_reason;
    uint64_t comments;

    struct pull_request_t {
      std::string url;
      std::string html_url;
      std::string diff_url;
      std::string patch_url;
    };
    std::optional<pull_request_t> pull_request;

    std::optional<std::string> closed_at;
    std::string created_at;
    std::optional<std::string> updated_at;

  };
}

BOOST_FUSION_ADAPT_STRUCT(gh::repos::issue_t::pull_request_t, url, html_url, diff_url,
 patch_url);

BOOST_FUSION_ADAPT_STRUCT(gh::repos::issue_t, id, url, repository_url, labels_url,
 comments_url, events_url, html_url, number, state, title, body, user, labels, 
 assignee, assignees, milestone, locked, active_lock_reason, comments, pull_request,
  closed_at, created_at, updated_at);

namespace gh::repos {

  //! Issues in a repo
  using issues = std::vector<issue_t>;
}

namespace gh {

  using namespace std::string_literals;

  inline void list_issues(std::string owner, std::string repository, 
    std::function<void(repos::issues&&)>&& result_handler,
    std::string state = "open",
    std::optional<auth> auth = std::nullopt,
    const std::string& api_endpoint = "https://api.github.com"s ) {

    using namespace xxhr;
    std::function<void(xxhr::Response&&)> response_handler;
    repos::issues all_issues;

    auto do_request = [&](std::string url) {
      if (auth) {
        GET(url,
          Authentication{auth->user, auth->pass},
          on_response = response_handler);
      } else {
        GET(url, on_response = response_handler);
      }
    };   

    response_handler = [&](auto&& resp) {
      if ( (!resp.error) && (resp.status_code == 200) ) {
        auto page = pre::json::from_json<repos::issues>(resp.text);
        all_issues.insert(all_issues.end(), page.begin(), page.end());

        auto next_page = gh::detail::pagination::get_next_page_url(resp);
        
        if(next_page) {
          do_request(next_page.value());
        }
        else {
          result_handler({ all_issues.begin(), all_issues.end()});
        }
      } else {
        throw std::runtime_error( "err : "s + std::string(resp.error) + "status: "s 
            + std::to_string(resp.status_code) + " accessing : "s + resp.url );
      }
    };

    auto url = api_endpoint + "/repos/"s + owner + "/" + repository + "/issues?state=" + state + "&" + gh::detail::pagination::get_per_page_query_string();
    do_request(url);
  }

  inline void get_issue(std::string owner, std::string repository, uint64_t issue_number,
    std::function<void(repos::issue_t&&)>&& result_handler,
    std::optional<auth> auth = std::nullopt,
    const std::string& api_endpoint = "https://api.github.com"s ) {

    using namespace xxhr;
    auto url = api_endpoint + "/repos/"s + owner + "/" + repository + "/issues/" + std::to_string(issue_number);

    auto response_handler = [&](auto&& resp) {
      if ( (!resp.error) && (resp.status_code == 200) ) {
        result_handler(pre::json::from_json<repos::issue_t>(resp.text));
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
