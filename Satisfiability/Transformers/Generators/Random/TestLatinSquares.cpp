// Oliver Kullmann, 18.12.2020 (Swansea)
/* Copyright 2020 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <iostream>

#include <ProgramOptions/Environment.hpp>

#include "LatinSquares.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.0",
        "19.12.2020",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/TestLatinSquares.cpp",
        "GPL v3"};

  using namespace LatinSquares;

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  {assert(not valid_basic(ls_row_t{},0));
   assert(not valid_basic(ls_row_t{},1));
   assert(not valid_basic(ls_row_t{0},0));
   assert(valid_basic(ls_row_t{0},1));
   assert(not valid_basic(ls_row_t{0},2));
   assert((valid_basic(ls_row_t{0,1},2)));
   assert((not valid_basic(ls_row_t{0,2},2)));
   assert((valid_basic(ls_row_t{0,0},2)));
  }

  {assert(all_different(ls_row_t{0}));
   assert((all_different(ls_row_t{0,1})));
   assert((not all_different(ls_row_t{0,0})));
   assert((not all_different(ls_row_t{1,1})));
   assert((all_different(ls_row_t{2,1,0})));
   assert((not all_different(ls_row_t{2,1,2})));
  }

  {assert((valid(ls_t{{0}})));
   assert((valid(ls_t{{0,1},{1,0}})));
   assert((valid(ls_t{{0,1,2},{2,0,1},{1,2,0}})));
  }

  {assert(valid_basic(first_basic(1)));
   assert(valid_basic(first_basic(2)));
   ls_t L = first_basic(3);
   assert((L == ls_t{{0,0,0},{0,0,0},{0,0,0}}));
   assert(next_basic(L));
   assert((L == ls_t{{1,0,0},{0,0,0},{0,0,0}}));
   assert(next_basic(L));
   assert((L == ls_t{{2,0,0},{0,0,0},{0,0,0}}));
   assert(next_basic(L));
   assert((L == ls_t{{0,1,0},{0,0,0},{0,0,0}}));
  }

  {for (ls_dim_t N = 1; N <= 3; ++N)
     assert(trivial_count_all_ls(N) == all_ls[N]);
  }
}