#include <gh/search.hxx>

#include <pre/json/to_json.hpp>

int main(int argc, char** argv) {

  {
    std::cout << "pre ::: " << std::endl;
    gh::query_code_search("utils.hpp path:pre/bytes/",
      [](auto&& possibles) {
        std::cout << pre::json::to_json(possibles).dump(2) << std::endl;
      }
    );
  }

  {
    std::cout << "fusion ::: " << std::endl;
    gh::query_code_search("adapt_struct.hpp path:boost/fusion/include",
      [](auto&& possibles) {
        std::cout << pre::json::to_json(possibles).dump(2) << std::endl;
      }
    );
  }



  return 0;
}
