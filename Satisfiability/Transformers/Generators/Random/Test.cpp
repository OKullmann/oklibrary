// Oliver Kullmann, 2.3.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <vector>
#include <utility>
#include <iostream>

#include <cassert>
#include <cmath>

#include <ProgramOptions/Environment.hpp>
#include <Numerics/FloatingPoint.hpp>

#include "Distributions.hpp"
#include "Algorithms.hpp"
#include "Tests.hpp"
#include "ClauseSets.hpp"

namespace {

  const Environment::ProgramInfo pi{
        "0.2.12",
        "18.4.2019",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/Test.cpp",
        "GPL v3"};

  using namespace RandGen;

  // The numerical values as specified by the C++ standard:
  constexpr gen_uint_t specval = 9981545732273789042ULL;
  constexpr seed_t specseed = 5489u;

  // The ith generated values using the empty seed-sequence:
  constexpr gen_uint_t valempty_1 = 835052665647855778ULL;
  constexpr gen_uint_t valempty_10000 = 12990417185246102803ULL;
  // For the uniform distribution in [0,2^50):
  constexpr gen_uint_t valempty_2p50_10000 = valempty_10000 / iexp2(64-50);
  static_assert(valempty_2p50_10000 == 792872142654181ULL);
}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, pi, argc, argv))
  return 0;

  randgen_t g;
  g.discard(9999 + 1 + 3 + 2);

  const std::vector v0{1,2,3,4,5,6,7};
  std::vector v(v0);
  randgen_t gcopy(g);
  shuffle(v.begin(), v.end(), randgen_t(g)); // second form
  const std::vector vs{5,2,1,7,6,4,3};
  assert(v == vs);
  assert(g == gcopy);
  v = v0;
  shuffle(v.begin(), v.end(), g); // first form
  assert(v == vs);
  assert(g != gcopy);
  v = v0;
  shuffle(v.begin(), v.end(), gcopy); // first form
  assert(v == vs);
  assert(g == gcopy);
  shuffle(v.begin(), v.end(), g); // first form
  const std::vector vs2{6,4,7,5,2,1,3};
  assert(v == vs2);
  assert(g != gcopy);
  gcopy.discard(6);
  assert(g == gcopy); // Remark: not guaranteed in general, due to possible discarded calls of g() in UniformRange.
  shuffle(v.begin(), v.end(), std::move(g)); // second form
  const std::vector vs3{3,6,4,5,1,2,7};
  assert(v == vs3);
  assert(g != gcopy);
  v = vs2;
  shuffle(v.begin(), v.end(), std::move(gcopy)); // second form
  assert(v == vs3);
  g.seed(); gcopy.seed();
  assert(g == gcopy);
  assert(g == randgen_t(specseed));

  {Count_true c;
   assert(*c == 0);
   c(true); assert(*c == 1);
   c(false); assert(*c == 1);
  }

  {CountRuns c(true);
   using res_t = CountRuns::res_t;
   assert((*c == res_t{1,1}));
   c(true);
   assert((*c == res_t{1,2}));
   c(true);
   assert((*c == res_t{1,3}));
   c(false);
   assert((*c == res_t{2,3}));
   c(false);
   assert((*c == res_t{2,3}));
   c(true);
   assert((*c == res_t{3,4}));
  }

  {LongestRun lr(true); // 1
   using res_t = LongestRun::res_t;
   assert((*lr == res_t{1,0,1,1}));
   lr(true); // 11
   assert((*lr == res_t{2,0,1,2}));
   lr(true); // 111
   assert((*lr == res_t{3,0,1,3}));
   lr(false); // 1110
   assert((*lr == res_t{3,1,2,3}));
   lr(true); // 11101
   assert((*lr == res_t{3,1,3,4}));
   lr(true); // 111011
   assert((*lr == res_t{3,1,3,5}));
   lr(true); // 1110111
   assert((*lr == res_t{3,1,3,6}));
   lr(true); // 11101111
   assert((*lr == res_t{4,1,3,7}));
   lr(false); lr(false); lr(false); lr(false); // 111011110000
   assert((*lr == res_t{4,4,4,7}));
   lr(false); // 1110111100000
   assert((*lr == res_t{4,5,4,7}));
   for (int i=0; i<5; ++i) lr(true); // 111011110000011111
   assert((*lr == res_t{5,5,5,12}));
  }

  {RandGen_t g;
   const RandGen_t copy(g);
   assert((choose_kn(0,0,g) == vec_eseed_t{}));
   assert((choose_kn(0,1,g) == vec_eseed_t{}));
   assert((choose_kn(1,0,g) == vec_eseed_t{}));
   assert((choose_kn(5,4,g) == vec_eseed_t{}));
   assert((choose_kn(1,1,g) == vec_eseed_t{0}));
   assert((choose_kn(5,5,g) == vec_eseed_t{0,1,2,3,4}));
   assert(g == copy);
  }

  {RandGen_t g({0});
   // Sequence of UniformRange<RandGen_t> for n=10,...,1: 3,1,6,3,1,1,2,1,0,0
   assert((choose_kn_inclusion(10,10,g) == vec_eseed_t{3,1,6,9,8,5,2,4,0,7}));
  }
  {RandGen_t g({0});
   assert((choose_kn(5,10,g) == vec_eseed_t{3,1,6,9,8}));
  }
  {RandGen_t g({0});
   assert((choose_kn(6,10,g) == vec_eseed_t{0,2,4,5,7,8}));
  }

}
