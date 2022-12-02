#include <gh/refs.hxx>

#include <cstdlib>
#include <pre/json/to_json.hpp>

int main(int argc, char** argv) {

  gh::get_commit("cpp-pre", "json", "29a0a7e9770aba4e04413c5245536bb0e2400285", [](gh::commits::commit_t&& commit) {
    std::cout << pre::json::to_json(refs).dump(2) << std::endl;
  });

  return 0;
}
