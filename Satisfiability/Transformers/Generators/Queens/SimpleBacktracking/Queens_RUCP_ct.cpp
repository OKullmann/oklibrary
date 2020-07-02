// Oliver Kullmann 25.4.2020 (Swansea)
/* Copyright 2020 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

USAGE:

> ./Queens_RUCP_ct
16 1 1 14772512 47318154 179766962 7386256 64236195 5735944

uses the default N=16, and output

 use
 - "make SETN=-DNUMQUEENS=X" for compilation to have N=X
 - "make SET(E)RTYPES=-D(E)RTYPES=0/1" for using different implementations
   (see below).

(N+1)/2 parallel threads are used.
Via "-h" help-information is obtained, via "-v" version-information.

More conveniently, use

> ./Call_QueensRUCPct [N] [rt] [ert]

for performing first the compilation with
 - NUMQUEENS = N
 - RTYPES = rt
 - ERTYPES = ert,
and then running the program, with an R-header, and additional performance
data. E.g.

 ./Call_QueensRUCPct 15 0 0
N rt ert sol nds uc r2s r2u cu ut wt st pp mm
15 0 0 2279184 7995732 29812560 1248961 10966795 901275 0.66 2.34 0.00 352% 6816

Classes Row, Row_uint implement one row of the board, while ExtRow,
ExtRow_uint implement the extensions for the diagonals. "uint" means
that the implementation uses unsigned integers, while otherwise std::bitset
is used.
Via macros RTYPES, ERTYPES these choices are regulated, with values
 0=bitset, 1=uint (the default).
See Call_QUeensRUCPct for details.

The core function is ucp(Board&), which performs complete row-unit-propagation,
while for columns exactly the inconsistencies are detected (i.e., empty
columns).


TODOS:

-1. OK Replace std::is_pod_v by
      std::is_standard_layout_v and std::is_trivial_v
    since it's deprecated with C++20.
    - The question is where this should be placed.
      DONE: for now placing the macro is_pod(X) into central places.


0. AB, OK Write tests (in the usual dedicated testfile).
    - The statistics need to be precisely determined for small cases (at least
      up to N=5) by hand.
    - Also the behaviour of UCP needs to be exactly specified and tested.
    - Start test-program. DONE

1. AB See the todos in Board.hpp.

2. AB Document the various concepts (rows, extended rows, boards).

3. OK After 0-2, version 1.0 is reached.

4. AB See the todos in Backtracking.hpp

5. AB See the todos in Rows.hpp.

6. AB, OK Symmetry-breaking is done in main; likely this should be outsourced.

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
      "0.9.16",
      "2.7.2020",
      __FILE__,
      "Oliver Kullmann",
      "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Queens/SimpleBacktracking/Queens_RUCP_ct.cpp",
      "GPL v3"};


  using namespace Dimensions;

  // Abbreviations for the implementation-choices:
  typedef Rows::ChoiceRT<rt> R;
  template<class X> using ER = ExtRows::ChoiceERT<ert,X>;


const Environment::addvo_fot AO = [](std::ostream& out) {
  out << "\n** Constants: **\n"
         "  N=" << N << "\n"
         "  Row-type     : " << rt << "\n"
         "  Ext-row-type : " << ert << "\n";
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
    "   ert = " << int(ert) << ",\n outputting the line\n"
    "   N rt ert solution_count node_count unit-clauses\n"
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

  for (size_t i = 0; i < N; ++i) {
    Board::DoubleSweep<R> B(i);
    B.ucp<ER>(res);
    if (not B.satisfied() and not B.falsified()) {
      if (i < (N+1)/2) {
        jobs.push_back(std::async(std::launch::async,
                                  Backtracking::count<R, ER>, B));
        results.push_back({});
      }
      else
        results[i - (N+1)/2].add_duplication();
    }
  }
  assert(jobs.size() == results.size());
  for (size_t i = 0; i < jobs.size(); ++i) results[i] += jobs[i].get();
  for (const auto& r : results) res += r;

  if (res.num_sols() != Recursion::exact_value(N)) {
    std::cerr << "\nERROR[" << proginfo.prg << "]: The statistics are\n"
              << N << " " << res << "\nbut the correct count for N=" << N
              << " is " << Recursion::exact_value(N) << ".\n\n";
    return 1;
  }
  std::cout << N << " " << int(rt) << " " << int(ert) << " " << res << "\n";
}
