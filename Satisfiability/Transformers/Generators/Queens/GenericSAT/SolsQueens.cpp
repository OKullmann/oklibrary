// Oliver Kullmann, 22.11.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

USAGE:

> ./SolsQueens

runs with fixed N.

With

> ./RSolsQueens N

first the version with (compile-time constant) N is compiled, and then
it is run.


TODOS:

1. We need the same statistics also for the modular solutions.

2. We should also have vertex-degree-statistics (min, max, mean).

 */

#include <iostream>
#include <string>
#include <fstream>

#include <ProgramOptions/Environment.hpp>
#include <Numerics/FloatingPoint.hpp>

#include "Backtracking.hpp"
#include "NQueens.hpp"
#include "Heuristics.hpp"
#include "Solutions.hpp"
#include "Recursion.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.2.4",
        "25.11.2019",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Queens/GenericSAT/SolsQueens.cpp",
        "GPL v3"};

bool show_usage(const int argc, const char* const argv[]) {
  assert(argc >= 1);
  if (argc != 2 or not Environment::is_help_string(argv[1])) return false;
  const std::string& program = proginfo.prg;
  std::cout << "USAGE:\n"
  "> " << program << " [-v | --version]\n"
  " shows version information and exits.\n"
  "> " << program << " [-h | --help]\n"
  " shows help information and exits.\n"
  "Run make with arguments \"-B CPPFLAGS=-DNQUEENS=12\" for e.g. N=12.\n"
;
  return true;
}

  using namespace Solutions;

  constexpr auto N =
#ifndef NQUEENS
  10
#else
  NQUEENS
#endif
;

  template <class ActiveClauseSet>
  struct ListSolutions {
    solution_vector<N> V;
    ListSolutions() { V.reserve(Recursion::exact_value(N)); }
    void sat(const ActiveClauseSet& F) {
      V.push_back(extract<N>(F.board()));
    }
    void unsat(const ActiveClauseSet&) noexcept {}
  };

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  std::cout << Environment::Wrap(proginfo, Environment::OP::dimacs);
  using Environment::DHW;
  using Environment::DWW;
  using Environment::qu;
  std::cout << DHW{"Parameters"}
            << DWW{"command-line"};
  Environment::args_output(std::cout, argc, argv);
  std::cout << "\n"
            << DWW{"N"} << (unsigned)N << "\n"
; std::cout.flush();

  NQueens::AmoAlo_board Fq(N);
  Backtracking::CountSat<NQueens::AmoAlo_board, Heuristics::TawHeuristics<>, Trees::NoOpTree, Backtracking::EmptyUSAT, Backtracking::Statistics<Backtracking::EmptyUSAT>, ListSolutions<NQueens::AmoAlo_board>> B;
  const auto rFq = B(Fq);
  const auto num_solutions = rFq.solutions;
  assert(num_solutions == Recursion::exact_value(N));
  assert(num_solutions == B.L.V.size());
  std::cout << num_solutions << std::endl;
  std::sort(B.L.V.begin(), B.L.V.end());

  const auto f1 = frequencies(determine_ccs1<N>(B.L.V));
  std::cout << f1 << std::endl;
  assert(not f1.first.empty());
  using FloatingPoint::float80;
  std::cout << float80((--f1.first.cend())->first) / num_solutions << "\n";

  const auto f2 = frequencies(determine_ccs2<N>(B.L.V));
  std::cout << f2 << "\n";
  assert(not f2.first.empty());
  std::cout << float80((--f2.first.cend())->first) / num_solutions << "\n";
}
