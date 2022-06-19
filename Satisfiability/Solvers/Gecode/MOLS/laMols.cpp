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

1. Counting all 18 Euler-squares (mutually orthogonal latin squares)
of order 6, which are reduced:

MOLS> ./laMols 5 "@squares A B aux\nls A B aux\nred A\nrred B\nrprod B aux A\n" "" count dom enu wdL "" "" 1 3 "" ""
# N: 5
# k=3 total_num_sq=3: "@squares A B aux\nls A B aux\nred A\nrred B\nrprod B aux A\n"
#   num_uc=5 num_eq=0 num_peq=1
# no_ps
# num_runs=1
# threads=3
# rt=count-solutions(count)
# no_stopping
#   propagation-level: domain-prop(dom)
#   la-branching-type: enumerative-branching(enu)
#   distance-type: weighted-delta-literals(wdL)
#   la-order-heuristic: ascending-order(asc)
#   la-reduction-type: relaxed-pruning(relpr)
#   commit-distance: 1
#   weights: 0 0 1 2 3 4
  N       rt  pl lbt  dis   lbo    lar gcd     satc           t        ppc     flvs     gnds    gd st      nds      lvs    inds
  5    count dom enu  wdL   asc  relpr   1       18       0.119        129        3        7     2  0        4        3       1
      vals      props     elvals     prunes      mprune      probes    rounds      solc        tr   qelvals   qprunes
   210.250     11.500     15.500     14.500     131.500     246.750     1.750     4.500     0.022     0.075     0.059
   206.000      0.000      0.000     12.000      97.000     192.000     1.000     0.000     0.014     0.000     0.047
   223.000     17.000     23.000     17.000     204.000     277.000     2.000     6.000     0.034     0.112     0.064
     8.500      7.853     10.661      2.887      48.966      37.792     0.500     3.000     0.009     0.052     0.008
        mu         w      ltau       mind      meand       maxd       sdd        dp        tb
   148.000     3.000     0.065     17.000     17.000     17.000     0.000     0.000     0.026
   148.000     3.000     0.065     17.000     17.000     17.000     0.000     0.000     0.026
   148.000     3.000     0.065     17.000     17.000     17.000     0.000     0.000     0.026
     0.000     0.000     0.000      0.000      0.000      0.000     0.000     0.000     0.000

Explicitly given parameters:
 - propagation-level "pl"
 - branching-type "lbt"
 - distance "wdL"
 - implementation-detail "gcd"
 - number of parallel threads = 3.
 - stopping parameter (no stopping by default).
For branching-order ("lbo"), some details on the
reduction-implementation ("lar"), and the weights
the default is used (indicated by the empty string --
all parameters need always to be specified).

*/

/*

BUGS:

*/

/* TODOS:

See Todos in rlaMols, gcMols and LookaheadBranching.

0. Provide global statistics "open total assignments"
   - Let muld be the measure log_2(# total assignments).
   - This is not most efficient for solving, but it is very natural;
     so perhaps it should always be provided:
     - Likely "vals" for la-reduction should be the measure mu0 (so
       that it is zero when the problem has been solved).
     - "mu" for wdL is naturally as given by the weights (generalising
       mu0).
     - For newvars possibly mu then is muld; but we always want it?
   - We want to have as global variable the number of open total
     assignments -- this can be used as a good achievement-measure
     also for unsatisfiable instances.
   - This global variable ota can be rather easily computed by setting
     the variable initially to exp2(muld), and then after every
     la-reduction, for the original muld0 before the reduction and muld
     after the reduction:
         ota := ota - exp2(muld0) + exp2(muld).
     - And after the branching has been computed, the current ota is updated
       in the same way, for each child.
     - Since this update is needed for each branch (so that the update can
       "pick up the old value" exp2(muld0)), we only compute ota for
       laMols.
     - Then we don't need the update in the middle, but can use in the
       above ota-formula "exp2(muld)" for each child, except for the case
       that we found a leaf, where then "exp2(muld) = 0", that is, only
       the subtraction takes place.
   - Perhaps what is reported is log2(ota) (the smaller this number,
     the more work has been done).
     One likely needs to output this number with full precision.
   - Perhaps more informative is the ratio
       # total-assignments-handled / # total assignments
       = (exp2(muld_root) - ota) / exp2(muld_root)
       = 1 - ota / exp2(muld_root).
   - float80 should be sufficient for a good deal of problems.

1. Early abortion of runs
    - Later also allowing bounds for total runtime; which should perhaps
      anyway be part of the general statistics (not separately handled
      by the solver).

2. Early abortion of branches
   - Considering now say "depth > 10" or "mu < 10000" for making a node,
     after la-reduction, a leaf.
   - Usual syntax: "depth, 10" and "mu, 10000".
   - The current handling of "leafcount" needs to be generalised.
   - Then we need to count three types of leaves: falsified, satisfied,
     and completed-early.
   - One could also include criteria related to the branching, e.g.,
     ltau too bad.
   - So perhaps here we handle this after the branching-computation?

3. Once we checked all statistics, remove the gecode-statistics on nodes
   and failed leaves.

4. Should the reporting of ltau use engineering notation?

*/


