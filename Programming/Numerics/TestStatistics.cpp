// Oliver Kullmann, 8.11.2021 (Swansea)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <iostream>

#include <cassert>
#include <cmath>

#include <ProgramOptions/Environment.hpp>
#include <Numerics/FloatingPoint.hpp>

#include "Statistics.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.5",
        "17.4.2021",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/TestStatistics.cpp",
        "GPL v3"};

  using namespace GenStats;
  namespace FP = FloatingPoint;

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  {typedef BasicStats<FP::UInt_t, FP::float80> bst;
   bst S;
   assert((S == bst{0,0,0,FP::P264m1,-FP::P264m1-1}));
   assert(S.min() == FP::P264m1);
   assert(S.max() == -FP::P264m1-1);
   assert(S.amean() == 0);
   assert(S.var_population() == 0);
   assert(S.var_unbiased() == 0);
   assert(S.sd_population() == 0);
   assert(S.sd_corrected() == 0);
   S+=1;
   assert((S == bst{1,1,1,1,1}));
   assert(S.amean() == 1);
   assert(S.var_population() == 0);
   assert(S.var_unbiased() == 0);
   assert(S.sd_population() == 0);
   assert(S.sd_corrected() == 0);
   S+=0; (S+=2)+=3;
   assert(S.min() == 0);
   assert(S.max() == 3);
   assert(S.N == 4);
   assert(S.sum == 6);
   assert(S.sum_sq == 14);
   assert(S.amean() == 1.5L);
   assert(S.var_population() == 1.25L);
   assert(S.var_unbiased() == 5.0L / 3.0L);
   assert(S.sd_population() == std::sqrt(1.25L));
   assert(S.sd_corrected() == std::sqrt(5.0L/3.0L));
  }

  {StatsStore<FP::Int_t, FP::float80> ss;
   assert(ss.N() == 0);
   assert(ss.sum() == 0);
   assert(ss.amean() == 0);
   assert(ss.sum_sqd() == 0);
   assert(ss.var_population() == 0);
   assert(ss.sd_population() == 0);
   assert(ss.median() == 0);
   ss += 1;
   assert(ss.N() == 1);
   assert(ss.sum() == 1);
   assert(ss.amean() == 1);
   assert(ss.sum_sqd() == 0);
   assert(ss.var_population() == 0);
   assert(ss.sd_population() == 0);
   assert(ss.median() == 1);
   ss += 2;
   assert(ss.N() == 2);
   assert(ss.sum() == 3);
   assert(ss.amean() == 1.5);
   assert(ss.sum_sqd() == 0.5);
   assert(ss.var_population() == 0.25);
   assert(ss.sd_population() == 0.5);
   assert(ss.median() == 1.5);
   assert(ss.var_population() == 0.25);
   assert(ss.sd_population() == 0.5);
   ss += -1;
   assert(ss.N() == 3);
   assert(ss.sum() == 2);
   assert(ss.amean() == 2.0L/3);
   assert(ss.sum_sqd() == 42.0L/9);
   assert(ss.var_population() == 42.0L/27);
   assert(ss.sd_population() == FP::sqrt(42.0L/27));
   assert(ss.median() == 1);
   assert(ss.var_population() == 42.0L/27);
   assert(ss.sd_population() == FP::sqrt(42.0L/27));
  }

  {typedef RandVal<FP::float80> RV;
   RV rv(2, {});
   assert(not rv.sorted);
   assert(rv.a(0) == 0);
   assert(rv.b(0) == 1);
   assert(rv.a(1) == 0);
   assert(rv.b(1) == 1);
   rv.a(0,-1);
   assert(rv.a(0) == -1);
   rv.b(1,2);
   assert(rv.b(1) == 2);
   assert(rv.N == RV::default_N);
   rv.N = 20000;
   assert(rv.N == 20000);
   typedef RV::vec_t v_t;
   // Intervals [-1,1] and [0,2]:
   const auto res = rv.run([](const v_t& v){return v[0]+v[1];});
   assert(res.min() == -0.96904338503248971986L);
   assert(res.max() == 2.9817165562207108166L);
   const auto res2 = rv.run([](const v_t& v){return v[0]*v[1];});
   assert(res2.min() == -1.9541231351623961616L);
   assert(res2.max() == 1.9898062578958443037L);
   typedef RV::function_t f_t;
   f_t diff = [](const v_t& v){return v[0]-v[1];};
   auto res3 = rv.run(diff);
   assert(res3.min() == -2.9820686473523859189L);
   assert(res3.max() == 0.97118761954145433376L);
   rv.sorted = true;
   res3 = rv.run(diff);
   assert(res3.min() == -2.9859418709374049908L);
   assert(res3.max() == -4.9237024167701304127e-05L);
  }
}
