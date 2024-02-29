
#include <gh/issues.hxx>

#include <cstdlib>
#include <pre/json/to_json.hpp>
#include "utils.hpp"

int main(int argc, char** argv) {
  gh::list_issues("nxxm", "nxxm", [](gh::repos::issues&& issues) {
    for (auto issue : issues) { 
      std::cout << pre::json::to_json(issue).dump(2) << std::endl;
    }
  }, "all");

  gh::get_issue("nxxm", "nxxm", 1, [](gh::repos::issue_t&& issue) {
    std::cout << pre::json::to_json(issue).dump(2) << std::endl;
  });

  auto auth = test_utils::get_auth();


  gh::get_issue("daminetreg", "lib-tftp-server", 1, [](gh::repos::issue_t&& issue) {
    std::cout << pre::json::to_json(issue).dump(2) << std::endl;
  }, auth);

  // making sure paging works
  gh::list_issues("boostorg", "process", [](gh::repos::issues&& issues) {
    assertm(issues.size() > 100, "Expecting at least 100 issues there (confirms we queried multiple pages)");
  }, "all", auth);

  return 0;
}