#include <iostream>
#include <string>
#include <ostream>
#include <fstream>
#include <string_view>

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
        "0.13.2",
        "19.6.2022",
        __FILE__,
        "Oliver Kullmann and Oleg Zaikin",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Solvers/Gecode/MOLS/laMols.cpp",
        "GPL v3"};

  const std::string error = "ERROR[" + proginfo.prg + "]: ";
  constexpr int commandline_args = 13;

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
      " distance branch-order la-type gcd threads weights"
      " (stop-type,stop-value)*\n\n"
      " - N            : \";\"-separated list of \"a[,b][,c]\"-sequences\n"
      " - file_cond    : filename/string for conditions-specification\n"
      " - file_ps      : filename/string for partial-squares-specification\n"
      " - run-type     : " << Environment::WRPO<RT>{} << "\n" <<
      " - prop-level   : " << Environment::WRPO<PropO>{} << "\n" <<
      " - branch-type  : " << Environment::WRPO<LBRT>{} << "\n" <<
      " - distance     : " << Environment::WRPO<DIS>{} << "\n" <<
      " - branch-order : " << Environment::WRPO<LBRO>{} << "\n" <<
      " - la-reduction : " << Environment::WRPO<LAR>{} << "\n" <<
      " - gcd          : Gecode commit-distance; list as for N\n"
      " - threads      : floating-point for number of threads\n"
      " - weights      : comma-separated list of weights for distance\n"
      "   - specials   : " << Environment::WRPO<SPW>{} << "\n" <<
      "   - variables  : " << Environment::WRPO<EXW>{} << "\n" <<
      " - stop-type    : " << Environment::WRPO<LRST>{} << "\n\n" <<
      "Here\n"
      "  - file_ps can be the empty string (no partial instantiation)\n"
      "  - to use a string instead of a filename, a leading \"@\" is needed\n"
      "  - the six algorithmic options can be lists (all combinations)\n"
      "  - these lists can have a leading + (inclusion) or - (exclusion)\n"
      "  - weights are patterns, with the last entry used for filling (thus"
      " the tail is always constant)\n"
      "  - stop-values are unsigned int; times in seconds\n"
      "  - pairs of stop-types/values are separated by \"|\"\n"
      "  - for sat-solving and enumeration, output goes to file \"" <<
      "SOLUTIONS_" << proginfo.prg << "_N_timestamp\".\n\n"
