// Oliver Kullmann, 27.3.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  A Gecode-based solver for general MOLS-LS-related problems.
  using (only) branching strategies as provided by Gecode itself

Examples:

1. Counting reduced ls's for N=7 for all propagation-levels, all
   variable-selections except of first-var, and the default
   value-selection:

MOLS> ./gcMols 7 data/SpecsCollection/LSred "" count - -first "" ""


2. Counting reduced symmetric unipotent ls's for N=8, using minimum-domain
   for the variable-selection and all propagation-levels:

MOLS> ./gcMols 8 data/SpecsCollection/LSredsymmuni "" count "-" "mindom" "" ""
# N=8
# k=1 total_num_sq=1
# num_ps=0
# rt=count-solutions
# num_runs=4
# propagation: domain-prop default-prop values-prop bounds-prop
# variable-heuristics: min-dom-var
# order-heuristics: bin-branch-min
domain-prop min-dom-var bin-branch-min 6240 0.046 360765 64 12607 11
default-prop min-dom-var bin-branch-min 6240 0.032 302824 1000 14479 13
values-prop min-dom-var bin-branch-min 6240 0.029 253886 1000 14479 13
bounds-prop min-dom-var bin-branch-min 6240 0.051 325700 487 13453 12

*/

/* TODOS:

-1. As an option: output the count resp. the solutions immediately when
   obtained.
    - So that one has results when the whole computation needs to be aborted.
    - One can activate this by using "+count" and "+enum".
    - These should likely go to Options::RT.
    - Perhaps three functions "is_sat, is_count, is_enum" for RTs are
      then helpful.
    - Additionally, when catching SIGUSR1, output the current results.

0. R-header
    - Can't hurt for now if always output.

1. Output:
    - The information on the conditions should include the filename.
    - Perhaps the output-filename should contain a hash of the parameters.

2. Defaults:
    - We have for propagation-level the strongest, and for variable-choice
      the a weak selection currently as default.
    - Perhaps we should have, after some experimentation, for all three choices
      "the best".
    - Or perhaps just changing the current var-selection-default to using
      min-dom by default.

3. Using the short forms of options for the statistics-output:
    - So that it becomes shorter (and it's easier to remember, since the
      same as for input).
    - Perhaps all output should use the short forms (other than the help)?

4. Further options for Gecode:
    - DONE Figure 9.7 in MPG.pdf: threads, clone in any case.
    - Later considering cutoff (for restarts) and stop (terminating
      when taking too long).

5. Implement an additional parallelisation-mode "scanning"
    - In our standard way, for running the combinations in parallel.
    - The thread-commandline-input then just becomes a pair, separated
      by comma (first the scan-threads, then the Gecode-threads).
    - If gecode-threads != 1, output a warning when counting or enumerating
      (that for a large number of solutions it becomes inefficient, since
      only one counting/output-instance (bottleneck)).

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
        "0.7.3",
        "4.4.2022",
        __FILE__,
        "Oliver Kullmann and Oleg Zaikin",
        "https://github.com/OKullmann/OKlib-MOLS/blob/master/Satisfiability/Solvers/Gecode/MOLS/gcMols.cpp",
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
      " - branchvar  : " << Environment::WRP<BHV>{} << "\n" <<
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
  const list_bhv_t bvarv = read_opt<BHV>(argc, argv, 6, "bvar",
                                        "variable-heuristics");
  const list_bho_t bordv = read_opt<BHO>(argc, argv, 7, "bord",
                                        "order-heuristics");
  const double threads = read_threads(argc, argv);

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
  Environment::out_line(std::cout, bvarv);
  std::cout << "\n# order-heuristics: ";
  Environment::out_line(std::cout, bordv);
  if (with_output) std::cout << "\n# output-file " << outfile;
  std::cout << std::endl;

  for (const PropO po : pov) {
    const EncCond enc(ac, ps, prop_level(po));
    for (const BHV bvar : bvarv)
      for (const BHO bord : bordv) {
        const GBasicSR res =
          solver_gc(enc, rt, var_branch(bvar), val_branch(bord), threads);
        std::cout << po<<" "<<bvar<<" "<<bord<<" " << res.b.sol_found << " ";
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
