// Oliver Kullmann, 25.7.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <iostream>
#include <string>
#include <fstream>

#include <cstdlib>

#include <ProgramOptions/Environment.hpp>

#include "Backtracking.hpp"
#include "NQueens.hpp"
#include "Heuristics.hpp"
#include "InOut.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.0.1",
        "25.7.2019",
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
    "> " << program << " N\n"
    " computes the solution-count for the board of dimension N.\n"
;
    return true;
  }


}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  const ChessBoard::coord_t N = InOut::interprete(argc, argv, "ERROR[" + proginfo.prg + "]: ");

  NQueens::AmoAlo_board Fq(N);

  Backtracking::CountSatRC<NQueens::AmoAlo_board, Heuristics::MinLengthRC<>> B;
  std::cout << B(Fq) << "\n";

}
