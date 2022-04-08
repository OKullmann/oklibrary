// Oliver Kullmann, 8.4.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Verifying solutions

*/

#ifndef VERIFICATION_CewvYixXoa
#define VERIFICATION_CewvYixXoa

#include <vector>
#include <map>
#include <utility>

#include <Transformers/Generators/Random/LatinSquares.hpp>

#include "Conditions.hpp"

namespace Verification {

  namespace RG = RandGen;
  namespace CD = Conditions;

  typedef Conditions::size_t size_t;

  typedef std::vector<size_t> ls_row_t;
  typedef std::vector<ls_row_t> ls_t;

  bool orthogonal(const ls_t& ls1, const ls_t& ls2) {
    const size_t N = ls1.size();
    if (ls2.size() != N) return false;
    typedef std::pair<size_t,size_t> p_t;
    typedef std::map<p_t, size_t> m_t;
    m_t m;
    for (size_t i = 0; i < N; ++i) {
      const ls_row_t& row1 = ls1[i];
      if (row1.size() != N) return false;
      const ls_row_t& row2 = ls2[i];
      if (row2.size() != N) return false;
      for (size_t j = 0; j < N; ++j)
        if (++m[{row1[j], row2[j]}] == 2) return false;
    }
    return true;
  }

}

#endif
