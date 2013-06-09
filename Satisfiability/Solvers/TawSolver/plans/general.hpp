// Oliver Kullmann, 8.6.2013 (Swansea)
/* Copyright 2013 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Solvers/TawSolver/plans/general.hpp
  \brief Plans regarding updating and improving the TawSolver


  \todo Timing
  <ul>
   <li> On csltok (2.7 GHz):
   \verbatim
# Old: 588a9cf1d823c5e59090ad20938b1708e6961643

> tawSolver VanDerWaerden_2-3-12_134.cnf
V_VARS: 134, N_CLAUSES: 5172
N_UNITS: 20977611, N_BRANCHES: 1721206, N_BACK: 860593

46.44 46.11 46.01 46.25

> tawSolver VanDerWaerden_2-3-12_135.cnf
V_VARS: 135, N_CLAUSES: 5251
N_UNITS: 21978048, N_BRANCHES: 1790733, N_BACK: 895366

49.33 49.45 49.24 49.67 48.09

# New: fb0212392a795e09e3c1ae7d188ad30d0f4e57cf

> tawSolver VanDerWaerden_2-3-12_134.cnf
45.85 46.08 45.40 46.04

> tawSolver VanDerWaerden_2-3-12_135.cnf
47.71 47.96 47.75 47.22 47.58

# A bit faster: Perhaps due to saving superfluous calls to getrusage.
# It might be, that actually most of the time for these calls is not recorded
# in these times, since it belongs to "sys" (when using "time") ?!

# New: 0762218ce28bc0a8e9e6e87d092d1a6d86e1b7c6

> tawSolver VanDerWaerden_2-3-12_134.cnf
44.99 45.77 45.18 45.37

> tawSolver VanDerWaerden_2-3-12_135.cnf
48.36 47.98 47.92 48.41 46.99
# at the same time(!) for the previous version: 49.29 48.88 47.69

# So apparently the introduction of log2s saved 1% run-time.
   \endverbatim
   </li>
  </ul>


  \todo Improving the clause-bitsets
  <ul>
   <li> The following should be transferred to a the general library.
   Otherwise this todo is completed. </li>
   <li> http://graphics.stanford.edu/~seander/bithacks.html
   contains various bit-level operations. </li>
   </li>
   <li> At
   http://graphics.stanford.edu/~seander/bithacks.html#IntegerLogObvious
   we find (slightly adapted):
   \verbatim
// IF YOU KNOW v IS A POWER OF 2:
unsigned int v;  // 32-bit value to find the log2 of
const unsigned int b[] = {0xAAAAAAAA, 0xCCCCCCCC, 0xF0F0F0F0, 0xFF00FF00, 0xFFFF0000};
unsigned int r = (v & b[0]) != 0;
for (i = 4; i > 0; --i) r |= ((v & b[i]) != 0) << i;

# The values b[0], ..., b[4]:
1010 1010 1010 1010 1010 1010 1010 1010
1100 1100 1100 1100 1100 1100 1100 1100
1111 0000 1111 0000 1111 0000 1111 0000
1111 1111 0000 0000 1111 1111 0000 0000
1111 1111 1111 1111 0000 0000 0000 0000
   \endverbatim
   </li>
   <li> The result computed is
     (bool(v&b[0]) << 0) | ... | (bool(v&b[4]) << 4)
   Note that the order doesn't matter. </li>
   <li> A proper C++ implementation of "log2s" ("s" for "special"):
   \verbatim
#incluce <limits>
#include <cassert>
#include <cmath>

constexpr int B {std::numeric_limits<unsigned int>::digits};
static_assert(B == 32, "Type \"unsigned int\" has not 32 bits.");
constexpr int N = 5;

constexpr int log2s(const unsigned int v) {
  assert((unsigned int) std::exp2(std::log2(v)) == v);
  constexpr unsigned int b[N] = {0xAAAAAAAA,0xCCCCCCCC,0xF0F0F0F0,0xFF00FF00,0xFFFF0000};
  unsigned int r = (v & b[0]) != 0;
  for (int i = 1; i < N; ++i) r |= ((v & b[i]) != 0) << i;
  return r;
}
   \endverbatim
   </li>
   <li> For B=64 (N=6) we have:
   \verbatim
const unsigned int b[N] = {0xAAAAAAAAAAAAAAAA, 0xCCCCCCCCCCCCCCCC, 0xF0F0F0F0F0F0F0F0, 0xFF00FF00FF00FF00, 0xFFFF00000FFFF0000, 0xFFFFFFFF00000000};
   \endverbatim
   </li>
   <li> A recursive computation of the array b of length N, via the values
   bp(N,0), ..., bp(N,N-1) ("bp" for bit pattern"), is:
   \verbatim
constexpr unsigned int pow2(const unsigned e) { return (e==0)?1:2*pow2(e-1); }
constexpr unsigned int pow22(const unsigned e) {return pow2(pow2(e));}

constexpr unsigned int bp(const unsigned N, const unsigned i) {
  // assert(N >= 0);
  // assert(i >= 0);
  // assert(i < N);
  return (i < N-1) ? bp(N-1,i) * (1 + pow22(N-1)) : pow22(N) - pow22(N-1);
}
   \endverbatim
   </li>
   <li> Then the general computation is:
   \verbatim
#include <limits>
#include <cassert>
#include <cmath>

constexpr int log2(const unsigned int n) { return (n <= 1) ? 0 : 1+log2(n/2); }

constexpr int B {std::numeric_limits<unsigned int>::digits};
constexpr unsigned int N = log2((unsigned int) B);
static_assert(pow2(N) == (unsigned int) B, "Number of bits in \"unsigned int\" not a power of 2.");

static_assert(N==5 or N==6, "Unexpected size of type \"unsigned int\".");
const unsigned int b[6] {bp(N,0),bp(N,1),bp(N,2),bp(N,3),bp(N,4), (N==6) ? bp(N,5) : 0};
// Unfortunately there is no reasonable way in C++ to just define b[N].

inline int log2s(const unsigned int v) {
  assert((unsigned int) std::exp2(std::log2(v)) == v);
  unsigned int r = (v & b[0]) != 0;
  for (unsigned int i = 1; i < N; ++i) r |= ((v & b[i]) != 0) << i;
  return r;
}
   \endverbatim
   </li>
   <li> As constexpr-functions:
   \verbatim
constexpr int log2sr(const unsigned v, const unsigned i) {
  // assert(i < N);
  return (i == 0) ? ((v & bp(N,0)) != 0)  : log2sr(v, i-1) | (((v & bp(N,i)) != 0) << i);
}
constexpr unsigned int log2sc(const unsigned int v) {
  return log2sr(v, N-1);
}
   \endverbatim
   (Remark: In OKs repository under "B149Bitposition.cpp" a more general,
   templatised version can be found.)
   </li>
  </ul>

*/
