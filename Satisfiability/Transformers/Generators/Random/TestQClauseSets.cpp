// Oliver Kullmann, 9.6.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <iostream>

#include <cassert>

#include <ProgramOptions/Environment.hpp>

#include "DQCNF.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.0.2",
        "29.6.2019",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/TestDQCNF.cpp",
        "GPL v3"};

  using namespace RandGen;

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  {assert(not valid(block_v{}));
   assert((not valid(block_v{{1,Q::fa}})));
   assert((valid(block_v{{1,Q::ex}})));
   assert((not valid(block_v{{{2,3},Q::ex}})));
   assert((valid(block_v{{5,Q::fa},{{6,10},Q::fa},{{11,14},Q::ex}})));
   assert((not valid(block_v{{5,Q::fa},{{6,10},Q::fa},{{10,14},Q::ex}})));
   assert((not valid(block_v{{5,Q::fa},{{5,10},Q::fa},{{11,14},Q::ex}})));
  }

  {assert(read_block_v("") == block_v{});
   assert(read_block_v("  \n\n  \t ") == block_v{});
   assert((read_block_v("123") == block_v{{123,Q::ex}}));
   assert((read_block_v("a10 123") == block_v{{10,Q::fa}, {{11,133},Q::ex}}));
   assert((read_block_v("a10 a10 123") == block_v{{10,Q::fa}, {{11,20},Q::fa}, {{21,143},Q::ex}}));
   assert((read_block_v(" a10 \n a10 e1 \n 123\n") == block_v{{10,Q::fa}, {{11,20},Q::fa}, {{21,21},Q::ex}, {{22,144},Q::ex}}));
  }
}
