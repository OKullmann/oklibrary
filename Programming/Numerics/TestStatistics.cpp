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
        "0.1.2",
        "6.3.2021",
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
}
