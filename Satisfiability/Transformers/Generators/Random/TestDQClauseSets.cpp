// Oliver Kullmann, 17.7.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <iostream>

#include <cassert>

#include <ProgramOptions/Environment.hpp>

#include "Numbers.hpp"
#include "DQClauseSets.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.0.3",
        "26.8.2019",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/TestDQClauseSets.cpp",
        "GPL v3"};

  using namespace RandGen;

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  {assert((extract_numvars({{2,Q::both},{1,Q::fa},{{2,2},Q::ex}}) == ae_numvars{1,1}));
   assert((extract_numvars({{7,Q::both},{2,Q::fa},{{3,5},Q::ex},{{6,6},Q::fa},{{7,7},Q::ex}}) == ae_numvars{3,4}));
  }

  {assert((num_dependencies({{1,Q::ex},{1,Q::ex}}) == 0));
   assert((num_dependencies({{10,Q::ex},{1,Q::ex},{{2,10},Q::ex}}) == 0));
   assert((num_dependencies({{10,Q::both},{9,Q::fa},{{10,10},Q::ex}}) == 9));
   assert((num_dependencies({{20,Q::both},{9,Q::fa},{{10,10},Q::ex},{{11,15},Q::ex},{{16,16},Q::fa},{{17,20},Q::ex}}) == 6*9 + 4*10));
  }
}
