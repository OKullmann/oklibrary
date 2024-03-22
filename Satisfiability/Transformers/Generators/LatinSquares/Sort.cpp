// Oliver Kullmann, 22.3.2024 (Swansea)
/* Copyright 2024 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  Sorting general tables in alphanumerical order

*/

#include <iostream>
#include <algorithm>

#include <ProgramOptions/Environment.hpp>

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.0",
        "22.3.2024",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/LatinSquares/Sort.cpp",
        "GPL v3"};

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
      "> " << proginfo.prg << "\n\n"
      "reads from standard input, and outputs to standard output.\n\n"
 ;
    return true;
  }

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  using tokens_t = Environment::tokens_t;
  using table_t = std::vector<tokens_t>;
  table_t input_table = Environment::split2_cutoff(std::cin, '\n', 0);
  std::ranges::stable_sort(input_table, Environment::LexAlphaNum<tokens_t>{});
  Environment::print2dformat(std::cout, input_table);
}
