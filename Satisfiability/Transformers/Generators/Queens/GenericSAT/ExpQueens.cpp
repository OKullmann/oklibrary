// Oliver Kullmann, 14.8.2018 (Swansea)
/* Copyright 2018, 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <iostream>
#include <string>
#include <fstream>

#include <cstdlib>

#include "Backtracking.hpp"
#include "NQueens.hpp"
#include "Heuristics.hpp"
#include "InOut.hpp"

namespace {

const std::string version = "0.4.17";
const std::string date = "19.2.2019";
const std::string program = "ExpQueens"
#ifndef NDEBUG
  "_debug"
#endif
;
const std::string error = "ERROR[" + program + "]: ";

void show_usage() noexcept {
  const std::string prompt = "\n> " + program;
  std::cout << "USAGE in two information-only and three main forms:\n"
    "\n> " << program << "\n"
    " shows usage information and exists.\n"
    "\n> " << program << " (-v | --version)\n"
    " shows version informations and exits.\n"
    "\n The following main usage-forms all start with N, the order of the square.\n"
    "  If N has a leading \"+\", then the backtracking-tree is output in .tlp-format.\n"
    " Round brackets \"()\" indicate a list of possible options (as characters), square brackets \"[]\" indicate optional arguments.\n"
    "\n> " << program << " N\n"
    " uses NQeens::TawHeuristics.\n"
    "\n> " << program << " N (f,r,s)\n"
    " uses first-open, random, or square for the branching heuristics.\n"
    "\n> " << program << " N r [seed]\n"
    " uses seed for the random form.\n";
  std::exit(0);
}

void version_information() noexcept {
  std::cout << program << ":\n"
   " Version: " << version << "\n"
   " Last change date: " << date << "\n"
#ifdef NDEBUG
   " Compiled with NDEBUG\n"
#else
   " Compiled without NDEBUG\n"
#endif
#ifdef __OPTIMIZE__
   " Compiled with optimisation options\n"
#else
   " Compiled without optimisation options\n"
#endif
   " Compilation date: " __DATE__ " " __TIME__ "\n"
#ifdef __GNUC__
   " Compiler: g++, version " __VERSION__ "\n"
#else
   " Compiler not gcc\n"
#endif
  ;
  std::exit(0);
}

}

int main(const int argc, const char* const argv[]) {
  using namespace std::string_literals;
  if (argc == 1) show_usage();
  if (argc == 2 and argv[1] == "-v"s) version_information();

  const ChessBoard::coord_t N = InOut::interprete(argc, argv, error);
  const bool tree_output = argv[1][0] == '+';
  const std::string option = (argc == 2) ? "" : argv[2];

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
      Trees::output(file, B.T, "ExpQueens, version = " + version, "TawHeuristics");
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
      Trees::output(file, B.T, "ExpQueens, version = " + version, "TawHeuristics+ne");
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
      Trees::output(file, B.T, "ExpQueens, version = " + version, "AntiTaw");
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
      Trees::output(file, B.T, "ExpQueens, version = " + version, "FirstOpenHeuristics");
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
        Trees::output(file, B.T, "ExpQueens, version = " + version, "RandomHeuristics");
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
        Trees::output(file, B.T, "ExpQueens, version = " + version, "RandomHeuristics");
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
      Trees::output(file, B.T, "ExpQueens, version = " + version, "EnumSquareHeuristics");
      return 0;
    }
  }
}
