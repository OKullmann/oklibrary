// Oliver Kullmann, 8.6.2013 (Swansea)
/* Copyright 2013 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Solvers/TawSolver/plans/general.hpp
  \brief Plans regarding updating and improving the TawSolver


  \todo Create milestones


  \todo Provide information
  <ul>
   <li> On the ExternalSources page the data needs to be entered. </li>
   <li> http://sourceforge.net/projects/tawsolver/ </li>
  </ul>


  \todo Timing
  <ul>
   <li> Test instances created by
   \verbatim
> VanderWaerdenCNF-O3-DNDEBUG 3 12 134
> VanderWaerdenCNF-O3-DNDEBUG 3 12 135
   \endverbatim
   </li>
   <li> On csltok (2.7 GHz) resp. cswsok (3GHz):
   \verbatim
# Id: 588a9cf1d823c5e59090ad20938b1708e6961643

> tawSolver VanDerWaerden_2-3-12_134.cnf
V_VARS: 134, N_CLAUSES: 5172
N_UNITS: 20977611, N_BRANCHES: 1721206, N_BACK: 860593

csltok: 46.44 46.11 46.01 46.25

> tawSolver VanDerWaerden_2-3-12_135.cnf
V_VARS: 135, N_CLAUSES: 5251
N_UNITS: 21978048, N_BRANCHES: 1790733, N_BACK: 895366

csltok: 49.33 49.45 49.24 49.67 48.09


# ID: fb0212392a795e09e3c1ae7d188ad30d0f4e57cf

> tawSolver VanDerWaerden_2-3-12_134.cnf
csltok: 45.85 46.08 45.40 46.04
cswsok: 41.14 41.11 41.14 41.03

> tawSolver VanDerWaerden_2-3-12_135.cnf
csltok: 47.71 47.96 47.75 47.22 47.58
cswsok: 44.10 43.80 43.63 43.80 43.77

# A bit faster: Perhaps due to saving superfluous calls to getrusage.
# It might be, that actually most of the time for these calls is not recorded
# in these times, since it belongs to "sys" (when using "time") ?!


# Id: 0762218ce28bc0a8e9e6e87d092d1a6d86e1b7c6

> tawSolver VanDerWaerden_2-3-12_134.cnf
csltok: 44.99 45.77 45.18 45.37

> tawSolver VanDerWaerden_2-3-12_135.cnf
csltok: 48.36 47.98 47.92 48.41 46.99
# at the same time(!) for the previous version: 49.29 48.88 47.69

# So apparently the introduction of log2s saved 1% run-time.


# Id: 2407136679fc921445acde574e1d5b3cef1f718f
# Now with g++ version 4.7.3.

> tawSolver VanDerWaerden_2-3-12_134.cnf
s SATISFIABLE
c number_of_variables                   134
c number_of_clauses                     5172
c running_time(sec)                     41.28
c number_of_nodes                       1721206
c number_of_binary_nodes                860593
c number_of_1-reductions                20977611
c file_name                             VanDerWaerden_2-3-12_134.cnf
v 1 2 3 4 5 6 7 8 9 -10 11 12 13 14 15 16 17 18 -19 20 21 22 23 24 25 26 27 28 -29 30 31 -32 33 34 35 -36 37 -38 39 40 41 42 43 44 45 -46 47 48 -49 50 -51 52 53 54 -55 56 57 58 59 60 61 62 63 64 65 66 -67 -68 69 70 71 72 73 74 75 76 77 78 79 -80 81 82 83 -84 85 -86 87 88 -89 90 91 92 93 94 95 96 -97 98 -99 100 101 102 -103 104 105 -106 107 108 109 110 111 112 113 114 115 -116 117 118 119 120 121 122 123 124 -125 126 127 128 129 130 131 132 133 134 0

csltok: 45.61 45.93 45.72 45.45 45.51
cswsok: 41.28 41.31 41.17 41.17 41.25

> tawSolver VanDerWaerden_2-3-12_135.cnf
s UNSATISFIABLE
c number_of_variables                   135
c number_of_clauses                     5251
c running_time(sec)                     43.76
c number_of_nodes                       1790733
c number_of_binary_nodes                895366
c number_of_1-reductions                21978048
c file_name                             VanDerWaerden_2-3-12_135.cnf

csltok: 49.22 48.80 49.14 48.68 49.18
cswsok: 43.76 43.83 43.57 43.64 44.00

# ID: a5f506fae59b572b2dee6aeb0330676d65641acc
# Using a std::vector for the changes-array, checking for overflow (this
# should cost the most time), and allowing clauses of length
# up to 64.

> tawSolver VanDerWaerden_2-3-12_134.cnf
s SATISFIABLE
c number_of_variables                   134
c number_of_clauses                     5172
c running_time(sec)                     41.79
c number_of_nodes                       1721206
c number_of_binary_nodes                860593
c number_of_1-reductions                20977611
c max_number_changes                    9912
c file_name                             VanDerWaerden_2-3-12_134.cnf

cswsok: 41.75 41.79 41.65 41.48 41.84

> tawSolver VanDerWaerden_2-3-12_135.cnf
 UNSATISFIABLE
c number_of_variables                   135
c number_of_clauses                     5251
c running_time(sec)                     44.81
c number_of_nodes                       1790733
c number_of_binary_nodes                895366
c number_of_1-reductions                21978048
c max_number_changes                    10029
c file_name                             VanDerWaerden_2-3-12_135.cnf

cswsok: 44.81 44.25 44.41 44.83 44.58

# Got apparently 1 - 2% slower.

# ID: 583ab3bbb194c94126373765958689d28e847639
# Corrected computations.

> tawSolver VanDerWaerden_2-3-12_134.cnf
cswsok: 46.21 46.25
> tawSolver64 VanDerWaerden_2-3-12_134.cnf
cswsok: 48.38 48.60

> tawSolver VanDerWaerden_2-3-12_135.cnf
cswsok: 48.99 49.14
> tawSolver64 VanDerWaerden_2-3-12_135.cnf
cswsok: 51.65 51.43

# The corrected computations make it more than 10% slower.

# ID: f1157a664c98c72c1a299339ae2690bc25c1fb22
# Input is now handled dynamically.

> tawSolver VanDerWaerden_2-3-12_134.cnf
cswsok: 46.21 46.57 46.10 46.36
> tawSolver64 VanDerWaerden_2-3-12_134.cnf
cswsok: 48.49 48.93 48.84 48.80

> tawSolver VanDerWaerden_2-3-12_135.cnf
cswsok: 48.98 49.84 49.37 49.33
> tawSolver64 VanDerWaerden_2-3-12_135.cnf
cswsok: 52.03 51.85 51.92 52.08

# ID: 0350beb15e9ddcc72f3eac9a7f40c9e92045ea47
# Now using (local) std::stack for storing unit-clauses (saving all
# realloc-calls in dpll()).

> tawSolver VanDerWaerden_2-3-12_134.cnf
cswsok: 45.39 45.50 45.46 44.31 43.51 43.48
# the first three calls had higher cpu-load
> tawSolver64 VanDerWaerden_2-3-12_134.cnf
cswsok: 47.49 47.82 47.46 47.82

> tawSolver VanDerWaerden_2-3-12_135.cnf
cswsok: 46.74 46.49 46.35 46.48
> tawSolver64 VanDerWaerden_2-3-12_135.cnf
cswsok: 50.71 50.45 50.63 50.39

# ID: 01cf7bccbc306bb96b5b7b33a3104a56fcb1452f
# Various type-corrections.

> tawSolver VanDerWaerden_2-3-12_134.cnf
cswsok: 42.42 42.41 42.29 42.36
> tawSolver64 VanDerWaerden_2-3-12_134.cnf
cswsok: 47.09 47.06 47.00 46.98

> tawSolver VanDerWaerden_2-3-12_135.cnf
cswsok: 45.34 45.41 45.04 45.39
> tawSolver64 VanDerWaerden_2-3-12_135.cnf
cswsok: 50.26 50.11 50.15 50.30

# ID: 691fe428fe57f20b22e963697cdd74a3e79f450c
# Explicitly requested loop-unrolling from gcc.
# following times on otherwise idle cswsok:

> tawSolver VanDerWaerden_2-3-12_134.cnf
cswsok: 38.12 38.15 38.31 37.96
> tawSolver64 VanDerWaerden_2-3-12_134.cnf
cswsok: 41.86 41.94 42.01 41.94

> tawSolver VanDerWaerden_2-3-12_135.cnf
cswsok: 40.22 40.18 40.21 40.38
> tawSolver64 VanDerWaerden_2-3-12_135.cnf
cswsok: 44.41 44.55 44.56 44.53

# ID 2da9342c0399452746517de41340ece4720fe495
# perhaps major change: pos now first.

> oklib timing
cswsok (otherwise idle):
tawSolver VanDerWaerden_2-3-12_134.cnf
36.94 37.17 37.02 36.88 37.07
tawSolver64 VanDerWaerden_2-3-12_134.cnf
42.05 42.23 41.80 42.28 42.01
tawSolver VanDerWaerden_2-3-12_135.cnf
39.20 39.51 39.35 39.59 39.59
tawSolver64 VanDerWaerden_2-3-12_135.cnf
44.63 45.19 44.95 44.79 44.78

# apparently the 32-bit version got faster, while the 64-bit version got
# a bit slower.
   \endverbatim
   </li>
  </ul>


  \todo Provide Doxygen documentation


  \todo Introduce macros : DONE
  <ul>
   <li> DONE (no longer needed)
   MAX_NUMBER_CLAUSES, MAX_NUMBER_VARIABLES. </li>
   <li> And GIT_ID. </li>
   <li> DONE (different design)
   And CLAUSE_CONTENT (default "unsigned int"). </li>
   <li> DONE (not needed)
   Then a static_assert is needed, that the type for CLAUSE_CONTENT
   is unsigned integral. </li>
  </ul>


  \todo DONE
  Check number of variables and clauses
  <ul>
   <li> Errors must happen if there are too many variables or clauses. </li>
  </ul>


  \todo Version information : DONE
  <ul>
   <li> DONE
   Like the OKsolver, when using command-line argument "-v", the solver
   should output version information and date of compilation. </li>
   <li> DONE
   Let's also handle "--version". </li>
   <li> Also the value of MAX_CLAUSE_LENGTH, LIT_TYPE and GIT_ID. </li>
   <li> DONE
   And the git-ID (this needs to be added for the OKsolver as well). </li>
   <li> There should be an official version of the tawSolver:
    <ol>
     <li> The one on the solver's homepage is 1.0.0. </li>
     <li> DONE (using 1.3 now)
     Let's call the current version 1.1 (improved handling of maximal
     clause-length, improved output, code improvements). </li>
    </ol>
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
