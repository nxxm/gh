#pragma once

#include <string>

namespace gh {

  /**
   * A structure to authenticate against github.
   */
  struct auth {
    //! 
    std::string user;

    //! password or token (in case your account is setup to use 2FA.
    std::string pass;
  };
}
