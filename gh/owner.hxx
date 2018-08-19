#pragma once

#include <cstdint>
#include <string>
#include <list>
#include <boost/fusion/include/adapt_struct.hpp>

#include <xxhr/xxhr.hpp>

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

