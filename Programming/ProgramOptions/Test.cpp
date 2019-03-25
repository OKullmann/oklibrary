// Oliver Kullmann, 19.3.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <cassert>
#include <iostream>

#include "Environment.hpp"

namespace {

  const Environment::ProgramInfo pi{
        "0.1.1",
        "25.3.2019",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Programming/ProgramOptions/Test.cpp",
        "GPL v3"};
}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, pi, argc, argv))
  return 0;

  using namespace Environment;

  {assert(replace("xxyxzxxyx", 'x', 'y') == "yyyyzyyyy");
   assert(basename("abc.def") == "abc");
   assert(auto_prg("abc.def") == "abc_debug");
   assert((split(",a,cf ,x\n,", ',') == tokens_t{"","a","cf ","x\n"}));
   assert(transform_spaces("\n \n ab  cd\n e  \n") == "ab cd e");
  }

  {ProgramInfo pi("AAA", "BBB", "CCC");
   assert(pi.vrs == "AAA");
   assert(pi.date == "BBB");
   assert(pi.prg == "CCC_debug");
   assert(pi.aut == "");
   assert(pi.url == "");
   assert(pi.lic == "");
  }
  {ProgramInfo pi("AAA", "BBB", "CCC.D", "x", "y", "z");
   assert(pi.vrs == "AAA");
   assert(pi.date == "BBB");
   assert(pi.prg == "CCC_debug");
   assert(pi.aut == "x");
   assert(pi.url == "y");
   assert(pi.lic == "z");
  }
  {ProgramInfo pi("AAA", "BBB", "CCC.D", "EEE", "FFF", "GGG", NP::given);
   assert(pi.vrs == "AAA");
   assert(pi.date == "BBB");
   assert(pi.prg == "CCC.D");
   assert(pi.aut == "EEE");
   assert(pi.url == "FFF");
   assert(pi.lic == "GGG");
  }

}
