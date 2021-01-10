// Oliver Kullmann, 18.4.2019 (Swansea)
/* Copyright 2019, 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <iostream>

#include <cassert>

#include <ProgramOptions/Environment.hpp>
#include <Numerics/FloatingPoint.hpp>

#include "Tests.hpp"
#include "KolmogorovSmirnov.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.4.0",
        "10.1.2021",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/TestTests.cpp",
        "GPL v3"};

  using namespace RandGen;
  using namespace KolSmir;

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

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

  {using namespace FloatingPoint;
   typedef std::vector<float80> S;
   assert(ks_D_value<S>({}) == minfinity);
   assert(ks_D_value<S>({0}) == 1);
   assert(ks_D_value<S>({1}) == 1);
   assert(ks_D_value<S>({0.5}) == 0.5);
   assert(ks_D_value<S>({0.3L}) == 0.7L);
   assert(ks_D_value<S>({0.7L}) == 0.7L);
   assert((ks_D_value<S>({0,0}) == 1));
   assert((ks_D_value<S>({1,1}) == 1));
   assert((ks_D_value<S>({0,1}) == 0.5));
   assert((ks_D_value<S>({0,0.5}) == 0.5));
   assert((ks_D_value<S>({0,0.4}) == 0.6));
   assert((accuracy(Prob64{7,15}, ks_D_value<S>({0.1, Prob64{1,5}, 0.6})) <= 1));
   assert((ks_D_value<S>({0.1, 0.2, 0.2, 0.3, 0.3, Prob64{326,1000}, 0.7, 0.8, 0.9, 1}) == Prob64{274,1000}));
  }

  {using namespace FloatingPoint;
   assert(ks_P(1,0) == 1);
   assert(ks_P(2,0) == 1);
   assert(ks_P(3,0) == 1);
   assert(ks_P(1,1) == 0);
   assert(ks_P(2,1) == 0);
   assert(ks_P(1,0.5L) == 1);
   assert(ks_P(2,0.5L) == 0.5L);
   assert(ks_P(2,0.3L) == 0.98L);
   assert(ks_P(2,0.4L) == 0.82L);
   assert(accuracy(1 - 599364867645744586275603.0L / 953674316406250000000000.0L, ks_P(10, 0.274L)) < 10);
   assert(accuracy(1 - 0.99676943191713676985L, ks_P(2000, 0.04L)) < 1e11L);
   assert(accuracy(1 - 0.99999893956930568118L, ks_P(2000, 0.06L)) < 1e9L);
   assert(accuracy(1 - 0.99945234913828052085L, ks_P(16000, 0.016L)) < 1e9L);
  }

  {using namespace FloatingPoint;
   assert((epval_prob({0}) == ExtremePVal{pinfinity,1,0}));
   assert((epval_prob({0,0,0}) == ExtremePVal{pinfinity,3,0}));
   assert((epval_prob({1}) == ExtremePVal{0,1,1}));
   assert((epval_prob({0.2L}) == ExtremePVal{0,1,1}));
   assert((epval_prob({0.2L,0.3L,1,1}) == ExtremePVal{0,4,1}));
   assert((epval_prob({0.1L}) == ExtremePVal{1,1,0.1L}));
   assert((epval_prob({0.05L,0.1L,0.1L,0.2L}) == ExtremePVal{1,3,0.0037L}));
   {const auto P = epval_prob({0.01L,0.05L,0.1L,0.1L,0.2L});
    assert(P.level == 2);
    assert(P.count == 1);
    assert(accuracy(0.0490099501L, P.p) <= 10);}
   {const auto P = epval_prob({0.0005L,0.001L,0.01L,0.05L,0.1L,0.1L,0.2L});
    assert(P.level == 3);
    assert(P.count == 2);
    assert(accuracy(2.093010491603499e-5L, P.p) <= 1e5L);}
  }

  {using namespace FloatingPoint;
   assert(accuracy(2.0L / 9, rapfac(3)) <= 1);

   for (unsigned i = 4; i <= 100; ++i) {
     const float80 x = float80(i) / 100;
     assert(accuracy(1-ks_P(60, x), Pomeranz(60, x)) <= 1e5L);
   }

   assert(cdfSpecial(1,0) == 0);
   assert(cdfSpecial(1,0.5L) == 0);
   assert(cdfSpecial(1,1) == 1);
   assert(cdfSpecial(2,1) == 1);
   assert(cdfSpecial(2,2) == 1);
   assert(cdfSpecial(2,0) == 0);
   assert(cdfSpecial(3,0.5L) == -1);

   assert(fbarSpecial(1,0) == 1);
   assert(fbarSpecial(1,0.5L) == 1);
   assert(fbarSpecial(1,1) == 0);
   assert(fbarSpecial(2,1) == 0);
   assert(fbarSpecial(2,2) == 0);
   assert(fbarSpecial(2,0) == 1);
   assert(fbarSpecial(3,0.5L) == -1);
  }

}
