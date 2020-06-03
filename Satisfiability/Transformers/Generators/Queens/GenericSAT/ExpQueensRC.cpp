// Oliver Kullmann, 25.7.2019 (Swansea)
/* Copyright 2019, 2020 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Meaning of codes:

Heuristics:

0-5 are from Heuristics::LRC (lengths):

 0 : min
 1 : max
 2 : minrows
 3 : maxrows
 4 : mincolumns
 5 : maxcolumns

6-7 are from Heuristics::FRC:

 6 : firstrow
 7 : firstcolumns


Caching schemes (from Caching::CS):

0 : none
1 : full caching, using std::map
2 : full symmetric caching, using std::map
2 : full symmetric caching, using std::unordered_map

Symmetry breaking schemes:

0 : none
1 : Heuristics::InitialSymbreaking

*/

/* TODOS

1. Experiment with different orders of the branching-rowss/columns, to
   estimate the influence on caching.
   - A quick testing, just alternating the order of nodes in the backtracking
     algorithm (left-right and right-left), shows that the statistics are not
     invariant (and the changes for symmetric caching are bigger, indicating
     a small improvement).
   - One heuristics is "easier subproblems first", which can be estimated by
     the tawSolver-heuristic.
   - One can also use random permutations of the vector (1,...,N).
   - This needed a different basic branching-algorithm as currently in
     Backtracking::CountSatRC, since now the whole branching needs to be
     computed in advance.
   - Perhaps first the subproblems are put into a vector.
   - If that vector has length 0, add this to r3u and backtrack.
   - If that vector has length 1, add this to r3f, and perhaps avoid
     the recursive call.
   - Otherwise pass this vector by reference to the heuristic (provided by
     another template-parameter), which sorts it (swapping should here only
     swap the internal references to the vectors, not the elements -- we need
     to check that).

2. Define precisely "nodes", "leaves" etc. (especially in the presence of
   caching):
    - Currently cache-hits are counted as leaves; since the "nodes" would be
      here part of a dag, this does not seem appropriate.

3. Output the trees (for visualisation).

4. How to communicate the option-ranges to script "GenericSAT/Run" ?

5. Provide optimisation-algorithms, such that for example for N=8 one can
   search for a smallest RC-branching tree with symmetric caching.

6. Update tests.

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
        "0.8.0",
        "2.6.2020",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Queens/GenericSAT/ExpQueensRC.cpp",
        "GPL v3"};

  constexpr ChessBoard::coord_t N_default = 11;

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    const std::string& program = proginfo.prg;
    std::cout <<
    "> " << program << " [-ri | -rh]\n"
    " prints the R-info resp. -header and exits.\n"
    "\nThe main computation, using positional arguments, with\n"
    " the first value shown as default-value (for optional trailing arguments):\n\n"
    "> " << program << " [N=" << N_default << "]"
    " [heuristics=0.." << Heuristics::maxHeurOptions << "]"
    " [caching=0.." << Caching::maxCachOptions << "]"
    " [symmetry-breaking=0,1]"
    " [output-mode=" << Environment::wrpi(Environment::OP::dimacs) << "]\n\n"
    " computes the solution-count and statistics for the board of dimension N.\n"
    "\nThe different output-modes are:\n"
    "  - d = Dimacs\n"
    "  - s = simple\n"
    "  - e = explained\n"
    "  - rh = with R-header\n"
    "  - rd = R-data only\n"
    "  - rf = R-header and R-data\n"
    "Here the last two (rd or rf) expect timing+memory-data, achieved by prefixing the call, e.g.\n"
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
    std::cout << " N heur cache syb sol nds lvs h munds hts r2s r2u chts q mcs flf t mem" << std::endl;
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

  template <Heuristics::LRC h, class CACHING=Backtracking::EmptyCACHING>
  using CSIBLRC = CSRC<Heuristics::InitialSymBreaking<Heuristics::ByLengthRC<h>>, CACHING>;
  template <Heuristics::FRC h, class CACHING=Backtracking::EmptyCACHING>
  using CSIFRC = CSRC<Heuristics::InitialSymBreaking<Heuristics::ByFirstRC<h>>, CACHING>;

#define MLR(co) (symbreak?CSIBLRC<co>(N)():CSBLRC<co>(N)())
#define MLRC(co,ca) (symbreak?CSIBLRC<co,ca>(N)():CSBLRC<co,ca>(N)())

#define MFR(co) (symbreak?CSIFRC<co>(N)():CSFRC<co>(N)())
#define MFRC(co,ca) (symbreak?CSIFRC<co,ca>(N)():CSFRC<co,ca>(N)())

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;
  using OP = Environment::OP;
  if (Environment::is_rheader(argc, argv)) {
    output_R_attributes();
    return  0;
  }
  if (Environment::is_rinfo(argc, argv)) {
    std::cout << Environment::Wrap(proginfo, OP::rh);
    return  0;
  }

  Environment::Index index;
  const ChessBoard::coord_t N =argc <= index ? N_default : InOut::interprete(argv[index++], "ERROR[" + proginfo.prg + "]: ", true);
  const int heuristics = argc <= index ? 0 : std::stoi(argv[index++]);
  using CS = Caching::CS;
  const CS caching = argc <= index ? CS::none : CS(std::stoi(argv[index++]));
  const bool symbreak = argc <= index ? false : std::stoul(argv[index++]);
  const OP output_choice = (argc <= index) ? OP::dimacs :
    std::get<OP>(Environment::translate<OP>()(argv[index++], ','));
  Backtracking::StatisticsRC::op = output_choice;
  index.deactivate();

  if (output_choice != OP::rh)
    std::cout << Environment::Wrap(proginfo, output_choice);

  // Parameter-output:
  using Environment::DWW;
  if (output_choice == OP::dimacs) {
    using Environment::DHW;
    using Environment::qu;
    std::cout << DHW{"Parameters"}
              << DWW{"command-line"};
    Environment::args_output(std::cout, argc, argv);
    std::cout << "\n"
              << DWW{"N"} << N << "\n"
              << DWW{"caching"} << caching << "\n"
              << DWW{"symmetry-breaking"} << symbreak << "\n"
;
  }
  else if (output_choice == OP::rh or output_choice == OP::rf)
    output_R_attributes();
  else if (output_choice == OP::explained) {
    std::cout << "\n  ** Parameter values **\n\n";
    std::cout << "  The parameter line was: ";
    Environment::args_output(std::cout, argc, argv);
    std::cout << "\n  N=" << N << "\n"
              << "  Heuristics as int: " << heuristics << "\n"
              << "  Heuristics plain: ";
    if (heuristics <= Heuristics::maxLRC)
      std::cout << Heuristics::LRC(heuristics);
    else std::cout << Heuristics::FRC(heuristics);
    std::cout << "\n  Caching: " << caching << "\n";
    std::cout << "  Symmetry-breaking: " << std::boolalpha << symbreak << "\n\n";
  }
  if (Environment::isR(output_choice) or output_choice == OP::simple)
    std::cout << N << " " << heuristics << " " << int(caching) << " " << symbreak;
  std::cout.flush();

  using Caching::CS;
  using std::cout;
  if (heuristics <= Heuristics::maxLRC) {
    using Heuristics::LRC;
    const LRC hrc = LRC(heuristics);
    if (output_choice == OP::dimacs)
      cout << DWW{"heuristics"} << hrc << std::endl;
    switch (hrc) {
    case LRC::max :
      switch (caching) {
      case CS::none : cout << MLR(LRC::max); goto END;
      case CS::full_ordered : cout << MLRC(LRC::max,FCm); goto END;
      case CS::fullsym_ordered : cout << MLRC(LRC::max,FSCm); goto END;
      default : cout << MLRC(LRC::max,FSCh); goto END;}
    case LRC::minrows :
      switch (caching) {
      case CS::none : cout << MLR(LRC::minrows); goto END;
      case CS::full_ordered : cout << MLRC(LRC::minrows,FCm); goto END;
      case CS::fullsym_ordered : cout << MLRC(LRC::minrows,FSCm); goto END;
      default : cout << MLRC(LRC::minrows,FSCh); goto END;}
    case LRC::maxrows :
      switch (caching) {
      case CS::none : cout << MLR(LRC::maxrows); goto END;
      case CS::full_ordered : cout << MLRC(LRC::maxrows,FCm); goto END;
      case CS::fullsym_ordered : cout << MLRC(LRC::maxrows,FSCm); goto END;
      default : cout << MLRC(LRC::maxrows,FSCh); goto END;}
    case LRC::mincolumns :
      switch (caching) {
      case CS::none : cout << MLR(LRC::mincolumns); goto END;
      case CS::full_ordered : cout << MLRC(LRC::mincolumns,FCm); goto END;
      case CS::fullsym_ordered : cout << MLRC(LRC::mincolumns,FSCm); goto END;
      default : cout << MLRC(LRC::mincolumns,FSCh); goto END;}
    case LRC::maxcolumns :
      switch (caching) {
      case CS::none : cout << MLR(LRC::maxcolumns); goto END;
      case CS::full_ordered : cout << MLRC(LRC::maxcolumns,FCm); goto END;
      case CS::fullsym_ordered : cout << MLRC(LRC::maxcolumns,FSCm); goto END;
      default : cout << MLRC(LRC::maxcolumns,FSCh); goto END;}
    default :
      switch (caching) {
      case CS::none : cout << MLR(LRC::min); goto END;
      case CS::full_ordered : cout << MLRC(LRC::min,FCm); goto END;
      case CS::fullsym_ordered : cout << MLRC(LRC::min,FSCm); goto END;
      default : cout << MLRC(LRC::min,FSCh); goto END;}
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
      case CS::none : cout << MFR(FRC::column); goto END;
      case CS::full_ordered : cout << MFRC(FRC::column,FCm); goto END;
      case CS::fullsym_ordered : cout << MFRC(FRC::column,FSCm); goto END;
      default : cout << MFRC(FRC::column,FSCh); goto END;}
    default :
      switch (caching) {
      case CS::none : cout << MFR(FRC::row); goto END;
      case CS::full_ordered : cout << MFRC(FRC::row,FCm); goto END;
      case CS::fullsym_ordered : cout << MFRC(FRC::row,FSCm); goto END;
      default : cout << MFRC(FRC::row,FSCh); goto END;}
    }
  }
  END :
  if (output_choice == OP::simple or output_choice == OP::rh) cout << "\n";

}
