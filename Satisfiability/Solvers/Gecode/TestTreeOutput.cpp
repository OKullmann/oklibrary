// Oleg Zaikin, 9.3.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <sstream>

#include <cassert>

#include "TreeOutput.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.0.2",
        "10.3.2022",
        __FILE__,
        "Oleg Zaikin and Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Solvers/Gecode/TestTreeOutput.cpp",
        "GPL v3"};

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  {std::ostringstream oss;
   TreeOutput::TreeLog tl(&oss, TreeOutput::LogLvlO::none);
   tl.add(0, 1, 0, {0,1});
   assert(oss.str() == "");
   };

  {std::ostringstream oss;
   TreeOutput::TreeLog tl(&oss, TreeOutput::LogLvlO::reduced);
   tl.add(0, 1, 0, {0,1});
   //std::cout << oss.str();
   assert(oss.str() == "0 1 0 2 0 1\n");
   };

   {std::ostringstream oss;
   TreeOutput::TreeLog tl(&oss, TreeOutput::LogLvlO::reduced);
   tl.add(1, 2, 2, {7,8,9});
   //std::cout << oss.str();
   assert(oss.str() == "1 2 2 3 7 8 9\n");
   };

   {std::ostringstream oss;
   TreeOutput::TreeLog tl(&oss, TreeOutput::LogLvlO::reduced);
   tl.add(0, 1, 0, {0,1});
   tl.add(1, 2, 2, {7,8,9});
   //std::cout << oss.str();
   assert(oss.str() == "0 1 0 2 0 1\n1 2 2 3 7 8 9\n");
   };

}
