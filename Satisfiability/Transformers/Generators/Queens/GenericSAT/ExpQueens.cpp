// Oliver Kullmann, 14.8.2018 (Swansea)
/* Copyright 2018, 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

BUGS:

1. The implementation of RandGen::shuffle left the first element of the
   sequence always unchanged.
    - This should have been caughed in the tests in TestSimTaw.cpp: every
      random permutation would have put {1,1} first.
    - Also the abandoned evaluation of comparing the randomness of
      data/ExperimentR1/2.R could have reflected on this.
    - The tests in TestSimTaw.cpp need update (and expansion); currently
      disabled.
    - A new ExperimentR3.R is needed, and comparison with the old data.
    - Experiments better also have the Git ID stated.
    - And better naming conventions for the files in subdirectory data
      are needed.

TODOS:

0. Update to new standard

1. All combinations:

  - For experimenting with counting queens, we have the following main
    aspects:
     - algorithmic
     - collection of statistics
  - The algorithmic aspect has the dimensions:
     - active clause-set (for propagation)
     - branching heuristics
     - unsat test.
  - The statistics-aspect has the main alternatives:
     - tree output, basically collecting "all statistics", for smaller trees
     - summary statistics, for larger trees.
  - Currently below in the main-function, only certain combinations are
    provided. We need a system which allows us to naturally obtain all
    combinations of these possibilities.
  - One way is to hardcode all the possible combinations, as done currently.
  - However this has multiplicative complexity, in the source-code, and in the
    executable, due to the presence of all template-parameter combinations.
  - We need to change this to additive complexity.
  - Also the output needs a complete overhaul, to completely represent the
    performed computations:
     - The new tools given by Random/Environment.hpp should be employed.
     - This needs to include ExpQueens version, information on the machine,
       the compilation, and the git-id.
  - We also update Makefile, using Random/Makefile as a role model:
     - Git-ID, machine name, and bogomips to be provided.
     - Add macro numerics_options for additional optimisation options.
     - Options used for compilation to be provided.

*/

#include <iostream>
#include <string>
#include <fstream>

#include <ProgramOptions/Environment.hpp>

