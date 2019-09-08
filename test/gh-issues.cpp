
#include <gh/issues.hxx>

#include <cstdlib>
#include <pre/json/to_json.hpp>

int main(int argc, char** argv) {
  gh::list_issues("nxxm", "nxxm", [](gh::repos::issues&& issues) {
    for (auto issue : issues) { 
      std::cout << pre::json::to_json(issue).dump(2) << std::endl;
    }
  }, "open");


  gh::auth auth{ 
    std::getenv("GH_USER"),
    std::getenv("GH_PASS")
  };

  gh::list_issues("cpp-pre", "json", [](gh::repos::issues&& issues) {
    for (auto issue : issues) { 
      std::cout << pre::json::to_json(issue).dump(2) << std::endl;
    }
  },"all", auth);

  return 0;
}