#include <gh/search.hxx>

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

  auto possibles = gh::code_search("utils.hpp path:pre/bytes/");
  std::cout << possibles.dump(2) << std::endl;

  return 0;
}