// Oliver Kullmann, 28.14.2021 (Swansea)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Small trees for RPL (reverse pure literals)

*/

#include <string>
#include <iostream>

#include "RPL_trees.hpp"

#include <ProgramOptions/Environment.hpp>

namespace {

  const Environment::ProgramInfo proginfo{
        "0.3.1",
        "29.4.2021",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Quantification/MAUT/RPL_trees.cpp",
        "GPL v3"};

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
      "> " << proginfo.prg << " [input]\n\n"
      " input     : standard input " << "\n\n"
      ;
    std::cout <<
    " computes a splitting tree and prints the tree-sizes:\n\n"
    "  - Trailing arguments can be left out, then using their default-values.\n"
    "  - Arguments \"\" (the empty string) yield also the default-values,\n"
      ;
    return true;
  }

  MAUT::size_t nodes = 1, leaves = 0;

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  const MAUT::ClauseSet F = MAUT::read(std::cin);
  assert(valid(F));
  std::cout << F;
  MAUT::Pass pa(F.dp.n);
  const auto init_reduced = MAUT::add_pure(pa, F);
  assert(init_reduced == (F.dp.n - F.s.no) + F.s.pv);
  assert(pa.size() == init_reduced);
  if (init_reduced == F.dp.n) {leaves = 1; return 0;}

}
