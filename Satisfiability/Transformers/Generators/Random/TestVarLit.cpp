// Oliver Kullmann, 13.3.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <iostream>
#include <sstream>

#include <cassert>

#include <ProgramOptions/Environment.hpp>

#include "VarLit.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.0",
        "13.3.2022",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/TestVarLit.cpp",
        "GPL v3"};

  using namespace RandGen;

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  {std::ostringstream ss;
   ss << Var(0) << " " << Var(2);
   assert(ss.str() == "0 2");
   ss.str("");
   ss << Lit(0,0) << " " << Lit(0,1) << " " << Lit(1,-1) << " " << Lit(1,1);
   assert(ss.str() == "-0 0 -1 1");
  }

}
