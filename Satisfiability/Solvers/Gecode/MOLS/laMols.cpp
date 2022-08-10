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

1. Counting the 432 Euler-squares (pairs of mutually orthogonal latin squares)
of order 6, which are row-reduced:

MOLS> ./laMols 5 "@squares A B aux\nls A B aux\nrred A B\nrprod B aux A\n" "" count dom enu wdL "" "" 1 3 "0.1" "" ""
# command-line: "./laMols" "5" "@squares A B aux\nls A B aux\nrred A\nrred B\nrprod B aux A\n" "" "count" "dom" "enu" "wdL" "" "" "1" "3" "0.1" "" ""
# N: 5
# k=3 total_num_sq=3: "@squares A B aux\nls A B aux\nrred A\nrred B\nrprod B aux A\n"
#   num_uc=5 num_eq=0 num_peq=1
# no_ps
# num_runs=1
# threads=3
# rt=count-solutions(count)
# no_stopping
# output-options: show-info(+info) show-weights(+w) show-headers(+headers) perform_computations(+computations) all no-negation-sign(+sign) show-stopped(+stop) arithmetic-mean(ave) inner-node(inode)
#   propagation-level: domain-prop(dom)
#   la-branching-type: enumerative-branching(enu)
#   distance-type: weighted-delta-literals(wdL)
#   la-order-heuristic: ascending-order(asc)
#   la-reduction-type: relaxed-pruning(relpr)
#   commit-distance: 1
#   weights: 0.1 0.1 0.1 -> 0 0 1 1.0717734625362931642 1.1486983549970350067 1.2311444133449162843
  N       rt  pl lbt  dis   lbo    lar gcd     satc           t        ppc st      nds      lvs          nsel
  5    count dom enu  wdL   asc  relpr   1      432       0.406       5225  0      125       72  4.754312e-02
   mu0    qfppc  pprunes  pmprune  pprobes   rounds  solc         tr  pelvals       dp
148.38  0.67925   8.8165   137.55   173.45   1.6792     0  0.0045545  0.62896    2.566
   143        0        0   133.33   133.14        1     0   0.001784        0        0
   180        1    16.35   138.46   200.69        2     0   0.010505   1.3793        3
8.5355  0.47123   6.3903   1.1415    26.46  0.47123     0  0.0018559  0.51141  0.72083
    mu0    qfppc  pprunes  pmprune  pprobes   rounds  solc         tr  pelvals  dp
 129.67   1.4135     6.52   48.707   118.75      1.5     6   0.003619   15.222   4
    129     1.25   1.1628   23.077   64.615        1     6   0.000965   13.846   4
    130   1.6667   12.308   88.462   176.15        2     6    0.01024   17.054   4
0.47471  0.10048   3.0145   18.665   40.289  0.50351     0  0.0015323  0.76313   0
      estlvs
7.216275e+01
6.739697e+01
7.454563e+01
3.393564e+00
    dm0        w  ltausp      minp     meanp      maxp        sdd         tb
0.90566   2.3396  2.7476   0.44239   0.44497   0.45012  0.0036448  0.0040173
      0        2  1.0456      0.25      0.25      0.25          0   0.001605
      2        4  3.4742       0.5       0.5       0.5   0.016098   0.006283
0.74069  0.51677  0.8992  0.085275  0.081622  0.074711  0.0068016  0.0012064

*/

/*

BUGS:

*/

