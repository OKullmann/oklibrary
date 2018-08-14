// Oliver Kullmann, 14.8.2018 (Swansea)

#include <iostream>
#include <string>

#include "Backtracking.hpp"
#include "NQueens.hpp"
#include "InOut.hpp"

namespace {

const std::string version = "0.2.1";
const std::string date = "14.8.2018";
const std::string program = "ExpQueens"
#ifndef NDEBUG
  "_debug"
#endif
;
const std::string error = "ERROR[" + program + "]: ";

}

int main(const int argc, const char* const argv[]) {

  const ChessBoard::coord_t N = InOut::interprete(argc, argv, error);
  NQueens::AmoAlo_board Fq(N);
  Backtracking::CountSat<NQueens::AmoAlo_board, NQueens::TawHeuristics> B;
  const auto rFq = B(Fq);
  std::cout << rFq;

}
