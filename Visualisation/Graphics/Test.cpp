// Oliver Kullmann, 20.2.2021 (Swansea)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <iostream>
#include <vector>
#include <array>

#include <cassert>

#include <ProgramOptions/Environment.hpp>

#include "Plot.hpp"

/*

*/


namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.0",
        "20.2.2021",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Visualisation/Graphics/Test.cpp",
        "GPL v3"};
  
  using namespace Plot;

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  typedef std::array<double, 2> dp_t;
  typedef std::vector<dp_t> pv_t;

  {const UnitCubes<double> P(pv_t{}, 0.0, 0.0, 0.0, 0.0);
   assert(P.pv.empty());
  }
  {const UnitCubes<double> P(pv_t{{-1,1},{-2,2}}, -5.0, -5.0, 7.0, 7.0);
   assert((P.pv == pv_t{{0,0},{0,0}}));
  }
  {const UnitCubes<double> P(pv_t{{-1,1},{-2,2}}, 0.0, 0.0, 1.0, 2.0);
   assert((P.pv == pv_t{{0,-1},{0,1}}));
  }
  {const UnitCubes<double> P(pv_t{{-1,1},{-2,3}}, -2.0, -1.0, 1.0, 2.0);
   assert((P.pv == pv_t{{1,-1},{-1,1}}));
  }
  {const UnitCubes<double> P(pv_t{{-1,-1},{-3,3}}, -2.0, -1.0, 1.0, 2.0);
   assert((P.pv == pv_t{{1,-1},{-1,1}}));
  }

}