/* TODOS:

See Todos in rlaMols, gcMols and LookaheadBranching.

-4. Output of tree-logging
   - DONE Fixed file for output; name as for output of solutions, but
     with "TREE" instead of "SOLUTIONS".
   - DONE Also for rlaMols.
   - DONE Should (r)laParams know about this, by annother boolean "tree_logging"?
     Perhaps that's best here, since there are some difficulties associated
     with tree-logging.
   - The data provided is:
      - DONE the node-data: id, pid, branch, and depth (the last item is
        redundant, but should help to check for correctness of the
        interpretation);
        DONE (known now)
        the width of the branching is not available at this point, which
        is a problem for rlaMols, so perhaps, in RlaBranching::commit,
        the computation of v is most foreward for non-leaves?

        Perhaps rlaMols should have branching data (as laMols has)?
        Likely best leaving for a later revision.
      - DONE branching-data for inner nodes: variable and values
      - DONE ReductionStatistics
      - MeasureStatistics, BranchingStatistics (these two only for laMols)
      - also only for laMols (for inner nodes): tau-value, and for the
        branches distances and probabilities.
   - New formatting options "-tree", "+tree".
   - DONE (without changing the existing system)
     The add-member in rlaStats gets a new (reference-)parameter, the
     NodeData (in GenericMols1): from this the depth is read, and the id is
     set to the current (total) node-count (starting with 1).
     (So ReductionStatistics doesn't need the depth-variable anymore.)
   - In this function also the basic tree-data (node-data and reduction-
     statistics) are output (if activated, that is, iff the pointer
     is not null).
   - DONE (not via add, but using dedicated functions)
     Both overloads of "add" in laStats use the pointer from the rlaStats-
     object and output further statistics (measures and branching-statistics).
   - Possibly for the output of tau-value and branching variable,
     and for the distances, probabilities and values of the branches, there
     is an additional function.
   - DONE Actually branching-variable and branching-values are also needed for
     rlaMols (while tau and probs are only for laMols).
     So this output-function belongs to rlaStats.
   - For binary branching one needs to indicate whether the value is set or
     unset; possibly by using "+" and "-" (where one needs to distinguish
     between +0 and -0).
   - DONE Another member needs to be added to NodeData, identifying the
     branch; this is set in update_clone, with new signature
       update_clone(unsigned a).
   - Perhaps it should be an option to call flush after every addition
     of data to the file?

-3. Reject non-positive weights for wdL
   - This leads to infinite loops.
   - Perhaps one should also provide a "strict" input-format, where the
     exact number of weights must be provided (no extension or cutting),
     in order to avoid input-errors.

-2. Complete the info:
   - program-name, version-number

-1. Controlling the output:
   - "+-cond": output of full conditions (+cond -> +info).
     normal and batchmode: off.
   - "+-commentout": also statistics-output with leading "#"
     normal: on, batchmode: off.
   - "+-hex": in case of solution-output, use hexadecimal notation.
   - Perhaps for the weights-info also N could be output, together with
     the number of essential weights?
     Like "N=8,numw=6". One could also state the number of given weights.

0. Provide global statistics "open total assignments"
   - Let muld be the measure log_2(# total assignments), where the
     number of total assignments is N^(N^2 * K), where
       K = cond.num_squares() (so n = N^2 * K).
   - This is not most efficient for solving, but it is very natural;
     so perhaps it should always be provided:
     - DONE
       Likely "vals" for la-reduction should be the measure mu0 (so
       that it is zero when the problem has been solved); currently it is
       "sumdomsizes".
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
   - A problem here is that pseudo-leaves should also compute their branchings,
     so that ota is accurate for them.
     So perhaps pseudoleaves should have their data submitted to the global
     statistics not together with the leaves, but later, together with
     the inner nodes.
     This seems also appropriate for the other branching-measures, to obtain
     them for the pseudoleaves (perhaps best separately).
   - All distances compute pairs (distance, Delta muld).
     After the reduction, exp2(muld) is subtracted, and after the
     branching has been computed, for each branch exp2(newmuld)
     is added.
   - The node itself maintains muld (like depth): the commit updates
     this value (subtracting Delta muld).
   - Perhaps what is reported is log2(ota) (the smaller this number,
     the more work has been done).
     One likely needs to output this number with full precision.
   - Perhaps more informative is the ratio
       # total-assignments-handled / # total assignments
       = (exp2(muld_root) - ota) / exp2(muld_root)
       = 1 - ota / exp2(muld_root).
   - float80 should be sufficient for a good deal of problems.

   - CHANGE: it seems more informative to have each node reporting
     on the change in ota; see "Update member vals_ of BranchingStatistics"
     in LookaheadBranching.hpp.
     Additionally it should also be interesting, now as a global variable,
     to see the total current change achieved (the above "ota").
     This is just one global variable, maintained by the branching statistics.

1. Early abortion of runs
    - Allowing bounds for total (wallclock) runtime; which should perhaps
      anyway be part of the general statistics (not separately handled
      by the solver).

2. Early abortion of branches
   - Considering now say "depth >= 10" or "mu <= 10000" for making a node,
     after la-reduction, a leaf.
   - Usual syntax: "depth, 10" and "mu, 10000".
   - The current handling of "leafcount" needs to be generalised.
   - Then we need to count three types of leaves: falsified, satisfied,
     and "completed-early" (perhaps "closed"?).
   - One could also include criteria related to the branching, e.g.,
     ltau too bad.
   - So we handle this after the branching-computation, just expanding
     S->add(stats0, stats1) by adding the information on whether we
     have a "pseudo-leaf" (no branching), and in such a case adding
     the reduction-statistics to the third type of reduction-statistics.
     We have an early check-point for pseudo-leaves, which can then
     expand the operation S->add(stats0).
   - Instead of just having "ClosingData" (similar to "StoppingData"),
     now this object also handles the decision whether the branch
     is to be closed.

3. Better values for qfppc
   - In case no values were eliminated, perhaps qfppc=1 is then
     more appropriate.

*/


