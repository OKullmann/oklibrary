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

#include "Combinatorics.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.0.5",
        "11.5.2025",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Bicliques/TestCombinatorics.cpp",
        "GPL v3"};

  using namespace Combinatorics;

  template <class X>
  constexpr bool eqp(const X& lhs, const X& rhs) noexcept {
    return lhs == rhs;
  }

  template<typename F>
  auto makelist(const F f, const UInt_t start, const UInt_t end) {
    return std::views::iota(start, end + 1) |
      std::views::transform(f) | std::ranges::to<std::vector>();
  }
}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  {assert(eqp(makelist([](UInt_t i){return AntiDiagonal{PaTy::full}.sum(i);}, 0, 30),
          {0,1,1,2,2,2,3,3,3,3,4,4,4,4,4,5,5,5,5,5,5,6,6,6,6,6,6,6,7,7,7}));
   assert(eqp(makelist([](UInt_t i){return AntiDiagonal{PaTy::fullneq}.sum(i);}, 0, 30),
          {1,1,2,2,3,3,3,3,4,4,4,4,5,5,5,5,5,5,6,6,6,6,6,6,7,7,7,7,7,7,7}));
   assert(eqp(makelist([](UInt_t i){return AntiDiagonal{PaTy::sorted}.sum(i);}, 0, 30),
          {0,1,2,2,3,3,4,4,4,5,5,5,6,6,6,6,7,7,7,7,8,8,8,8,8,9,9,9,9,9,10}));
   assert(eqp(makelist([](UInt_t i){return AntiDiagonal{PaTy::sortedneq}.sum(i);}, 0, 30),
          {1,2,3,3,4,4,5,5,5,6,6,6,7,7,7,7,8,8,8,8,9,9,9,9,9,10,10,10,10,10,11}));
  }

}
