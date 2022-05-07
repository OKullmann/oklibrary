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
#include "Verification.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.2.2",
        "7.5.2022",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Solvers/Gecode/MOLS/TestOrthogonalArrays.cpp",
        "GPL v3"};

  using namespace OrthogonalArrays;
  using namespace BasicLatinSquares;
  using namespace Verification;

  template <class X>
  constexpr bool eqp(const X& lhs, const X& rhs) noexcept {
    return lhs == rhs;
  }

  const soa_t example1 = {{0,0,0},{1,1,0},{0,1,1},{1,0,1}};
  const soa_t example2 = {
    {0,0,0,0,0}, {0,1,1,1,1}, {0,2,2,2,2}, {0,3,3,3,3},
    {1,0,3,1,2}, {1,1,2,0,3}, {1,2,1,3,0}, {1,3,0,2,1},
    {2,0,1,2,3}, {2,1,0,3,2}, {2,2,3,0,1}, {2,3,2,1,0},
    {3,0,2,3,1}, {3,1,3,2,0}, {3,2,0,1,3}, {3,3,1,0,2}};
  const oa_t example3 = {
    {0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2},
    {0,0,0,1,1,1,2,2,2,0,0,0,1,1,1,2,2,2,0,0,0,1,1,1,2,2,2},
    {0,1,2,0,1,2,0,1,2,0,1,2,0,1,2,0,1,2,0,1,2,0,1,2,0,1,2},
    {0,0,0,1,1,1,2,2,2,2,2,2,0,0,0,1,1,1,1,1,1,2,2,2,0,0,0},
    {0,1,2,1,2,0,2,0,1,0,1,2,1,2,0,2,0,1,0,1,2,1,2,0,2,0,1}};
  const soa_t example4 = {
    {0,0,0,0},{0,0,1,1},{0,1,0,1},{0,1,1,0},
    {1,0,0,1},{1,0,1,0},{1,1,0,0},{1,1,1,1}};

  template <size_t k>
  constexpr void test_allcombinations(const size_t N) {
    using OA = SOrthArr<k>;
    for (size_t i = 1; i <= N; ++i) {
      OA oa(oa2soa(allcombinations(N,k)));
      assert(oa.N == N);
      assert(oa.nblocks == std::pow(N,k));
      assert(oa.k == k);
      assert(oa.rep == 1);
      assert(oa.trows == oa.nblocks);
      assert(oa.valid());
    }
  }

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  {assert(maxval(oa_row_t{}) == 0);
   assert(maxval(oa_row_t{0,7,4}) == 7);
   assert(maxval(soa_t{}) == 0);
   assert(maxval(oa_t{}) == 0);
   assert(maxval(soa_t{{0,4,2},{7,2}}) == 7);
   assert(maxval(oa_t{{11,4,2},{7,2}}) == 11);
   assert(maxsize(soa_t{}) == 0);
   assert(maxsize(oa_t{}) == 0);
   assert(maxsize(soa_t{{0,4,2},{7,2}}) == 3);
   assert(maxsize(oa_t{{4,2},{7,2,100}}) == 3);
  }

  {assert(eqp(projection(oa_t{}, {}), {}));
   assert(eqp(projection(oa_t{{1,2,3},{4,5},{6,7,8},{}}, {}), {{},{},{},{}}));
   assert(eqp(projection(oa_t{{1,2,3},{4,5},{},{6,7,8}}, {0,0}),
              {{1,1},{4,4},{6,6}}));
   assert(eqp(projection(oa_t{{1,2,3},{4,5},{},{6,7,8}}, {2,1,0,1}),
              {{3,2,1,2},{8,7,6,7}}));
   assert(eqp(projection(oa_t{}, {}, true), {}));
   assert(eqp(projection(oa_t{{1,2,3},{4,5},{6,7,8},{}}, {}, true),
              {{},{},{},{}}));
   assert(eqp(projection(oa_t{{1,2,3},{4,5},{},{6,7,8}}, {0,0}, true),
              {{1,1},{4,4},{6,6}}));
   assert(eqp(projection(oa_t{{1,2,3},{4,5},{},{6,7,8}}, {2,1,0,1}, true),
              {{3,2,1,2},{8,7,6,7}}));
   assert(eqp(projection(oa_t{{6,7,8},{4,5},{},{1,2,3}}, {2,1,0,1}, true),
              {{3,2,1,2},{8,7,6,7}}));
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

  {assert(eqp(rarr2soa({}), {}));
   assert(eqp(rarr2soa({{{1,3},{5,7,9}}}), {{1,3,5,7,9}}));
   assert(eqp(rarr2soa({{{1,3},{5,7,9}},{{0,0},{}},{{1,4},{77}}}),
              {{1,3,5,7,9},{0,0},{1,4,77}}));
   assert(eqp(rarr2oa({{{1,3},{5,7,9}},{{0,0},{}},{{1,4},{77}}}),
              {{0,0},{1,3,5,7,9},{1,4,77}}));
  }
  {assert(eqp(oa2rarr<oa_t>({}), {}));
   assert(eqp(oa2rarr<oa_t>({{0}}), {}));
   assert(eqp(oa2rarr<oa_t>({{0,0}}), {{{0,0},{}}}));
   assert(eqp(oa2rarr<oa_t>({{1,3,5,7,9},{0,0},{1,4,77},{0,0,88}}),
              {{{1,3},{5,7,9}},{{0,0},{88}},{{1,4},{77}}}));
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
  {assert(eqp(oa2rarr(oa_t{{0,0,0}}), {{{0,0},{0}}}));
   assert(eqp(rarr2lls({{{0,0},{0}}}), {{{0}}}));
  }
  {RG::RandGen_t g({1,2,66});
   for (size_t N = 1; N <= 6; ++N)
     for (size_t i = 0; i < N*N; ++i) {
       const ls_t L1 = random_ls(N,g), L2 = random_ls(N,g);
       {const soa_t oa = lls2soa({L1});
        const SOrthArr2 O(oa);
        assert(O.valid());
        const auto L = oa2lls(oa);
        assert(L.size() == 1);
        assert(L[0] == L1);
       }
       {const soa_t oa = lls2soa({L1,L2});
        const SOrthArr2 O(oa);
        assert(O.valid() == orthogonal(L1,L2));
        const auto L = oa2lls(oa);
        assert(L.size() == 2);
        assert(L[0] == L1); assert(L[1] == L2);
       }
     }
  }

  {RG::RandGen_t g({9,11});
   for (size_t N = 1; N <= 6; ++N)
     for (size_t i = 0; i < N*N; ++i) {
       const ls_t L = random_ls(N,g);
       {const auto LL123 = project_lls({L}, {0,1,2});
        assert(LL123.size() == 1);
        const ls_t& L123 = LL123[0];
        assert(ls(L123));
        assert(L123 == L);}
       {const auto LL213 = project_lls({L}, {1,0,2});
        assert(LL213.size() == 1);
        const ls_t& L213 = LL213[0];
        assert(ls(L213));
        assert(L213 == transposition(L));}
       {const auto LL132 = project_lls({L}, {0,2,1});
        assert(LL132.size() == 1);
        const ls_t& L132 = LL132[0];
        assert(ls(L132));
        assert(L132 == rinverse(L));}
       {const auto LL312 = project_lls({L}, {2,0,1});
        assert(LL312.size() == 1);
        const ls_t& L312 = LL312[0];
        assert(ls(L312));
        assert(L312 == transposition(rinverse(L)));}
       {const auto LL321 = project_lls({L}, {2,1,0});
        assert(LL321.size() == 1);
        const ls_t& L321 = LL321[0];
        assert(ls(L321));
        assert(L321 == cinverse(L));}
       {const auto LL231 = project_lls({L}, {1,2,0});
        assert(LL231.size() == 1);
        const ls_t& L231 = LL231[0];
        assert(ls(L231));
        assert(L231 == transposition(cinverse(L)));}
     }
  }

  {assert(eqp(allcombinations(0,0), {}));
   assert(eqp(allcombinations(1,0), {}));
   assert(eqp(allcombinations(1,1), {{0}}));
   assert(eqp(allcombinations(3,1), {{0},{1},{2}}));
   assert(eqp(allcombinations(1,2), {{0,0}}));
   assert(eqp(allcombinations(2,2), {{0,0},{0,1},{1,0},{1,1}}));
   assert(eqp(allcombinations(3,2), {{0,0},{0,1},{0,2},{1,0},{1,1},{1,2},{2,0},{2,1},{2,2}}));
   assert(eqp(allcombinations(1,3), {{0,0,0}}));
   assert(eqp(allcombinations(2,3), {{0,0,0},{0,0,1},{0,1,0},{0,1,1},{1,0,0},{1,0,1},{1,1,0},{1,1,1}}));

   test_allcombinations<1>(10);
   test_allcombinations<2>(10);
   test_allcombinations<3>(8);
   test_allcombinations<4>(6);
   test_allcombinations<5>(5);
   test_allcombinations<6>(4);
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

  {const SOrthArr0 oa({});
   static_assert(SOrthArr0::str == 0);
   assert(oa.oa == soa_t{});
   assert(oa.N == 0);
   assert(oa.nblocks == 1);
   assert(oa.k == 0);
   assert(oa.rep == 0);
   assert(oa.trows == 0);
   assert(oa.valid());
  }
  {for (size_t k = 0; k < 10; ++k) {
     const SOrthArr0 oa({oa_row_t(k,0)});
     assert(oa.N == (k==0 ? 0 : 1));
     assert(oa.nblocks == 1);
     assert(oa.k == k);
     assert(oa.rep == 1);
     assert(oa.trows == 1);
     assert(oa.valid());
   }
  }
  {const SOrthArr0 oa({{0,0},{0,1},{0,2}});
   assert(oa.N == 3);
   assert(oa.nblocks == 1);
   assert(oa.k == 2);
   assert(oa.rep == 3);
   assert(oa.trows == 3);
   assert(oa.valid());
  }
  {const SOrthArr0 oa({{0,0},{0,1},{0}});
   assert(oa.N == 2);
   assert(oa.nblocks == 1);
   assert(oa.k == 2);
   assert(oa.rep == 3);
   assert(oa.trows == 3);
   assert(not oa.propfactors());
   assert(not oa.valid());
  }

  {const SOrthArr1 oa({});
   static_assert(SOrthArr1::str == 1);
   assert(oa.oa == soa_t{});
   assert(oa.N == 0);
   assert(oa.nblocks == 0);
   assert(oa.k == 0);
   assert(oa.rep == 0);
   assert(oa.trows == 0);
   assert(oa.valid());
  }
  {for (size_t k = 0; k < 10; ++k) {
     const SOrthArr1 oa({oa_row_t(k,0)});
     assert(oa.N == (k==0 ? 0 : 1));
     assert(oa.nblocks == (k==0 ? 0 : 1));
     assert(oa.k == k);
     assert(oa.rep == (k==0 ? 0 : 1));
     assert(oa.trows == 1);
     assert(oa.valid() == (k != 0));
    }
  }
  {for (size_t k = 1; k <= 10; ++k)
     for (size_t v = 1; v <= 10; ++v) {
       oa_t S(v);
       for (size_t i = 0; i < v; ++i) S[i] = oa_row_t(k, i);
       const SOrthArr1 oa(oa2soa(S));
       assert(oa.N == v);
       assert(oa.nblocks == v);
       assert(oa.k == k);
       assert(oa.rep == 1);
       assert(oa.trows == v);
       assert(oa.valid());
     }
  }
  {const SOrthArr1 oa(oa2soa(gtransposition(oa_t{{0,1,0,1},{0,1,1,0}})));
   assert(oa.N == 2);
   assert(oa.nblocks == 2);
   assert(oa.k == 2);
   assert(oa.rep == 2);
   assert(oa.trows == 4);
   assert(oa.valid());
  }
  {const SOrthArr1 oa(oa2soa(gtransposition(oa_t{{0,1,0,1},{0,1,0,1}})));
   assert(oa.N == 2);
   assert(oa.nblocks == 2);
   assert(oa.k == 2);
   assert(oa.rep == 1);
   assert(oa.trows == 2);
   assert(oa.valid());
  }
  {const SOrthArr1 oa(oa2soa(gtransposition(oa_t{{0,2}})));
   assert(oa.N == 3);
   assert(oa.nblocks == 3);
   assert(oa.k == 1);
   assert(oa.rep == 1);
   assert(oa.trows == 2);
   assert(not oa.valid());
  }
  {const SOrthArr1 oa(oa2soa(gtransposition(oa_t{{0,3,2,1},{3,2,1,0}})));
   assert(oa.N == 4);
   assert(oa.nblocks == 4);
   assert(oa.k == 2);
   assert(oa.rep == 1);
   assert(oa.trows == 4);
   assert(oa.valid());
  }
  {const SOrthArr1 oa(oa2soa(gtransposition(oa_t{{0,3,2,1},{3,2,1,3}})));
   assert(oa.N == 4);
   assert(oa.nblocks == 4);
   assert(oa.k == 2);
   assert(oa.rep == 1);
   assert(oa.trows == 4);
   assert(not oa.valid());
  }

  {const SOrthArr2 oa({});
   static_assert(SOrthArr2::str == 2);
   assert(oa.oa == soa_t{});
   assert(oa.N == 0);
   assert(oa.nblocks == 0);
   assert(oa.k == 0);
   assert(oa.rep == 0);
   assert(oa.trows == 0);
   assert(oa.valid());
  }
  {for (size_t k = 0; k < 10; ++k) {
     const SOrthArr2 oa({oa_row_t(k,0)});
     assert(oa.N == (k==0 ? 0 : 1));
     assert(oa.nblocks == (k==0 ? 0 : 1));
     assert(oa.k == k);
     assert(oa.rep == (k==0 ? 0 : 1));
     assert(oa.trows == 1);
     assert(oa.valid() == (k != 0));
   }
  }

  {const SOrthArr2 oa(example1);
   assert(oa.oa == example1);
   assert(oa.N == 2);
   assert(oa.nblocks == 4);
   assert(oa.k == 3);
   assert(oa.rep == 1);
   assert(oa.trows == 4);
   assert(oa.valid());
  }
  {const SOrthArr2 oa(example2);
   assert(oa.oa == example2);
   assert(oa.N == 4);
   assert(oa.nblocks == 16);
   assert(oa.k == 5);
   assert(oa.rep == 1);
   assert(oa.trows == 16);
   assert(oa.valid());
  }
  {const SOrthArr2 oa(oa2soa(gtransposition(example3)));
   assert(oa.N == 3);
   assert(oa.nblocks == 9);
   assert(oa.k == 5);
   assert(oa.rep == 3);
   assert(oa.trows == 27);
   assert(oa.valid());
  }
  {const SOrthArr3 oa(example4);
   assert(oa.oa == example4);
   assert(oa.N == 2);
   assert(oa.nblocks == 8);
   assert(oa.k == 4);
   assert(oa.rep == 1);
   assert(oa.trows == 8);
   assert(oa.valid());
  }

  {RG::RandGen_t g({1,2});
   for (size_t N = 1; N <= 6; ++N)
     for (size_t i = 0; i < N*N; ++i) {
       const ls_t L = random_ls(N,g);
       const soa_t oa = lls2soa({L});
       assert(oa.size() == N*N);
       const SOrthArr2 O(oa);
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
       const SOrthArr2 O2(lls2soa({L2}));
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
