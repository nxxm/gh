#include <gh/search.hxx>

#include <cstdlib>
#include <pre/json/to_json.hpp>

int main(int argc, char** argv) {

  gh::auth auth{ 
    std::getenv("GH_USER"),
    std::getenv("GH_PASS")
  };

  {
    std::cout << "pre ::: " << std::endl;
    gh::query_code_search(auth, "utils.hpp path:pre/bytes/",
      [](auto&& possibles) {
        std::cout << pre::json::to_json(possibles).dump(2) << std::endl;
      }
    );
  }

  {
    std::cout << "fusion ::: " << std::endl;
    gh::query_code_search(auth, "adapt_struct.hpp path:boost/fusion/include",
      [](auto&& possibles) {
        std::cout << pre::json::to_json(possibles).dump(2) << std::endl;
      }
    );
  }



  return 0;
}
