// Oliver Kullmann 25.4.2020 (Swansea)
/* Copyright 2020 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

USAGE:

> ./Queens_RUCP_ct

uses the default N=16; use "make SETN=-DNUMQUEENS=X" for compilation to have
N=X. Or, more conveniently, use

> ./Call_QueensRUCPct N

for performing first the compilation, and then running the program.

(N+1)/2 parallel threads are used.

Via "-h" help-information is obtained, via "-v" version-information.

Classes Row, Row_uint implement one row of the board, while ExtRow,
ExtRow_uint implement the extensions for the diagonals. "uint" means
that the implementation uses unsigned integers, while otherwise std::bitset
is used.

Setting the typedefs R, ER (just before the main-function) regulates which
of the 2*2=4 possibilities is activated.

The core function is ucp(Board&), which performs complete row-unit-propagation,
while for columns exactly the inconsistencies are detected (i.e., empty
columns).

The recursion is handled by function count(Board).


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

1. OK Improved output:
    - The version-information should contain N and information on which of
      the 2*2 configurations is used. DONE
    - So the Environment-function for showing version-information needs
      to become customisable; by a function-object, which prints additional
      information. DONE
    - The configuration should also show up in the output.
      So amendment of the R-header is needed.
    - There should be enumerated constants, as global variables,
      and according to their values, the aliases R, ER are defined
      (via partial specialisation). DONE
    - These constants are placed after definition of N. DONE

2. AB See the todos in Board.hpp.

3. AB Document the various concepts (rows, extended rows, boards).

4. OK After 0-3, version 1.0 is reached.

5. AB See the todos in Backtracking.hpp

6. AB See the todos in Rows.hpp.

7. AB, OK Symmetry-breaking is done in main; likely this should be outsourced.

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
      "0.9.10",
      "1.7.2020",
      __FILE__,
      "Oliver Kullmann",
      "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Queens/SimpleBacktracking/Queens_RUCP_ct.cpp",
      "GPL v3"};


  // The implementation choices:
  constexpr Rows::Rtypes rt = Rows::Rtypes::uint;
  constexpr ExtRows::ERtypes ert = ExtRows::ERtypes::uint;


  using namespace Dimensions;

  // Abbreviations for the implementation-choices:
  typedef Rows::ChoiceRT<rt> R;
  template<class X> using ER = ExtRows::ChoiceERT<ert,X>;


const Environment::addvo_fot AO = [](std::ostream& out) {
  out << "\n** Constants: **\n"
         "  N=" << N << "\n"
         "  Row-type : " << rt << "\n"
         "  Ext-row-type : " << ert << "\n";
};


bool show_usage(const int argc, const char* const argv[]) {
  assert(argc >= 1);
  if (not Environment::help_header(std::cout, argc, argv, proginfo))
    return false;
  std::cout <<
    "> " << proginfo.prg << "\n"
    " runs the program for built-in N = " << N << ", outputting the line\n"
    "   N solution_count node_count\n"
    "\n"
    "> ./Call_QueensRUCPct N\n"
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
  std::cout << N << " " << res << "\n";
}
