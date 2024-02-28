#include <boost/exception/diagnostic_information.hpp> 
#include <gh/pagination.hxx>
#include <algorithm>
#include <cstdlib>
#include <string>
#include <sstream>
#include <iostream>
#include "utils.hpp"

int main(int argc, char** argv) {
  using namespace std::string_literals;
  size_t returnCode = 0;


  std::string link_header_value = R"(<https://api.github.com/repositories/1300192/issues?page=2>; rel="prev", <https://api.github.com/repositories/1300192/issues?page=4>; rel="next", <https://api.github.com/repositories/1300192/issues?page=515>; rel="last", <https://api.github.com/repositories/1300192/issues?page=1>; rel="first")";
  std::string link_header_value_no_next = R"(<https://api.github.com/repositories/1300192/issues?page=2>; rel="prev", <https://api.github.com/repositories/1300192/issues?page=515>; rel="last", <https://api.github.com/repositories/1300192/issues?page=1>; rel="first")";


  auto test_chapter = [&](std::string title, auto&& fn) {
    std::cout << "🧪 Test: " << title << std::flush;
    bool success = true;
    std::string exception_message = "";

    try {
      fn();
    }
    catch(...) {
      success = false;
      returnCode++;
      exception_message = boost::current_exception_diagnostic_information();      
    }

    if(success) {
      std::cout << " PASS ✅" << std::endl;
    }
    else {
      std::cout << " FAIL 🟥\nException message: " << exception_message << std::endl;
    }
  };


  test_chapter("Basics / gh::detail::pagination::parse_gh_pagination_header() ",
    [&]() {     
      auto val = gh::detail::pagination::parse_gh_pagination_header(link_header_value);

      assertm(val.find(gh::detail::pagination::PAGE_LINK_FIRST) != val.end(), "counldn't find link PAGE_LINK_FIRST");
      assertm(val.find(gh::detail::pagination::PAGE_LINK_LAST) != val.end(), "counldn't find link PAGE_LINK_LAST");
      assertm(val.find(gh::detail::pagination::PAGE_LINK_NEXT) != val.end(), "counldn't find link PAGE_LINK_NEXT");
      assertm(val.find(gh::detail::pagination::PAGE_LINK_PREVIOUS) != val.end(), "counldn't find link PAGE_LINK_PREVIOUS");

      assertm(val.find(gh::detail::pagination::PAGE_LINK_FIRST)->second     == "https://api.github.com/repositories/1300192/issues?page=1",   "Wrong value");
      assertm(val.find(gh::detail::pagination::PAGE_LINK_LAST)->second      == "https://api.github.com/repositories/1300192/issues?page=515", "Wrong value");
      assertm(val.find(gh::detail::pagination::PAGE_LINK_NEXT)->second      == "https://api.github.com/repositories/1300192/issues?page=4",   "Wrong value");
      assertm(val.find(gh::detail::pagination::PAGE_LINK_PREVIOUS)->second  == "https://api.github.com/repositories/1300192/issues?page=2",   "Wrong value");
    }
  );

  // without next page:  
  test_chapter("Basics / gh::detail::pagination::parse_gh_pagination_header() without next page",
    [&]() {    
      auto val = gh::detail::pagination::parse_gh_pagination_header(link_header_value_no_next);

      assertm(val.find(gh::detail::pagination::PAGE_LINK_NEXT) == val.end(), "counldn't find link PAGE_LINK_NEXT");

      // everything else should be the same
      assertm(val.find(gh::detail::pagination::PAGE_LINK_FIRST) != val.end(), "counldn't find link PAGE_LINK_FIRST");
      assertm(val.find(gh::detail::pagination::PAGE_LINK_LAST) != val.end(), "counldn't find link PAGE_LINK_LAST");    
      assertm(val.find(gh::detail::pagination::PAGE_LINK_PREVIOUS) != val.end(), "counldn't find link PAGE_LINK_PREVIOUS");

      assertm(val.find(gh::detail::pagination::PAGE_LINK_FIRST)->second     == "https://api.github.com/repositories/1300192/issues?page=1",   "Wrong value");
      assertm(val.find(gh::detail::pagination::PAGE_LINK_LAST)->second      == "https://api.github.com/repositories/1300192/issues?page=515", "Wrong value");
      assertm(val.find(gh::detail::pagination::PAGE_LINK_PREVIOUS)->second  == "https://api.github.com/repositories/1300192/issues?page=2",   "Wrong value");
    }
  );


  // helpers for simulated XXHR responses
  auto mk_error_OK = []() { return xxhr::Error{xxhr::ErrorCode::OK}; };

  auto mk_headers_WITH_links = [&link_header_value, &link_header_value_no_next](bool with_next) {
    return xxhr::Header{
      { "Access-Control-Allow-Origin", "*" },
      { "Connection", "Keep-Alive" },
      { "Content-Encoding", "gzip" },
      { "Content-Type", "text/html; charset=utf-8" },
      { "Date", "Mon, 18 Jul 2016 16:06:00 GMT" },
      { "link", (with_next ? link_header_value : link_header_value_no_next)  }
    };
  };

  auto mk_headers_NO_links = []() {
    return xxhr::Header{
      { "Access-Control-Allow-Origin", "*" },
      { "Connection", "Keep-Alive" },
      { "Content-Encoding", "gzip" },
      { "Content-Type", "text/html; charset=utf-8" },
      { "Date", "Mon, 18 Jul 2016 16:06:00 GMT" }
    };
  };  

  // tests with full links:
  test_chapter("gh::detail::pagination::has_gh_pagination_links() with next page",
    [&]() {    
      xxhr::Response resp{ 
        200, 
        mk_error_OK(), 
        "<h1>Text</h1>", 
        mk_headers_WITH_links(true), 
        "https://api.github.com/repositories/1300192/issues?page=3"s,
        xxhr::Cookies{} 
      };

      assertm(gh::detail::pagination::has_gh_pagination_links(resp) == true, "Expected that the xxhr::Reponse has link: headers");

      auto next_page = gh::detail::pagination::get_next_page_url(resp);
      assertm(next_page.has_value(), "Should have had a next() page");
      assertm(next_page.value() == "https://api.github.com/repositories/1300192/issues?page=4", "Unexpected value for next() page (should be page == 4)");
    }
  );

  

  // tests with links but no next page:
  test_chapter("gh::detail::pagination::has_gh_pagination_links() WITOUT next page",
    [&]() { 
      xxhr::Response resp{ 
        200, 
        mk_error_OK(), 
        "<h1>Text</h1>", 
        mk_headers_WITH_links(false /* without NEXT page */), 
        "https://api.github.com/repositories/1300192/issues?page=3"s, 
        xxhr::Cookies{}
      };

      assertm(gh::detail::pagination::has_gh_pagination_links(resp) == true, "Expected that the xxhr::Reponse has link: headers");

      auto next_page = gh::detail::pagination::get_next_page_url(resp);
      assertm(next_page.has_value() == false, "Should NOT have had a next() page");
    }
  );


  // tests nothing breaks WITHOUT links
  test_chapter("gh::detail::pagination::has_gh_pagination_links() WITHOUT header links",
    [&]() { 
      xxhr::Response resp{
        200,
        mk_error_OK(),
        "<h1>Text</h1>", 
        mk_headers_NO_links(), 
        "https://api.github.com/repositories/1300192/issues?page=3"s,
        xxhr::Cookies{}
      };

      assertm(gh::detail::pagination::has_gh_pagination_links(resp) == false, "Expected that this xxhr::Reponse has NO link: headers"); 

      auto next_page = gh::detail::pagination::get_next_page_url(resp);
      assertm(next_page.has_value() == false, "Should NOT have had a next() page");
    }
  );


  return returnCode;
}
