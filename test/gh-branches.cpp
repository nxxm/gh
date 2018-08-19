#include <gh/branches.hxx>
#include <pre/json/to_json.hpp>

int main(int argc, char** argv) {

  gh::list_branches("daminetreg", "xxhr", [](gh::repos::branches&& branches) {
    std::cout << pre::json::to_json(branches).dump(2) << std::endl;
  });

  gh::list_branches("boostorg", "fusion", [](gh::repos::branches&& branches) {
    std::cout << pre::json::to_json(branches).dump(2) << std::endl;
  });

  return 0;
}
