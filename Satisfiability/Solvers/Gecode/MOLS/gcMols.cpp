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

1. Counting all 9408 reduced ls's for N=6
    - for all propagation-levels
    - the default branching-type (binary branching)
    - all variable-selections except of first-var,
    - the default branching-order,
    - commit-distance 1,
    - 1 thread,
    - and no stopping:

MOLS> ./gcMols 6 data/SpecsCollection/LSred "" count "-" "" -first "" 1 1 ""
...

2. Counting all 6240 reduced symmetric unipotent ls's for N=8, using minimum-domain
   for the variable-selection and all propagation-levels:

MOLS> ./gcMols 8 data/SpecsCollection/LSredsymmuni "" count "-" "" "mindom" "" 1 1 ""
# N: 8
# k=1 total_num_sq=1: "data/SpecsCollection/LSredsymmuni"
#   num_uc=4 num_eq=0 num_peq=0
# no_ps
# num_runs=4
# threads=1
# rt=count-solutions(count)
# no_stopping
#   propagation-level: domain-prop(dom) default-prop(def) values-prop(val) bounds-prop(bnd)
#   branching-type: binary-branching(bin)
#   variable-heuristic: min-dom-var(mindom)
#   order-heuristic: ascending-order(asc)
#   commit-distance: 1
   N       rt  pl  bt        bv   bo  gcd        satc           t           ppc        flvs        gnds     gd st
   8    count dom bin    mindom  asc    1        6240       0.062        270963          64       12607      9  0
   8    count def bin    mindom  asc    1        6240       0.030        236479        1000       14479      9  0
   8    count val bin    mindom  asc    1        6240       0.028        206400        1000       14479      9  0
   8    count bnd bin    mindom  asc    1        6240       0.043        261107         487       13453      9  0

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

1. Output:
    - More statistics on the partial squares given should be output:
       - total number of squares involved
       - total number of cells involved
       - total number of removed values.
    - Perhaps the output-filename should contain a hash of the parameters.
    - Progress-bar:
     - One could have a monitoring-depth (see also the OKsolver).

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
        "0.15.0",
        "24.6.2022",
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
      " - stop-type    : " << Environment::WRPO<STO>{} << "\n\n" <<
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

  const Environment::wvec_t widths{9, 11, 13, 11, 11, 6, 2};

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
    const auto state = FloatingPoint::fixed_width(out, precision);
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

  commandline_output(std::cout, argc, argv);
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
