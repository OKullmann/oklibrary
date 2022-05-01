// Oliver Kullmann, 27.4.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* TODOS:

1. It should be possible to use constexpr and static_assert for GenLS;
   check once updating to gcc version gcc 11.3 (or later).

*/

#include <iostream>

#include <cassert>

#include <ProgramOptions/Environment.hpp>

#include "OrthogonalArrays.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.3",
        "1.5.2022",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Solvers/Gecode/MOLS/TestOrthogonalArrays.cpp",
        "GPL v3"};

  using namespace OrthogonalArrays;

  template <class X>
  constexpr bool eqp(const X& lhs, const X& rhs) noexcept {
    return lhs == rhs;
  }

  const oa_t example1 = {{0,0,0},{1,1,0},{0,1,1},{1,0,1}};
  const oa_t example2 = {
    {0,0,0,0,0}, {0,1,1,1,1}, {0,2,2,2,2}, {0,3,3,3,3},
    {1,0,3,1,2}, {1,1,2,0,3}, {1,2,1,3,0}, {1,3,0,2,1},
    {2,0,1,2,3}, {2,1,0,3,2}, {2,2,3,0,1}, {2,3,2,1,0},
    {3,0,2,3,1}, {3,1,3,2,0}, {3,2,0,1,3}, {3,3,1,0,2}};

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  {assert(maxval(ls_row_t{}) == 0);
   assert(maxval(ls_row_t{0,7,4}) == 7);
   assert(maxval(oa_t{}) == 0);
   assert(maxval(oa_t{{0,4,2},{7,2}}) == 7);
  }

  {// projection XXX

  }

  {assert(eqp(subsets(0,0), {{}}));
   assert(eqp(subsets(0,1), {}));
   assert(eqp(subsets(1,0), {{}}));
   assert(eqp(subsets(1,1), {{0}}));
   assert(eqp(subsets(1,2), {}));
   assert(eqp(subsets(2,0), {{}}));
   assert(eqp(subsets(2,1), {{0},{1}}));
   assert(eqp(subsets(2,2), {{0,1}}));
   assert(eqp(subsets(2,3), {}));
   assert(eqp(subsets(3,0), {{}}));
   assert(eqp(subsets(3,1), {{0},{1},{2}}));
   assert(eqp(subsets(3,2), {{0,1},{0,2},{1,2}}));
   assert(eqp(subsets(3,3), {{0,1,2}}));
   assert(eqp(subsets(3,4), {}));
   assert(eqp(subsets(4,0), {{}}));
   assert(eqp(subsets(4,1), {{0},{1},{2},{3}}));
   assert(eqp(subsets(4,2), {{0,1},{0,2},{1,2},{0,3},{1,3},{2,3}}));
   assert(eqp(subsets(4,3), {{0,1,2},{0,1,3},{0,2,3},{1,2,3}}));
   assert(eqp(subsets(4,4), {{0,1,2,3}}));
   assert(eqp(subsets(4,5), {}));
   {bool caught = false;
    try { subsets(68,34); }
    catch(std::runtime_error&) { caught = true; }
    assert(caught);
   }
  }

  {static_assert(eqp(rest<1>({0}), {}));
   static_assert(eqp(rest<2>({0,1}), {1}));
   static_assert(eqp(rest<3>({0,1,2}), {1,2}));
  }

  {assert(GenLS<0>::create(3) == 3);
   assert(GenLS<1>::create(5) == ls_row_t(5,5));
   assert(GenLS<2>::create(7) == ls_t(7,ls_row_t(7,7)));
   assert(GenLS<3>::create(4) == GLS<3>(4, ls_t(4,ls_row_t(4,4))));
  }

  {typedef GenLS<0> G;
   static_assert(G::apply(G::create(77), {}) == 77);
  }
  {typedef GenLS<1> G;
   const auto S = G::create(33);
   assert(G::apply(S, index_t<0>{}) == S);
   assert(G::apply(S, index_t<1>{2}) == 33);
  }
  {typedef GenLS<2> G;
   const auto S = G::create(5);
   assert(G::apply(S, index_t<0>{}) == S);
   assert(G::apply(S, index_t<1>{2}) == ls_row_t(5,5));
   assert(G::apply(S, index_t<2>{2,3}) == 5);
  }
  {typedef GenLS<3> G;
   const auto S = G::create(4);
   assert(G::apply(S, index_t<0>{}) == S);
   assert(G::apply(S, index_t<1>{2}) == GenLS<2>::create(4));
   assert(G::apply(S, index_t<2>{2,3}) == ls_row_t(4,4));
   assert(G::apply(S, index_t<3>{2,3,0}) == 4);
  }

  {const OrthArr2 oa(example1);
   static_assert(OrthArr2::str == 2);
   assert(oa.oa == example1);
   assert(oa.N == 2);
   assert(oa.nblocks == 4);
   assert(oa.k == 3);
   assert(oa.rep == 1);
   assert(oa.trows == 4);
   assert(oa.valid());
  }
  {const OrthArr2 oa(example2);
   assert(oa.oa == example2);
   assert(oa.N == 4);
   assert(oa.nblocks == 16);
   assert(oa.k == 5);
   assert(oa.rep == 1);
   assert(oa.trows == 16);
   assert(oa.valid());
  }

}
