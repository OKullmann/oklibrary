// Oliver Kullmann, 14.8.2018 (Swansea)

#include <iostream>
#include <string>

#include <cstdlib>

#include "Backtracking.hpp"
#include "NQueens.hpp"
#include "InOut.hpp"

namespace {

const std::string version = "0.2.4";
const std::string date = "16.8.2018";
const std::string program = "ExpQueens"
#ifndef NDEBUG
  "_debug"
#endif
;
const std::string error = "ERROR[" + program + "]: ";

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

  if (argc == 2 and std::string(argv[1]) == "-v") version_information();
  const ChessBoard::coord_t N = InOut::interprete(argc, argv, error);
  const std::string option = (argc == 2) ? "" : argv[2];
  NQueens::AmoAlo_board Fq(N);
  if (option == "") {
    Backtracking::CountSat<NQueens::AmoAlo_board, NQueens::TawHeuristics> B;
    const auto rFq = B(Fq);
    std::cout << rFq;
  }
  else {
    Backtracking::CountSat<NQueens::AmoAlo_board, NQueens::FirstOpen> B;
    const auto rFq = B(Fq);
    std::cout << rFq;
  }
}
