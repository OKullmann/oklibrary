// Oliver Kullmann 27.5.2018 (Swansea)
/* Copyright 2018, 2019, 2020 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Computes the N-Queens count for N given as macro NUMQUEENS, e.g. for N=16,
  using the makefile in the same directory as this file:

> make SETN=-DNUMQUEENS=16
> ./qcount_ct

  "ct" stands for "compile-time".
  The same basic algorithm as NQueens.cpp, but using std::bitset<N>.

TODOS:

1. Update C++
 - DONE Use Environment::ProgramInfo and show_usage.

2. Update makefile
 - Update options:
  - DONE (for now): Need to experiment with optimisation options.
  - Need to update the style.
  - Need to introduce the new possibilities for Environment.hpp.

3. Implement complete propagation, as an option
  - Just using the same "sweeping movements upwards" for the
    AMO-propagation.
  - Detection of rows without open field is just one equality-check.
  - And checking whether a unit-clause has been produced just via
    bitset::count.
  - All the rows with count = 1 are added up (as "or") into a word U
    (for unit-clauses). This word can then be set and propagated by the
    same sweeping movement (which works in parallel for arbitrarily many units,
    i.e., queens-placements).
  - Indeed, once a new unit-clause was found, it should be set *immediately*
    in U; one needs to notice whether such an addition took place, in which
    case the loop for U is repeated.

4. Provide basic parallelism (at the base-line, for N/2 threads).

*/


#include <bitset>
#include <iostream>
#include <string>

#include <ProgramOptions/Environment.hpp>

#include <cstdlib>
#include <cstdint>
#include <cassert>

namespace {

const Environment::ProgramInfo proginfo{
      "1.0.5",
      "29.4.2020",
      __FILE__,
      "Oliver Kullmann",
      "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Queens/SimpleBacktracking/NQueens_ct.cpp",
      "GPL v3"};

typedef std::size_t size_t;
#ifndef NUMQUEENS
# error "NUMQUEENS must be defined."
#endif
constexpr size_t N=NUMQUEENS;

bool show_usage(const int argc, const char* const argv[]) {
  assert(argc >= 1);
  if (not Environment::help_header(std::cout, argc, argv, proginfo))
    return false;
  std::cout <<
    "> " << proginfo.prg << "\n"
    " runs the program for built-in N = " << N << ".\n"
    "\n"
    "> ./Call_QueensRUCPct N qcount_ct\n"
    " compiles optimised and debugging forms of this program, and runs\n"
    " the optimised version, with added user-time (s) and max-memory (kb).\n"
;
  return true;
}

typedef std::uint_fast64_t count_t; // counting solutions
typedef std::bitset<N> queen_t;


/* Helper functions for queen_t: */

inline queen_t setbits(const size_t m) noexcept {
  assert(m <= N);
  queen_t res;
  for (size_t i = 0; i < m; ++i) res[i] = true;
  return res;
}

inline queen_t setneighbours(queen_t x, const size_t i) noexcept {
  assert(i < N);
  x[i] = true;
  if (i != 0) x[i-1] = true;
  if (i+1 != N) x[i+1] = true;
  return x;
}
inline queen_t set(queen_t x, const size_t i) noexcept {
  assert(i < N);
  x[i] = true;
  return x;
}
inline queen_t setrightneighbour(queen_t x, const size_t i) noexcept {
  assert(i < N);
  if (i != 0) x[i-1] = true;
  return x;
}
inline queen_t setleftneighbour(queen_t x, const size_t i) noexcept {
  assert(i < N);
  if (i+1 != N) x[i+1] = true;
  return x;
}


count_t count=0, nodes=0;

inline void backtracking(const queen_t avail,
  const queen_t columns, const queen_t fdiag, const queen_t fantid,
  const size_t size) noexcept {
  assert(avail.any());
  assert(columns.count() == size);

  ++nodes;
  const size_t sp1 = size+1;
  assert(sp1 < N);
  const queen_t sdiag = fdiag >> 1;
  const queen_t santid = fantid << 1;
  const queen_t forb0(columns | sdiag | santid);

  if (forb0.all()) return;
  if (sp1+1 == N) {
    for (size_t i = 0; i < N; ++i)
      count += bool(avail[i] and not setneighbours(forb0,i).all());
  }
  else
    for (size_t i = 0; i < N; ++i) {
      if (not avail[i]) continue;
      const queen_t newcolumns(set(columns,i));
      const queen_t ndiag(setrightneighbour(sdiag,i));
      const queen_t nantid(setleftneighbour(santid,i));
      const queen_t newavail(~(newcolumns | ndiag | nantid));
      if (newavail.any()) backtracking(newavail,newcolumns,ndiag,nantid,sp1);
    }
}

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  std::cout << N << " ";
  if (N % 2 == 0) {
    backtracking(setbits(N/2), 0, 0, 0, 0);
    std::cout << 2*count << " " << nodes << "\n";
  }
  else {
    backtracking(setbits(N/2), 0, 0, 0, 0);
    const count_t half = count; count = 0;
    backtracking(queen_t().set(N/2), 0, 0, 0, 0);
    std::cout << 2*half + count << " " << nodes << "\n";
  }
}
