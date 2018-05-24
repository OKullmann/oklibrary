// Oliver Kullmann, 19.5.2018 (Swansea)
// Copyright 2011 Haiou Shen
// Copyright 2018 Oliver Kullmann
// Started as a copy of the site
// https://sites.google.com/site/haioushen/search-algorithm/solvean-queensproblemusingsatsolver

/*

  Usage:

> ./qgen N filename

creates the standard CNF-representation of the N-Queens problem.

> ./qgen N filename "Q"|"R"|"B"

for third argument "R" or "B" creates the standard CNF-representation for
the N-Rooks resp. 2N-2-Bishops problem.

> ./qgen N filename "g"

create the N-Queens problem, but does not include the 2N "long" ALO-clauses
(so this is effectively the N-Queens graph, and solution-counting yields
the vertex-cover count of the N-Queens graph).

> ./qgen N filename "Q"|"R"|"B" "g"|"S"

now allows also the graph-versions (without ALO-clauses) for Rooks- and
Bishops-versions; "S" stands for "SAT".

So we get 6 problem-types, which for the solution-counts are as follows:

1. Solutions for the N-Queens problem (https://oeis.org/A000170):
> ./qgen N filename
> ./qgen N filename Q
> ./qgen N filename Q S

2. Partial solutions for the N-Queens problem (https://oeis.org/A287227):
> ./qgen N filename g
> ./qgen N filename Q g

3. Solutions for the N-Rooks problem (this is N!):
> ./qgen N filename R
> ./qgen N filename R S

4. Partial solutions for the N-Rooks problem (https://oeis.org/A002720):
> ./qgen N filename R g

5. Solutions for the Bishops-problem, with ALO for the nontrivial diagonals
   and antidiagonals:
> ./qgen N filename B
> ./qgen N filename B S

This variation sems not to have been considered yet; counts for 1<=N<=10 are:
2 4 10 16 56 64 416 256 3968 1024
For even N this appears the problem of placing 2N-2 bishops (the maximum
number), whose count is 2^N.

6. Partial solutions for the N-Bishops problem (https://oeis.org/A201862):
> ./qgen N filename B g

*/

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <cstdint>
#include <limits>

namespace {

typedef std::int64_t lit_t; // literals
typedef std::vector<lit_t> cl_t; // clauses
typedef std::vector<cl_t> cls_t; // clause-sets

typedef std::uint32_t coord_t; // coordinates
typedef cl_t::size_type size_t;

enum class ConstraintType { Q, R, B };

ConstraintType translate(const std::string& inp) {
  if (inp=="R") return ConstraintType::R;
  if (inp=="B") return ConstraintType::B;
  return ConstraintType::Q;
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

}

int main(const int argc, const char* const argv[]) {

  if (argc < 3 or argc > 5) {
    std::cout << "Usage[qgen]: N filename [Q|R|B] [S|g]\n";
    return 0;
  }

  const unsigned long arg1 = std::stoul(argv[1]);
  if (arg1 > std::numeric_limits<coord_t>::max()) {
    std::cerr << " First argument (N) too large.\n";
    return 1;
  }

  const std::string arg3 = (argc < 4) ? "Q" : argv[3];
  const std::string arg4 = (argc < 5) ? "S" : argv[4];

  if (not ((argc == 3) or
       (argc == 4 and (arg3=="Q" or arg3=="R" or arg3=="B" or arg3=="g")) or
       (argc == 5 and (arg3=="Q" or arg3=="R" or arg3=="B") and (arg4=="g" or arg4=="S")))) {
    std::cerr << "Wrong third or fourth argument.\n";
    return 1;
  }

  const ConstraintType con_t = translate(arg3);
  const bool ALO = arg3!="g" and arg4!="g";

  std::ofstream fout(argv[2]);
  if (not fout) {
    std::cerr << "Error opening file \"" << argv[2] << "\".\n";
    return 1;
  }

  const coord_t N = arg1;
  const lit_t nvar = N*N;
  cls_t F;
  cl_t vars; vars.reserve(N);

  if (con_t != ConstraintType::B) {
    for (coord_t i=0; i<N; ++i) {
      // row constraints
      for (coord_t j=0; j<N; ++j) vars.push_back(var(i,j,N));
      if (ALO) alo(vars,F);
      amo(vars,F);
      vars.clear();
    }
    for (coord_t i=0; i<N; ++i) {
      // column constraints
      for (coord_t j=0; j<N; ++j) vars.push_back(var(j,i,N));
      if (ALO) alo(vars,F);
      amo(vars,F);
      vars.clear();
    }
  }

  if (con_t != ConstraintType::R) {
    // diagonal constraints
    if (N >= 2) {
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
