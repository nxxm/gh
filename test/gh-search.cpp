#include <octoxxit/search.hxx>

int main(int argc, char** argv) {

  octoxxit::detail::http_get("api.github.com", "443", "/");

  return 0;
}
