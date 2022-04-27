// Oliver Kullmann, 27.4.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  Tools for handling orthogonal arrays

  For basic definitions see
  https://en.wikipedia.org/wiki/Orthogonal_array

  We consider here only simple orthogonal arrays (no repeated rows).

*/

#ifndef ORTHOGONALARRAYS_TgDVWG9CBQ
#define ORTHOGONALARRAYS_TgDVWG9CBQ

#include <vector>
#include <set>

#include <cassert>
#include <cmath>

#include "Conditions.hpp"

namespace OrthogonalArrays {

  typedef Conditions::size_t size_t;

  // As in Verification.hpp:
  typedef std::vector<size_t> ls_row_t;
  typedef std::vector<ls_row_t> ls_t;


  using oa_row_t = ls_row_t; // the rows of the orthogonal array
  typedef std::set<oa_row_t> oa_t; // underlying concrete alias type of oa's


  struct OrthArr {
    oa_t oa;

    const size_t N; // the number of "levels" or "values": 0, ..., N-1
    const size_t k; // number of "factors" (columns)

    const size_t str = 2; // strength
    const size_t rep = 1; // repetitions ("index")

    const size_t trows = rep * std::pow(N, str);
    // total number of rows ("number of experimental runs")

    OrthArr(const size_t N, const size_t k) noexcept : N(N), k(k) {}
    OrthArr(const size_t N, const size_t k, const size_t s, const size_t r)
      noexcept : N(N), k(k), str(s), rep(r) {}

  };

}

#endif
