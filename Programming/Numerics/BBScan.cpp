// Oliver Kullmann, 15.9.2023 (Swansea)
/* Copyright 2023 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Applying a function, given as script, to a grid of points

*/

#include <iostream>

#include <cassert>
#include <cstdlib>

#include <ProgramOptions/Environment.hpp>
#include <Transformers/Generators/Random/Numbers.hpp>
#include <Numerics/NumInOut.hpp>

#include "OptTypes.hpp"
#include "Sampling.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.0.8",
        "15.9.2023",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Programming/Numerics/BBScan.cpp",
        "GPL v3"};

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  namespace FP = FloatingPoint;
  namespace OS = Optimisation;
  namespace SP = Sampling;

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg
         << " grid script seeds threads\n\n"
    " grid           : " << "filename\n"
    " script         : " << "string\n"
    " seeds          : " << "sequence, can contain \"t\" or \"r\"\n"
    " threads        : " << "natural number >= 0\n\n"
    " applies the script to the points of the script :\n\n"
    "  - the output is stored in file \"BBs_grid_script_timestamp\"\n"
    "  - here \"grid\" and \"script\" are simplifications of the"
    " corresponding strings.\n\n"
;
    return true;
  }

  OS::index_t read_threads(const std::string& s) {
    OS::index_t res;
    try { res = FP::touint(s); }
    catch (const std::exception& e) {
      std::cerr << error <<
        "Reading-error for number of threads, with error-message\n  " <<
        e.what();
      std::exit(int(OS::Error::faulty_parameters));
    }
    return res;
  }

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  if (argc != 5) {
    std::cerr << error <<
      "Exactly four arguments (grid, script, seeds, threads)"
      " needed, but " << argc-1 << " provided.\n";
    return int(OS::Error::missing_parameters);
  }

  const std::string grid = argv[1];
  std::ifstream grids(grid);
  if (not grids) {
    std::cerr << error <<
      "Reading-error for grid-file \"" << grid << "\".\n";
    return int(OS::Error::faulty_parameters);
  }
  const std::string script = argv[2];
  if (script.empty()) {
    std::cerr << error <<
      "Empty script-string.\n";
    return int(OS::Error::faulty_parameters);
  }
  const RandGen::vec_eseed_t seeds = RandGen::extract_seeds(argv[3]);
  const bool randomised = not seeds.empty();
  const OS::index_t threads = read_threads(argv[4]);

  const std::string output = SP::scanning_output(grid, script);
  const auto [I,x] = OS::read_scanning_info(grids);
  const OS::index_t N = I.size();

  if (threads == 0) { // dry run
    std::cout << "# ";
    Environment::args_output(std::cout, argc, argv);
    std::cout << "\n# " << output << "\n# N=" << N << " #points=";
    OS::vec_t dummy(N);
    std::cout << size_scanning(std::get<0>(
                   SP::prepare_scanning(x,I,seeds,randomised,dummy))) << "\n";
    return 0;
  }

}
