// Oliver Kullmann, 27.3.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  A Gecode-based solver for general MOLS-LS-related problems.
  using (only) branching strategies as provided by Gecode itself

  Remarks on the statistics output:
   - "t" is the solver-time.
   - "prop" is the number of propagations -- but what does that mean?
   - "flvs" is the number of unsatisfiable leaves in the backtracking tree
     -- but what is the "backtracking tree"?
   - "nds" is the number of nodes in the backtracking tree
     -- again, what is the "backtracking tree"?
   - "h" is the depth of the backtracking tree
     -- again, what is the precise definition?

Examples:

1. Counting reduced ls's for N=7
    - for all propagation-levels
    - the default branching-type (binary branching)
    - all variable-selections except of first-var,
    - and the default branching-order:

MOLS> ./gcMols 6 data/SpecsCollection/LSred "" count "-" "" -first "" 1


2. Counting reduced symmetric unipotent ls's for N=8, using minimum-domain
   for the variable-selection and all propagation-levels:

MOLS>  ./gcMols 8 data/SpecsCollection/LSredsymmuni "" count "-" "" "mindom" "" ""
# N=8
# k=1 total_num_sq=1
# num_ps=0
# num_runs=4
# threads=1
# rt=count-solutions(count)
#   propagation: domain-prop(dom) default-prop(def) values-prop(val) bounds-prop(bnd)
#   branching-type: binary-branching(binbr)
#   variable-heuristics: min-dom-var(mindom)
#   order-heuristics: ascending-order(asc)
dom binbr mindom asc 6240 0.046 360765 64 12607 11
def binbr mindom asc 6240 0.030 302824 1000 14479 13
val binbr mindom asc 6240 0.029 253886 1000 14479 13
bnd binbr mindom asc 6240 0.050 325700 487 13453 12

*/

