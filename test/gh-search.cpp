#include <gh/search.hxx>

#include <cstdlib>
#include <pre/json/to_json.hpp>
#include "utils.hpp"

int main(int argc, char** argv) {

  auto auth = test_utils::get_auth();

  {
    std::cout << "pre ::: " << std::endl;
    gh::query_code_search( "helpers.hpp path:src/ language:CMake", // ~500 results
      [](auto&& possibles) {
        assertm(possibles.size() > 100, "Should be more than 100 results (pagination test)");
        std::cout << pre::json::to_json(possibles).dump(2) << std::endl;
      },
      auth
    );
  }

  return 0;
}
