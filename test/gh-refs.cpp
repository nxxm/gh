#include <gh/refs.hxx>

#include <cstdlib>
#include <pre/json/to_json.hpp>
#include "utils.hpp"

int main(int argc, char** argv) {

  gh::get_refs("cpp-pre", "json", gh::filter_refs::ALL, [](gh::git_data::refs&& refs) {
    std::cout << pre::json::to_json(refs).dump(2) << std::endl;
  });

  gh::get_ref("boostorg", "fusion", "tags/boost-1.67.0", [](gh::git_data::ref_t&& tag) {
    std::cout << pre::json::to_json(tag).dump(2) << std::endl;
  });

  gh::get_refs("grpc", "grpc", gh::filter_refs::ALL, [](gh::git_data::refs&& refs) {
    std::cout << "Successfully retrieved " << refs.size() << " refs from github.com/grpc/grpc - which is a PASS" << std::endl;
  });

  auto auth = test_utils::get_auth();

  gh::get_refs("boostorg", "fusion", gh::filter_refs::TAGS, [](gh::git_data::refs&& tags) {
    std::cout << pre::json::to_json(tags).dump(2) << std::endl;
  }, auth);

  gh::get_ref("boostorg", "fusion", "tags/boost-1.67.0", [](gh::git_data::ref_t&& tag) {
    std::cout << pre::json::to_json(tag).dump(2) << std::endl;
  }, auth);

  gh::get_refs("grpc", "grpc", gh::filter_refs::ALL, [](gh::git_data::refs&& refs) {
    std::cout << "Successfully retrieved " << refs.size() << " refs from github.com/grpc/grpc with AUTHENTICATION - which is a PASS" << std::endl;
  }, auth);

  gh::get_tag("nlohmann", "json", "0ca0fe433eb70cea0d5761079c0c5b47b736565b", [](gh::git_data::tag_t&& annotated) {
    std::cout << "Successfully retrieved 0ca0fe433eb70cea0d5761079c0c5b47b736565b: " << annotated.tag << " pointing to commit " << annotated.object.sha << std::endl;
  }, auth);

  return 0;
}
