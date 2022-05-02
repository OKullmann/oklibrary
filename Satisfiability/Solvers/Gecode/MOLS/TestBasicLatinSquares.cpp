// Oliver Kullmann, 1.5.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <iostream>

#include <cassert>

#include <ProgramOptions/Environment.hpp>

#include "BasicLatinSquares.hpp"
#include "Verification.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.0",
        "2.5.2022",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Solvers/Gecode/MOLS/TestBasicLatinSquares.cpp",
        "GPL v3"};

  using namespace BasicLatinSquares;
  namespace VR = Verification;

  template <class X>
  constexpr bool eqp(const X& lhs, const X& rhs) noexcept {
    return lhs == rhs;
  }

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  {assert(eqp(sls2bls({}), {}));
   assert(eqp(sls2bls({{0}}), {{0}}));
   assert(eqp(sls2bls({{0},{1,2}}), {{0},{1,2}}));
  }

  {assert(extract(PS::Cell(0)) == 0);
   assert(extract(PS::Cell(1)) == 0);
   assert(extract(PS::Cell({1,1,0,1})) == 2);
  }
  {assert(eqp(extract(PS::prow_t{}), {}));
   assert(eqp(extract(PS::prow_t{{{1,1,0,1}},{{1,0}}}), {2,1}));
  }
  {assert(eqp(extract(PS::psquare_t{}), {}));
   assert(eqp(extract(PS::psquare_t{{},{}}), {{},{}}));
   assert(eqp(extract(PS::psquare_t{
                        PS::prow_t{{{1,1,0,1}},{{1,0}}},
                        PS::prow_t{{{1,0,1,0}},{{1,1,0}}}}),
              {{2,1},{1,2}}));
  }

  {assert(alldiffelem(ls_row_t{}));
   assert(alldiffelem(ls_row_t{1}));
   assert(alldiffelem(ls_row_t{1,2}));
   assert(not alldiffelem(ls_row_t{1,2,1}));
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

  {assert(apply({{7}},{0,0}) == 7);
   assert(apply({{7,8,9},{0,1}},{1,1}) == 1);
  }
  {ls_t S{{3}};
   auto& e = applym(S, {0,0});
   assert(e == 3);
   e = 2;
   assert(S == ls_t{{2}});
  }

  {static_assert(refl(0,1) == 0);
   static_assert(refl(0,2) == 1);
   static_assert(refl(1,2) == 0);
  }
  {static_assert(eqp(refl_d({3,4}), {4,3}));
   static_assert(eqp(refl_ad({3,4}, 5), {0,1}));
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

  {ls_t S;
   transpositionm(S);
   assert(S == ls_t{});
   S = {{1,2,3},{4,5,6},{7,8,9}};
   transpositionm(S);
   assert(eqp(S, {{1,4,7},{2,5,8},{3,6,9}}));
  }
  {ls_t S;
   antitranspositionm(S);
   assert(S == ls_t{});
   S = {{1,2},{3,4},};
   antitranspositionm(S);
   assert(eqp(S, {{4,2},{3,1}}));
   S = {{1,2,3},{4,5,6},{7,8,9}};
   antitranspositionm(S);
   assert(eqp(S, {{9,6,3},{8,5,2},{7,4,1}}));
  }

  {const ls_t S{{1,2,3},{4,5,6},{7,8,9}};
   const ls_t St = transposition(S);
   const ls_t Sr = antitransposition(St);
   assert((Sr == ls_t{{9,8,7},{6,5,4},{3,2,1}}));
  }

  {assert(eqp(rproduct({}, {}), {}));
   assert(eqp(cproduct({}, {}), {}));
  }

  {RG::RandGen_t g({1234});
   for (size_t N = 1; N <= 6; ++N)
     for (size_t i = 0; i < 100; ++i) {
       const ls_t A = random_sq(N, g), B = random_sq(N, g),
         tA = transposition(A), tB = transposition(B) ;
       assert(sqprop(A)); assert(sqprop(B)); assert(sqprop(tA));
       assert(sqprop(tB));
       assert(cproduct(A,B) == transposition(rproduct(tA, tB)));
       assert(rproduct(A,B) == transposition(cproduct(tA, tB)));
       const ls_t C = random_rls(N, g), D = random_rls(N, g),
         iC = rinverse(C), iD = rinverse(D);
       assert(rls(C)); assert(rls(D)); assert(rls(iC)); assert(rls(iD));
       assert(rls(rproduct(C,D)));
       assert(rproduct(C, iC) == neutral_rls(N));
       assert(rproduct(iC, C) == neutral_rls(N));
       assert(rproduct(D, iD) == neutral_rls(N));
       assert(rproduct(iD, D) == neutral_rls(N));
       const ls_t E = random_cls(N, g), F = random_cls(N, g),
         iE = cinverse(E), iF = cinverse(F);
       assert(cls(E)); assert(cls(F)); assert(cls(iE)); assert(cls(iF));
       assert(cls(cproduct(E,F)));
       assert(cproduct(E, iE) == neutral_cls(N));
       assert(cproduct(iE, E) == neutral_cls(N));
       assert(cproduct(F, iF) == neutral_cls(N));
       assert(cproduct(iF, F) == neutral_cls(N));
     }
   for (size_t N = 1; N <= 5; ++N)
     for (size_t i = 0; i < 10; ++i) {
       const ls_t L = random_ls(N, g);
       assert(ls(L));
       assert(ls(rinverse(L))); assert(ls(cinverse(L)));
       const auto [S1, S2] = random_ortho_rls(N, g);
       assert(rls(S1)); assert(rls(S2));
       assert(VR::orthogonal(S1,S2));
       const auto [S3, S4] = random_ortho_cls(N, g);
       assert(cls(S3)); assert(cls(S4));
       assert(VR::orthogonal(S3,S4));
     }
  }

}
