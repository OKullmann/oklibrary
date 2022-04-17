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

1. Update to the new standard (see gcMols)
    - Here we have exactly one option more than with gcMols, namely
      the la-implementation, with three possibilities:
       lazy, eager, seager
    - This option shall be penultimative.

2. Additional to the gecode-statistics, our statistics on the reduction
   need to be provided.

*/

/*

BUGS:

*/

#include <iostream>
#include <string>
#include <ostream>

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
        "0.1.1",
        "17.4.2022",
        __FILE__,
        "Oliver Kullmann and Oleg Zaikin",
        "https://github.com/OKullmann/OKlib-MOLS/blob/master/Satisfiability/Solvers/Gecode/MOLS/rlaMols.cpp",
        "GPL v3"};

  const std::string error = "ERROR[" + proginfo.prg + "]: ";
  constexpr int commandline_args = 9;

  using namespace Conditions;
  using namespace Encoding;
  using namespace PartialSquares;
  using namespace Solvers;
  using namespace Options;
  using namespace CommandLine;
  namespace FP = FloatingPoint;

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg <<
      " N file_cond file_ps run-type prop-level la-type branchvar branchval"
      " threads\n\n"
      " - file_cond  : filename for conditions-specification\n"
      " - file_ps    : filename for partial-squares-specification\n"
      " - run-type   : " << Environment::WRPO<OP::RT>{} << "\n" <<
      " - prop-level : " << Environment::WRPO<OP::PropO>{} << "\n" <<
      " - la-type    : " << Environment::WRPO<OP::LAT>{} << "\n" <<
      " - branchvar  : " << Environment::WRPO<OP::BHV>{} << "\n" <<
      " - branchval  : " << Environment::WRPO<OP::BHO>{} << "\n" <<
      " - threads    : floating-point for number of threads\n\n"
      "Here\n"
      "  - file_ps can be the empty string (no partial instantiation)\n"
      "  - the three algorithmic options can be lists (all combinations)\n"
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
  const AConditions ac = read_ac(argc, argv);
  const PSquares ps = read_ps(argc, argv, N);
  const RT rt = read_rt(argc, argv);
  const list_propo_t pov = read_opt<PropO>(argc, argv, 5, "po", "propagation");
  const list_lat_t latv = read_opt<LAT>(argc, argv, 6, "la", "lookahead");
  const list_bhv_t bvarv = read_opt<BHV>(argc, argv, 7, "bvar",
                                        "variable-heuristics");
  const list_bho_t bordv = read_opt<BHO>(argc, argv, 8, "bord",
                                        "order-heuristics");
  const double threads = FP::to_float64(argv[9]);

  const std::string outfile = output_filename(proginfo.prg, N);

  const bool with_output =
    rt == RT::sat_solving or rt == RT::enumerate_solutions;
  const size_t num_runs = pov.size() * bvarv.size() * bordv.size();
  if (with_output and num_runs != 1) {
    std::cerr << error << "For solution-output the number of runs must be 1,"
      " but is " << num_runs << ".\n";
    return 1;
  }
  std::ostream* const out = with_output ? new std::ofstream(outfile) : nullptr;
  if (with_output and (not out or not *out)) {
    std::cerr << error << "Can not open file \"" << outfile << "\" for "
      "writing.\n";
    return 1;
  }

  /* The above needs update, so that this works:
    info_output(std::cout,
              N, ac, ps, rt, pov, brtv, bvarv, gbov, num_runs, threads,
              outfile, with_output);
  */
  std::cout << "#   la-implementation: ";
  Environment::out_line(std::cout, latv);
  std::cout << std::endl;

  [[deprecated]] assert(latv.size() == 1);

  for (const PropO po : pov) {
    const EC::EncCond enc(ac, ps, prop_level(po));
    for (const BHV bvar : bvarv)
      for (const BHO bord : bordv) {
        const GBasicSR res =
          solver_rla(enc, rt, latv[0], var_branch(bvar), val_branch(bord),
            threads);
        using Environment::W0;
        std::cout << W0(po) << " " << W0(latv[0]) << " " << W0(bvar) << " "
                  << W0(bord) << " "
                  << res.b.sol_found << " ";
        FP::out_fixed_width(std::cout, 3, res.ut);
        std::cout << " " << res.gs.propagate << " " << res.gs.fail <<
          " " << res.gs.node << " " << res.gs.depth;
        std::cout << std::endl;
        if (with_output)
          Environment::out_line(*out, res.b.list_sol, "\n");
      }
  }
  if (out) delete out;
}
