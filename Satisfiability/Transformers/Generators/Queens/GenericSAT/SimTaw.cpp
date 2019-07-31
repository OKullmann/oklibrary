// Oliver Kullmann, 23.7.2018 (Swansea)
/* Copyright 2018, 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* TODOS:

1. Update to new standard

*/

#include <iostream>
#include <string>

#include "ChessBoard.hpp"
#include "Backtracking.hpp"
#include "NQueens.hpp"
#include "Heuristics.hpp"
#include "InOut.hpp"

namespace {

const std::string version = "0.4.12";
const std::string date = "31.7.2019";
const std::string program = "SimTaw"
#ifndef NDEBUG
  "_debug"
#endif
;
const std::string error = "ERROR[" + program + "]: ";

constexpr ChessBoard::coord_t N_default = 10;
}

int main(const int argc, const char* const argv[]) {

  const ChessBoard::coord_t N = argc == 1 ? N_default : InOut::interprete(argv[1], error);
  typedef NQueens::AmoAlo_board AmoAlo;
  AmoAlo Fq(N);
  Backtracking::CountSat<AmoAlo, Heuristics::TawHeuristics<AmoAlo>> B;
  const auto rFq = B(Fq);
  std::cout << rFq.solutions << " " << rFq.nodes << "\n";
  return 0;
}
