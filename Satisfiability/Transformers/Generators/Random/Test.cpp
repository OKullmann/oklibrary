// Oliver Kullmann, 2.3.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <iostream>

#include <cassert>

#include <ProgramOptions/Environment.hpp>

#include "Tests.hpp"

namespace {

  const Environment::ProgramInfo pi{
        "0.2.16",
        "18.4.2019",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/Test.cpp",
        "GPL v3"};

  using namespace RandGen;

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, pi, argc, argv))
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

}
