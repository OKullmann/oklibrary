// Oleg Zaikin, 6.3.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  A Gecode-based solver for general MOLS-LS-related problems.
  using look-ahead for reduction and branching

*/

/* TODOS:

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
        "0.1.0",
        "5.4.2022",
        __FILE__,
        "Oliver Kullmann and Oleg Zaikin",
        "https://github.com/OKullmann/OKlib-MOLS/blob/master/Satisfiability/Solvers/Gecode/MOLS/laMols.cpp",
        "GPL v3"};

  const std::string error = "ERROR[" + proginfo.prg + "]: ";
  constexpr int commandline_args = 8;

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
      " N file_cond file_ps run-type prop-level branchvar branchval"
      " threads\n\n"
      " - file_cond  : filename for conditions-specification\n"
      " - file_ps    : filename for partial-squares-specification\n"
      " - run-type   : " << Environment::WRP<RT>{} << "\n" <<
      " - prop-level : " << Environment::WRP<PropO>{} << "\n" <<
      " - lookahead  : " << Environment::WRP<LAH>{} << "\n" <<
      " - branchval  : " << Environment::WRP<BHO>{} << "\n" <<
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
  const list_lah_t lahv = read_opt<LAH>(argc, argv, 6, "la", "lookahead");
  const list_bho_t bordv = read_opt<BHO>(argc, argv, 7, "bord",
                                        "order-heuristics");
  const double threads = read_threads(argc, argv);

  const std::string outfile = output_filename(proginfo.prg, N);

  const bool with_output =
    rt == RT::sat_solving or rt == RT::enumerate_solutions;
  const size_t num_runs = pov.size() * lahv.size() * bordv.size();
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

  std::cout << "# N=" << N << "\n"
               "# k=" << ac.k << " " << "total_num_sq=" <<
               ac.num_squares() << "\n"
               "# num_ps=" << ps.psqs.size() << "\n" <<
               "# rt=" << rt << "\n"
               "# num_runs=" << num_runs << "\n"
               "# threads=" << threads << "\n"
               "# propagation: ";
  Environment::out_line(std::cout, pov);
  std::cout << "\n# variable-heuristics: ";
  Environment::out_line(std::cout, lahv);
  std::cout << "\n# order-heuristics: ";
  Environment::out_line(std::cout, bordv);
  if (with_output) std::cout << "\n# output-file " << outfile;
  std::cout << std::endl;

  for (const PropO po : pov) {
    const EncCond enc(ac, ps, prop_level(po));
    for (const LAH lah : lahv)
      for (const BHO bord : bordv) {
        //const GBasicSR res =
        //  solver_gc(enc, rt, var_branch(bvar), val_branch(bord), threads);

        const GBasicSR res;
        std::cout << po<<" "<<lah<<" "<<bord<<" " << res.b.sol_found << " ";
        FloatingPoint::out_fixed_width(std::cout, 3, res.ut);
        std::cout << " " << res.gs.propagate << " " << res.gs.fail <<
          " " << res.gs.node << " " << res.gs.depth;
        std::cout << std::endl;
        if (with_output)
          Environment::out_line(*out, res.b.list_sol, "\n");
      }
  }
  if (out) delete out;
}
