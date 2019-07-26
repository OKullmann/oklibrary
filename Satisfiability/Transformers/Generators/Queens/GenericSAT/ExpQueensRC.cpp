// Oliver Kullmann, 25.7.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* TODOS

1. Output parameters

2. Compute statistics

3. Implement min-length (rank) only for columns

4. Implement first-row and first-column (new class in Heuristics.hpp).

*/

#include <iostream>
#include <string>

#include <ProgramOptions/Environment.hpp>

#include "Backtracking.hpp"
#include "NQueens.hpp"
#include "Heuristics.hpp"
#include "InOut.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.0.4",
        "26.7.2019",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Queens/GenericSAT/ExpQueensRC.cpp",
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
    "> " << program << " N [0|1|2]\n"
    " computes the solution-count for the board of dimension N, "
    "using min/max/first rows/columns (default: min).\n"
;
    return true;
  }


}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  const ChessBoard::coord_t N = InOut::interprete(argc, argv, "ERROR[" + proginfo.prg + "]: ");
  const Heuristics::LRC minmax = argc == 2 ? Heuristics::LRC::min : Heuristics::LRC(std::stoi(argv[2]));

  NQueens::AmoAlo_board Fq(N);

  switch (minmax) {
  case Heuristics::LRC::min : {
    Backtracking::CountSatRC<NQueens::AmoAlo_board, Heuristics::ByLengthRC<Heuristics::LRC::min>> B;
    std::cout << B(Fq) << "\n";
    return 0;}
  case Heuristics::LRC::max : {
    Backtracking::CountSatRC<NQueens::AmoAlo_board, Heuristics::ByLengthRC<Heuristics::LRC::max>> B;
    std::cout << B(Fq) << "\n";
    return 0;}
  default : {
    Backtracking::CountSatRC<NQueens::AmoAlo_board, Heuristics::ByLengthRC<Heuristics::LRC::minrows>> B;
    std::cout << B(Fq) << "\n";
    return 0;}
  }

}
