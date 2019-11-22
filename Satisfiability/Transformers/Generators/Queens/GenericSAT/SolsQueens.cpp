// Oliver Kullmann, 22.11.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

 */

#include <iostream>
#include <string>
#include <fstream>

#include <ProgramOptions/Environment.hpp>

#include "Backtracking.hpp"
#include "NQueens.hpp"
#include "Heuristics.hpp"
#include "Solutions.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.0.2",
        "22.11.2019",
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
;
  return true;
}

  using namespace Solutions;

  constexpr auto N = N_default;

  template <class ActiveClauseSet>
  struct ListSolutions {
    solution_vector<N> V;
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
  assert(num_solutions == B.L.V.size());
  std::cout << num_solutions << std::endl;
  std::sort(B.L.V.begin(), B.L.V.end());
  std::cout << frequencies(determine_1ccs<N>(B.L.V)) << "\n";
  std::cout << frequencies(determine_2ccs<N>(B.L.V)) << "\n";

}
