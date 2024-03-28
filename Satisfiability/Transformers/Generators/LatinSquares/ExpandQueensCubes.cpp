// Oliver Kullmann, 28.3.2024 (Swansea)
/* Copyright 2024 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Epanding a cubing for "Queen 0" to k dimensions, by translation.

   - The dimension N is the (constant) size of the initial "cubing".
   - The initial "cubing" are the N-Pan-Queens solutions with one queen
     sitting top-left (coming from standard input).
   - Represented as vectors of size N of elements from {0, ..., N-1}.

*/

#include <iostream>

#include <cstdint>

#include <ProgramOptions/Environment.hpp>

#include "Algorithms.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.0.2",
        "28.3.2024",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/LatinSquares/ExpandQueensCubes.cpp",
        "GPL v3"};

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
      "> " << proginfo.prg <<
      " k""\n\n"
      "reads from standard input, and outputs to standard output.\n\n"
 ;
    return true;
  }

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  

}
