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

#include "Conditions.hpp"
#include "Encoding.hpp"
#include "PartialSquares.hpp"
#include "Solvers.hpp"
#include "Options.hpp"
#include "CommandLine.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.5.2",
        "4.6.2022",
        __FILE__,
        "Oliver Kullmann and Oleg Zaikin",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Solvers/Gecode/MOLS/laMols.cpp",
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
      " distance branch-order la-type gcd threads\n\n"
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
      " - threads      : floating-point for number of threads\n\n"
      "Here\n"
      "  - file_ps can be the empty string (no partial instantiation)\n"
      "  - to use a string instead of a filename, a leading \"@\" is needed\n"
      "  - the six algorithmic options can be lists (all combinations)\n"
      "  - these lists can have a leading + (inclusion) or - (exclusion)\n"
      "  - for sat-solving and enumeration, output goes to file \"" <<
      "SOLUTIONS_" << proginfo.prg << "_N_timestamp\".\n\n"
;
    return true;
  }

  constexpr size_t sep_spaces = 2;
  constexpr size_t prec = 3;
  const Environment::wvec_t widths{8, 11, 10, 8, 8, 5, 8, 8};
  constexpr size_t wN = 4, wgcd = 5;

  void rh(std::ostream& out) {
    out.width(wN); out << "N" << " ";
    Environment::header_policies<RT, PropO, LBRT, DIS, LBRO, LAR>(out);
    out.width(wgcd); out << "gcd" << " ";
    out << std::string(sep_spaces, ' ');
    Environment::print1d(out,
      std::make_tuple("satc", "t", "ppc", "flvs", "gnds", "gd", "larc", "lvs"),
      widths);
    out << "\n";
  }

  void rs(std::ostream& out, const laSR& res) {
    const auto state = FloatingPoint::fixed_width(out, prec);
    out << std::string(sep_spaces, ' ');
    Environment::print1d(out,
      std::make_tuple(res.b.sol_found, res.ut,
                      res.gs.propagate, res.gs.fail, res.gs.node, res.gs.depth,
                      res.S.N(), res.lvs),
      widths);
    out << "\n";
    res.S.out(out, {"vals", "props", "elvals", "prunes",
                    "mprune", "probes", "rounds", "solc",
                    "tr", "qelvals", "qprunes"});
    res.S1.out(out, {"tb"});
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
  const list_lbrt_t brtv = read_opt<LBRT>(argc, argv, 6, "brt",
                                          "branching-type");
  const list_dis_t disv = read_opt<DIS>(argc, argv, 7, "bvar",
                                        "distance");
  const list_lbro_t brov = read_opt<LBRO>(argc, argv, 8, "gbo",
                                          "order-heuristics");
  const list_lar_t larv = read_opt<LAR>(argc, argv, 9, "lar",
                                        "lookahead-reduction");
  const list_unsigned_t gcdv = read_comdist(argc, argv, 10);
  const size_t num_runs =
    list_N.size()*pov.size()*brtv.size()*disv.size()*brov.size()
    *larv.size()*gcdv.size();

  const double threads = read_threads(argc, argv, 11);

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
  algo_output(std::cout, std::make_tuple(pov, brtv, disv, brov, larv));
  additional_output(std::cout, gcdv);
  std::cout.flush();

  for (const size_t N : list_N)
    for (const PropO po : pov) {
      const EncCond enc(ac, ps0 ? ps0.value() : PSquares(N,psquares_t{}),
                        prop_level(po));
      for (const LBRT brt : brtv)
        for (const DIS dis : disv)
          for (const LBRO bro : brov)
            for (const LAR lar : larv)
              for (unsigned gcd : gcdv) {
                const laSR res =
                  lasolver(enc, rt, brt, dis, bro, lar,
                           gcd, threads, log);
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
              }
    }
  if (out) delete out;
}
