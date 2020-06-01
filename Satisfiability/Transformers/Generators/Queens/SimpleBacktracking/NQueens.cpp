// Oliver Kullmann 25.5.2018 (Swansea)
/* Copyright 2018, 2019, 2020 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

The basic idea is the same as explained at
  https://helloacm.com/n-queen-problem-in-back-tracing-bit-logics/
and indeed the same as the program from 2002 by Jeff Somers
  http://users.rcn.com/liusomers/nqueen_demo/nqueens.html

Variable numrows there is our size, lsb is next, bitfield is avail, mask
is all_columns. The array aQueenBitCol contains the values of our variable
column, aQueenBitPosDiag of fdiag, aQueenBitNegDiag of fantid.
An array is used there to simulate the recursion.
The additional aQueenBitRes there just represents the solution for printing.

  Usage:

> ./qcount N

Output of N, the solution count, the number of calls of the recursive
procedure, the total node-count, and the number of leaves; e.g.

> ./qcount 8
8 92 615 1793 674

That is, 92 solutions (nonattacking placements of 8 queens on the 8x8 board),
using 1793 nodes in the backtracking tree altogether (615 recursive calls,
using symmetry-breaking in the first row), with 674 leaves.

Without an argument, the default-value for N is used.

For help and version:

> ./qcount -h
> ./qcount -v

For compilation, the Makefile in the same directory as this file can be used.
It produces also the debugging-version "qcount_debug".
To set NMAX (maximal allowed value of N; default 64), use e.g.
> CXXFLAGS="-DNMAX=32" make


Data

1. The total node-count for 1 <= N <= 5 is
     1, 3, 6, 17, 54.

TODOS:

1. Update to C++17: DONE
2. Update makefile, and use the new framework (Environment.hpp).

3. Use signals (like the tawSolver), to show current counts.
4. Use futures for parallelisation: DONE (in NQueenspar.cpp)
5. Simulate the recursion by a stack (as done in Jeff Somers's original
   program, whose runtime is very similar to the runtime of this program
   (version 1.0)). OK doesn't expect a speed-up, but one can try.
6. The encapsulated version 1.1.5 9b55e2a1f60384b5accbb18b51b946b1311e31f9
   had clearly better coding quality, so one should investigate whether
   one can't get the better runtime of version 1.1.4.
    - Or perhaps one should ignore the impaired runtime?
    - The slowdown seems to be always largest on csltok. Why is this?
      It is the oldest processor, but should that matter??
7. Update license.
    - There isn't code left from the original, and the basic idea is anyway
      from the Somers-implementation.

*/

#include <iostream>
#include <string>

#include <cassert>

#include <ProgramOptions/Environment.hpp>

#include "NQueens.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "1.3.2",
        "1.6.2020",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Queens/SimpleBacktracking/NQueens.cpp",
        "GPL v3"};

using namespace Queens;

constexpr input_t N_default = 14;
static_assert(N_default <= maxN);

// The recursive counting-function;
// using bit-positions 0, ..., N-1 for the columns 1, ..., N:

input_t N;
count_t count = 0, calls = 0, inner_nodes = 0, leaves = 0;

queen_t all_columns; // the first N bits 1, the rest 0

// Idea: size-many rows (from bottom) have been processed, now consider the
// next row, and try to place the next queen in some column.
inline void backtracking(queen_t avail,
  const queen_t columns, const queen_t fdiag, const queen_t fantid,
  const input_t size) noexcept {
  // avail: columns available (set to 1) for this invocation (only)
  // columns: the current placement of queens (set to 1)
  // fdiag: forbidden columns due to diagonal constraints (set to 1)
  // fantid: forbidden columns due to antidiagonal constraints (set to 1)
  assert(size == 0 or avail == (~(columns|fdiag|fantid) & all_columns));
  //assert(std::bitset<maxN>(columns).count() == size);
  ++calls;
  assert(avail);
  const queen_t sdiag = fdiag>>1, santid = fantid<<1;
  const queen_t newavail0 = ~(columns | sdiag | santid) & all_columns;
  if (not newavail0) {
    assert(inner_nodes != 0);
    --inner_nodes; ++leaves;
    return;
  }
  queen_t next = keeprightmostbit(avail); // could be any bit, but that seems fastest
  const input_t sp1 = size+1; // due to the placement of next
  assert(sp1 < N);
  if (sp1+1 == N) { // the current row is the penultimate
    do {
      count += bool(newavail0 & ~(next | next>>1 | next<<1));
      ++leaves;
    }
    while (next = keeprightmostbit(avail^=next));
  }
  else
    do {const queen_t newcolumns = columns|next,
          nextrs = next>>1, nextls = next<<1,
          newdiag = sdiag | nextrs, newantid = santid | nextls,
          newavail = newavail0 & ~(next | nextrs | nextls);
      ++inner_nodes;
      if (newavail) backtracking(newavail,newcolumns,newdiag,newantid,sp1);
      else { --inner_nodes; ++leaves; }
    } while (next = keeprightmostbit(avail^=next));
}

  bool show_usage(const int argc, const char* const argv[]) {
    assert(argc >= 1);
    if (argc != 2 or not Environment::is_help_string(argv[1])) return false;
    const std::string& program = "./qcount";
    std::cout << "USAGE:\n"
    "> " << program << " [-v | --version]\n"
    " shows version information and exits.\n"
    "> " << program << " [-h | --help]\n"
    " shows help information and exits.\n"
    "> " << program << " N\n"
    " computes the solution- and node-counts (recursive calls, total node-count, leaves)\n"
    " for the board of dimension N.\n"
    "The default-value of N is " << (unsigned long) N_default << ".\n"
;
    return true;
  }

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  const unsigned long arg1 = argc < 2 ? N_default : std::stoul(argv[1]);
  if (arg1 <= 1) { std::cout << arg1 << " 1 0 1 1\n"; return 0; }
  if (arg1 == 2) { std::cout << arg1 << " 0 0 3 2\n"; return 0; }
  if (arg1 == 3) { std::cout << arg1 << " 0 0 6 3\n"; return 0; }

  if (arg1 > maxN) {
    std::cerr << " N <= " << (unsigned long) maxN << " required.\n"; return 1;
  }
  N = arg1;
  std::cout << (unsigned long) N << " "; std::cout.flush();

  all_columns = setrightmostbits(N);
  // Using mirror-symmetry around vertical axis:
  if (N % 2 == 0) {
    backtracking(setrightmostbits(N/2), 0, 0, 0, 0);
    const count_t total_count = 2*count;
    std::cout << total_count << " " << calls << " " << 2*(inner_nodes+leaves) + 1 + total_count << " " << 2*leaves << "\n";
  } else {
    backtracking(setrightmostbits(N/2), 0, 0, 0, 0);
    const count_t half = count; count = 0;
    const count_t half_calls = calls; calls = 0;
    const count_t half_inner_nodes = inner_nodes; inner_nodes = 0;
    const count_t half_leaves = leaves; leaves = 0;
    backtracking(one(N/2), 0, 0, 0, 0);
std::cerr << "\n" << leaves << "\n";
    const count_t total_count = 2*half + count;
    std::cout << total_count << " " << half_calls + calls << " " << 2*(half_inner_nodes+half_leaves) + inner_nodes+leaves + 1 + total_count << " " << 2*half_leaves + leaves << "\n";
  }
}
