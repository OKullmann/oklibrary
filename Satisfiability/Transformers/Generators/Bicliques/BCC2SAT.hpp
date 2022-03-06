// Oliver Kullmann, 6.3.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  General definitions regarding the SAT-translation of the bcc-problem

*/

#ifndef BCC2SAT_LsdBtFBZpI
#define BCC2SAT_LsdBtFBZpI

#include <string>

#include <Numerics/NumInOut.hpp>

#include "Bicliques2SAT.hpp"

namespace BCC2SAT {

  typedef Bicliques2SAT::var_t var_t;
  constexpr var_t default_B = 2;

  var_t read_var_t(const std::string& s, const var_t def) {
    if (s.empty()) return def;
    return FloatingPoint::toUInt(s);
  }

  enum class Error {
    missing_parameters = 1,
    bad_sb = 2,
    found_unsat = 20,
  };

}

#endif
