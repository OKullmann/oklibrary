// Oliver Kullmann 27.5.2018 (Swansea)
/* Copyright 2018, 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Computes the N-Queens count for N given as macro NN, e.g. for N=16, using
  the makefile in the same directory as this file:

> make SETN=-DNN=16
> ./qcount_ct

  "ct" stands for "compile-time".
  The same basic algorithm as NQueens.cpp, but using std::bitset<N>.

  Version 1.0, 28.5.2018.

TODOS:

1. Establish timing

Using the updated makefile, now with N=16.

Timing 17/3/2019

csltok (gcc 8.3.0):
NicolosiCounting> time ./qcount_ct
14772512 355451208
real    0m16.799s
user    0m16.670s
sys     0m0.081s

With new optimisation-options
  -Ofast -DNDEBUG -march=native -fwhole-program -static
compared to old ones
  -Ofast -DNDEBUG -ffinite-math-only -funroll-loops -fwhole-program -fno-math-errno -funsafe-loop-optimizations
we get
NicolosiCounting> time ./qcount_ct
14772512 355451208
real    0m16.911s
user    0m16.868s
sys     0m0.002s

There seems to be a slowdown.

Playing with options:
 - "-static" might yield some slowdown
 - "-funroll-loops" might yield some slowdown
 - "-funsafe-loop-optimizations" might yield some speedup

Unclear whether -static has an effect on qcount; so let's leave it out here.

cs-wsok:
NicolosiCounting> time ./qcount_ct
14772512 355451208
real    0m12.677s
user    0m12.668s
sys     0m0.002s

With new options:
csoliver@cs-wsok:~/OKplatform/OKsystem/OKlib/Satisfiability/Transformers/Generators/Queens/NicolosiCounting> time ./qcount_ct
14772512 355451208
real    0m12.320s
user    0m12.312s
sys     0m0.000s

Possibly small improvement.


csverify:
NicolosiCounting$ time ./qcount_ct
14772512 355451208
real    0m10.153s
user    0m10.149s
sys     0m0.004s

With new options:
icolosiCounting$ time ./qcount_ct
14772512 355451208
real    0m9.209s
user    0m9.205s
sys     0m0.004s

This should be an improvement.


2. Update C++

3. Update makefile
 - Update options:
  - Need to experiment with optimisation options.

*/


#include <iostream>
#include <cassert>
#include <bitset>

namespace {

typedef std::size_t size_t;
#ifndef NN
# error "NN must be defined."
#endif
constexpr size_t n=NN;

typedef std::uint_fast64_t count_t; // counting solutions
typedef std::bitset<n> queen_t;

queen_t setbits(const size_t m) {
  assert(m <= n);
  queen_t res;
  for (size_t i = 0; i < m; ++i) res[i] = true;
  return res;
}

inline queen_t setneighbours(queen_t x, const size_t i) noexcept {
  assert(i < n);
  x[i] = true;
  if (i != 0) x[i-1] = true;
  if (i+1 != n) x[i+1] = true;
  return x;
}
inline queen_t set(queen_t x, const size_t i) noexcept {
  assert(i < n);
  x[i] = true;
  return x;
}
inline queen_t setrightneighbour(queen_t x, const size_t i) noexcept {
  assert(i < n);
  if (i != 0) x[i-1] = true;
  return x;
}
inline queen_t setleftneighbour(queen_t x, const size_t i) noexcept {
  assert(i < n);
  if (i+1 != n) x[i+1] = true;
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
  assert(sp1 < n);
  const queen_t sdiag = fdiag >> 1;
  const queen_t santid = fantid << 1;
  const queen_t forb0(columns | sdiag | santid);
  if (forb0.all()) return;
  if (sp1+1 == n) {
    for (size_t i = 0; i < n; ++i)
      count += bool(avail[i] and not setneighbours(forb0,i).all());
  }
  else
    for (size_t i = 0; i < n; ++i) {
      if (not avail[i]) continue;
      const queen_t newcolumns(set(columns,i));
      const queen_t ndiag(setrightneighbour(sdiag,i));
      const queen_t nantid(setleftneighbour(santid,i));
      const queen_t newavail(~(newcolumns | ndiag | nantid));
      if (newavail.any()) backtracking(newavail,newcolumns,ndiag,nantid,sp1);
    }
}

}

int main() {
  if (n % 2 == 0) {
    backtracking(setbits(n/2), 0, 0, 0, 0);
    std::cout << 2*count << " " << nodes << "\n";
  }
  else {
    backtracking(setbits(n/2), 0, 0, 0, 0);
    const count_t half = count; count = 0;
    backtracking(queen_t().set(n/2), 0, 0, 0, 0);
    std::cout << 2*half + count << " " << nodes << "\n";
  }
}
