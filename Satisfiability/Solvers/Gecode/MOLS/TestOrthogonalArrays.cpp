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
#include "BasicLatinSquares.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.5",
        "3.5.2022",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Solvers/Gecode/MOLS/TestOrthogonalArrays.cpp",
        "GPL v3"};

  using namespace OrthogonalArrays;
  using namespace BasicLatinSquares;

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
  const ls_t example3 = {
    {0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2},
    {0,0,0,1,1,1,2,2,2,0,0,0,1,1,1,2,2,2,0,0,0,1,1,1,2,2,2},
    {0,1,2,0,1,2,0,1,2,0,1,2,0,1,2,0,1,2,0,1,2,0,1,2,0,1,2},
    {0,0,0,1,1,1,2,2,2,2,2,2,0,0,0,1,1,1,1,1,1,2,2,2,0,0,0},
    {0,1,2,1,2,0,2,0,1,0,1,2,1,2,0,2,0,1,0,1,2,1,2,0,2,0,1}};

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

  {assert(eqp(rarr2oa({}), {}));
   assert(eqp(rarr2oa({{{1,3},{5,7,9}}}), {{1,3,5,7,9}}));
   assert(eqp(rarr2oa({{{1,3},{5,7,9}},{{0,0},{}},{{1,4},{77}}}),
              {{1,3,5,7,9},{0,0},{1,4,77}}));
  }
  {assert(eqp(lls2rarr({}), {}));
   assert(eqp(lls2rarr({{},{}}), {}));
   assert(eqp(lls2rarr({{{0,1},{2,3,4}},{}}),
              {{{0,0},{0}},{{0,1},{1}},{{1,0},{2}},{{1,1},{3}},{{1,2},{4}}}));
   assert(eqp(lls2rarr({{{0,1},{2,3,4}},{{5},{6,7},{8}}}),
              {{{0,0},{0,5}},{{0,1},{1}},
               {{1,0},{2,6}},{{1,1},{3,7}},{{1,2},{4}},
               {{2,0},{8}}}));
   assert(eqp(lls2rarr({{{0,1},{2,3,4}},{{5},{6,7},{8}},{{9,10,11}}}),
              {{{0,0},{0,5,9}},{{0,1},{1,10}},{{0,2},{11}},
               {{1,0},{2,6}},{{1,1},{3,7}},{{1,2},{4}},
               {{2,0},{8}}}));
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
  {const OrthArr2 oa(ls2oa(gtransposition(example3)));
   assert(oa.N == 3);
   assert(oa.nblocks == 9);
   assert(oa.k == 5);
   assert(oa.rep == 3);
   assert(oa.trows == 27);
   assert(oa.valid());
  }

  {RG::RandGen_t g({1,2});
   for (size_t N = 1; N <= 6; ++N)
     for (size_t i = 0; i < N*N; ++i) {
       const ls_t L = random_ls(N,g);
       const oa_t oa = lls2oa({L});
       assert(oa.size() == N*N);
       const OrthArr2 O(oa);
       assert(O.oa == oa);
       assert(O.str == 2);
       assert(O.N == N);
       assert(O.k == 3);
       assert(O.rep == 1);
       assert(O.nblocks == N*N);
       assert(O.valid());

       ls_t L2(L);
       if (L2[0][0] == 0) L2[0][0] = 1;
       else L2[0][0] = 0;
       assert(not rls(L2)); assert(not cls(L2));
       const OrthArr2 O2(lls2oa({L2}));
       assert(O2.str == 2);
       const size_t nN = std::max(size_t(2),N);
       assert(O2.N == nN);
       assert(O2.k == 3);
       assert(O2.rep == 1);
       assert(O2.nblocks == nN*nN);
       assert(not O2.valid());
     }
  }

}
