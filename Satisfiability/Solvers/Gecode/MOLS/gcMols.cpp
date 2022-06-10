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
   - "prop" is the number of calls of the function propagate(). This function
     performs contraint propagation for a set of variables and a constraint.
     Details are described in sections I and II of the documentation.
   - "flvs" is the number of unsatisfiable leaves in the backtracking tree
     -- but what is the "backtracking tree"?
   - "nds" is the number of nodes in the backtracking tree
     -- again, what is the "backtracking tree"?
   - "h" is the depth of the backtracking tree
     -- again, what is the precise definition?

Examples:

1. Counting reduced ls's for N=6
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

Compare also with todos in rlaMols.

-6. Better time-information
     - Additional to total user-time we need also total wallclock-time.

-5. Improve parameter on propagation-level
     - The Gecode-text mentions in Subsection 4.3.5 the two further
       possibilities "IPL_BASIC" and "IPL_ADVANCED", which we should
       offer.
     - Furthermore it is stated that all six possibilities can form
       "disjunctions". It is not said what that means, but perhaps it
       means "union".
     - So perhaps the true value of pl is that of a bit-field with 6
       bits, where at least one bit is set.
     - We can use the order "dom,def,val,bnd,bas,adv".
     - It seems "def,val,bnd <= dom".
     - But apparently NOT "bas <= adv".

-4. Add parameters for "cloning-distance" and "cloning-adaptation":
     - This are the variables res.c_d, res.a_d = 1 for the
       make_options functions;
     - DONE We can use value "0" for "ignore" for c_d, or we use the empty
       sequence. But having 0 in a sequence is useful.
     - Perhaps we combine these two (related) parameters into one, using
       "|" for separation (if only one component, then this means c_d).
     - DONE The c_d-component is a sequence.
     - It seems sensible to use rations 0 <= r <= 1 for a_d, where then
         a_d = r * c_d.
     - For this RandGen::Prob64 (in Random/Numbers.hpp) can be used.
     - We allow a comma-separated list of such fractions.
     - A rounding-function is needed to get the result of r * c_d.
       Likely a member-function of Prob64; perhaps the best rounding-mode
       here is rounding away from zero (or "rounding to odd"?).
     - Is a_d = 0 useful? Seems to have the same effect as c_d = 1.
       So it is not really needed; perhaps the rounding mode should be
       "rounding up" (always for halfway cases).
       Or perhaps using always the ceiling?
     - So we can use say "1,5 | 0/1, 1/2, 1/1", which considers <= 5 * 3 = 15
       possibilities (c_d = 1, ..., 5, a_d = 0, 1/2, 1).
     - When we have a nonempty a_d-list, then also c_d must be non-empty.
     - For a given c_d-list, one should contract equal a_d-values (keeping
       the first); but actually sorting the a_d-values (with duplicates
       removed) should be avoided (unclear which order is best)>


-3. For mode "count", why do we get different trees (numbers of nodes etc.)
    between "asc" and "desc"?
     - All branches need to be explored, and there is no learning, thus
       the counts should be equal?!?

-2. OZ: Specify *precisely* the four statistics-outputs of Gecode
        ("prop flvs nds h"):
         - "ppc" = calls to propagation
           (apparently not including the lookahead for (r)laMols)
         - "flvs": "failed leaves" -- unsatisfiable leaves of the
           backtracking-tree
         - "gnds": these are our nodes plus leaves (the "search tree"
           of Gecode makes leaves always only-children of the current
           node)
         - "gd": maximal size of call-stack -- apparently always height
            of backtracking tree + 1 or + 2
         - For (r)laMols: it seems we always have
             lvs = flvs, larc + flvs = gnds.

-1. Catching SIGUSR1, output the current results:
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
        "0.14.0",
        "10.6.2022",
#ifndef SIMBRANCH
        __FILE__,
#else
        "s" __FILE__,
