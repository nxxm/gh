#pragma once

#include <string>
#include <map>
#include <xxhr/xxhr.hpp>
#include <xxhr/util.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>

namespace gh::detail::pagination {

    const size_t MAX_PAGE_SIZE = 100;
    const size_t MIN_PAGE_SIZE = 5;
    const size_t DEFAULT_PAGE_SIZE = 30;

    const std::string PAGE_LINK_FIRST = "first";
    const std::string PAGE_LINK_LAST = "last";
    const std::string PAGE_LINK_NEXT = "next";
    const std::string PAGE_LINK_PREVIOUS = "prev";

    const std::string HEADER_NAME_PAGE_LINKS = "link";

    inline bool has_gh_pagination_links(const xxhr::Response &resp) {
        return resp.header.find(HEADER_NAME_PAGE_LINKS) != resp.header.end();
    }
    
    inline std::map<std::string, std::string> parse_gh_pagination_header(std::string header_value) {
        // as per doc https://docs.github.com/en/rest/using-the-rest-api/using-pagination-in-the-rest-api?apiVersion=2022-11-28, the header comes as:
        // link: <https://api.github.com/repositories/1300192/issues?page=2>; rel="prev", <https://api.github.com/repositories/1300192/issues?page=4>; rel="next", <https://api.github.com/repositories/1300192/issues?page=515>; rel="last", <https://api.github.com/repositories/1300192/issues?page=1>; rel="first"
        using namespace std::string_literals;
    
        std::map<std::string, std::string> result;

        std::vector<std::string> link_parts;
        boost::split(link_parts, header_value, boost::is_any_of(","));

        for(auto lp : link_parts) {

            size_t pos_semicolon = lp.find(';', 0);

            std::string url_fragment = lp.substr(0, pos_semicolon); // should look like ' <https://api.github.com/repositories/1300192/issues?page=2>'
            boost::trim_all_if(url_fragment, boost::is_any_of("<> "));


            std::string rel_fragment = lp.substr(pos_semicolon + 1 /* skip the semicolon */); 
            boost::trim_all(rel_fragment);  // should now look like 'rel="page-link-name"' 

            static const std::string rel_prefix = "rel=";
            if(boost::starts_with(rel_fragment, rel_prefix)) {
                rel_fragment = rel_fragment.substr(rel_prefix.length());
                boost::trim_if(rel_fragment, boost::is_any_of("\""));
            }
            else {
                throw std::runtime_error("Cannot parse malformed GH pagination header: link: "s + header_value);
            }

            result.insert({ rel_fragment, url_fragment });
        }

        return result;
    }


    inline std::optional<std::string> get_next_page_url(const xxhr::Response &resp) {

        if(has_gh_pagination_links(resp)) {
            auto links = parse_gh_pagination_header(resp.header.at(HEADER_NAME_PAGE_LINKS));
            auto next_page_it = links.find(PAGE_LINK_NEXT);
            if(next_page_it != links.end()) {
                return next_page_it->second;              
            }
        }

        return std::nullopt;
    }
  
}
