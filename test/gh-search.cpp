#include <gh/search.hxx>

#include <pre/json/to_json.hpp>

int main(int argc, char** argv) {

  //octoxxit::detail::http_get("api.github.com", "443", "/");


/*Github search query

Many results for - adapt_struct.hpp path:include/boost/fusion/include/
Can be tweaked with : adapt_struct.hpp path:include/boost/fusion/include/ user:boostorg

Works - utils.hpp path:pre/bytes/*/

  //https://api.github.com/search/code?q={query}{&page,per_page,sort,order}

  // TODO: help enduser create personal access token
  // Octoxit dev token  
  //auto token="5c8bc510c7880fcb0db28410218665d707564b3f";


  /*


  auto found_code = octoxxit::detail::http_get("api.github.com", "443", "/search/code?q=utils.hpp+path%3Apre%2Fbytes%2F");
  auto some = nlohmann::json::parse(found_code);

   std::cout << some.dump(2) << std::endl;
  */

  {
    std::cout << "pre ::: " << std::endl;
    auto possibles = gh::query_code_search("utils.hpp+path%3Apre%2Fbytes%2F" /*"utils.hpp path:pre/bytes/"*/);

    std::cout << pre::json::to_json(possibles).dump(2) << std::endl;
  }

  {
    std::cout << "fusion ::: " << std::endl;
    auto possibles = gh::query_code_search("adapt_struct.hpp%20path%3a%2fboost%2ffusion%2finclude%2f" /*"adapt_struct.hpp path:/boost/fusion/include/"*/);
    std::cout << pre::json::to_json(possibles).dump(2) << std::endl;
  }



  return 0;
}
