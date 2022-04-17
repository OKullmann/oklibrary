// Oleg Zaikin, 13.4.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  A Gecode-based solver for general MOLS-LS-related problems.
  using (only) branching strategies as provided by Gecode itself.
  Additionally, the lookahead-reduction is used.

Examples:

*/

/* TODOS:

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
        "0.1.0",
        "17.4.2022",
        __FILE__,
        "Oliver Kullmann and Oleg Zaikin",
        "https://github.com/OKullmann/OKlib-MOLS/blob/master/Satisfiability/Solvers/Gecode/MOLS/rlaMols.cpp",
        "GPL v3"};

  const std::string error = "ERROR[" + proginfo.prg + "]: ";
  constexpr int commandline_args = 9;

  namespace CO = Conditions;
  namespace EC = Encoding;
  namespace PS = PartialSquares;
  namespace SO = Solvers;
  namespace OP = Options;
  namespace CL = CommandLine;
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

  const size_t N = CL::read_N(argc, argv);
  const CO::AConditions ac = CL::read_ac(argc, argv);
  const PS::PSquares ps = CL::read_ps(argc, argv, N);
  const OP::RT rt = CL::read_rt(argc, argv);
  const CL::list_propo_t pov = CL::read_opt<OP::PropO>(argc, argv, 5, "po", "propagation");
  const CL::list_lat_t latv = CL::read_opt<OP::LAT>(argc, argv, 6, "la", "lookahead");
  const CL::list_bhv_t bvarv = CL::read_opt<OP::BHV>(argc, argv, 7, "bvar",
                                        "variable-heuristics");
  const CL::list_bho_t bordv = CL::read_opt<OP::BHO>(argc, argv, 8, "bord",
                                        "order-heuristics");
  const double threads = FP::to_float64(argv[9]);

  const std::string outfile = CL::output_filename(proginfo.prg, N);

  const bool with_output =
    rt == OP::RT::sat_solving or rt == OP::RT::enumerate_solutions;
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

  std::cout << "# N=" << N << "\n"
               "# k=" << ac.k << " " << "total_num_sq=" <<
               ac.num_squares() << "\n"
               "# num_ps=" << ps.psqs.size() << "\n" <<
               "# num_runs=" << num_runs << "\n"
               "# threads=" << threads << "\n"
               "# rt=" << rt << "\n"
               "# propagation: ";
  Environment::out_line(std::cout, pov);
  Environment::out_line(std::cout, latv);
  std::cout << "\n# variable-heuristics: ";
  Environment::out_line(std::cout, bvarv);
  std::cout << "\n# order-heuristics: ";
  Environment::out_line(std::cout, bordv);
  if (with_output) std::cout << "\n# output-file " << outfile;
  std::cout << std::endl;

  assert(latv.size() == 1);

  for (const OP::PropO po : pov) {
    const EC::EncCond enc(ac, ps, prop_level(po));
    for (const OP::BHV bvar : bvarv)
      for (const OP::BHO bord : bordv) {
        const SO::GBasicSR res =
          SO::solver_rla(enc, rt, latv[0], var_branch(bvar), val_branch(bord),
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
