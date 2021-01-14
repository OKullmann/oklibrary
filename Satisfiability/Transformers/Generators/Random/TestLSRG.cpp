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
        "0.1.1",
        "14.1.2021",
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
     for (RG::gen_uint_t g = 0; g < 3; ++g) {
       const auto res = random_ls(N, "7,8,9", GenO(g));
       assert(valid(std::get<0>(res)));
     }
  }

  {const auto [L, seeds, basic_size] =
     random_ls(1, "0", Selection(1), GenO::jm, StRLS::none);
   assert(is_square(L));
   assert(valid(L));
   assert(L == ls_t{{0}});
  }

  {const auto [L, seeds, basic_size] =
     random_ls(1, "1", Selection(1), GenO::jm, StRLS::none);
   assert(is_square(L));
   assert(valid(L));
   assert(L == ls_t{{0}});
  }

  /*
  {RG::RandGen_t g({1});
   ls_ip_t I = ls2lsip(cyclic_ls(4));
   jm_next(std::cerr, I, g);
  }
  */

}
