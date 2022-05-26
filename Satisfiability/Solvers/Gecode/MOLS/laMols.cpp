// Oleg Zaikin, 6.3.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  A Gecode-based solver for general MOLS-LS-related problems.
  using look-ahead for reduction and branching

Examples:

1. Finding an Euler square for N=6
    - for dom propagation-level
    - the default branching-type (binary branching)
    - and the default branching-order:

MOLS> ./laMols 6 data/SpecsCollection/Euler "" satd dom binbr 0,2,3,4,5 asc eag 1

*/

/*

BUGS:

1.

MOLS> ./laMols_debug 6 data/SpecsCollection/Euler "" satd "" "" "" "0,1,0.5,0.25,0.2" ""
# N=6
# k=3 total_num_sq=3
# num_ps=0
# rt=sat-decision
# num_runs=1
# threads=1
# propagation: domain-prop
# lookahead-type: binary-super-eager
# order-heuristic: bin-branch-min
# lookahead-weights: 0 1 0.5 0.25 0.2
domain-prop binary-super-eager bin-branch-min 1 0.000 135 0 1 0

MOLS> ./laMols_debug 6 data/SpecsCollection/Euler "" sats "" "" "" "0,1,0.5,0.25,0.2" ""
# N=6
# k=3 total_num_sq=3
# num_ps=0
# rt=sat-solving
# num_runs=1
# threads=1
# propagation: domain-prop
# lookahead-type: binary-super-eager
# order-heuristic: bin-branch-min
# lookahead-weights: 0 1 0.5 0.25 0.2
# output-file SOLUTIONS_laMols_debug_6_1649926294938638745
laMols_debug: Solvers.hpp:281: Solvers::GBasicSR Solvers::lasolver(const Encoding::EncCond&, Solvers::RT, Options::LAT, Options::BHO, LookaheadBranching::vec_t, double): Assertion `EC::EncCond::unit(leaf->var())' failed.
Aborted (core dumped)

The problem seems "binary-super-eager".

*/

/* TODOS:

0. Provide the two branching orders as in our paper (using the distance,
    ascending and descending):
      ascdist, descdist.

1. It is important that here our own precise and complete statistics
    are used.
    - This is addition to the gecode-statistics.

*/


#include <iostream>
#include <string>
#include <ostream>
#include <fstream>

#include <ProgramOptions/Environment.hpp>
#include <Numerics/NumInOut.hpp>
#include <Numerics/FloatingPoint.hpp>

#include "Conditions.hpp"
#include "Encoding.hpp"
#include "PartialSquares.hpp"
#include "Solvers.hpp"
#include "Options.hpp"
#include "CommandLine.hpp"
#include "LookaheadBranching.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.4.3",
        "22.5.2022",
        __FILE__,
        "Oliver Kullmann and Oleg Zaikin",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Solvers/Gecode/MOLS/laMols.cpp",
        "GPL v3"};

  const std::string error = "ERROR[" + proginfo.prg + "]: ";
  constexpr int commandline_args = 10;

  using namespace Conditions;
  using namespace Encoding;
  using namespace PartialSquares;
  using namespace Solvers;
  using namespace Options;
  using namespace CommandLine;

  typedef EC::size_t size_t;

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg <<
      " N file_cond file_ps run-type prop-level la-type branchvar branchval"
      " threads\n\n"
      " - file_cond    : filename for conditions-specification\n"
      " - file_ps      : filename for partial-squares-specification\n"
      " - run-type     : " << Environment::WRPO<RT>{} << "\n" <<
      " - prop-level   : " << Environment::WRPO<PropO>{} << "\n" <<
      " - branch-type  : " << Environment::WRPO<BRT>{} << "\n" <<
      " - la-weights   : N-1 comma-separated weigths for calculating" <<
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

  /*
  const size_t N = read_N(argc, argv);
  const auto [ac, name_ac] = read_ac(argc, argv);
  const auto [ps, name_ps] = read_ps(argc, argv, N);
  const RT rt = read_rt(argc, argv);

  const list_propo_t pov = read_opt<PropO>(argc, argv, 5, "po",
                                           "propagation");
  const list_brt_t brtv = read_opt<BRT>(argc, argv, 6, "brt",
                                        "branching-type");
  const list_gbo_t gbov = read_opt<GBO>(argc, argv, 7, "gbo",
                                        "gc-order-heuristics");
  const list_lar_t larv = read_opt<LAR>(argc, argv, 8, "lar",
                                        "lookahead-reduction");
  const LookaheadBranching::vec_t wghts = read_weights(argc, argv, N, 9);
  const size_t num_runs = pov.size()*brtv.size()*gbov.size()*larv.size();

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
              N, ac, name_ac, ps, name_ps, rt,
              num_runs, threads, outfile, with_file_output);
  algo_output(std::cout, std::make_tuple(pov, brtv, gbov, larv));
  // XXX
  std::cout << "\n# lookahead-weights: ";
  Environment::out_line(std::cout, wghts);
  std::cout << std::endl;
  */
}
