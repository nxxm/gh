
#include <gh/issues.hxx>

#include <cstdlib>
#include <pre/json/to_json.hpp>

int main(int argc, char** argv) {
  gh::list_issues("nxxm", "nxxm", [](gh::repos::issues&& issues) {
    for (auto issue : issues) { 
      std::cout << pre::json::to_json(issue).dump(2) << std::endl;
    }
  }, "all");

  gh::get_issue("nxxm", "nxxm", 1, [](gh::repos::issue_t&& issue) {
    std::cout << pre::json::to_json(issue).dump(2) << std::endl;
  });

  gh::auth auth{ 
    std::getenv("GH_USER"),
    std::getenv("GH_PASS")
  };

  gh::get_issue("daminetreg", "lib-tftp-server", 1, [](gh::repos::issue_t&& issue) {
    std::cout << pre::json::to_json(issue).dump(2) << std::endl;
  }, auth);

  gh::list_issues("cpp-pre", "json", [](gh::repos::issues&& issues) {
    for (auto issue : issues) { 
      std::cout << pre::json::to_json(issue).dump(2) << std::endl;
    }
  },"all", auth);

  return 0;
}
