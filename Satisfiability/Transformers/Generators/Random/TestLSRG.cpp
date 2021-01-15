// Oleg Zaikin, 13.1.2021 (Swansea)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <cassert>

#include <ProgramOptions/Environment.hpp>

#include "LSRG.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.4",
        "15.1.2021",
        __FILE__,
        "Oliver Kullmann and Oleg Zaikin",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/TestLSRG.cpp",
        "GPL v3"};

    using namespace LSRG;
    using namespace LatinSquares;
}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  // N=1..20  majm, jm, ma:
  {for (ls_dim_t N = 1; N <= 12; ++N)
     for (RG::gen_uint_t g = 0; g < 3; ++g) {
       const auto L = std::get<0>(random_ls(N, "7,8,9", GenO(g)));
       assert(valid(L));
     }
  }

  // N=1  majm, jm, ma:
  {for (RG::gen_uint_t g = 0; g < 3; ++g) {
    const auto L = std::get<0>(random_ls(1, "7,8,9", GenO(g)));
    assert(valid(L));
    assert(L == ls_t{{0}});
   }
  }

  // N=2  majm, jm, ma:
  {for (RG::gen_uint_t g = 0; g < 3; ++g) {
    const auto L = std::get<0>(random_ls(2, "7,8,9", GenO(g)));
    assert(valid(L));
    assert((L == ls_t{{0,1},{1,0}} or L == ls_t{{1,0},{0,1}}));
   }
  }

  {const auto L = std::get<0>(random_ls(3, "7,8,9", GenO::majm));
   assert(valid(L));
   assert((L == ls_t{{1,2,0},{2,0,1},{0,1,2}}));
  }

  {const auto L = std::get<0>(random_ls(3, "7,8,9", GenO::jm));
   assert(valid(L));
   assert((L == ls_t{{1,0,2},{2,1,0},{0,2,1}}));
  }

  {const auto L = std::get<0>(random_ls(3, "7,8,9", GenO::ma));
   assert(valid(L));
   assert((L == ls_t{{1,2,0},{0,1,2},{2,0,1}}));
  }

  /* XXX
  {const auto L = std::get<0>(random_ls(10, "7,8,9", GenO::majm));
   assert(valid(L));
   assert((L == ls_t{
     {7,0,3,4,1,6,5,9,8,2},
     {1,3,8,6,2,9,4,7,5,0},
     {4,2,0,7,8,1,6,3,9,5},
     {3,7,5,2,4,8,9,6,0,1},
     {9,8,1,0,6,5,2,4,7,3},
     {2,5,6,3,9,4,7,0,1,8},
     {8,4,7,5,3,0,1,2,6,9},
     {5,1,4,9,0,7,3,8,2,6},
     {6,9,2,8,5,3,0,1,4,7},
     {0,6,9,1,7,2,8,5,3,4}}));
  }

  {const auto L = std::get<0>(random_ls(10, "7,8,9", GenO::jm));
   assert(valid(L));
   assert((L == ls_t{
     {3,5,4,9,2,7,6,1,8,0},
     {1,4,7,6,5,0,3,8,2,9},
     {8,6,0,5,3,4,9,2,7,1},
     {9,1,5,2,7,3,8,6,0,4},
     {5,3,2,0,8,1,4,9,6,7},
     {7,0,6,8,1,9,2,3,4,5},
     {4,7,3,1,6,8,5,0,9,2},
     {6,8,1,4,9,2,0,7,5,3},
     {0,2,9,7,4,6,1,5,3,8},
     {2,9,8,3,0,5,7,4,1,6}}));
  }

  {const auto L = std::get<0>(random_ls(10, "7,8,9", GenO::ma));
   assert(valid(L));
   assert((L == ls_t{
     {0,9,2,4,3,7,5,6,8,1},
     {8,6,1,2,5,4,7,9,0,3},
     {4,3,0,5,9,2,8,1,6,7},
     {6,7,5,8,2,0,1,4,3,9},
     {1,0,9,7,6,5,3,8,4,2},
     {5,4,3,1,8,9,6,7,2,0},
     {7,5,6,0,4,1,2,3,9,8},
     {9,2,8,6,7,3,4,0,1,5},
     {2,1,4,3,0,8,9,5,7,6},
     {3,8,7,9,1,6,0,2,5,4}}));
  }
  */

  /*
  {RG::RandGen_t g({1});
   ls_ip_t I = ls2lsip(cyclic_ls(4));
   jm_next(std::cerr, I, g);
  }
  */

}
