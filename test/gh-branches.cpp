#include <gh/branches.hxx>

#include <cstdlib>
#include <pre/json/to_json.hpp>

int main(int argc, char** argv) {

  gh::list_branches("boostorg", "fusion", [](gh::repos::branches&& branches) {
    std::cout << pre::json::to_json(branches).dump(2) << std::endl;
  });


  gh::auth auth{ 
    std::getenv("GH_USER"),
    std::getenv("GH_PASS")
  };

  gh::list_branches("daminetreg", "xxhr", [](gh::repos::branches&& branches) {
    std::cout << pre::json::to_json(branches).dump(2) << std::endl;
  }, auth);

  gh::list_branches("boostorg", "fusion", [](gh::repos::branches&& branches) {
    std::cout << pre::json::to_json(branches).dump(2) << std::endl;
  }, auth);

  return 0;
}
