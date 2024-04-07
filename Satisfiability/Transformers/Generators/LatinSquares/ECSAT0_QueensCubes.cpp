// Oliver Kullmann, 3.4.2024 (Swansea)
/* Copyright 2024 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Reads m "queens cubes" of order N, and creates the SAT-instance with
  N*m variables directly representing the corresponding exact-cover problem.
*/

#include <iostream>

#include <cstdlib>

#include <ProgramOptions/Environment.hpp>

#include "PQOptions.hpp"
#include "Algorithms.hpp"
#include "ECEncoding.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.0.7",
        "7.4.2024",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/LatinSquares/ECSAT0_QueensCubes.cpp",
        "GPL v3"};

  const std::string error = "ERROR[" + proginfo.prg + "]: ";
  constexpr int commandline_args = 1;

  const std::string prefix = "ECSAT0_QC_", suffix = ".cnf";

  using CT = PQOptions::CT;

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
      "> " << proginfo.prg <<
      " constraint-type\n\n"
      " - constraint-type : " << Environment::WRPO<CT>{} << "\n\n" <<
      "reads from standard input, establishes N, m, and creates file\n"
      "  " << prefix << "N_m" << suffix << " :\n\n"
      "  - for options the first possibility is the default, "
        "triggered by the empty string.\n\n"
 ;
    return true;
  }

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  if (argc != commandline_args + 1) {
    std::cerr << error << "Exactly " << commandline_args << " command-line"
      " arguments needed (constraint-type), but the real number is "
              << argc-1 << ".\n";
    return 1;
  }

  const CT ct = [&argv]{const auto ct0 = Environment::read<CT>(argv[1]);
    if (not ct0) {
      std::cerr << error << "The constraint-type could not be read from"
        " string \"" << argv[1] << "\".\n";
      std::exit(1);
    }
    return ct0.value();}();

  const auto init_cubes = Algorithms::read_queens_cubing(std::cin);
  if (init_cubes.m == 0) {
    std::cout << "Empty input.\n";
    return 0;
  }
  const auto encoding = ECEncoding::EC0Encoding(init_cubes, ct);

  const std::string filename = prefix + std::to_string(init_cubes.N) +
    "_" + std::to_string(init_cubes.m) + suffix;
  std::ofstream file(filename);
  if (not file) {
    std::cerr << error << "Can not open file \"" << filename << "\" for"
      " writing.\n";
    return 1;
  }

  std::cout << encoding.dp; std::cout.flush();
}
