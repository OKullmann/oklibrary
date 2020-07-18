// Oliver Kullmann 25.4.2020 (Swansea)
/* Copyright 2020 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

USAGE:

> ./Queens_RUCP_ct
16 1 0 0  14772512 47318154 179766962 7386256 64236195 5735944

uses the built-in N (N=16 by default), and outputs statistics (see below).
floor((N+1)/2 )parallel threads are used.
Via "-h" help-information is obtained, via "-v" version-information.

Use
 - "make SETN=-DNUMQUEENS=X" for compilation with N=X
 - "make SET(E)RTYPES=-D(E)RTYPES=0/1" for using different implementations
   of rows and extended rows (see below)
 - "make SETBTYPES=-DBTYPES=0/1" for backtracking in recursive/non-recursive form.

More conveniently, use

> ./Call_QueensRUCPct [N=16] [rt=1] [ert=0] [bt=0]

for performing first the compilation with
 - NUMQUEENS = N
 - RTYPES = rt
 - ERTYPES = ert,
 - BTYPES = bt
and then running the program, output with an R-header and additional
performance data. E.g.

> ./Call_QueensRUCPct 15 0 0 1
N rt ert bt  sol nds uc r2s r2u cu  ut wt st pp mm
15 0 0 1  2279184 7995732 29812560 1248961 10966795 901275  0.72 2.67 0.00 372% 6832

Classes Row, Row_uint implement one row of the board, while ExtRow,
ExtRow_uint implement the extensions for the diagonals. "uint" means
that the implementation uses unsigned integers, while otherwise std::bitset
is used.
Via macros RTYPES, ERTYPES these choices are regulated, with values
 0=bitset, 1=uint.
See Call_QUeensRUCPct for details.

The core function is ucp(Board&), which performs complete row-unit-propagation,
while for columns exactly the inconsistencies are detected (i.e., empty
columns).


TODOS:

-1. OK Replace std::is_pod_v by
      std::is_standard_layout_v and std::is_trivial_v
    since it's deprecated with C++20.
    - The most important aspect (for us) seems to be captured by
      std::is_trivially_copyable.
    - std:is_trivial adds requirements on construction.
    - The question is where this should be placed.
      DONE: for now placing the macro is_pod(X) into central places.


0. AB, OK Write tests (in the usual dedicated testfile).
    - The statistics need to be precisely determined for small cases (at least
      up to N=5) by hand.
    - Also the behaviour of UCP needs to be exactly specified and tested.
    - Start test-program. DONE
    - Also application-tests are needed; see GenericSAT, or better,
      Quantification/DQCNF.

1. OK, AB See basic todos in Board.hpp, Rows.hpp, ExtRows.hpp.

2. AB Document the various concepts (rows, extended rows, boards).

3. OK After 0-2, version 1.0 is reached.

4. AB, OK Symmetry-breaking is done in main; likely this should be outsourced.

5. AB The node-counts need to be compared to that of the complete approach
   ("TCA2": GenericSAT, standard propagation, FirstRow-heuristics), and that of
   the restricted approach ("TCA1": NQueens.cpp).

6. OK, AB: Setting the three implementation-macros should be improved.

*/

#include <iostream>
#include <future>
#include <vector>

#include <cassert>

#include <ProgramOptions/Environment.hpp>

#include "../GenericSAT/Recursion.hpp"

#include "Dimensions.hpp"
#include "Rows.hpp"
#include "ExtRows.hpp"
#include "Statistics.hpp"
#include "Board.hpp"
#include "Backtracking.hpp"

namespace {

const Environment::ProgramInfo proginfo{
      "0.11.8",
      "18.7.2020",
      __FILE__,
      "Oliver Kullmann",
      "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Queens/SimpleBacktracking/Queens_RUCP_ct.cpp",
      "GPL v3"};


  using namespace Dimensions;

  // Abbreviations for the implementation-choices:
  typedef Rows::ChoiceRT<rt> R;

const Environment::addvo_fot AO = [](std::ostream& out) {
  out << "\n** Constants: **\n"
         "  N=" << N << "\n"
         "  Row-type     : " << rt << "\n"
         "  Backtracking-type : " << bt << "\n"
;
};


bool show_usage(const int argc, const char* const argv[]) {
  assert(argc >= 1);
  if (not Environment::help_header(std::cout, argc, argv, proginfo))
    return false;
  std::cout <<
    "> " << proginfo.prg << "\n"
    " runs the program for built-in\n"
    "   N   = " << N << "\n"
    "   rt  = " << int(rt) << "\n"
    "   ert = " << int(ert) << "\n"
    "   bt  = " << int(bt) << ",\n outputting the line\n"
    "   N rt ert bt solution_count node_count unit-clauses\n"
    " plus the number of leaves realised via ucp as satisfiable or row/column-unsatisfiable."
    "\n\n"
    "> ./Call_QueensRUCPct [N=16] [rt=1] [ert=0]\n"
    " compiles optimised and debugging forms of this program, and runs\n"
    " the optimised version, with added user-time (s) and max-memory (kb).\n"
;
  return true;
}

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv, AO))
    return 0;
  if (show_usage(argc, argv)) return 0;

  using Statistics::NodeCounts;
  std::vector<std::future<NodeCounts>> jobs;
  std::vector<NodeCounts> results;
  NodeCounts res(true);

  // The following ad-hoc scheme for symmetry-breaking works since only for
  // N=4 we have some i with ucp-decision and some without, and here there
  // are no "gaps" in jobs/results, since i=0 has no ucp-decision.
  for (size_t i = 0; i < N; ++i) {
    Board::DoubleSweep<R> B(i);
    B.ucp(res);
    if (not B.decided()) {
      if (i < (N+1)/2) {
        if constexpr (bt == Btypes::recursive)
          jobs.push_back(std::async(std::launch::async,
                                  Backtracking::count<R>, B));
        else
          jobs.push_back(std::async(std::launch::async,
                                  Backtracking::countnr<R>, B));
        results.push_back({});
      }
      else
        results[(N-1) - i].add_duplication();
    }
    else assert(N <= 4);

  }
  assert(jobs.size() == results.size());
  assert(N > 3 or jobs.empty());
  for (size_t i = 0; i < jobs.size(); ++i) results[i] += jobs[i].get();
  for (const auto& r : results) res += r;

  if (res.num_sols() != Recursion::exact_value(N)) {
    std::cerr << "\nERROR[" << proginfo.prg << "]: The statistics are\n"
              << res << "\nbut the correct count for N=" << N
              << " is " << Recursion::exact_value(N) << ".\n\n";
    return 1;
  }
  std::cout << N << " " << int(rt) << " " << int(ert) << " " << int(bt) <<
   "  " << res << "\n";

}
