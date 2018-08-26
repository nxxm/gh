#include <gh/branches.hxx>

#include <cstdlib>
#include <pre/json/to_json.hpp>

int main(int argc, char** argv) {
  
  gh::auth auth{ 
    std::getenv("GH_USER"),
    std::getenv("GH_PASS")
  };

  gh::list_branches(auth, "daminetreg", "xxhr", [](gh::repos::branches&& branches) {
    std::cout << pre::json::to_json(branches).dump(2) << std::endl;
  });

  gh::list_branches(auth, "boostorg", "fusion", [](gh::repos::branches&& branches) {
    std::cout << pre::json::to_json(branches).dump(2) << std::endl;
  });

  return 0;
}
