#include <gh/refs.hxx>

#include <cstdlib>
#include <pre/json/to_json.hpp>

int main(int argc, char** argv) {

  gh::auth auth{ 
    std::getenv("GH_USER"),
    std::getenv("GH_PASS")
  };

  gh::get_refs(auth, "cpp-pre", "json", gh::filter_refs::ALL, [](gh::git_data::refs&& refs) {
    std::cout << pre::json::to_json(refs).dump(2) << std::endl;
  });

  gh::get_refs(auth, "boostorg", "fusion", gh::filter_refs::TAGS, [](gh::git_data::refs&& tags) {
    std::cout << pre::json::to_json(tags).dump(2) << std::endl;
  });

  gh::get_ref(auth, "boostorg", "fusion", "tags/boost-1.67.0", [](gh::git_data::ref_t&& tag) {
    std::cout << pre::json::to_json(tag).dump(2) << std::endl;
  });

  return 0;
}
