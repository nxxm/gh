
#include <gh/repos.hxx>

#include <cassert>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <pre/json/to_json.hpp>
#include "utils.hpp"

int main(int argc, char** argv) {
  gh::repos_get("daminetreg", "xxhr", [](gh::repos::repository_t&& r) {
    std::cout << pre::json::to_json(r).dump(2) << std::endl;
  });

  auto auth = test_utils::get_auth();

  gh::repos_get("cpp-pre", "json", [](gh::repos::repository_t&& r) {
    std::cout << pre::json::to_json(r).dump(2) << std::endl;
  }, auth);


  gh::repos_get("lazytiger", "gumbo-query", [](gh::repos::repository_t&& r) {
    std::cout << pre::json::to_json(r).dump(2) << std::endl;
  }, auth);


  gh::repos_get("nxxm", "test-gh-client-release", [](gh::repos::repository_t&& r) {
    assertm(r.is_private == true, "Repo is expected to be private");
  }, auth);

  {
    std::stringstream repo_name_ss; 
    repo_name_ss << "gh-api-test-" << std::time(0);
    std::string repo_name = repo_name_ss.str();

    gh::repos::create_repository_t repo_data{
      .name = repo_name, 
      .is_private = true
    };

    
    gh::create_repo(repo_data, 
      [&auth](gh::repos::repository_t&& r) {
        assertm(r.is_private == true, "Repo is expected to be private");
        std::cout << pre::json::to_json(r).dump(2) << std::endl;

        gh::delete_repo(r.owner.login, r.name, auth);
      },
      auth);

    gh::create_repo("tipibuild", repo_data, 
      [&auth](gh::repos::repository_t&& r) {
        assertm(r.is_private == true, "Repo is expected to be private");
        std::cout << pre::json::to_json(r).dump(2) << std::endl;

        gh::delete_repo(r.owner.login, r.name, auth);
      },
      auth);

  }




  return 0;
}
