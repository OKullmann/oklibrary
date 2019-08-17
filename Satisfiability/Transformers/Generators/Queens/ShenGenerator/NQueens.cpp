// Oliver Kullmann, 19.5.2018 (Swansea)
// Copyright 2011 Haiou Shen
// Copyright 2018 Oliver Kullmann
// Started as a copy of the site
// https://sites.google.com/site/haioushen/search-algorithm/solvean-queensproblemusingsatsolver

/*

  Usage:

> ./qgen N

creates the standard CNF-SAT-representation of the N-Queens problem.
Here and for the other options, the output is always to the file with
standard-name

  (Queens|Rooks|Bishops|BishopSemirook)(Problem|Graph)_N.cnf

where in case N has 1 or 2 digits, 2 resp. 1 zeros are padded to the left.

> ./qgen N "Q"|"R"|"B"|"BR"

with second argument "R", "B" or "BR" creates the standard CNF-representation
for the N-Rooks resp. Maximal-Bishops resp. Bishop+Semirooks problem.

> ./qgen N "g"

create the N-Queens problem, but does not include the 2N "long" ALO-clauses
(so this is effectively the N-Queens graph, and solution-counting yields
the vertex-cover count of the N-Queens graph).

> ./qgen N "Q"|"R"|"B"|"BR" "g"|"S"

finally allows also the graph-versions (without ALO-clauses) for Rooks- and
Bishops-versions; "S" stands for "SAT".

For compilation the Makefile in the same directory as this file can
be used (standard C++11).

The NxN fields of the chess board have coordinates (indices) as follows
(using N=4):

 (3,0) (3,1) (3,2) (3,3)
 (2,0) (2,1) (2,2) (2,3)
 (1,0) (1,1) (1,2) (1,3)
 (0,0) (0,1) (0,2) (0,3)

The numbering of variables is row-wise, starting at the bottom row (using
again N=4):

 13 14 15 16
 9  10 11 12
 5  6  7  8
 1  2  3  4

Examples:

> ./qgen 8
creates the problem, in file "QueensProblem_008.cnf", whose 92 solutions
represent the nonattacking placements of 8 queens on an 8x8 chess board.
The same is obtained by
> ./qgen 8 Q
or
> ./qgen 8 Q S

> ./qgen 8 g
creates the problem, in file "QueensGraph_008.cnf", whose 118969 solutions
represent all nonattacking placements of queens on an 8x8 chess board.
The same is obtained by
> ./qgen 8 Q g

> ./qgen 8 R
creates the problem, in file "RooksProblem_008.cnf", whose 40320 solutions
represent the nonattacking placements of 8 rooks on an 8x8 chess board.
The same is obtained by
> ./qgen 8 R S

> ./qgen 8 R g
creates the problem, in file "RooksGraph_008.cnf", whose 1441729 solutions
represent all nonattacking placements of rooks on an 8x8 chess board.

> ./qgen 8 B
creates the problem, in file "BishopsProblem_008.cnf", whose 256 solutions
represent the nonattacking maximal placements of bishops on an 8x8 chess board.
(This is the same here as all nonattacking placements of 14 bishops.)
The same is obtained by
> ./qgen 8 B S

> ./qgen 8 B g
creates the problem, in file "BishopsGraph_008.cnf", whose 82609921 solutions
represent all nonattacking placements of bishops on an 8x8 chess board.


Altogether there are 8 problem-types, which, with links to the
solution-counts (if available), are as follows:

1. Solutions for the N-Queens problem (https://oeis.org/A000170):
> ./qgen N
> ./qgen N Q
> ./qgen N Q S
For N#2,3 this is the number of maximum independent sets in the Queens-graph.

Count for N=13: 73712

2. Partial solutions for the N-Queens problem (https://oeis.org/A287227):
> ./qgen N
> ./qgen N g
> ./qgen N Q g
This is the number of independent sets in the Queens-graph.

Count for N=10: 7535369

3. Solutions for the N-Rooks problem (this is N!):
> ./qgen N R
> ./qgen N R S
This is the number of maximum (here equivalently: maximal) independent sets
in the Rooks-graph.

Count for N=10: 3628800

4. Partial solutions for the N-Rooks problem (https://oeis.org/A002720):
> ./qgen N R g
This is the number of independent sets in the Rooks-graph.

Count for N=10: 234662231

5. Solutions for the Bishops-problem, with ALO for the (nontrivial) diagonals
   and antidiagonals:
> ./qgen N B
> ./qgen N B S
This variation seems not to have been considered yet;
counts for 1 <= N <= 12 are:
  1 4 10 16 56 64 416 256 3968 1024 43520 4096
For even N this is the problem of placing 2N-2 bishops (the maximum
number), whose count is 2^N.
In general this is the number of nonattacking bishop-placements
covering all fields, or, equivalently, the number of maximal independent
sets in the Bishops-graph (for odd N >= 3 they don't need to be maximum).

6. Partial solutions for the N-Bishops problem (https://oeis.org/A201862):
> ./qgen N B g
This is the number of independent sets in the Bishops-graph.

Count for N=9: 2319730026

7. Solutions for the Bishop+Semirooks-problem (https://oeis.org/A185056),
   that is, as with Queens, but no condition on the columns:
> ./qgen N BR
> ./qgen N BR S

Count for N=11: 56433455

8. Partial solutions for the Bishop+Semirooks-problem:
> ./qgen N BR g
This variation is not on OEIS, the counts for 1 <= N <= 10 are:
  2, 7, 32, 193, 1366, 11291, 105014, 1103241, 12772308, 162520767

*/