#include <iostream>
#include <string>
#include <ostream>
#include <fstream>
#include <string_view>

#include <ProgramOptions/Environment.hpp>
#include <Numerics/NumInOut.hpp>
#include <Numerics/NumBasicFunctions.hpp>

#include "Conditions.hpp"
#include "Encoding.hpp"
#include "PartialSquares.hpp"
#include "Solvers.hpp"
#include "Options.hpp"
#include "CommandLine.hpp"
#include "LookaheadReduction.hpp"
#include "LookaheadBranching.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.100.3",
        "8.8.2022",
        __FILE__,
        "Oliver Kullmann and Oleg Zaikin",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Solvers/Gecode/MOLS/laMols.cpp",
        "GPL v3"};

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  using namespace Conditions;
  using namespace Encoding;
  using namespace PartialSquares;
  using namespace Solvers;
  using namespace Options;
  using namespace CommandLine;
  using namespace LookaheadReduction;
  using namespace LookaheadBranching;

  constexpr int commandline_args = commandline_args_laMols;

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg <<
      " has " << commandline_args << " command-line arguments:\n"
      " N  file_cond  file_ps  run-type\n"
      "   prop-level  branch-type  distance  branch-order  la-type  gcd\n"
      "   threads  weights  (stop-type,stop-value)*  formatting\n\n"
      " - N            : \";\"-separated list of \"a[,b][,c]\"-sequences\n"
      " - file_cond    : filename/string for conditions-specification\n"
      " - file_ps      : filename/string for partial-squares-specification\n"
      " - run-type     : " << Environment::WRPO<RT>{} << "\n" <<
      " - prop-level   : " << Environment::WRPO<PropO>{} << "\n" <<
      " - branch-type  : " << Environment::WRPO<LBRT>{} << "\n" <<
      " - distance     : " << Environment::WRPO<DIS>{} << "\n" <<
      " - branch-order : " << Environment::WRPO<LBRO>{} << "\n" <<
      " - la-type      : " << Environment::WRPO<LAR>{} << "\n" <<
      " - gcd          : Gecode commit-distance; list as for N\n"
      " - threads      : floating-point for number of threads\n"
      " - weights      : comma-separated list of weights for distance\n"
      "   - specials   : " << Environment::WRPO<SPW>{} << "\n" <<
      "   - variables  : " << Environment::WRPO<EXW>{} << "\n" <<
      " - stop-type    : " << Environment::WRPO<LRST>{} << "\n" <<
      " - formatting   : comma-separated list of\n" <<
      "   - info       : " << Environment::WRPO<Info>{} << "\n" <<
      "   - weights    : " << Environment::WRPO<Weights>{} << "\n" <<
      "   - headers    : " << Environment::WRPO<Headers>{} << "\n" <<
      "   - compute    : " << Environment::WRPO<Computations>{} << "\n" <<
      "   - values     : " << Environment::WRPO<SIVA>{} << "\n" <<
      "   - negation   : " << Environment::WRPO<NEG>{} << "\n" <<
      "   - stop-info  : " << Environment::WRPO<STOP>{} << "\n" <<
      "   - stat-type  : " << Environment::WRPO<STAT>{} << "\n" <<
      "   - node-type  : " << Environment::WRPO<NOTY>{} << "\n\n" <<
      "Here\n"
      "  - to use a string instead of a filename, a leading \"@\" is needed\n"
      "  - file_ps can be the empty string (no partial instantiation)\n"
      "  - the six algorithmic options can be lists (all combinations)\n"
      "    - these lists can have a leading + (inclusion) or - (exclusion)\n"
      "  - for branch-order \"rand\" a comma-separated seed-sequence can be"
      " given after \";\"\n"
      "    - this sequence can include \"t\" (timestamp) and \"r\" (random)\n"
      "  - weights are patterns, with the last entry used for filling (thus"
      " the tail is always constant)\n"
      "    - the default for weights (empty string) is \"all specials\"\n"
      "    - for input \"cin\", the weights are read from standard-input,"
      " space-separated,\n"
      "        and batch-mode is used (no additionaly info-output)\n"
      "  - stop-values are unsigned int; times in seconds\n"
      "    - pairs of stop-types/values are separated by \"|\"\n"
      "  - formatting uses the given defaults for fields not specified\n"
      "    - these are flipped in batch-mode for the first three fields)\n"
      "    - the final three fields are only relevant for single-values\n"
      "  - for sat-solving and enumeration, output goes to file \"" <<
      "SOLUTIONS_" << proginfo.prg << "_N_timestamp\".\n\n"