;
    return true;
  }

  constexpr size_t sep_spaces = 0;
  constexpr size_t prec = 3;
  const Environment::wvec_t widths{8, 11, 10, 8, 8, 5, 2, 8, 8, 7};
  constexpr size_t wN = 3, wgcd = 4;

  void rh(std::ostream& out) {
    out.width(wN); out << "N" << " ";
    Environment::header_policies<RT, PropO, LBRT, DIS, LBRO, LAR>(out);
    out.width(wgcd); out << "gcd" << " ";
    out << std::string(sep_spaces, ' ');
    Environment::print1d(out,
      std::make_tuple("satc", "t", "ppc", "flvs", "gnds", "gd", "st",
                      "nds", "lvs", "inds"),
      widths);
    out << "\n";
  }

  void rs(std::ostream& out, const laSR& res) {
    const auto state = FloatingPoint::fixed_width(out, prec);
    out << std::string(sep_spaces, ' ');
    Environment::print1d(out,
      std::make_tuple(res.b.sol_found, res.ut,
        res.gs.propagate, res.gs.fail, res.gs.node, res.gs.depth, res.stopped,
        res.S.N(), res.lvs, res.S1.N()),
      widths);
    out << "\n";
    res.S.out(out, {"vals", "props", "elvals", "prunes",
                    "mprune", "probes", "rounds", "solc",
                    "tr", "qelvals", "qprunes"});
    res.S1.out(out, {"mu", "w", "ltau",
                   "mind", "meand", "maxd", "sdd", "dp", "tb"});
    FloatingPoint::undo(out, state);
  }

  size_t mult(const size_t basis, const list_size_t& list_N,
              const list_lbrt_t& brtv, const list_dis_t& disv,
              const WGenerator& wg) noexcept {
    size_t sum = 0;
    for (const size_t N : list_N)
      for (const LBRT brt : brtv)
        for (const DIS dis : disv)
          sum += wg.size(N, brt, dis);
    return basis * sum;
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
  const list_lbrt_t brtv = read_opt<LBRT>(argc, argv, 6, "brt",
                                          "branching-type");
  const list_dis_t disv = read_opt<DIS>(argc, argv, 7, "dis",
                                        "distance");
  const list_lbro_t brov = read_opt<LBRO>(argc, argv, 8, "bro",
                                          "order-heuristics");
  const list_lar_t larv = read_opt<LAR>(argc, argv, 9, "lar",
                                        "lookahead-reduction");
  const list_unsigned_t gcdv = read_comdist(argc, argv, 10);

  const double threads = read_threads(argc, argv, 11);

  const WGenerator wg = read_weights(argc, argv, 12);

  const size_t num_runs =
    mult(pov.size()*brov.size()*larv.size()*gcdv.size(),
         list_N, brtv, disv, wg);

  const auto stod = read_rlast(argc, argv, 13);

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
  algo_output(std::cout, std::make_tuple(pov, brtv, disv, brov, larv));
  cd_output(std::cout, gcdv);
  seed_output(std::cout, wg);
  std::cout.flush();

  for (const size_t N : list_N)
    for (const PropO po : pov) {
      const EncCond enc(ac, ps0 ? ps0.value() : PSquares(N,psquares_t{}),
                        prop_level(po));
      for (const LBRT brt : brtv)
        for (const DIS dis : disv) {
          const auto wv = wg(N, brt, dis);
          for (const auto& weights0 : wv) {
            weights_output(std::cout, weights0);
            const weights_t* const weights = &weights0.w;
            for (const LBRO bro : brov)
              for (const LAR lar : larv)
                for (unsigned gcd : gcdv) {
                  const laSR res =
                    lasolver(enc, rt, brt, dis, bro, lar,
                             gcd, threads, weights, stod, log);
                  if (with_log and
                      rt != RT::enumerate_with_log and
                      rt != RT::unique_s_with_log)
                    std::cout << "\n";
                  rh(std::cout);
                  std::cout.width(wN); std::cout << N << " ";
                  Environment::data_policies(std::cout,
                    std::make_tuple(rt, po, brt, dis, bro, lar));
                  std::cout.width(wgcd); std::cout << gcd << " ";
                  rs(std::cout, res);
                  if (with_file_output)
                    Environment::out_line(*out, res.b.list_sol, "\n");
                  std::cout.flush();
                }
          }
        }
    }
  if (out) delete out;
}
