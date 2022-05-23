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

#include <sstream>
#include <string>

#include <cassert>

#ifndef CASES_tqVXGkU1YS
#define CASES_tqVXGkU1YS

#include <gecode/int.hh>
#include <gecode/search.hh>

#include <Numerics/FloatingPoint.hpp>

#include "Conditions.hpp"
#include "Parsing.hpp"
#include "PartialSquares.hpp"
#include "Encoding.hpp"

namespace Cases {

  using namespace Conditions;
  using namespace Parsing;
  using namespace PartialSquares;
  using namespace Encoding;

  namespace FP = FloatingPoint;

  EncCond encoding(const std::string condstr, const std::string psstr,
    const size_t N) noexcept {
    std::istringstream in_cond(condstr);
    std::istringstream in_ps(psstr);
    const AConditions ac = ReadAC()(in_cond);
    const PSquares ps = PSquares(N, in_ps);
    const EncCond enc(ac, ps);
    return enc;
  }

  struct Square {
  private:
    size_t N = 0;
  public:
    Square(const size_t N) : N(N) {}
    size_t solc() const noexcept { return FP::pow(N, N*N); }
    EncCond enc() const noexcept { return encoding("squares A\n", "", N); };
  };

  struct TrivialLatinSquare {
    // XXX
  };

}

#endif
