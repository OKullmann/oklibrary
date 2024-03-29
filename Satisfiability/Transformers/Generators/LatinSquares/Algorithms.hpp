// Oliver Kullmann, 28.3.2024 (Swansea)
/* Copyright 2024 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  General algorithms (tools)

*/

#ifndef ALGORITHMS_SuuIcB9aND
#define ALGORITHMS_SuuIcB9aND

#include <vector>
#include <type_traits>
#include <istream>

#include <cassert>

#include <Numerics/NumTypes.hpp>
#include <Transformers/Generators/Bicliques/Algorithms.hpp>
#include <Solvers/Gecode/MOLS/OrthogonalArrays.hpp> // subsets(N, k)

namespace Algorithms {

  typedef FloatingPoint::UInt_t UInt_t;

  typedef std::vector<UInt_t> vector_t;
  static_assert(std::is_same_v<vector_t, OrthogonalArrays::ls_row_t>);
  static_assert(std::is_same_v<vector_t, OrthogonalArrays::oa_row_t>);
  typedef std::vector<vector_t> cubing_t;

  struct Cubing_t {
    UInt_t N;
    UInt_t m; // size of A
    cubing_t A;
  };
  Cubing_t read_queens_cubing(std::istream& in) {
    Cubing_t res{};
    res.A = FloatingPoint::read_gentable<UInt_t>(in);
    res.m = res.A.size();
    if (not res.A.empty()) res.N = res.A[0].size();
    return res;
  }
  
}

#endif
