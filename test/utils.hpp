#pragma once
#include <cassert>
#include <gh/auth.hxx>

#define assertm(exp, msg) assert(((void)msg, exp))


namespace test_utils {

  //! \brief get the required information from the environment variables GH_USER and GH_PASS
  inline gh::auth get_auth() {

    auto env_user = std::getenv("GH_USER");
    auto env_pass = std::getenv("GH_PASS");

    if(!env_user || !env_pass) {
      std::cout << "This test needs the environment variables GH_USER and GH_PASS to be defined and holding valid github.com credentials\nExiting" << std::endl;
      std::exit(1);
    }

    return gh::auth{ env_user, env_pass };
  }
}