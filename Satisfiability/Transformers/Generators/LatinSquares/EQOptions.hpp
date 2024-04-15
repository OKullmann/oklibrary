// Oliver Kullmann, 1.4.2024 (Swansea)
/* Copyright 2024 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  - enum class OT (dimacs, cube_index, count_only)

*/

#ifndef EQOPTIONS_REO5LOHe6f
#define EQOPTIONS_REO5LOHe6f

#include <ProgramOptions/Environment.hpp>

namespace EQOptions {

  // Output-type:
  enum class OT {
    dimacs = 0,
    dimacs_ec0 = 1,
    cube_index = 2,
    count_only = 3
  };
  constexpr int OTsize = int(OT::count_only) + 1;
}

namespace Environment {
  template <> struct RegistrationPolicies<EQOptions::OT> {
    static constexpr const char* sname = "ot";
    static constexpr int size = EQOptions::OTsize;
    static constexpr std::array<const char*, size>
      string {"d", "dec0", "ci", "count"};
    static constexpr std::array<const char*, size>
      estring {"dimacs", "dimacs-EC0SAT", "cube-index", "count-only"};
  };
}

namespace EQOptions{

  std::ostream& operator <<(std::ostream& out, const OT ot) {
    return out << Environment::W2(ot);
  }

}

#endif