#endif
        "Oliver Kullmann and Oleg Zaikin",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Solvers/Gecode/MOLS/gcMols.cpp",
        "GPL v3"};

  const std::string error = "ERROR[" + proginfo.prg + "]: ";
  constexpr int commandline_args = 11;

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
      " N file_cond file_ps run-type prop-level branch-type"
      " branch-var branch-order gcd threads stop-type,stop-value\n\n"
      " - N            : \";\"-separated list of \"a[,b][,c]\"-sequences\n"
      " - file_cond    : filename/string for conditions-specification\n"
      " - file_ps      : filename/string for partial-squares-specification\n"
      " - run-type     : " << Environment::WRPO<RT>{} << "\n" <<
      " - prop-level   : " << Environment::WRPO<PropO>{} << "\n" <<
      " - branch-type  : " << Environment::WRPO<BRT>{} << "\n" <<
      " - branch-var   : " << Environment::WRPO<BHV>{} << "\n" <<
      " - branch-order : " << Environment::WRPO<GBO>{} << "\n" <<
      " - gcd          : Gecode commit-distance; list as for N\n"
      " - threads      : floating-point for number of threads\n"
      " - stop-type    :  " << Environment::WRPO<STO>{} << "\n\n" <<
      "Here\n"
      "  - file_ps can be the empty string (no partial instantiation)\n"
      "  - to use a string instead of a filename, a leading \"@\" is needed\n"
      "  - the four algorithmic options can be lists (all combinations)\n"
      "  - these lists can have a leading + (inclusion) or - (exclusion)\n"
      "  - stop-values are unsigned int; times in seconds\n"
      "  - for sat-solving and enumeration, output goes to file \"" <<
      "SOLUTIONS_" << proginfo.prg << "_N_timestamp\"\n"
      "  - for file-output, solutions are stored and transferred at the end\n"
      "  - for logging-output, solutions are not stored.\n\n"
;
    return true;
  }

  constexpr size_t sep_spaces = 2;
  constexpr size_t prec = 3;
  const Environment::wvec_t widths{9, 11, 13, 11, 11, 6, 2};
  constexpr size_t wN = 4, wgcd = 5;

  void rh(std::ostream& out) {
    out.width(wN); out << "N" << " ";
    Environment::header_policies<RT, PropO, BRT, BHV, GBO>(out);
    out.width(wgcd); out << "gcd" << " ";
    out << std::string(sep_spaces, ' ');
    Environment::print1d(out,
      std::make_tuple("satc", "t", "ppc", "flvs", "gnds", "gd", "st"),
      widths);
    out << std::endl;
  }

  void rs(std::ostream& out, const GBasicSR& res) {
    const auto state = FloatingPoint::fixed_width(out, prec);
    out << std::string(sep_spaces, ' ');
    Environment::print1d(out,
      std::make_tuple(res.b.sol_found, res.ut,
        res.gs.propagate, res.gs.fail, res.gs.node, res.gs.depth, res.stopped),
      widths);
    out.flush();
    FloatingPoint::undo(out, state);
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

  const auto list_N = read_N(argc, argv);
  const auto [ac, name_ac] = read_ac(argc, argv);
  const auto [ps0, name_ps] = read_ps(argc, argv, list_N);
  const RT rt = read_rt(argc, argv);

  const list_propo_t pov = read_opt<PropO>(argc, argv, 5, "po",
                                           "propagation");
  const list_brt_t brtv = read_opt<BRT>(argc, argv, 6, "brt",
                                        "branching-type");
  const list_bhv_t bvarv = read_opt<BHV>(argc, argv, 7, "bvar",
                                        "gc-variable-heuristics");
  const list_gbo_t gbov = read_opt<GBO>(argc, argv, 8, "gbo",
                                        "gc-order-heuristics");
  const list_unsigned_t gcdv = read_comdist(argc, argv, 9);

  const size_t num_runs =
    list_N.size()*pov.size()*brtv.size()*bvarv.size()*gbov.size()*gcdv.size();

  const double threads = read_threads(argc, argv, 10);
  const GcStoppingData stod = read_gcst(argc, argv, 11);

  const std::string outfile = output_filename(proginfo.prg, list_N);

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
              list_N, ac, name_ac, ps0, name_ps, rt,
              num_runs, threads, outfile, with_file_output);
  st_output(std::cout, stod);
  algo_output(std::cout, std::make_tuple(pov, brtv, bvarv, gbov));
  cd_output(std::cout, gcdv);
  std::cout.flush();

  if (num_runs != 1) rh(std::cout);
  for (const size_t N : list_N)
    for (const PropO po : pov) {
      const EncCond enc(ac, ps0 ? ps0.value() : PSquares(N,psquares_t{}),
                        prop_level(po));
      for (const BRT brt : brtv)
        for (const BHV bvar : bvarv)
          for (const GBO gbo : gbov)
            for (unsigned gcd : gcdv) {
              const GBasicSR res =
                solver_gc(enc, rt, brt, bvar, gbo, gcd, threads, stod, log);
              if (with_log and
                  rt != RT::enumerate_with_log and rt != RT::unique_s_with_log)
                std::cout << "\n";
              if (num_runs == 1) rh(std::cout);
              std::cout.width(wN); std::cout << N << " ";
              Environment::data_policies(std::cout,
                std::make_tuple(rt, po, brt, bvar, gbo));
              std::cout.width(wgcd); std::cout << gcd << " ";
              rs(std::cout, res);
              std::cout << std::endl;
              if (with_file_output)
                Environment::out_line(*out, res.b.list_sol, "\n");
            }
    }
  if (out) delete out;
}
