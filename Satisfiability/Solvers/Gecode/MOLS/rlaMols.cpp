// Oleg Zaikin, 13.4.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  A Gecode-based solver for general MOLS-LS-related problems:
   - using (only) branching strategies as provided by Gecode itself,
   - but with the lookahead-reduction.

Examples:

*/

/* TODOS:

0. Additional to the gecode-statistics, our statistics on the reduction
   need to be provided.

1. A customised brancher with Gecode's default branching strategies is needed:
   - Handled by LookaheadBranching::RlaBranching.
   - Check via unit tests that the solver and gcMols give equal results.

*/

/*

BUGS:

*/

#include <iostream>
#include <string>
#include <ostream>
#include <fstream>

#include <cassert>

#include <ProgramOptions/Environment.hpp>
#include <Numerics/NumInOut.hpp>

#include "Conditions.hpp"
#include "Encoding.hpp"
#include "PartialSquares.hpp"
#include "Solvers.hpp"
#include "Options.hpp"
#include "CommandLine.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.3.0",
        "22.5.2022",
        __FILE__,
        "Oliver Kullmann and Oleg Zaikin",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Solvers/Gecode/MOLS/rlaMols.cpp",
        "GPL v3"};

  const std::string error = "ERROR[" + proginfo.prg + "]: ";
  constexpr int commandline_args = 10;

  using namespace Conditions;
  using namespace Encoding;
  using namespace PartialSquares;
  using namespace Solvers;
  using namespace Options;
  using namespace CommandLine;

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg <<
      " N file_cond file_ps run-type prop-level branch-type branch-var"
      " branch-order la-type threads\n\n"
      " - file_cond    : filename for conditions-specification\n"
      " - file_ps      : filename for partial-squares-specification\n"
      " - run-type     : " << Environment::WRPO<RT>{} << "\n" <<
      " - prop-level   : " << Environment::WRPO<PropO>{} << "\n" <<
      " - branch-type  : " << Environment::WRPO<BRT>{} << "\n" <<
      " - branch-var   : " << Environment::WRPO<BHV>{} << "\n" <<
      " - branch-order : " << Environment::WRPO<GBO>{} << "\n" <<
      " - la-reduction : " << Environment::WRPO<LAR>{} << "\n" <<
      " - threads      : floating-point for number of threads\n\n"
      "Here\n"
      "  - file_ps can be the empty string (no partial instantiation)\n"
      "  - the algorithmic options can be lists (all combinations)\n"
      "  - these lists can have a leading + (inclusion) or - (exclusion)\n"
      "  - for sat-solving and enumeration, output goes to file \"" <<
      "SOLUTIONS_" << proginfo.prg << "_N_timestamp\".\n\n"
;
    return true;
  }

}


int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  if (argc !=  commandline_args + 1) {
    std::cerr << error << "Exactly " << commandline_args << " command-line"
      " arguments needed, but the real number is " << argc-1 << ".\n";
    return 1;
  }

  const size_t N = read_N(argc, argv);
  const auto [ac, name_ac] = read_ac(argc, argv);
  const auto [ps, name_ps] = read_ps(argc, argv, N);
  const RT rt = read_rt(argc, argv);

  const list_propo_t pov = read_opt<PropO>(argc, argv, 5, "po",
                                           "propagation");
  const list_brt_t brtv = read_opt<BRT>(argc, argv, 6, "brt",
                                        "branching-type");
  const list_bhv_t bvarv = read_opt<BHV>(argc, argv, 7, "bvar",
                                        "gc-variable-heuristics");
  const list_gbo_t gbov = read_opt<GBO>(argc, argv, 8, "gbo",
                                        "gc-order-heuristics");
  const list_lar_t larv = read_opt<LAR>(argc, argv, 9, "lar",
                                        "lookahead-reduction");
  const size_t num_runs =
    brtv.size()*pov.size()*bvarv.size()*gbov.size()*larv.size();

  const double threads = read_threads(argc, argv, 10);

  const std::string outfile = output_filename(proginfo.prg, N);

  const bool with_file_output = Options::with_file_output(rt);
  if (with_file_output and num_runs != 1) {
    std::cerr << error << "For solution-output the number of runs must be 1,"
      " but is " << num_runs << ".\n";
    return 1;
  }
  std::ostream* const out = with_file_output ?
    new std::ofstream(outfile) : nullptr;
  if (with_file_output and (not out or not *out)) {
    std::cerr << error << "Can not open file \"" << outfile << "\" for "
      "writing.\n";
    return 1;
  }

  info_output(std::cout,
              N, ac, name_ac, ps, name_ps,
              rt, pov, brtv, bvarv, gbov, num_runs, threads,
              outfile, with_file_output);
  std::cout << "#   la-reduction: ";
  Environment::out_line(std::cout, larv);
  std::cout << std::endl;

}