#include "Backtracking.hpp"
#include "NQueens.hpp"
#include "Heuristics.hpp"
#include "InOut.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.5.0",
        "30.7.2019",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Queens/GenericSAT/ExpQueens.cpp",
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
  "\nThe following main usage-forms all start with N, the order of the square.\n"
  " If N has a leading \"+\", then the backtracking-tree is output in .tlp-format.\n"
  " Round brackets \"()\" in the following show the list of possible"
  " options (as characters), square brackets \"[]\" indicate optional arguments.\n"
  "\n> " << program << " N\n"
  " uses NQeens::TawHeuristics.\n"
  "\n> " << program << " N (f,r,s)\n"
  " uses first-open, random, or square for the branching heuristics.\n"
  "\n> " << program << " N r [seed]\n"
  " uses seed for the random form.\n";
  return true;
}

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  const ChessBoard::coord_t N = InOut::interprete(argc, argv, "ERROR[" + proginfo.prg + "]: ");
  const bool tree_output = argv[1][0] == '+';
  const std::string option = (argc == 2) ? "" : argv[2];

  std::cout << Environment::Wrap(proginfo, Environment::OP::dimacs);
  using Environment::DHW;
  using Environment::DWW;
  using Environment::qu;
  std::cout << DHW{"Parameters"}
            << DWW{"command-line"};
  std::cout << qu(argv[0]);
  for (int i = 1; i < argc; ++i) std::cout << " " << qu(argv[i]);
  std::cout << "\n"
            << DWW{"N"} << N << "\n"
            << DWW{"tree_output"} << std::boolalpha << tree_output << "\n";

  NQueens::AmoAlo_board Fq(N);
  if (option == "") {
    if (not tree_output) {
      Backtracking::CountSat<NQueens::AmoAlo_board, Heuristics::TawHeuristics<>> B;
      const auto rFq = B(Fq);
      std::cout << rFq;
      return 0;
    }
    else {
      Backtracking::CountSat<NQueens::AmoAlo_board, Heuristics::TawHeuristics<>, Trees::BasicTree> B;
      const auto rFq = B(Fq);
      std::cout << rFq;
      const std::string filename = "ExpQueens_" + std::to_string(N) + "_Taw_Basic.tlp";
      std::ofstream file{filename};
      Trees::output(file, B.T, "ExpQueens, version = " + proginfo.vrs, "TawHeuristics");
      return 0;
    }
  }
  if (option == "+ne") {
    if (not tree_output) {
      Backtracking::CountSat<NQueens::AmoAlo_board, Heuristics::TawHeuristics<>, Trees::NoOpTree, Backtracking::NotEnoughDiags> B;
      const auto rFq = B(Fq);
      std::cout << rFq;
      return 0;
    }
    else {
      Backtracking::CountSat<NQueens::AmoAlo_board, Heuristics::TawHeuristics<>, Trees::BasicTree, Backtracking::NotEnoughDiags> B;
      const auto rFq = B(Fq);
      std::cout << rFq;
      const std::string filename = "ExpQueens_" + std::to_string(N) + "_Taw_+ne.tlp";
      std::ofstream file{filename};
      Trees::output(file, B.T, "ExpQueens, version = " + proginfo.vrs, "TawHeuristics+ne");
      return 0;
    }
  }
  else if (option == "a") {
    if (not tree_output) {
      Backtracking::CountSat<NQueens::AmoAlo_board, Heuristics::AntiTaw<>> B;
      const auto rFq = B(Fq);
      std::cout << rFq;
      return 0;
    }
    else {
      Backtracking::CountSat<NQueens::AmoAlo_board, Heuristics::AntiTaw<>, Trees::BasicTree> B;
      const auto rFq = B(Fq);
      std::cout << rFq;
      const std::string filename = "ExpQueens_" + std::to_string(N) + "_AntiTaw_Basic.tlp";
      std::ofstream file{filename};
      Trees::output(file, B.T, "ExpQueens, version = " + proginfo.vrs, "AntiTaw");
      return 0;
    }
  }
  else if (option == "f") {
    if (not tree_output) {
      Backtracking::CountSat<NQueens::AmoAlo_board, Heuristics::FirstOpen> B;
      const auto rFq = B(Fq);
      std::cout << rFq;
      return 0;
    }
    else {
      Backtracking::CountSat<NQueens::AmoAlo_board, Heuristics::FirstOpen, Trees::BasicTree> B;
      const auto rFq = B(Fq);
      std::cout << rFq;
      const std::string filename = "ExpQueens_" + std::to_string(N) + "_First_Basic.tlp";
      std::ofstream file{filename};
      Trees::output(file, B.T, "ExpQueens, version = " + proginfo.vrs, "FirstOpenHeuristics");
      return 0;
    }
  }
  else if (option == "r") {
    if (not tree_output) {
      if (argc == 3) {
        Backtracking::CountSat<NQueens::AmoAlo_board, Heuristics::FirstOpenRandom> B(N);
        const auto rFq = B(Fq);
        std::cout << rFq;
        return 0;
      } else {
        typedef Heuristics::FirstOpenRandom::seed_t seed_t;
        const seed_t seed = std::stoul(argv[3]);
        typedef Heuristics::FirstOpenRandom::vec_seed_t vec_seed_t;
        Backtracking::CountSat<NQueens::AmoAlo_board, Heuristics::FirstOpenRandom> B(N,vec_seed_t{{seed}});
        const auto rFq = B(Fq);
        std::cout << rFq;
        return 0;
      }
    }
    else {
      if (argc == 3) {
        Backtracking::CountSat<NQueens::AmoAlo_board, Heuristics::FirstOpenRandom, Trees::BasicTree> B(N);
        const auto rFq = B(Fq);
        std::cout << rFq;
        const std::string filename = "ExpQueens_" + std::to_string(N) + "_Random_Basic.tlp";
        std::ofstream file{filename};
        Trees::output(file, B.T, "ExpQueens, version = " + proginfo.vrs, "RandomHeuristics");
        return 0;
      } else {
        typedef Heuristics::FirstOpenRandom::seed_t seed_t;
        const seed_t seed = std::stoul(argv[3]);
        typedef Heuristics::FirstOpenRandom::vec_seed_t vec_seed_t;
        Backtracking::CountSat<NQueens::AmoAlo_board, Heuristics::FirstOpenRandom, Trees::BasicTree> B(N,vec_seed_t{{seed}});
        const auto rFq = B(Fq);
        std::cout << rFq;
        const std::string filename = "ExpQueens_" + std::to_string(N) + "_Random_" + std::to_string(seed) + "_Basic.tlp";
        std::ofstream file{filename};
        Trees::output(file, B.T, "ExpQueens, version = " + proginfo.vrs, "RandomHeuristics");
       return 0;
      }
    }
  }
  else if (option == "s") {
    if (not tree_output) {
      Backtracking::CountSat<NQueens::AmoAlo_board, Heuristics::FirstOpenRandom> B(ChessBoard::enum_square(N));
      const auto rFq = B(Fq);
      std::cout << rFq;
      return 0;
    }
    else {
      Backtracking::CountSat<NQueens::AmoAlo_board, Heuristics::FirstOpenRandom, Trees::BasicTree> B(ChessBoard::enum_square(N));
      const auto rFq = B(Fq);
      std::cout << rFq;
      const std::string filename = "ExpQueens_" + std::to_string(N) + "_Square_Basic.tlp";
      std::ofstream file{filename};
      Trees::output(file, B.T, "ExpQueens, version = " + proginfo.vrs, "EnumSquareHeuristics");
      return 0;
    }
  }
}
