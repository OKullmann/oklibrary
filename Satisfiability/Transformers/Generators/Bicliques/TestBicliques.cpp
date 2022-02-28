// Oliver Kullmann, 28.2.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <iostream>

#include <cassert>

#include <ProgramOptions/Environment.hpp>

#include "Bicliques.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.0",
        "28.2.2022",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Bicliques/TestBicliques.cpp",
        "GPL v3"};

  using namespace Bicliques;

  template <class X>
  constexpr bool eqp(const X& lhs, const X& rhs) noexcept {
    return lhs == rhs;
  }
}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  {AdjVecUInt G(Graphs::GT::und, 0);
   assert(valid(list_t{}, G));
   assert(not valid(list_t{0}, G));
   assert(valid({list_t{}, list_t{}}, G));
   assert(not valid({list_t{0}, list_t{}}, G));
   assert(not valid({list_t{}, list_t{0}}, G));
  }

  {

  }

}
