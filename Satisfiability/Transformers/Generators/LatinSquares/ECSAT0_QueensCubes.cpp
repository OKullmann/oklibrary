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

LatinSquares> for N in 5 7 11 13; do for F in prime seco secouep; do CPandiagonal $N "" | clasp 0 | CP_clasp_first_columns.awk -v N=$N | ./ECSAT0_QueensCubes_debug $F "" | awk '/^c N/{printf "%d ", $3}/^c co/{printf "%s ", $4}/^c n/{printf "%d ", $3}/^c c /{print $3}'; done; done
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

LatinSquares> N=17; time for F in prime seco secouep; do CPandiagonal $N "" | clasp 0 | CP_clasp_first_columns.awk -v N=$N | ./ECSAT0_QueensCubes $F "" | awk '/^c co/{printf "%s ", $4}/^c n/{printf "%d ", $3}/^c c /{print $3}'; done
"prime" 140692 5912731359
"seco" 211004 5331040183
"secouep" 211004 5331110495
real	59m50.848s
user	59m52.904s
sys	0m0.170s

Counting the solutions via ctawSolver:

LatinSquares> N=5; CPandiagonal $N "" | clasp 0 | CP_clasp_first_columns.awk -v N=$N | ./ECSAT0_QueensCubes_debug + 0
ECSAT0_QC_5_2_prime0.cnf
p cnf 10 30
LatinSquares> ctawSolver ECSAT0_QC_5_2_prime0.cnf | awk '/solutions/'
c number_of_solutions                   2
LatinSquares> N=7; CPandiagonal $N "" | clasp 0 | CP_clasp_first_columns.awk -v N=$N | ./ECSAT0_QueensCubes_debug + ""
ECSAT0_QC_7_4_prime0.cnf
p cnf 28 301
LatinSquares> ctawSolver ECSAT0_QC_7_4_prime0.cnf | awk '/solutions/'
c number_of_solutions                   4
LatinSquares> N=11; CPandiagonal $N "" | clasp 0 | CP_clasp_first_columns.awk -v N=$N | ./ECSAT0_QueensCubes_debug + 0
ECSAT0_QC_11_8_prime0.cnf
p cnf 88 3399
LatinSquares> ctawSolver ECSAT0_QC_11_8_prime0.cnf | awk '/solutions/'
c number_of_solutions                   8

LatinSquares> N=13; CPandiagonal $N "" | clasp 0 | CP_clasp_first_columns.awk -v N=$N | ./ECSAT0_QueensCubes_debug + 0
ECSAT0_QC_13_348_prime0.cnf
p cnf 4524 6009991
LatinSquares> ctawSolver ECSAT0_QC_13_348_prime0.cnf EC13OUT
XXX running server2 XXX
LatinSquares$ clasp 0 -q ECSAT0_QC_13_348_prime0.cnf
XXX running server2 XXX

LatinSquares$ LatinSquares$ time cryptominisat5 --verb=0 --printsol,s 0 --maxsol 20000 --threads 100 ECSAT0_QC_13_348_prime0.cnf > OUTCR
^C*** INTERRUPTED ***
real    274m14.815s
user    10890m9.738s
sys     128m7.439s
Found 1711 solutions


TODOS:

1. More efficient determination of non-disjointness
    - Currently a brute-force approach is taken, only
      using that cubes with the same cube-index but
      different column-indices are disjoint.
    - At least only half of the work should be needed (two disjointness-
      clauses should be created at the same time).

*/

#include <iostream>
#include <utility>
#include <ostream>
#include <sstream>

#include <cstdlib>

#include <ProgramOptions/Environment.hpp>

#include "PQOptions.hpp"
#include "Algorithms.hpp"
#include "ECEncoding.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.4",
        "13.4.2024",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/LatinSquares/ECSAT0_QueensCubes.cpp",
        "GPL v3"};

  const std::string error = "ERROR[" + proginfo.prg + "]: ";
  constexpr int commandline_args = 2;

  using CT = PQOptions::CT;
  using AC = ECOptions::AC;
  using Algorithms::Cubing_t;

  const std::string prefix = "ECSAT0_QC_", suffix = ".cnf";
  std::string output_filename(const Cubing_t& C, const CT ct,
                              const AC ac) noexcept {
    std::stringstream res;
    res << prefix << C.N << "_" << C.m << "_" << Environment::W0(ct) <<
      Environment::W0(ac) << suffix;
    return res.str();
  }

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
      "> " << proginfo.prg <<
      " [+]constraint-type add-constraints\n\n"
      " - constraint-type : " << Environment::WRPO<CT>{} << "\n" <<
      " - add-constraint  : " << Environment::WRPO<AC>{} << "\n\n" <<
      "reads from standard input and establishes N, m:\n\n"
      "  - if \"+\" used, creates file " << prefix << "N_m_ctypeatype" <<
        suffix << "\n"
      "    (otherwise just statistics are output)\n"
      "  - for the options the first possibility is the default, "
        "triggered by the empty string.\n\n"
 ;
    return true;
  }

  std::pair<CT,bool> read_ct(const std::string& s) noexcept {
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
                  const int argc, const char* const argv[],
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
        << DWW{"Constraint_type"} << enc.ct << "\n"
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
  const AC ac = [&argv]{const auto ac0 = Environment::read<AC>(argv[2]);
    if (not ac0) {
      std::cerr << error << "The additional constraint could not be read from"
        " string \"" << argv[2] << "\".\n";
      std::exit(1);
    }
    return ac0.value();}();

  const Cubing_t init_cubes = Algorithms::read_queens_cubing(std::cin);
  if (init_cubes.m == 0) {
    std::cout << "Empty input.\n";
    return 0;
  }
  const auto encoding = ECEncoding::EC0Encoding(init_cubes, ct, ac);

  if (not output) {
    statistics(std::cout, encoding, argc, argv);
  }
  else {
    const std::string filename = output_filename(init_cubes, ct, ac);
    std::cout << filename << std::endl;
    std::ofstream file(filename);
    if (not file) {
      std::cerr << error << "Can not open file \"" << filename << "\" for"
        " writing.\n";
      return 1;
    }
    statistics(file, encoding, argc, argv, true);
    std::cout << encoding.dp; std::cout.flush();
    ECEncoding::ecsat0(file, encoding);
  }
}
