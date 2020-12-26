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
        "0.1.4",
        "26.12.2020",
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

  {assert(not valid_basic_partial(ls_row_t{},0));
   assert(not valid_basic_partial(ls_row_t{},1));
   assert(not valid_basic(ls_row_t{0},0));
   assert(valid_basic_partial(ls_row_t{0},1));
   assert(not valid_basic_partial(ls_row_t{0},2));
   assert((valid_basic_partial(ls_row_t{0,1},2)));
   assert((valid_basic_partial(ls_row_t{0,2},2)));
   assert((valid_basic_partial(ls_row_t{0,0},2)));
  }

  {assert(all_different(ls_row_t{0}));
   assert((all_different(ls_row_t{0,1})));
   assert((not all_different(ls_row_t{0,0})));
   assert((not all_different(ls_row_t{1,1})));
   assert((all_different(ls_row_t{2,1,0})));
   assert((not all_different(ls_row_t{2,1,2})));
  }

  {assert(all_different_partial(ls_row_t{0}));
   assert((all_different_partial(ls_row_t{0,1})));
   assert((not all_different_partial(ls_row_t{0,0})));
   assert((not all_different_partial(ls_row_t{1,1})));
   assert((all_different_partial(ls_row_t{2,1,0})));
   assert((not all_different_partial(ls_row_t{2,1,2})));
   assert((all_different_partial(ls_row_t{4,1,4,2})));
   assert((not all_different_partial(ls_row_t{4,1,4,1})));
  }

  {assert((valid(ls_t{{0}})));
   assert((valid(ls_t{{0,1},{1,0}})));
   assert((valid(ls_t{{0,1,2},{2,0,1},{1,2,0}})));
   assert((not valid(ls_t{{0,1,2},{2,0,1},{0,2,1}})));
  }

  {assert((valid_partial(ls_t{{0}})));
   assert((valid_partial(ls_t{{0,1},{1,0}})));
   assert((valid_partial(ls_t{{0,1,2},{2,0,1},{1,2,0}})));
   assert((not valid_partial(ls_t{{0,1,2},{2,0,1},{3,2,1}})));
   assert((valid_partial(ls_t{{0,1,2},{2,0,1},{3,2,3}})));
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

  {assert(cyclic_ls(1) == ls_t{{0}});
   assert((cyclic_ls(2) == ls_t{{0,1},{1,0}}));
   assert((cyclic_ls(3) == ls_t{{0,1,2},{1,2,0},{2,0,1}}));
  }

  {assert((find_first_duplication({0}) == index_pair_t{1,1}));
   assert((find_first_duplication({0,1}) == index_pair_t{2,2}));
   assert((find_first_duplication({0,0}) == index_pair_t{0,1}));
   assert((find_first_duplication({0,1,0}) == index_pair_t{0,2}));
   assert((find_first_duplication({0,1,1,0}) == index_pair_t{1,2}));
  }

  {assert(valid(Set{},0));
   assert(valid(Set{{0}},1));
   assert(valid(Set{{0,1}},2));
   assert(not valid(Set{{1,0}},2));
   assert(valid(Set{{2,4,6}},10));
   assert(not valid(Set{{2,1,6}},10));
   assert(not valid(Set{{11}},10));
   assert(not valid(Set{{1,1}},10));
  }

  {assert(valid(SetSystem{}));
   assert(valid(SetSystem{{{{0}}}}));
   assert(not valid(SetSystem{{{{1}}}}));
   assert(valid(SetSystem{{{{0,1}},{{1}}}}));
   assert(valid(SetSystem{{{{3}},{{}},{{}},{{}}}}));
  }

  {assert(is_sdr({},{}));
   assert((is_sdr({0},{{{{0}}}})));
   assert((is_sdr({2,1,0},{{{{0,2}},{{1}},{{0,2}}}})));
   assert((not is_sdr({2,1,0},{{{{0,2}},{{1}},{{1,2}}}})));
  }

  {assert(is_psdr({},{}));
   assert((is_psdr({0},{{{{0}}}})));
   assert((is_psdr({2,1,0},{{{{0,2}},{{1}},{{0,2}}}})));
   assert((not is_psdr({2,1,0},{{{{0,2}},{{1}},{{1,2}}}})));
   assert((is_psdr({2,1,3},{{{{0,2}},{{1}},{{1,2}}}})));
  }

}
