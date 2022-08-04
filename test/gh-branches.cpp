#include <gh/branches.hxx>

#include <cstdlib>
#include <pre/json/to_json.hpp>
#include "utils.hpp"

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

  // a repo with branch protections
  std::string repo_owner = std::getenv("GH_RELEASE_TEST_REPO_OWNER");
  std::string repo_name = std::getenv("GH_RELEASE_TEST_REPO_NAME");

  gh::list_branches(repo_owner, repo_name, [&](gh::repos::branches&& branches) {
    
    auto findit = std::find_if(branches.begin(), branches.end(), [](gh::repos::branch_t& b) { return b.is_protected; });
    
    assertm(findit != branches.end(), "No protected branch found in repo / was expected");
  }, auth);

  return 0;
}
