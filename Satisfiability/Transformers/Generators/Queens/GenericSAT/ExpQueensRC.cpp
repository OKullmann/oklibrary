// Oliver Kullmann, 25.7.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* TODOS

1. How to communicate the option-ranges to script "GenericSAT/Run" ?

2. Output the trees (for visualisation).

3. Experiment with different orders of the branching-rows/columns, to
   estimate the influence.

4. Provide optimisation-algorithms, such that for example for N=8 one can
   search for a smallest RC-branching tree with symmetric caching.

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
        "0.5.6",
        "10.8.2019",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Queens/GenericSAT/ExpQueensRC.cpp",
        "GPL v3"};

  constexpr ChessBoard::coord_t N_default = 11;

  bool show_usage(const int argc, const char* const argv[]) {
    assert(argc >= 1);
    if (argc != 2 or not Environment::is_help_string(argv[1])) return false;
    const std::string& program = proginfo.prg;
    std::cout << "USAGE:\n"
    "> " << program << " [-v | --version]\n"
    " shows version information and exits.\n"
    "> " << program << " [-h | --help]\n"
    " shows help information and exits.\n"
    "> " << program << " [-rh]\n"
    " prints the R-header and exits.\n"
    "Finally the main computation, using positional arguments, with\n"
    " the first value shown as default-value (for optional trailing arguments):\n\n"
    "> " << program << " [N=" << N_default << "]"
    " [heuristics=0.." << Heuristics::maxHeurOptions << "]"
    " [caching=0.." << Caching::maxCachOptions << "]"
    " [output-mode=d,rh,rd,rf]\n\n"
    " computes the solution-count and statistics for the board of dimension N.\n"
    "The different output-modes are: d=Dimacs, rh=R-header-only, rd=R-data-only, rf=R-full.\n"
    "For timing+memory-data in rd- or rf-mode, prefix the call, e.g.\n"
    "> /usr/bin/time -f\" %e %M\" ./" << program << " 12 0 2 rd\n"
    "producing the output\n"
    "> 12 0 2 14200 77102 43675 10 116 7 1055 3.075704225352112676 33426 0.08 4432\n"
    "Here the last two numbers are user-time in seconds,\n"
    "and max-memory in multiples of 1024 bytes (these numbers depend on the machine).\n"
    "Alternatively, use e.g.\n"
    "> ./Call ./" << program << " 12 0 2\n"
    "or, for using mode \"rf\"\n"
    "> ./Call ./" << program << " 12 0 2 rf\n"
    "(this pushes the additional argument \"rd\" added by Call out of consideration).\n"
;
    return true;
  }

  void output_R_attributes() {
    std::cout << " N heur cache sol nds lvs h munds hts chts q mcs flf t mem" << std::endl;
  }

  template <class BRANCHING, class CACHING=Backtracking::EmptyCACHING>
  using CSRC = Backtracking::CountSatRC<NQueens::AmoAlo_board, BRANCHING, CACHING>;

  template <Heuristics::LRC h, class CACHING=Backtracking::EmptyCACHING>
  using CSBLRC = CSRC<Heuristics::ByLengthRC<h>, CACHING>;
  template <Heuristics::FRC h, class CACHING=Backtracking::EmptyCACHING>
  using CSFRC = CSRC<Heuristics::ByFirstRC<h>, CACHING>;

  using FCm = Caching::FullCaching_map;
  using FSCm = Caching::FullSymCaching_map;
  using FSCh = Caching::FullSymCaching_hash;

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;
  using OP = Environment::OP;
  if (Environment::is_rheader(argc, argv)) {
    std::cout << Environment::Wrap(proginfo, OP::rh);
    output_R_attributes();
    return  0;
  }

  Environment::Index index;
  const ChessBoard::coord_t N =argc <= index ? N_default : InOut::interprete(argv[index++], "ERROR[" + proginfo.prg + "]: ", true);
  const int heuristics = argc <= index ? 0 : std::stoi(argv[index++]);
  using CS = Caching::CS;
  const CS caching = argc <= index ? CS::none : CS(std::stoi(argv[index++]));
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
    output_R_attributes();
  if (output_choice == OP::rd or output_choice == OP::rf) {
    std::cout << N << " " << heuristics << " " << int(caching);
    std::cout.flush();
  }
  if (output_choice == OP::rh) return 0;

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
      case CS::none : cout << CSBLRC<LRC::max>(N)(); return 0;
      case CS::full_ordered : cout << CSBLRC<LRC::max,FCm>(N)(); return 0;
      case CS::fullsym_ordered : cout << CSBLRC<LRC::max,FSCm>(N)(); return 0;
      default : cout << CSBLRC<LRC::max,FSCh>(N)(); return 0;}
    case LRC::minrows :
      switch (caching) {
      case CS::none : cout << CSBLRC<LRC::minrows>(N)(); return 0;
      case CS::full_ordered : cout << CSBLRC<LRC::minrows,FCm>(N)(); return 0;
      case CS::fullsym_ordered : cout << CSBLRC<LRC::minrows,FSCm>(N)(); return 0;
      default : cout << CSBLRC<LRC::minrows,FSCh>(N)(); return 0;}
    case LRC::maxrows :
      switch (caching) {
      case CS::none : cout << CSBLRC<LRC::maxrows>(N)(); return 0;
      case CS::full_ordered : cout << CSBLRC<LRC::maxrows,FCm>(N)(); return 0;
      case CS::fullsym_ordered : cout << CSBLRC<LRC::maxrows,FSCm>(N)(); return 0;
      default : cout << CSBLRC<LRC::maxrows,FSCh>(N)(); return 0;}
    case LRC::mincolumns :
      switch (caching) {
      case CS::none : cout << CSBLRC<LRC::mincolumns>(N)(); return 0;
      case CS::full_ordered : cout << CSBLRC<LRC::mincolumns,FCm>(N)(); return 0;
      case CS::fullsym_ordered : cout << CSBLRC<LRC::mincolumns,FSCm>(N)(); return 0;
      default : cout << CSBLRC<LRC::mincolumns,FSCh>(N)(); return 0;}
    case LRC::maxcolumns :
      switch (caching) {
      case CS::none : cout << CSBLRC<LRC::maxcolumns>(N)(); return 0;
      case CS::full_ordered : cout << CSBLRC<LRC::maxcolumns,FCm>(N)(); return 0;
      case CS::fullsym_ordered : cout << CSBLRC<LRC::maxcolumns,FSCm>(N)(); return 0;
      default : cout << CSBLRC<LRC::maxcolumns,FSCh>(N)(); return 0;}
    default :
      switch (caching) {
      case CS::none : cout << CSBLRC<LRC::min>(N)(); return 0;
      case CS::full_ordered : cout << CSBLRC<LRC::min,FCm>(N)(); return 0;
      case CS::fullsym_ordered : cout << CSBLRC<LRC::min,FSCm>(N)(); return 0;
      default : cout << CSBLRC<LRC::min,FSCh>(N)(); return 0;}
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
      case CS::none : cout << CSFRC<FRC::column>(N)(); return 0;
      case CS::full_ordered : cout << CSFRC<FRC::column,FCm>(N)(); return 0;
      case CS::fullsym_ordered : cout << CSFRC<FRC::column,FSCm>(N)(); return 0;
      default : cout << CSFRC<FRC::column,FSCh>(N)(); return 0;}
    default :
      switch (caching) {
      case CS::none : cout << CSFRC<FRC::row>(N)(); return 0;
      case CS::full_ordered : cout << CSFRC<FRC::row,FCm>(N)(); return 0;
      case CS::fullsym_ordered : cout << CSFRC<FRC::row,FSCm>(N)(); return 0;
      default : cout << CSFRC<FRC::row,FSCh>(N)(); return 0;}
    }
  }

}
