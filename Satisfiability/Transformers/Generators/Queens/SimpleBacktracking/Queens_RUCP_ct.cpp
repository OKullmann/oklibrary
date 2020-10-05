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
floor((N+1)/2) parallel threads are used.
Via "-h" help-information is obtained, via "-v" version-information.

Use
 - "make SETN=-DNUMQUEENS=X" for compilation with N=X
 - "make SETBTYPES=-DBTYPES=0/1" for backtracking in recursive/non-recursive form.

More conveniently, use

> ./Call_QueensRUCPct [N=16] [bt=1]

for performing first the compilation with
 - NUMQUEENS = N
 - BTYPES = bt
and then running the program, output with an R-header and additional
performance data. E.g.

> ./Call_QueensRUCPct 15 1
N bt  sol nds uc r2s r2u cu  ut wt st pp mm
15 1  2279184 7995732 29812560 1248961 10966795 901275  0.54 1.85 0.00 342% 6752

Class Row implements one row of the board, while
XXX

The core function is DoubleSweep::ucp(Statistics&), which performs complete
row-unit-propagation, while for columns exactly the inconsistencies are
detected (i.e., empty column-clauses).


BUGS:

1. All timings should be backed up (especially if they are new) by the
   corresponding log-data.
    - OZ needs to add all the missing data.
    - DONE Such log entries needed to be commited in the same commit as the
      change to overview.
    - DONE The one entry we have for amd1 is incomplete: for the first time, it
      must contain all data.
    - DONE The entry for amd1, 0.18.14, in Overview is inconsistent with the log-
      entry.

2. The change of version 0.18.19 shouldn't have been done, since it clearly
   makes runtime worse on the server.
    - Times for N=19 are missing (especially crucial to judge the development).

3. DONE In recent commits, OZ appears with three different e-mails addresses in the
   Git-history: There should be exactly one.
    - This just needs to take care of the git-configuration for each machine.

4. DONE Timing results for version 0.18.17 are missing.
    - In commit 3cfef4d9f2f05d7f974f01ef3691902be04fd5c4 there was data
      entered in Overview.
    - This was then silently overwritten in commit
      fb68f8f33d459b6a80f72e0ed165ebeb7ab37a59 (this should not happen;
      and there is no indication about that in the commit-message).
    - Best to check all data, together with Bug No 1 above.

5. In commit 801f7903ef93d884b69084b3c03ef52524abaaf3, information was
   lost without explanation of why removing it.
    - Wrapping lower to -1 was problematic.
    - bottom,top was beneficial on the server (the main machine), and thus
      likely needs to be retained.
    - When commenting on runtime-developments, the corresponding
      version-numbers and the data used should be given (otherwise the
      argumentation can not be checked).

6.  The style of the OKlibrary is to use complete sentences for
    commit-messages.
    - This is in line with the general philosophy of explicitness and
      precision.
    - It also emphasises that a commit should be as complete as possible.
      Currently the commits of OZ look a bit

TODOS:

-1. OK Replace std::is_pod_v by
      std::is_standard_layout_v and std::is_trivial_v
    since it's deprecated with C++20.
    - The moBst important aspect (for us) seems to be captured by
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

1. OK, AB See basic todos in Board.hpp, Rows.hpp.

2. AB Document the various concepts (rows, extended rows, boards).

3. OK After 0-2, version 1.0 is reached.

4. AB The node-counts need to be compared to that of the complete approach
   ("TCA2": GenericSAT, standard propagation, FirstRow-heuristics), and that of
   the restricted approach ("TCA1": NQueens.cpp).

5. OK, AB: Setting the two implementation-macros should be improved.

*/

#include <iostream>

#include <cassert>

#include <ProgramOptions/Environment.hpp>

#include "../GenericSAT/Recursion.hpp"

#include "Dimensions.hpp"
#include "SymmetryBreaking.hpp"

namespace {

const Environment::ProgramInfo proginfo{
      "0.18.19",
      "28.9.2020",
      __FILE__,
      "Oliver Kullmann",
      "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Queens/SimpleBacktracking/Queens_RUCP_ct.cpp",
      "GPL v3"};


using namespace Dimensions;

const Environment::addvo_fot AO = [](std::ostream& out) {
  out << "\n** Constants: **\n"
         "  N=" << N << "\n"
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
    "   bt  = " << int(bt) << ",\n outputting the line\n"
    "   N bt num_branches  solution_count node_count unit-clauses  min_nds_branch max_nds_branch\n"
    " plus the number of leaves realised via ucp as satisfiable or row/column-unsatisfiable."
    "\n\n"
    "> ./Call_QueensRUCPct [N=16] [bt=1]\n"
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
  std::cout << N << " " << int(bt) << " " << SymmetryBreaking::width_branching();
  std::cout.flush();

  using Statistics::NodeCounts;
  SymmetryBreaking::job_v jobs;
  SymmetryBreaking::result_v results;
  Statistics::NodeCounts res(true);
  SymmetryBreaking::branching(jobs, results, res);

  std::cout.flush();
  for (sizet i = 0; i < jobs.size(); ++i) results[i] += jobs[i].get();
  for (const auto& r : results) res += r;

  if (res.num_sols() != Recursion::exact_value(N)) {
    std::cerr << "\nERROR[" << proginfo.prg << "]: The statistics are\n"
              << res << "\nbut the correct count for N=" << N
              << " is " << Recursion::exact_value(N) << ".\n\n";
    return 1;
  }
  std::cout << "  " << res;

  if (results.empty()) std::cout << "  NA NA\n";
  else {
    std::sort(results.begin(), results.end());
    std::cout << "  " << results.front() << " " << results.back() << "\n";
  }

}
