// Oliver Kullmann, 3.4.2024 (Swansea)
/* Copyright 2024 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Reads m "queens cubes" of order N, and creates the SAT-instance with
  N*m variables directly representing the corresponding exact-cover problem.

EXAMPLES:

Statistics only:

LatinSquares> for N in 5 7 11 13; do for F in prime seco secouep; do CPandiagonal $N "" | clasp 0 | CP_clasp_first_columns.awk -v N=$N | ./ECSAT0_QueensCubes $F| awk '/^c N/{printf "%d ", $3}/^c co/{printf "%s ", $4}/^c n/{printf "%d ", $3}/^c c /{print $3}'; done; done
"prime" 5 10 30
"seco" 5 10 30
"secouep" 5 10 30
"prime" 7 28 301
"seco" 7 28 301
"secouep" 7 28 301
"prime" 11 88 3399
"seco" 11 110 3289
"secouep" 11 110 3311
"prime" 13 4524 6009991
"seco" 13 6760 5238571
"secouep" 13 6760 5240807

LatinSquares> N=17; time for F in prime seco secouep; do CPandiagonal $N "" | clasp 0 | CP_clasp_first_columns.awk -v N=$N | ./ECSAT0_QueensCubes $F | awk '/^c co/{printf "%s ", $4}/^c n/{printf "%d ", $3}/^c c /{print $3}'; done
"prime" 140692 3218145051
"seco" 211004 2636453875
"secouep" 211004 2636524187
real	29m0.715s
user	29m2.754s
sys	0m0.129s

Counting the solutions via ctawSolver:

LatinSquares> N=5; CPandiagonal $N "" | clasp 0 | CP_clasp_first_columns.awk -v N=$N | ./ECSAT0_QueensCubes_debug +
ECSAT0_QC_5_2.cnf
p cnf 10 30
LatinSquares> ctawSolver ECSAT0_QC_5_2.cnf | awk '/solutions/'
c number_of_solutions                   2
LatinSquares> N=7; CPandiagonal $N "" | clasp 0 | CP_clasp_first_columns.awk -v N=$N | ./ECSAT0_QueensCubes_debug +
ECSAT0_QC_7_4.cnf
p cnf 28 301
LatinSquares> ctawSolver ECSAT0_QC_7_4.cnf | awk '/solutions/'
c number_of_solutions                   4
LatinSquares> N=11; CPandiagonal $N "" | clasp 0 | CP_clasp_first_columns.awk -v N=$N | ./ECSAT0_QueensCubes_debug +
ECSAT0_QC_11_8.cnf
p cnf 88 3399
LatinSquares> ctawSolver ECSAT0_QC_11_8.cnf | awk '/solutions/'
c number_of_solutions                   8

LatinSquares> N=13; CPandiagonal $N "" | clasp 0 | CP_clasp_first_columns.awk -v N=$N | ./ECSAT0_QueensCubes_debug +
ECSAT0_QC_13_348.cnf
p cnf 4524 6009991
LatinSquares> ctawSolver ECSAT0_QC_13_348.cnf EC13OUT
XXX running server2 XXX


*/

#include <iostream>
#include <utility>
#include <ostream>

#include <cstdlib>

#include <ProgramOptions/Environment.hpp>

#include "PQOptions.hpp"
#include "Algorithms.hpp"
#include "ECEncoding.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.1",
        "7.4.2024",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/LatinSquares/ECSAT0_QueensCubes.cpp",
        "GPL v3"};

  const std::string error = "ERROR[" + proginfo.prg + "]: ";
  constexpr int commandline_args = 1;

  const std::string prefix = "ECSAT0_QC_", suffix = ".cnf";

  using CT = PQOptions::CT;

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
      "> " << proginfo.prg <<
      " [+]constraint-type\n\n"
      " - constraint-type : " << Environment::WRPO<CT>{} << "\n\n" <<
      "reads from standard input and establishes N, m:\n\n"
      "  - if \"+\" used, creates file " << prefix << "N_m" << suffix << "\n"
      "    (otherwise just statistics are output)\n"
      "  - for the option the first possibility is the default, "
        "triggered by the empty string.\n\n"
 ;
    return true;
  }

  std::pair<CT,bool> read_ct(const std::string& s) {
    if (s.empty()) return {};
    bool output = s.starts_with("+");
    const std::string s2 = output ? s.substr(1) : s;
    const auto ct0 = Environment::read<CT>(s2);
    if (not ct0) {
      std::cerr << error << "The constraint-type could not be read from"
        " string \"" << s2 << "\".\n";
      std::exit(1);
    }
    return {ct0.value(), output};
  }

  void statistics(std::ostream& out, const ECEncoding::EC0Encoding& enc,
                  const CT ct, const int argc, const char* const argv[],
                  const bool full = false) {
    using Environment::DHW;
    using Environment::DWW;
    if (full) {
      out << Environment::Wrap(proginfo, Environment::OP::dimacs);
      out << DHW{"Parameters"};
    }
    out << DWW{"command-line"};
    Environment::args_output(out, argc, argv);
    out << "\n"
        << DWW{"N"} << enc.N << "\n"
        << DWW{"m"} << enc.m << "\n"
        << DWW{"Constraint_type"} << ct << "\n"
        << DWW{"  Primary-n"} << enc.n0 << "\n"
        << DWW{"  Auxilliary-n"} << enc.naux << "\n"
        << DWW{"n"} << enc.n << "\n"
        << DWW{"  Exactly-One-clauses"} << enc.ceo << "\n"
        << DWW{"  Non-disjointness-clauses"} << enc.cbin << "\n"
        << DWW{"c"} << enc.c << "\n";
  }

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  if (argc != commandline_args + 1) {
    std::cerr << error << "Exactly " << commandline_args << " command-line"
      " arguments needed (constraint-type), but the real number is "
              << argc-1 << ".\n";
    return 1;
  }

  const auto [ct, output] = read_ct(argv[1]);

  const auto init_cubes = Algorithms::read_queens_cubing(std::cin);
  if (init_cubes.m == 0) {
    std::cout << "Empty input.\n";
    return 0;
  }
  const auto encoding = ECEncoding::EC0Encoding(init_cubes, ct);

  if (not output) {
    statistics(std::cout, encoding, ct, argc, argv);
  }
  else {
    const std::string filename = prefix + std::to_string(init_cubes.N) +
      "_" + std::to_string(init_cubes.m) + suffix;
    std::cout << filename << std::endl;
    std::ofstream file(filename);
    if (not file) {
      std::cerr << error << "Can not open file \"" << filename << "\" for"
        " writing.\n";
      return 1;
    }
    statistics(file, encoding, ct, argc, argv, true);
    std::cout << encoding.dp; std::cout.flush();
    ECEncoding::ecsat0(file, encoding);
  }
}
