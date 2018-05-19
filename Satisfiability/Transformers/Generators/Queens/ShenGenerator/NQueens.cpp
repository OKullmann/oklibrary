// Oliver Kullmann, 19.5.2018 (Swansea)
// Copyright 2011 Haiou Shen
// Copyright 2018 Oliver Kullmann
// Started as a copy of the site
// https://sites.google.com/site/haioushen/search-algorithm/solvean-queensproblemusingsatsolver

/*

  Usage:

> ./qgen N filename

creates the standard CNF-representation of the N-Queens problem.

> ./qgen N filename x

where x is anything, does not include the 2N "long" ALO-clauses
(so this is effectively the N-Queens graph, and solution-counting yields
the vertex-cover count of the N-Queens graph).

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

// One alo (at-least-one) constraint, then the amo (at-most-one) constraints:
void aloamo(const cl_t& variables, cls_t& cnf) {
  const size_t N = variables.size();
  cnf.push_back(variables);
  if (N <= 1) return;
  cl_t clause; clause.reserve(2);
  for (coord_t j=0; j<N-1; ++j)
    for (coord_t k=j+1; k<N; ++k) {
      clause.push_back(-variables[j]);
      clause.push_back(-variables[k]);
      cnf.push_back(clause);
      clause.clear();
    }
}

void amo(const cl_t& variables, cls_t& cnf) {
  const size_t N = variables.size();
  if (N <= 1) return;
  cl_t clause; clause.reserve(2);
  for (coord_t j=0; j<N-1; ++j)
    for (coord_t k=j+1; k<N; ++k) {
      clause.push_back(-variables[j]);
      clause.push_back(-variables[k]);
      cnf.push_back(clause);
      clause.clear();
    }
}

}

int main(const int argc, const char* const argv[]) {

  if (argc < 3) {
    std::cout << "Usage[NQueens]: N filename\n";
    return 0;
  }

  const bool no_ALO = (argc == 4) ? true : false;

  const unsigned long arg1 = std::stoul(argv[1]);
  if (arg1 > std::numeric_limits<coord_t>::max()) {
    std::cerr << "First argument (N) too large.\n";
    return 1;
  }
  const coord_t N = arg1;
  const lit_t numVars = N*N;
  lit_t** const VarName = new lit_t*[N];
  {lit_t kk=0;
   for (coord_t i=0; i<N; ++i) {
     VarName[i] = new lit_t[N];
     for (coord_t j=0; j<N; ++j) VarName[i][j] = ++kk;
   }
  }

  cls_t cnf;
  cl_t vars; vars.reserve(N);

  for (coord_t i=0; i<N; ++i) {
    // row constraints
    for (coord_t j=0; j<N; ++j) vars.push_back(VarName[i][j]);
    if (no_ALO) amo(vars,cnf); else aloamo(vars, cnf);
    vars.clear();
  }

  for (coord_t i=0; i<N; ++i) {
    // column constraints
    for (coord_t j=0; j<N; ++j) vars.push_back(VarName[j][i]);
    if (no_ALO) amo(vars,cnf); else aloamo(vars, cnf);
    vars.clear();
  }

  if (N >= 2) {
    // diagonal constraints
    for (coord_t i=0; i<N-1; ++i) {
      for (coord_t j=0; j<N-i; ++j) vars.push_back(VarName[j][i+j]);
      amo(vars, cnf);
      vars.clear();
    }
    for (coord_t i=1; i<N-1; ++i) {
      for (coord_t j=0; j<N-i; ++j) vars.push_back(VarName[j+i][j]);
      amo(vars, cnf);
      vars.clear();
    }
    for (coord_t i=0; i<N-1; ++i) {
      for (coord_t j=0; j<N-i; ++j) vars.push_back(VarName[j][N-1-i-j]);
      amo(vars, cnf);
      vars.clear();
    }
    for (coord_t i=1; i<N-1; ++i) {
      for (coord_t j=0; j<N-i; ++j) vars.push_back(VarName[j+i][N-1-j]);
      amo(vars, cnf);
      vars.clear();
    }
  }

  std::ofstream fout(argv[2]);
  if (not fout) {
    std::cerr << "Error opening file \"" << argv[2] << "\".\n";
    return 1;
  }
  fout << "p cnf " << numVars << " " << cnf.size() << "\n";
  for (const cl_t& C : cnf) {
    for (const lit_t x : C) fout << x << " ";
    fout << "0" << "\n";
  }
  fout << "\n";
}