;
    return true;
  }


  OutputOptions read_output_options(const std::string& s) {
    return Environment::translate<output_options_t>()(s,',');
  }
  void output_options(std::ostream& out, const OutputOptions& outopt) {
    out << "# output-options: ";
    Environment::print1d(out, outopt.options, {0});
    out << "\n";
  }

  constexpr size_t wnsel = 13;
  void rh(std::ostream& out) {
    out.width(wN); out << "N" << " ";
    Environment::header_policies<RT, PropO, LBRT, DIS, LBRO, LAR>(out);
    out.width(wgcd); out << "gcd" << " ";
    out << std::string(sep_spaces, ' ');
    rh_genstats(out);
    out << " "; out.width(wnsel); out << "nsel";
    /* to be actived once pseudo-leaves are possible:
    out << " "; out.width(wnds); out << "plvs";
    */
    out << "\n";
  }

  void rs(std::ostream& out, const laSR& res, const bool with_headers) {
    const auto state = FloatingPoint::fixed_width(out, precision);
    out << std::string(sep_spaces, ' ');
    rs_genstats(out, res);
    {const auto state = FloatingPoint::engineering_width(out, precision_engineering);
     using LookaheadBranching::float_t;
     const float_t lvs = res.S[1].N();
     const float_t variance = res.mS.sum()[0] - lvs*lvs;
     const float_t normalised_stddev = FloatingPoint::sqrt(variance) / lvs;
     out << " "; out.width(wnsel); out << normalised_stddev;
     FloatingPoint::undo(out, state);
    }
    /* to be actived once pseudo-leaves are possible:
    {const auto plvs = res.mS.N() - res.S[1].N();
     out << " "; out.width(wnds); out << plvs;}
    */
    out << "\n";

    {const auto state = FloatingPoint::default_width(out, precision+2);
     res.outS(out, with_headers);
     FloatingPoint::undo(out, state);
    }
    res.outmS(out, with_headers);
    {const auto state = FloatingPoint::default_width(out, precision+2);
     res.outbS(out, with_headers);
     FloatingPoint::undo(out, state);
    }
    FloatingPoint::undo(out, state);
  }
  void select(std::ostream& out, const laSR& res,
              const SIVA sv, const STAT st, const NOTY nt, const bool neg) {
    assert(sv != SIVA::all);

    const auto val = [&res,st,nt](const std::string& s){
      const auto i = ReductionStatistics::index(s);
      const size_t nti = size_t(nt);
      assert(nti < res.S.size());
      const auto& R = res.S[nti];
      switch (st) {
      case STAT::ave : return R.amean()[i];
      case STAT::min : return R.min()[i];
      case STAT::max : return R.max()[i];
      case STAT::stddev : return R.sd_corrected()[i];
      default :std::ostringstream ss;
        ss << "ERROR[laMols::single::val]: STAT-value " << int(st) <<
          " not handled.\n";
        throw std::runtime_error(ss.str());} };
    const auto valm = [&res,st](const std::string& s){
      const auto i = MeasureStatistics::index(s);
      const auto& R = res.mS;
      switch (st) {
      case STAT::ave : return R.amean()[i];
      case STAT::min : return R.min()[i];
      case STAT::max : return R.max()[i];
      case STAT::stddev : return R.sd_corrected()[i];
      default :std::ostringstream ss;
        ss << "ERROR[laMols::single::valm]: STAT-value " << int(st) <<
          " not handled.\n";
        throw std::runtime_error(ss.str());} };
    const auto valb = [&res,st](const std::string& s){
      const auto i = BranchingStatistics::index(s);
      const auto& R = res.bS;
      switch (st) {
      case STAT::ave : return R.amean()[i];
      case STAT::min : return R.min()[i];
      case STAT::max : return R.max()[i];
      case STAT::stddev : return R.sd_corrected()[i];
      default :std::ostringstream ss;
        ss << "ERROR[laMols::single::valb]: STAT-value " << int(st) <<
          " not handled.\n";
        throw std::runtime_error(ss.str());} };

    if (neg) out << "-";
    switch (sv) {
    case SIVA::satc : out << res.b.sol_found; return;
    case SIVA::t : out << res.ut; return;
    case SIVA::ppc : out << res.gs.propagate; return;
    case SIVA::nds : out << res.S[0].N()+res.S[1].N(); return;
    case SIVA::inds : out << res.S[0].N(); return;
    case SIVA::lvs : out << res.S[1].N(); return;

    case SIVA::mu0 : out << val("mu0"); return;
    case SIVA::qfppc : out << val("qfppc"); return;
    case SIVA::pprunes : out << val("pprunes"); return;
    case SIVA::pmprune : out << val("pmprune"); return;
    case SIVA::pprobes : out << val("pprobes"); return;
    case SIVA::rounds : out << val("rounds"); return;
    case SIVA::solc : out << val("solc"); return;
    case SIVA::tr : out << val("tr"); return;
    case SIVA::pelvals : out << val("pelvals"); return;
    case SIVA::dp : out << val("dp"); return;

    case SIVA::mu1 : out << valb("mu1"); return;
    case SIVA::w : out << valb("w"); return;
    case SIVA::ltau : out << valb("ltau"); return;
    case SIVA::minp : out << valb("minp"); return;
    case SIVA::meanp : out << valb("meanp"); return;
    case SIVA::maxp : out << valb("maxp"); return;
    case SIVA::sdd : out << valb("sdd"); return;
    case SIVA::tb : out << valb("tb"); return;

    case SIVA::estlvs : out << valm("estlvs"); return;

    default: {
      std::ostringstream ss;
      ss << "ERROR[laMols::single]: SIVA-value " << int(sv) <<
        " not handled.\n";
      throw std::runtime_error(ss.str());} }
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
      " arguments needed, but the real number is " << argc-1 << ":\n";
    commandline_output(std::cerr, argc, argv);
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
  const auto [brov, randgen, seeds] = read_lbro(argc, argv, 8);
  const list_lar_t larv = read_opt<LAR>(argc, argv, 9, "lar",
                                        "lookahead-reduction");
  const list_unsigned_t gcdv = read_comdist(argc, argv, 10);

  const double threads = read_threads(argc, argv, 11);
  if (threads != 1 and randgen) {
    std::cerr << error << "In the presence of branching-order rand the"
      " number of threads must be 1, but is " << threads << ".\n";
    return 1;
  }

  const auto [wg, batch_mode] = read_weights(argc, argv, 12);

  const size_t num_runs =
    mult(pov.size()*brov.size()*larv.size()*gcdv.size(),
         list_N, brtv, disv, wg);
  if (num_runs != 1 and batch_mode) {
    std::cerr << error << "In batch-mode the number of runs must be 1, but is "
              << num_runs << ".\n";
    return 1;
  }

  const auto stod = read_rlast(argc, argv, 13);

  OutputOptions::set_def(batch_mode);
  const auto outopt = read_output_options(argv[14]);


  const std::string outfile = output_filename(proginfo.prg, list_N);

  const bool with_file_output = Options::with_file_output(rt);
  if (with_file_output and batch_mode) {
    std::cerr << error << "In batch-mode there can not be file-output.\n";
    return 1;
  }
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
  if (with_log and batch_mode) {
    std::cerr << error << "In batch-mode there can not be log-output.\n";
    return 1;
  }
  std::ostream* const log = with_log ? &std::cout : nullptr;

  if (outopt.with_info()) {
    commandline_output(std::cout, argc, argv);
    info_output(std::cout,
                list_N, ac, name_ac, ps0, name_ps, rt,
                num_runs, threads, outfile, with_file_output);
    st_output(std::cout, stod);
    output_options(std::cout, outopt);
    algo_output(std::cout, std::make_tuple(pov, brtv, disv, brov, larv));
    if (randgen) oseed_output(std::cout, seeds);
    cd_output(std::cout, gcdv);
    wseed_output(std::cout, wg);
    std::cout.flush();
  }

  for (const size_t N : list_N)
    for (const PropO po : pov) {
      const EncCond enc(ac, ps0 ? ps0.value() : PSquares(N,psquares_t{}),
                        prop_level(po));
      for (const LBRT brt : brtv)
        for (const DIS dis : disv) {
          const auto wv = wg(N, brt, dis);
          for (const auto& weights0 : wv) {
            if (outopt.with_weights()) weights_output(std::cout, weights0);
            const weights_t* const weights = &weights0.w;
            for (const LBRO bro : brov)
              for (const LAR lar : larv)
                for (unsigned gcd : gcdv) {
                  const laSR res = outopt.with_computations() ?
                    lasolver(enc, rt, brt, dis, bro, lar,
                             gcd, threads, weights,
                             needs_randgen(bro) ? randgen.get() : nullptr,
                             stod, log, nullptr) :
                    laSR{};
                  if (with_log and
                      rt != RT::enumerate_with_log and
                      rt != RT::unique_s_with_log)
                    std::cout << "\n";
                  if (outopt.single_valued()) {
                    const auto old =
                      FloatingPoint::fullprec_float80(std::cout);
                    select(std::cout, res, outopt.values(),
                           outopt.stat(),outopt.node_type(),outopt.negated());
                    if (outopt.with_stop())
                      std::cout << " " << res.stopped;
                    std::cout << std::endl;
                    std::cout.precision(old);
                  }
                  else {
                    if (outopt.with_headers()) rh(std::cout);
                    std::cout.width(wN); std::cout << N << " ";
                    Environment::data_policies(std::cout,
                      std::make_tuple(rt, po, brt, dis, bro, lar));
                    std::cout.width(wgcd); std::cout << gcd << " ";
                    rs(std::cout, res, outopt.with_headers());
                    if (with_file_output)
                      Environment::out_line(*out, res.b.list_sol, "\n");
                    std::cout.flush();
                  }
                }
          }
        }
    }
  delete out;
}
