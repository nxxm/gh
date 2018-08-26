
#include <gh/repos.hxx>

#include <cstdlib>
#include <pre/json/to_json.hpp>

int main(int argc, char** argv) {

  gh::auth auth{ 
    std::getenv("GH_USER"),
    std::getenv("GH_PASS")
  };

  gh::repos_get(auth, "daminetreg", "xxhr", [](gh::repos::repository_t&& r) {
    std::cout << pre::json::to_json(r).dump(2) << std::endl;
  });

  gh::repos_get(auth, "cpp-pre", "json", [](gh::repos::repository_t&& r) {
    std::cout << pre::json::to_json(r).dump(2) << std::endl;
  });

  gh::repos_get(auth, "lazytiger", "gumbo-query", [](gh::repos::repository_t&& r) {
    std::cout << pre::json::to_json(r).dump(2) << std::endl;
  });

  return 0;
}