/* TODOS:

-2. OZ: Specify *precisely* the four statistics-outputs of Gecode
        ("prop flvs nds h"):
         - "prop" = calls to propagation (does this include the lookahead)?
         - "flvs": "failed leaves" -- what are they?
         - "nds": does every branching created the corresponding "nodes"?
         - "h": how does this relate to the backtracking-tree (maximal
           size of call-stack)?

-1. When catching SIGUSR1, output the current results:
     - According to
       https://en.cppreference.com/w/cpp/utility/program/signal
       the signal-handler must not access "an object with thread storage
       duration", and thus we couldn't have (easily) parallelisation for
       scanning.
     - At least for rlaMols, that shouldn't be a problem: the signal-handler
       can just output rlaStats.
     - For gcMols, the early abortion-mechanisms (seem below) need suffice.

0. R-header
    - Perhaps we need another option:
       - +- header
       - +- comments
       - +- computation
       - +- cond

1. Output:
    - More statistics on conditions should be output:
       - perhaps by default actually the complete conditions are reported,
         using options "+-cond".
    - More statistics on the partial squares given should be output:
       - total number of squares involved
       - total number of cells involved
       - total number of removed values.
    - Solution-output:
       - hex-output as an option; perhaps as "+-hex"
    - Perhaps the output-filename should contain a hash of the parameters.
    - Progress-bar:
     - As the OKsolver has it, but now using float80; compare there.
     - Only with laMols.
     - The total number of total assignments is the starting point:
         N^(N^2 * k).
     - Whenever a leaf is reached, the number of covered total assignments
       is subtracted.
     - Alternatively one could have a monitoring-depth (see also the OKsolver).

2. Defaults:
    - We have for propagation-level the strongest, and for variable-choice
      a weak selection currently as default.
    - Perhaps we should have, after some experimentation, for all three choices
      "the best".
    - Or perhaps just changing the current var-selection-default to using
      min-dom by default.

3. Implement an additional parallelisation-mode "scanning"
    - In our standard way, for running the combinations in parallel.
    - We also need parallelism when optimising weights.
    - Shouldn't be a problem with gcMols, but for rlaMols, the static
      objects in rlaStats and RlaBranching need to be put into a (non-movable)
      vector, so that each of the scan-threads does have its own objects.
    - The thread-commandline-input can then become a pair, separated
      by ":" (first the scan-threads, then the Gecode-threads).
    - If gecode-threads != 1, output a warning when counting or enumerating
      (that for a large number of solutions it becomes inefficient, since
      only one counting/output-instance (bottleneck)).

4. Cutoff-time
    - Specify a cutoff-time (wall-clock), after which computation is stopped.
    - Especially useful for scanning (searching for the best possibility).
    - Using the Gecode-stop-option can do that.
    - One stores the target wall-clock-time in the stop-object, and then
      for every check calls the current-wall-clock.
    - This parameter goes together with the threads-parameter,
      comma-separated, with trailing arguments optional.
    - Another stopping-criterion is the number of nodes (as given by
      Gecode).
    - Another statistics-item is needed, which specifies whether stopping
      took place (and for what reason).

5. Perhaps also for the N-parameter a range of values should be possible.


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
        "0.10.7",
        "22.5.2022",
        __FILE__,
        "Oliver Kullmann and Oleg Zaikin",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Solvers/Gecode/MOLS/gcMols.cpp",
        "GPL v3"};

  const std::string error = "ERROR[" + proginfo.prg + "]: ";
  constexpr int commandline_args = 9;

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
      " branch-order threads\n\n"
      " - file_cond    : filename for conditions-specification\n"
      " - file_ps      : filename for partial-squares-specification\n"
      " - run-type     : " << Environment::WRPO<RT>{} << "\n" <<
      " - prop-level   : " << Environment::WRPO<PropO>{} << "\n" <<
      " - branch-type  : " << Environment::WRPO<BRT>{} << "\n" <<
      " - branch-var   : " << Environment::WRPO<BHV>{} << "\n" <<
      " - branch-order : " << Environment::WRPO<GBO>{} << "\n" <<
      " - threads      : floating-point for number of threads\n\n"
      "Here\n"
      "  - file_ps can be the empty string (no partial instantiation)\n"
      "  - the four algorithmic options can be lists (all combinations)\n"
      "  - these lists can have a leading + (inclusion) or - (exclusion)\n"
      "  - for sat-solving and enumeration, output goes to file \"" <<
      "SOLUTIONS_" << proginfo.prg << "_N_timestamp\".\n\n"
;
    return true;
  }

  void rh(std::ostream& out) {
    out << "pl bt bh bo \t";
    GBasicSR::rh(out); out << std::endl;
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
  const size_t num_runs = pov.size()*brtv.size()*bvarv.size()*gbov.size();

  const double threads = read_threads(argc, argv, 9);

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

  const bool with_log = Options::with_log(rt);
  std::ostream* const log = with_log ? &std::cout : nullptr;

  info_output(std::cout,
              N, ac, name_ac, ps, name_ps, rt,
              num_runs, threads, outfile, with_file_output);
  algo_output(std::cout, std::make_tuple(pov, brtv, bvarv, gbov));
  std::cout.flush();

  if (num_runs != 1) rh(std::cout);
  for (const PropO po : pov) {
    const EncCond enc(ac, ps, prop_level(po));
    for (const BRT brt : brtv)
      for (const BHV bvar : bvarv)
        for (const GBO gbo : gbov) {
          const BHO bord = translate(brt, gbo);
        const GBasicSR res =
          solver_gc(enc, rt, var_branch(bvar), val_branch(bord), threads, log);
        if (with_log and
            rt != RT::enumerate_with_log and rt != RT::unique_s_with_log)
          std::cout << "\n";
        if (num_runs == 1) rh(std::cout);
        using Environment::W0;
        std::cout << W0(po) << " "
                  << W0(brt) << " " << W0(bvar) << " " << W0(gbo) << " \t";
        res.rs(std::cout);
        std::cout << std::endl;
        if (with_file_output)
          Environment::out_line(*out, res.b.list_sol, "\n");
      }
  }
  if (out) delete out;
}
