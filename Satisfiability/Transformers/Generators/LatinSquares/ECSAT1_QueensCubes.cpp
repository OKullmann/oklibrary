// Oliver Kullmann, 9.4.2024 (Swansea)
/* Copyright 2024 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Reads m "queens cubes" of order N, and creates the SAT-instance with
  N^3 + N*m variables:
   - the N^3 variables are the direct encoding of the N^2 cells, with any
     combination of alo/amo/eo and prime/seco/secouep
   - the N*m variables represent the queens-cubes, which now imply each
     exactly N cells; eo per "column", with one of prime/seco/secouep.

EXAMPLES:

LatinSquares> N=5; CPandiagonal $N "" | clasp 0 | CP_clasp_first_columns.awk -v N=$N | ./ECSAT1_QueensCubes_debug "" "" ""

XXX

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
"prime" 140692 5912731359
"seco" 211004 5331040183
"secouep" 211004 5331110495
real	59m50.848s
user	59m52.904s
sys	0m0.170s

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
LatinSquares$ clasp 0 -q ECSAT0_QC_13_348.cnf
XXX running server2 XXX

LatinSquares$ LatinSquares$ time cryptominisat5 --verb=0 --printsol,s 0 --maxsol 20000 --threads 100 ECSAT0_QC_13_348.cnf > OUTCR
^C*** INTERRUPTED ***
real    274m14.815s
user    10890m9.738s
sys     128m7.439s
Found 1711 solutions


*/

#include <iostream>
#include <ostream>
#include <sstream>

#include <cstdlib>

#include <ProgramOptions/Environment.hpp>

#include "PQOptions.hpp"
#include "Algorithms.hpp"
#include "ECEncoding.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.0.5",
        "11.4.2024",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/LatinSquares/ECSAT1_QueensCubes.cpp",
        "GPL v3"};

  const std::string error = "ERROR[" + proginfo.prg + "]: ";
  constexpr int commandline_args = 3;

  using CF = PQOptions::CF;
  using CT = PQOptions::CT;
  using Algorithms::UInt_t;
  using Algorithms::Cubing_t;

  const std::string prefix = "ECSAT1_QC_", suffix = ".cnf";
  std::string output_filename(const UInt_t N, const UInt_t m,
                              const CF cf1, const CT ct1,
                              const CT ct2) noexcept {
    std::ostringstream res(prefix, std::ios::ate);
    using Environment::W0;
    res << N << "_" << m << "_"
        << W0(cf1) << W0(ct1) << W0(ct2)
        << suffix;
    return res.str();
  }

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
      "> " << proginfo.prg <<
      " c-form1 c-type1 c-type2\n\n"
      " - constraint-form : " << Environment::WRPO<CF>{} << "\n" <<

      " - constraint-type : " << Environment::WRPO<CT>{} << "\n\n" <<
      "reads from standard input and establishes N, m:\n\n"
      "  - creates file " << prefix << "N_m_cform1ctype1ctype2" <<
        suffix << "\n"
      "  - for the options the first possibility is the default, "
        "triggered by the empty string.\n\n"
 ;
    return true;
  }

  CF read_cf(const std::string& s) noexcept {
    if (s.empty()) return {};
    const auto cf0 = Environment::read<CF>(s);
    if (not cf0) {
      std::cerr << error << "The constraint-form could not be read from"
        " string \"" << s << "\".\n";
      std::exit(1);
    }
    return cf0.value();
  }
  CT read_ct(const std::string& s) noexcept {
    if (s.empty()) return {};
    const auto ct0 = Environment::read<CT>(s);
    if (not ct0) {
      std::cerr << error << "The constraint-type could not be read from"
        " string \"" << s << "\".\n";
      std::exit(1);
    }
    return ct0.value();
  }

  void statistics(std::ostream& out, const ECEncoding::EC1Encoding& enc,
                  const int argc, const char* const argv[]) {
    using Environment::DHW;
    using Environment::DWW;
    out << Environment::Wrap(proginfo, Environment::OP::dimacs);
    out << DHW{"Parameters"};
    out << DWW{"command-line"};
    Environment::args_output(out, argc, argv);
    out << "\n"
        << DWW{"N"} << enc.N << "\n"
        << DWW{"m"} << enc.m << "\n"
        << DWW{"Constraints_cells"} << enc.cf1 << " " << enc.ct1 << "\n"
        << DWW{"Constraints_queens"} << enc.ct2 << "\n"
        << DWW{"   Primary-n-cells"} << enc.N3 << "\n"
        << DWW{"  Primary-n"} << enc.n0 << "\n"
        << DWW{"   Auxilliary-n-cells"} << enc.naux1 << "\n"
        << DWW{"   Auxilliary-n-queens"} << enc.naux2 << "\n"
        << DWW{"n"} << enc.n << "\n"
        << DWW{"  Clauses-cells"} << enc.ceo1 << "\n"
        << DWW{"  Clauses-queens"} << enc.ceo2 << "\n"
        << DWW{"  Connecting-2-clauses"} << enc.cbin << "\n"
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

  const CF cf1 = read_cf(argv[1]);
  const CT ct1 = read_ct(argv[2]);
  const CT ct2 = read_ct(argv[3]);

  const Cubing_t cubes = Algorithms::read_queens_cubing(std::cin);
  if (cubes.m == 0) {
    std::cout << "Empty input.\n";
    return 0;
  }

  const std::string filename = output_filename(cubes.N,cubes.m, cf1,ct1, ct2);
  std::cout << filename << std::endl;
  std::ofstream file(filename);
  if (not file) {
    std::cerr << error << "Can not open file \"" << filename << "\" for"
      " writing.\n";
    return 1;
  }

  const auto encoding = ECEncoding::EC1Encoding(cubes, cf1, ct1, ct2);

  statistics(file, encoding, argc,argv);
  std::cout << encoding.dp; std::cout.flush();
  ECEncoding::ecsat1(file, encoding);

}
