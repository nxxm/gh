
#include <gh/releases.hxx>
#include <cassert>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream> 
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

  gh::get_latest_release("tipi-build","cli", [](gh::releases::release_t&& r) {
    std::cout << pre::json::to_json(r).dump(2) << std::endl;
  }, auth);

  // testing the release creation
  std::string repo_owner = std::getenv("GH_RELEASE_TEST_REPO_OWNER");
  std::string repo_name = std::getenv("GH_RELEASE_TEST_REPO_NAME");

  std::stringstream release_tag_ss; 
  release_tag_ss << "test-" << std::time(0);
  std::string release_tag = release_tag_ss.str();

  gh::releases::create_release_t release_data{
    .tag_name = release_tag, 
    .target_commitish = "main",
    .name = "A test release",
    .body = "This is a test release!",
    .draft = true,
    .prerelease = false,
    .generate_release_notes = false
  };

  gh::create_release("nxxm", "test-gh-client-release", release_data, 
    [](gh::releases::release_t&& r) {
      assertm(r.tag_name == release_data.tag_name, "Release tag-name shall be the as set");
      assertm(r.name == release_data.name, "Release name shall be the as set");
      assertm(r.draft == release_data.draft, "Release draft status shall be the as set");
      std::cout << pre::json::to_json(r).dump(2) << std::endl;
    }, 
    auth);

  return 0;
}
