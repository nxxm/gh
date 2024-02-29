#include <gh/branches.hxx>

#include <cstdlib>
#include <pre/json/to_json.hpp>
#include "utils.hpp"

int main(int argc, char** argv) {

  gh::list_branches("boostorg", "fusion", [](gh::repos::branches&& branches) {
    std::cout << pre::json::to_json(branches).dump(2) << std::endl;
  });

  auto auth = test_utils::get_auth();

  gh::list_branches("daminetreg", "xxhr", [](gh::repos::branches&& branches) {
    std::cout << pre::json::to_json(branches).dump(2) << std::endl;
  }, auth);

  gh::list_branches("llvm", "llvm-project", [](gh::repos::branches&& branches) {
    assertm(branches.size() > 100, "We expect this to query multiple pages");

    // note on this:
    // the reason this aspect is tested is that the DTO sent by github does contain a "protected" property which gh:: remaps to "is_protected"
    // to circumnvent the use of the reserved C++ keyword in a struct/class.
    // 
    // We are expecting the llvm-project to have a few protected branches (e.g. at least main seems to be at
    // time of writing). 
    auto findit = std::find_if(branches.begin(), branches.end(), [](gh::repos::branch_t& b) { return b.is_protected; });    
    assertm(findit != branches.end(), "No protected branch found in repo / was expected");
  }, auth);

  return 0;
}
