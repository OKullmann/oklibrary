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

0. R-header

1. DONE Output:
    - DONE The standard R-style statistics (one line per run),
    - DONE For satisfying assignments the filename should
     - concatenate the given files,
     - plus the other paramaters
     - plus a timestamp.

2. Defaults:
    - We have for propagation-level the strongest, and for variable-choice
      the weakest selection.
    - Perhaps we should have, after some experimentation, for all three choices
      "the best".
    - Or perhaps just changing the current state by using min-dom by default.

3. Using the short forms of options for the statistics-output:
    - So that it becomes shorter (and it's easier to remember, since the
      same as for input).
    - Perhaps all output should use the short forms (other than the help)?

4. Further options for Gecode:
    - DONE Figure 9.7 in MPG.pdf: threads, clone in any case.
    - Later considering cutoff (for restarts) and stop (terminating
      when taking too long).

5. As an option: output the count resp. the solutions immediately when
   obtained.
    - So that one has results when the whole computation needs to be aborted.
    - Alternatively, when catching SIGURS1, output the current results.
      If this can be done, it would be better.

BUGS:

1. The Gecode-threads-mode seems completely broken:

MOLS> time ./gcMols 7 data/SpecsCollection/LSred "" count "val" "mindom" "" 1
real	0m37.914s
user	0m37.910s
sys	0m0.000s
MOLS> time ./gcMols 7 data/SpecsCollection/LSred "" count "val" "mindom" "" 2
real	2m19.322s
user	3m9.529s
sys	2m9.353s
MOLS> time ./gcMols 7 data/SpecsCollection/LSred "" count "val" "mindom" "" 3
real	2m1.541s
user	3m22.992s
sys	2m15.741s
MOLS> time ./gcMols 7 data/SpecsCollection/LSred "" count "val" "mindom" "" 4
real	1m57.393s
user	3m40.753s
sys	2m34.876s
MOLS> time ./gcMols 7 data/SpecsCollection/LSred "" count "val" "mindom" "" 5
real	2m3.880s
user	3m57.730s
sys	3m14.865s
MOLS> time ./gcMols 7 data/SpecsCollection/LSred "" count "val" "mindom" "" 6
real	2m5.210s
user	4m13.834s
sys	3m33.966s
MOLS> time ./gcMols 7 data/SpecsCollection/LSred "" count "val" "mindom" "" 0
real	2m18.693s
user	5m9.369s
sys	5m41.515s

There seems to be one global resource which blocks all parallelism.

So well, it thus seems necessary to roll our own thread-mechanism, which
just runs the different runs in parallel (handling as usual).
This mode should then also sort the results.

Trying on other computer:
MOLS$ time ./gcMols 7 data/SpecsCollection/LSred "" count "val" "mindom" "" 1
real	0m38.891s
user	0m38.884s
sys	0m0.004s
real	2m36.434s
user	3m24.301s
sys	2m13.847s

The bug is confirmed.


Investigating parallelism on the golomb-ruler example application:
$gecode-release-6.3.0/examples$ time ./golomb-ruler -threads 1 12
real	0m57.184s
user	0m57.174s
sys	0m0.008s
$gecode-release-6.3.0/examples$ time ./golomb-ruler -threads 2 12
real	0m31.030s
user	1m0.145s
sys	0m0.466s
$gecode-release-6.3.0/examples$ time ./golomb-ruler -threads 4 12
real	0m10.930s
user	0m41.911s
sys	0m0.494s

On this example, the parallelisation works quite well.
So it seems that the bug is problem-dependent.


The problem is in the counting (or enumeration): there one "global"
object res (in Solvers::gcsolver_basis) is accessed, and since there are
very many solutions, this is be the bottleneck.
For example for unsatisfiable instances of LSredsymmuni parallelism works.

How to do it differently in the Gecode-framework??
Doesn't seem possible; one should ask the forum.

So for now, possibly disabling threads != 1 for counting- and
enumeration-mode. If there are few solutions then it actually should
work, so perhaps a warning here?

Sure, since the main focus of the lookahead is on the sat-decision and
sat-solving modes, maybe for now throw a warning if threads != 1 is used for
counting and enumeration-mode. Counting suits well for testing the
correctness since the number of solutions can be compared. For this purpose 1
thread is enough.

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
        "0.7.0",
        "2.4.2022",
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
