// Oliver Kullmann, 11.5.2025 (Swansea)
/* Copyright 2025 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <iostream>
#include <ranges>
#include <vector>

#include <cassert>

#include <ProgramOptions/Environment.hpp>

#include "TestTools.hpp"
#include "Combinatorics.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.3",
        "20.5.2025",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Bicliques/TestCombinatorics.cpp",
        "GPL v3"};

  using namespace TestTools;
  using namespace Combinatorics;

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  {static_assert(eqp(makelist([](auto i){return AntiDiagonal{PaTy::full}.antid(i);}, 0, 30),
          {0,1,1,2,2,2,3,3,3,3,4,4,4,4,4,5,5,5,5,5,5,6,6,6,6,6,6,6,7,7,7}));
   static_assert(eqp(makelist([](auto i){return AntiDiagonal{PaTy::fullneq}.antid(i);}, 0, 30),
          {1,1,2,2,3,3,3,3,4,4,4,4,5,5,5,5,5,5,6,6,6,6,6,6,7,7,7,7,7,7,7}));
   static_assert(eqp(makelist([](auto i){return AntiDiagonal{PaTy::sorted}.antid(i);}, 0, 30),
          {0,1,2,2,3,3,4,4,4,5,5,5,6,6,6,6,7,7,7,7,8,8,8,8,8,9,9,9,9,9,10}));
   static_assert(eqp(makelist([](auto i){return AntiDiagonal{PaTy::sortedneq}.antid(i);}, 0, 30),
          {1,2,3,3,4,4,5,5,5,6,6,6,7,7,7,7,8,8,8,8,9,9,9,9,9,10,10,10,10,10,11}));
  }
  {using enum PaTy;
   for (const PaTy pt : {full, fullneq, sorted, sortedneq}) {
     const AntiDiagonal A{pt};
     for (UInt_t n = 0; n <= 100; ++n) {
       const UInt_t min = A.mintriv(n);
       assert(min == A.min(n));
       const UInt_t S = A.antid(n);
       assert(A.antid(min) == S);
       if (min > 0) assert(A.antid(min-1) < S);
     }
   }
  }
  {static_assert(eqp(makelist([](auto i){return AntiDiagonal{PaTy::full}(i);}, 0, 20),
          {{0,0},{0,1},{1,0},{0,2},{1,1},{2,0},{0,3},{1,2},{2,1},{3,0},{0,4},{1,3},{2,2},{3,1},{4,0},{0,5},{1,4},{2,3},{3,2},{4,1},{5,0}}));
   static_assert(eqp(makelist([](auto i){return AntiDiagonal{PaTy::fullneq}(i);}, 0, 20),
          {{0,1},{1,0},{0,2},{2,0},{0,3},{1,2},{2,1},{3,0},{0,4},{1,3},{3,1},{4,0},{0,5},{1,4},{2,3},{3,2},{4,1},{5,0},{0,6},{1,5},{2,4}}));
   static_assert(eqp(makelist([](auto i){return AntiDiagonal{PaTy::sorted}(i);}, 0, 20),
          {{0,0},{0,1},{0,2},{1,1},{0,3},{1,2},{0,4},{1,3},{2,2},{0,5},{1,4},{2,3},{0,6},{1,5},{2,4},{3,3},{0,7},{1,6},{2,5},{3,4},{0,8}}));
   static_assert(eqp(makelist([](auto i){return AntiDiagonal{PaTy::sortedneq}(i);}, 0, 20),
          {{0,1},{0,2},{0,3},{1,2},{0,4},{1,3},{0,5},{1,4},{2,3},{0,6},{1,5},{2,4},{0,7},{1,6},{2,5},{3,4},{0,8},{1,7},{2,6},{3,5},{0,9}}));
  }
  {using enum PaTy;
   for (const PaTy pt : {full, fullneq, sorted, sortedneq}) {
     const AntiDiagonal A{pt};
     for (UInt_t n = 0; n <= 10000; ++n)
       assert(A(A(n)) == n);
   }
  }

  {static_assert(eqp(makelist([](auto i){return CoLexicographic{PaTy::sorted}.secondc(i);}, 0, 30),
          {0,1,1,2,2,2,3,3,3,3,4,4,4,4,4,5,5,5,5,5,5,6,6,6,6,6,6,6,7,7,7}));
   static_assert(eqp(makelist([](auto i){return CoLexicographic{PaTy::sortedneq}.secondc(i);}, 0, 30),
          {1,2,2,3,3,3,4,4,4,4,5,5,5,5,5,6,6,6,6,6,6,7,7,7,7,7,7,7,8,8,8}));
  }
  {using enum PaTy;
   for (const PaTy pt : {sorted, sortedneq}) {
     const CoLexicographic C{pt};
     for (UInt_t n = 0; n <= 100; ++n) {
       const UInt_t min = C.mintriv(n);
       assert(min == C.min(n));
       const UInt_t v = C.secondc(n);
       assert(C.secondc(min) == v);
       if (min > 0) assert(C.secondc(min-1) < v);
     }
   }
  }
  {static_assert(eqp(makelist([](auto i){return CoLexicographic{PaTy::sorted}(i);}, 0, 20),
          {{0,0},{0,1},{1,1},{0,2},{1,2},{2,2},{0,3},{1,3},{2,3},{3,3},{0,4},{1,4},{2,4},{3,4},{4,4},{0,5},{1,5},{2,5},{3,5},{4,5},{5,5}}));
   static_assert(eqp(makelist([](auto i){return CoLexicographic{PaTy::sortedneq}(i);}, 0, 20),
          {{0,1},{0,2},{1,2},{0,3},{1,3},{2,3},{0,4},{1,4},{2,4},{3,4},{0,5},{1,5},{2,5},{3,5},{4,5},{0,6},{1,6},{2,6},{3,6},{4,6},{5,6}}));
  }
  {using enum PaTy;
   for (const PaTy pt : {sorted, sortedneq}) {
     const CoLexicographic C{pt};
     for (UInt_t n = 0; n <= 10000; ++n)
       assert(C(C(n)) == n);
   }
  }

}
