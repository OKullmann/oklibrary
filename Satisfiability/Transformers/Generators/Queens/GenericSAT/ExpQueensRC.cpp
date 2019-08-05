// Oliver Kullmann, 25.7.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* TODOS

1. Output parameters : DONE

2. Compute statistics : DONE

3. Implement min-length (rank) only for columns

4. Implement first-row and first-column:
    - class FirstRC in Heuristics.hpp : DONE

*/

#include <iostream>
#include <string>

#include <ProgramOptions/Environment.hpp>

#include "Backtracking.hpp"
#include "NQueens.hpp"
#include "Heuristics.hpp"
#include "InOut.hpp"
#include "Caching.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.3.0",
        "5.8.2019",
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
    "> " << program << " N [heuristics=0] [caching=0] [output-mode=d,rh,rd,rf]\n"
    " computes the solution-count for the board of dimension N.\n"
    "For timing-data in rd- or rf-mode, prefix the call with\n"
    "> /usr/bin/time -f\" %e\"\n"
;
    return true;
  }

  template <class BRANCHING, class CACHING=Backtracking::EmptyCACHING>
  using CSRC = Backtracking::CountSatRC<NQueens::AmoAlo_board, BRANCHING, CACHING>;

  template <Heuristics::LRC h, class CACHING=Backtracking::EmptyCACHING>
  using CSBLRC = CSRC<Heuristics::ByLengthRC<h>, CACHING>;
  template <Heuristics::FRC h, class CACHING=Backtracking::EmptyCACHING>
  using CSFRC = CSRC<Heuristics::ByFirstRC<h>, CACHING>;

  using FCm = Caching::FullCaching_map;
  using FSCm = Caching::FullSymCaching_map;

  constexpr ChessBoard::coord_t N_default = 11;

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  Environment::Index index;
  const ChessBoard::coord_t N =argc <= index ? N_default : InOut::interprete(argv[index++], "ERROR[" + proginfo.prg + "]: ", true);
  const int heuristics = argc <= index ? 0 : std::stoi(argv[index++]);
  using CS = Caching::CS;
  const CS caching = argc <= index ? CS::none : CS(std::stoi(argv[index++]));
  using OP = Environment::OP;
  const OP output_choice = (argc <= index) ? OP::dimacs :
    std::get<OP>(Environment::translate<OP>()(argv[index++], ','));
  Backtracking::StatisticsRC::op = output_choice;
  index.deactivate();

  std::cout << Environment::Wrap(proginfo, output_choice);

  // Parameter-output:
  using Environment::DWW;
  if (output_choice == OP::dimacs) {
    using Environment::DHW;
    using Environment::qu;
    std::cout << DHW{"Parameters"}
              << DWW{"command-line"};
    std::cout << qu(argv[0]);
    for (int i = 1; i < argc; ++i) std::cout << " " << qu(argv[i]);
    std::cout << "\n"
              << DWW{"N"} << N << "\n"
              << DWW{"caching"} << caching << "\n";
  }
  else if (output_choice == OP::rh or output_choice == OP::rf)
    std::cout << " N heur cache sol nds lvs h munds hts chts q t" << std::endl;
  if (output_choice == OP::rd or output_choice == OP::rf) {
    std::cout << N << " " << heuristics << " " << int(caching);
    std::cout.flush();
  }
  if (output_choice == OP::rh) return 0;

  NQueens::AmoAlo_board Fq(N);

  using Caching::CS;
  using std::cout;
  if (heuristics <= Heuristics::maxLRC) {
    using Heuristics::LRC;
    const LRC hrc = LRC(heuristics);
    if (output_choice == OP::dimacs)
      cout << DWW{"heuristics"} << hrc << std::endl;;
    switch (hrc) {
    case LRC::max :
      switch (caching) {
      case CS::none : cout << CSBLRC<LRC::max>()(Fq); return 0;
      case CS::full_ordered : cout << CSBLRC<LRC::max,FCm>()(Fq); return 0;
      default : cout << CSBLRC<LRC::max,FSCm>()(Fq); return 0;}
    case LRC::minrows :
      switch (caching) {
      case CS::none : cout << CSBLRC<LRC::minrows>()(Fq); return 0;
      case CS::full_ordered : cout << CSBLRC<LRC::minrows,FCm>()(Fq); return 0;
      default : cout << CSBLRC<LRC::minrows,FSCm>()(Fq); return 0;}
    default :
      switch (caching) {
      case CS::none : cout << CSBLRC<LRC::min>()(Fq); return 0;
      case CS::full_ordered : cout << CSBLRC<LRC::min,FCm>()(Fq); return 0;
      default : cout << CSBLRC<LRC::min,FSCm>()(Fq); return 0;}
    }
  }
  else {
    using Heuristics::FRC;
    const FRC hrc = FRC(heuristics - Heuristics::maxLRC - 1);
    if (output_choice == OP::dimacs)
      cout << DWW{"heuristics"} << hrc << std::endl;
    switch (hrc) {
    case FRC::column :
      switch (caching) {
      case CS::none : cout << CSFRC<FRC::column>()(Fq); return 0;
      case CS::full_ordered : cout << CSFRC<FRC::column,FCm>()(Fq); return 0;
      default : cout << CSFRC<FRC::column,FSCm>()(Fq); return 0;}
    default :
      switch (caching) {
      case CS::none : cout << CSFRC<FRC::row>()(Fq); return 0;
      case CS::full_ordered : cout << CSFRC<FRC::row,FCm>()(Fq); return 0;
      default : cout << CSFRC<FRC::row,FSCm>()(Fq); return 0;}
    }
  }

}
