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
        "0.1.2",
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

  {for (ls_dim_t N = 1; N <= 20; ++N)
     for (RG::gen_uint_t g = 0; g < 4; ++g) {
       const auto L = std::get<0>(random_ls(N, "7,8,9", GenO(g)));
       assert(valid(L));
       assert(is_square(L));
     }
  }

  {for (RG::gen_uint_t g = 0; g < 4; ++g) {
    const auto L = std::get<0>(random_ls(1, "7,8,9", GenO(g)));
    assert(valid(L));
    assert(is_square(L));
    assert(L == ls_t{{0}});
   }
  }

  /*
  {RG::RandGen_t g({1});
   ls_ip_t I = ls2lsip(cyclic_ls(4));
   jm_next(std::cerr, I, g);
  }
  */

}
