// Oliver Kullmann, 11.6.2025 (Swansea)
/* Copyright 2025 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <iostream>

#include <cassert>

#include <ProgramOptions/Environment.hpp>

#include "TestTools.hpp"
#include "TotalPermutation.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.0.5",
        "11.6.2025",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Bicliques/TestTotalPermutation.cpp",
        "GPL v3"};

  using namespace TestTools;
  using namespace TotalPermutation;

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  {using DimacsTools::DimacsClauseList;
   using DimacsTools::Lit;
   assert(eqp(seeds(DimacsClauseList{}),
              {0,0,0,4962778143753605888LLU}));
   assert(eqp(seeds(DimacsClauseList{{5,0},{}}),
              {5,0,0,17253265039582258950LLU}));
   assert(eqp(seeds((DimacsClauseList{{5,1},{{}}})),
              {5,1,11400714819323202583ULL,3816381400296288825ULL}));
   assert(eqp(seeds((DimacsClauseList{{5,2},{{Lit(1,1),Lit(2,-1)},{Lit(3,1)}}})),
              {5,2,17115513403910344103ULL,11180425951301671036ULL}));
  }

}
