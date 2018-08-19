#include <gh/refs.hxx>
#include <pre/json/to_json.hpp>

int main(int argc, char** argv) {

  gh::get_refs("cpp-pre", "json", gh::filter_refs::ALL, [](gh::git_data::refs&& refs) {
    std::cout << pre::json::to_json(refs).dump(2) << std::endl;
  });

  gh::get_refs("boostorg", "fusion", gh::filter_refs::TAGS, [](gh::git_data::refs&& tags) {
    std::cout << pre::json::to_json(tags).dump(2) << std::endl;
  });

  gh::get_ref("boostorg", "fusion", "tags/boost-1.67.0", [](gh::git_data::ref_t&& tag) {
    std::cout << pre::json::to_json(tag).dump(2) << std::endl;
  });

  return 0;
}
