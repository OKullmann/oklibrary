// Oliver Kullmann, 19.2.2021 (Swansea)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#ifndef MATCHING_GQwPciLmPn
#define MATCHING_GQwPciLmPn

#include <ProgramOptions/Environment.hpp>

namespace Matching {

  enum class MatO {lines=0, full=1, name=2}; // MUST correspond to Registration
  constexpr int MatOsize = 3;
  constexpr MatO default_mato = MatO::lines;
}
namespace Environment {
  template <>
  struct RegistrationPolicies<Matching::MatO> {
    static constexpr int size = Matching::MatOsize;
    static constexpr std::array<const char*, size> string
    {"lm", "fm", "ne"};
  };
}
namespace Matching {
  std::ostream& operator <<(std::ostream& out, const MatO m) {
    switch (m) {
    case MatO::lines : return out << "line-matching";
    case MatO::full : return out << "file-matching";
    default : return out << "name-encoded";}
  }

  enum class Error {
    pnumber = 1,
    file_open = 2,
    file_read = 3,
    eof = 4,
    empty_pattern_file = 5,
    regular_expression = 6,
    number_lines = 7,
    mismatch = 8,
    option = 9,
    encoding = 10,
  };

}

#endif