/* TODOS

1. Update to the new standard

2. Provide the modular versions MQ, MB, MBR

*/

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <limits>
#include <algorithm>

#include <cstdint>

namespace {

constexpr unsigned int number_field_size = 3;

typedef std::int64_t lit_t; // literals
typedef std::vector<lit_t> cl_t; // clauses
typedef std::vector<cl_t> cls_t; // clause-sets

typedef std::uint32_t coord_t; // coordinates
typedef cl_t::size_type size_t;

enum class ConstraintType { Q, R, B, BR };

ConstraintType translate(const std::string& inp) noexcept {
  if (inp=="R") return ConstraintType::R;
  if (inp=="B") return ConstraintType::B;
  if (inp=="BR") return ConstraintType::BR;
  return ConstraintType::Q;
}
std::string translate(const ConstraintType ct) {
  switch (ct) {
    case ConstraintType::Q : return "Queens";
    case ConstraintType::R : return "Rooks";
    case ConstraintType::B : return "Bishops";
    case ConstraintType::BR : return "BishopSemirooks";
  }
}
std::string problem_form(const bool alo) {
  return (alo) ? "Problem" : "Graph";
}

// At-most-one constraints:
inline void amo(const cl_t& v, cls_t& F) {
  const size_t N = v.size();
  if (N <= 1) return;
  for (coord_t i=0; i<N-1; ++i)
    for (coord_t j=i+1; j<N; ++j) F.push_back(cl_t{-v[i],-v[j]});
}
// At-least-one constraint:
inline void alo(const cl_t& v, cls_t& F) {
  F.push_back(v);
}

// The variables var(i,j), 0 <= i,j < N, are indexed 1, ..., N^2 row-wise:
inline constexpr lit_t var(const coord_t i, const coord_t j, const coord_t N) noexcept {
  return i*N + j + 1;
}

std::string translate(const coord_t N) {
  const std::string res = std::to_string(N);
  typedef std::string::size_type size_t;
  const size_t l = res.length();
  return std::string(std::max<size_t>(l, number_field_size) - l, '0') + res;
}
std::string filename(const coord_t N, const ConstraintType ct, const bool alo) {
  return translate(ct) + problem_form(alo) + "_" + translate(N) + ".cnf";
}

}

