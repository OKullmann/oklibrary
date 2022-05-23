// Oleg Zaikin, 23.5.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

Test cases for lookahead-reduction and solvers.

BUG:

TODOS:

0. Add cases needed for testing lookahead-reduction.
  - Square.
  - Trivial (empty) Latin square.

*/

#include <cassert>

#ifndef CASES_tqVXGkU1YS
#define CASES_tqVXGkU1YS

#include <gecode/int.hh>
#include <gecode/search.hh>

#include <Numerics/FloatingPoint.hpp>

#include "Conditions.hpp"

namespace Cases {

  namespace FP = FloatingPoint;
  namespace CD = Conditions;

  using size_t = CD::size_t;

  struct Square {
  private:
    size_t N = 0;
  public:
    Square(const size_t N) : N(N) {}
    size_t solc() const noexcept { return FP::pow(N, N*N); }
  };

  struct TrivialLatinSquare {
    // XXX
  };

}

#endif
