
#include <gh/repos.hxx>

#include <cstdlib>
#include <pre/json/to_json.hpp>

int main(int argc, char** argv) {
  gh::repos_get("daminetreg", "xxhr", [](gh::repos::repository_t&& r) {
    std::cout << pre::json::to_json(r).dump(2) << std::endl;
  });


  gh::auth auth{ 
    std::getenv("GH_USER"),
    std::getenv("GH_PASS")
  };

  gh::repos_get("cpp-pre", "json", [](gh::repos::repository_t&& r) {
    std::cout << pre::json::to_json(r).dump(2) << std::endl;
  }, auth);


  gh::repos_get("lazytiger", "gumbo-query", [](gh::repos::repository_t&& r) {
    std::cout << pre::json::to_json(r).dump(2) << std::endl;
  }, auth);

  return 0;
}
