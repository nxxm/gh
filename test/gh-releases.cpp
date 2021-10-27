
#include <gh/releases.hxx>

#include <cstdlib>
#include <pre/json/to_json.hpp>

int main(int argc, char** argv) {
  gh::get_latest_release("daminetreg", "trommeli", [](gh::releases::release_t&& r) {
    std::cout << pre::json::to_json(r).dump(2) << std::endl;
  });

  gh::get_release_by_tag("tipi-build","cli","v0.0.19", [](gh::releases::release_t&& r) {
    std::cout << pre::json::to_json(r).dump(2) << std::endl;
  });

  gh::auth auth{ 
    std::getenv("GH_USER"),
    std::getenv("GH_PASS")
  };

  gh::get_latest_release("cpp-pre", "json", [](gh::releases::release_t&& r) {
    std::cout << pre::json::to_json(r).dump(2) << std::endl;
  }, auth);

  return 0;
}
