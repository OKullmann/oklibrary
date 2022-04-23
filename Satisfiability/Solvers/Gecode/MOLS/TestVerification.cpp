// Oliver Kullmann, 8.4.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <iostream>

#include <cassert>

#include <ProgramOptions/Environment.hpp>

#include "Verification.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.6",
        "23.4.2022",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/OKlib-MOLS/blob/master/Satisfiability/Solvers/Gecode/MOLS/TestVerification.cpp",
        "GPL v3"};

  using namespace Verification;

  template <class X>
  constexpr bool eqp(const X& lhs, const X& rhs) noexcept {
    return lhs == rhs;
  }
}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  {assert(alldiffelem(ls_row_t{}));
   assert(alldiffelem(ls_row_t{1}));
   assert(alldiffelem(ls_row_t{1,2}));
   assert(not alldiffelem(ls_row_t{1,2,1}));
  }

  {assert(constant(ls_row_t{}));
   assert(constant(ls_row_t{1}));
   assert(not constant(ls_row_t{1,1,2}));
   assert(constant(ls_row_t{1,1,1}));
  }

  {assert(alldiffrows({}));
   assert(alldiffrows({{0,5},{4,2},{}}));
   assert(not alldiffrows({{0,5},{4,2},{},{1,0,2,3,0}}));
  }

  {assert(alldiffcols({}));
   assert(alldiffcols({{0,0}}));
   assert(not alldiffcols({{0,1,1},{1,0,1}}));
   assert(alldiffcols({{},{0,0},{1},{2,1},{},{3},{4,2,0}}));
   assert(not alldiffcols({{},{0,0},{1},{2,1},{},{3},{4,2,0},{5,3,0}}));
  }

  {assert(alldiffsq({}));
   assert(not alldiffsq({{0,0}}));
   assert(alldiffsq({{},{0,1},{1},{2,0},{},{3},{4,2,0}}));
  }

  {assert(sqshape({}));
   assert(sqshape({{0}}));
   assert(not sqshape({{0,0}}));
   assert(not sqshape({{0},{0}}));
  }
  {assert(sqval({}));
   assert(sqval({{0}}));
   assert(sqval({{0,0}}));
   assert(sqval({{0},{0}}));
   assert(not sqval({{1}}));
   assert(not sqval({{0,1}}));
   assert(sqval({{0},{1}}));
   assert(not sqval({{0},{2}}));
  }
  {assert(sqprop({}));
   assert(sqprop({{0}}));
   assert(sqprop({{0,0},{1,1}}));
  }

  {assert(eqp(extract_diagonal({}), {}));
   assert(eqp(extract_diagonal({{1}}), {1}));
   assert(eqp(extract_diagonal({{1},{},{3,4,5}}), {1,5}));
   assert(eqp(extract_diagonal({{1,2,3},{4,5,6},{7,8,9}}), {1,5,9}));
  }
  {assert(eqp(extract_antidiagonal({}), {}));
   assert(eqp(extract_antidiagonal({{1}}), {1}));
   assert(eqp(extract_antidiagonal({{1},{},{3,4,5}}), {3}));
   assert(eqp(extract_antidiagonal({{1,2,3},{4,5,6},{7,8,9}}), {3,5,7}));
  }

  {assert(diagonal({}));
   assert(diagonal({{0}}));
   assert(diagonal({{0,0}}));
   assert(diagonal({{0},{0,1}}));
   assert(not diagonal({{0},{1,0}}));
  }
  {assert(antidiagonal({}));
   assert(antidiagonal({{0}}));
   assert(antidiagonal({{0,0}}));
   assert(antidiagonal({{0},{0,1}}));
   assert(antidiagonal({{0},{1,0}}));
   assert(antidiagonal({{0,0},{1}}));
   assert(not antidiagonal({{0,0},{0}}));
  }

  {assert(unipotent({}));
   assert(unipotent({{0}}));
   assert(unipotent({{0,0}}));
   assert(unipotent({{0},{1,0}}));
   assert(not unipotent({{0},{0,1}}));
  }
  {assert(antiunipotent({}));
   assert(antiunipotent({{0}}));
   assert(antiunipotent({{0,0}}));
   assert(antiunipotent({{0},{0,1}}));
   assert(antiunipotent({{0},{1,0}}));
   assert(not antiunipotent({{0,1},{0}}));
   assert(antiunipotent({{1,0},{0}}));
  }

  {assert(idempotent({}));
   assert(idempotent({{0}}));
   assert(not idempotent({{1}}));
   assert(idempotent({{0,0}}));
   assert(idempotent({{0},{0,1}}));
   assert(not idempotent({{0},{1,0}}));
  }
  {assert(antiidempotent({}));
   assert(antiidempotent({{0}}));
   assert(antiidempotent({{0,0}}));
   assert(not antiidempotent({{0},{0,1}}));
   assert(not antiidempotent({{0},{1,0}}));
   assert(not antiidempotent({{0,1},{0}}));
   assert(not antiidempotent({{1,0},{0}}));
   assert(antiidempotent({{1,0},{1}}));
  }

  {assert(not rreduced({}));
   assert(rreduced({{}}));
   assert(rreduced({{0,1,2}}));
   assert(rreduced({{0,1,2},{},{4,4}}));
   assert(not rreduced({{0,1,3}}));
  }
  {assert(creduced({}));
   assert(not creduced({{}}));
   assert(creduced({{0,1,2}}));
   assert(creduced({{0,1,2},{1},{2,4}}));
   assert(not creduced({{0,1,3},{2}}));
  }
  {assert(not reduced({}));
   assert(not reduced({{}}));
   assert(reduced({{0,1,2}}));
   assert(reduced({{0,1,2},{1},{2,4}}));
   assert(not reduced({{0,1,3}}));
   assert(not reduced({{0,1,2},{2}}));
  }

  {assert(rls({}));
   assert(not rls({{0},{0}}));
   assert(rls({{0,1,2},{0,2,1},{0,1,2}}));
   assert(not rls({{0,1,2},{0,2,1},{0,1,3}}));
  }
  {assert(cls({}));
   assert(not cls({{0},{1}}));
   assert(cls({{0,0,0},{1,1,1},{2,2,2}}));
   assert(not cls({{0,0,0},{1,1,1},{2,2,3}}));
  }
  {assert(ls({}));
   assert(ls({{0}}));
   assert(ls({{0,1},{1,0}}));
   assert(not ls({{0,1},{1,1}}));
   assert(not ls({{0,1},{0,1}}));
  }

  {assert(orthogonal({},{}));
   assert(orthogonal({{0}},{{0}}));
   assert(orthogonal({{0,1},{1,0}},{{0,1},{0,1}}));
   assert(not orthogonal({{0,1},{1,0}},{{1,0},{0,1}}));
  }

  {const ls_t A = {
      {0,1,2,3,4,5,6},
      {2,3,6,5,0,4,1},
      {4,2,5,0,1,6,3},
      {6,4,1,2,3,0,5},
      {1,5,3,4,6,2,0},
      {3,0,4,6,5,1,2},
      {5,6,0,1,2,3,4}};
    assert(ls(A));
    assert(rreduced(A));
    assert(diagonal(A));
    assert(antidiagonal(A));
    const ls_t tA = {
      {0,2,4,6,1,3,5},
      {1,3,2,4,5,0,6},
      {2,6,5,1,3,4,0},
      {3,5,0,2,4,6,1},
      {4,0,1,3,6,5,2},
      {5,4,6,0,2,1,3},
      {6,1,3,5,0,2,4}};
    assert(ls(tA));
    assert(transposition(A) == tA);

    assert(orthogonal(A,tA));
    assert(ls(rproduct(A,rinverse(tA))));
    assert(ls(rproduct(tA,rinverse(A))));

    assert(not orthogonal(rinverse(A), rinverse(tA)));
    const ls_t L = rproduct(rinverse(tA), A);
    assert(rproduct(tA, L) == A);
    assert(rls(L));
    assert(not cls(L));
  }
}