int main(const int argc, const char* const argv[]) {

  if (argc < 2 or argc > 4) {
    std::cout << "Usage[qgen]: N [Q|R|B] [S|g]\n";
    return 0;
  }

  const unsigned long arg1 = std::stoul(argv[1]);
  if (arg1 > std::numeric_limits<coord_t>::max()) {
    std::cerr << " First argument (N) too large.\n";
    return 1;
  }
  const coord_t N = arg1;
  const lit_t nvar = N*N;

  const std::string arg2 = (argc < 3) ? "Q" : argv[2];
  const std::string arg3 = (argc < 4) ? "S" : argv[3];

  if (not ((argc == 2) or
       (argc == 3 and (arg2=="Q" or arg2=="R" or arg2=="B" or arg2=="BR" or arg2=="g")) or
       (argc == 4 and (arg2=="Q" or arg2=="R" or arg2=="B" or arg2=="BR") and (arg3=="g" or arg3=="S")))) {
    std::cerr << "Wrong second or third argument.\n";
    return 1;
  }

  const ConstraintType con_t = translate(arg2);
  const bool ALO = arg2!="g" and arg3!="g";

  const std::string name = filename(N,con_t,ALO);
  std::ofstream fout(name);
  if (not fout) {
    std::cerr << "Error opening file \"" << name << "\".\n";
    return 1;
  }

  cls_t F;
  cl_t vars; vars.reserve(N);
  // Row constraints:
  if (con_t != ConstraintType::B) {
    for (coord_t i=0; i<N; ++i) {
      for (coord_t j=0; j<N; ++j) vars.push_back(var(i,j,N));
      if (ALO) alo(vars,F);
      amo(vars,F);
      vars.clear();
    }
    if (con_t != ConstraintType::BR) {
      // Column constraints:
      for (coord_t i=0; i<N; ++i) {
        for (coord_t j=0; j<N; ++j) vars.push_back(var(j,i,N));
        if (ALO and N >= 2) alo(vars,F);
        amo(vars,F);
        vars.clear();
      }
    }
  }

  if (con_t != ConstraintType::R) {
    // diagonal constraints
    if (N == 1 and ALO and con_t == ConstraintType::B) alo(cl_t{1},F);
    else if (N >= 2) {
      // N-1 diags of length N, N-1, ..., 2, starting with main diagonal,
      // each starting at field (0,i) for 0 <= i <= N-2:
      for (coord_t i=0; i<N-1; ++i) {
        for (coord_t j=0; j<N-i; ++j) vars.push_back(var(j,i+j,N));
        if (ALO and con_t == ConstraintType::B) alo(vars,F);
        amo(vars, F);
        vars.clear();
      }
      // N-2 diags of length N-1, ..., 2, each starting at (i,0) for
      // 1 <= i <= N-2:
      for (coord_t i=1; i<N-1; ++i) {
        for (coord_t j=0; j<N-i; ++j) vars.push_back(var(j+i,j,N));
        if (ALO and con_t == ConstraintType::B) alo(vars,F);
        amo(vars, F);
        vars.clear();
      }
      // N-1 antidiags of length N, N-1, ..., 2, starting with main antidiag,
      // each starting at field (0,N-1-i) for 0 <= i <= N-2:
      for (coord_t i=0; i<N-1; ++i) {
        for (coord_t j=0; j<N-i; ++j) vars.push_back(var(j,N-1-i-j,N));
        if (ALO and con_t == ConstraintType::B) alo(vars,F);
        amo(vars, F);
        vars.clear();
      }
      // N-2 antidiags of length N-1, ..., 2, each starting at (i,N-1) for
      // 1 <= i <= N-2:
      for (coord_t i=1; i<N-1; ++i) {
        for (coord_t j=0; j<N-i; ++j) vars.push_back(var(j+i,N-1-j,N));
        if (ALO and con_t == ConstraintType::B) alo(vars,F);
        amo(vars, F);
        vars.clear();
      }
    }
  }

  fout << "p cnf " << nvar << " " << F.size() << "\n";
  for (const cl_t& C : F) {
    for (const lit_t x : C) fout << x << " ";
    fout << "0" << "\n";
  }
  fout << "\n";
}
