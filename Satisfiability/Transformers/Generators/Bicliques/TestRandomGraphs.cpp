// Oliver Kullmann, 16.8.2023 (Swansea)
/* Copyright 2023 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <iostream>

#include <cassert>

#include <ProgramOptions/Environment.hpp>

#include "RandomGraphs.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.0.2",
        "16.8.2023",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Bicliques/TestRandomGraphs.cpp",
        "GPL v3"};

  using namespace RandomGraphs;

  template <class X>
  constexpr bool eqp(const X& lhs, const X& rhs) noexcept {
    return lhs == rhs;
  }
}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  {RandGen::RandGen_t g{1,2,4};
   assert(eqp(independent_edges(5, {1,2}, g).alledges(),
              {{0,1},{0,4},{1,3},{1,4},{2,3},{2,4}}));
  }

}
