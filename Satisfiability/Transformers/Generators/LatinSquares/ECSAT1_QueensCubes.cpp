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

LatinSquares> N=5; CPandiagonal $N "" | clasp 0 | CP_clasp_first_columns.awk -v N=$N | ./ECSAT1_QueensCubes_debug "" "" "" ""
ECSAT1_QC_5_2_amoprimeprime0.cnf
p cnf 135 310
LatinSquares> ctawSolver ECSAT1_QC_5_2_amoprimeprime0.cnf | awk '/solutions/'
c number_of_solutions                   2

LatinSquares$ N=13; CPandiagonal $N "" | clasp 0 | CP_clasp_first_columns.awk -v N=$N | ./ECSAT1_QueensCubes_debug "" "" ""
ECSAT1_QC_13_348_amoprimeprime.cnf
p cnf 6721 856921
LatinSquares> time clasp 0 -q ECSAT1_QC_13_348_amoprimeprime.cnf
XXX running server2 XXX
LatinSquares$ N=13; CPandiagonal $N "" | clasp 0 | CP_clasp_first_columns.awk -v N=$N | ./ECSAT1_QueensCubes_debug "" "" secouep
ECSAT1_QC_13_348_amoprimesecouep.cnf
p cnf 8957 87737
LatinSquares> time clasp 0 -q ECSAT1_QC_13_348_amoprimesecouep.cnf
XXX running server2 XXX

On server3:
LatinSquares$ N=17; time CPandiagonal $N "" | clasp 0 | CP_clasp_first_columns.awk -v N=$N | ./ECSAT1_QueensCubes "" "" ""
ECSAT1_QC_17_8276_amoprimeprime.cnf
p cnf 145605 584544235
real    1m17.329s
user    1m6.445s
sys     0m11.247s
LatinSquares$ ls -l ECSAT1_QC_17_8276_amoprimeprime.cnf
9684161982
LatinSquares$ time cadical -q -n ECSAT1_QC_17_8276_amoprimeprime.cnf
s SATISFIABLE
real    1m49.018s
user    1m17.416s
sys     0m31.554s

LatinSquares$ N=17; time CPandiagonal $N "" | clasp 0 | CP_clasp_first_columns.awk -v N=$N | ./ECSAT1_QueensCubes "" "" seco ""
ECSAT1_QC_17_8276_amoprimeseco0.cnf
p cnf 215917 2853059
real    0m5.069s
user    0m5.377s
sys     0m0.110s
LatinSquares$ time cadical -q -n ECSAT1_QC_17_8276_amoprimeseco0.cnf
s SATISFIABLE
real    0m0.661s
user    0m0.480s
sys     0m0.180s
With solution:
LatinSquares$ cadical -q ECSAT1_QC_17_8276_amoprimeseco0.cnf | CP_clasp_first_columns.awk -v N=$N | ./CP_clasp_expand.awk
  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20
 18 19 20  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17
 17 18 19 20  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16
 16 17 18 19 20  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15
 15 16 17 18 19 20  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14
 14 15 16 17 18 19 20  0  1  2  3  4  5  6  7  8  9 10 11 12 13
 13 14 15 16 17 18 19 20  0  1  2  3  4  5  6  7  8  9 10 11 12
 12 13 14 15 16 17 18 19 20  0  1  2  3  4  5  6  7  8  9 10 11
 11 12 13 14 15 16 17 18 19 20  0  1  2  3  4  5  6  7  8  9 10
 10 11 12 13 14 15 16 17 18 19 20  0  1  2  3  4  5  6  7  8  9
  9 10 11 12 13 14 15 16 17 18 19 20  0  1  2  3  4  5  6  7  8
  8  9 10 11 12 13 14 15 16 17 18 19 20  0  1  2  3  4  5  6  7
  7  8  9 10 11 12 13 14 15 16 17 18 19 20  0  1  2  3  4  5  6
  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20  0  1  2  3  4  5
  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20  0  1  2  3  4
  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20  0  1  2  3
  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20  0  1  2
  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20  0  1  2  3  4  5
  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20  0  1  2  3  4
  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20  0  1  2  3
  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20  0  1  2

Non-cyclic:
LatinSquares$ N=17; time CPandiagonal $N "" | clasp 0 | CP_clasp_first_columns.awk -v N=$N | ./ECSAT1_QueensCubes "" "" seco nc
ECSAT1_QC_17_8276_amoprimeseconc.cnf
p cnf 215917 2861335
real    0m5.418s
user    0m5.980s
sys     0m0.059s
LatinSquares$ time cadical -q -n ECSAT1_QC_17_8276_amoprimeseconc.cnf
aborted after 4min



LatinSquares$ N=19; time CPandiagonal $N "" | clasp 0 | CP_clasp_first_columns.awk -v N=$N | ./ECSAT1_QueensCubes "" "" seco
ECSAT1_QC_19_43184_amoprimeseco.cnf
p cnf 1237565 18112548
real    2m15.017s
user    2m17.128s
sys     0m0.780s
LatinSquares$ time cadical -q -n ECSAT1_QC_19_43184_amoprimeseco.cnf
s SATISFIABLE
real    0m4.047s
user    0m3.008s
sys     0m1.037s
With solution:
LatinSquares$ cadical -q ECSAT1_QC_19_43184_amoprimeseco.cnf  | CP_clasp_first_columns.awk -v N=$N | ./CP_clasp_expand.awk
  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18
  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18  0
  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18  0  1
  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18  0  1  2
  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18  0  1  2  3
  5  6  7  8  9 10 11 12 13 14 15 16 17 18  0  1  2  3  4
  6  7  8  9 10 11 12 13 14 15 16 17 18  0  1  2  3  4  5
  7  8  9 10 11 12 13 14 15 16 17 18  0  1  2  3  4  5  6
  8  9 10 11 12 13 14 15 16 17 18  0  1  2  3  4  5  6  7
  9 10 11 12 13 14 15 16 17 18  0  1  2  3  4  5  6  7  8
 10 11 12 13 14 15 16 17 18  0  1  2  3  4  5  6  7  8  9
 11 12 13 14 15 16 17 18  0  1  2  3  4  5  6  7  8  9 10
 12 13 14 15 16 17 18  0  1  2  3  4  5  6  7  8  9 10 11
 13 14 15 16 17 18  0  1  2  3  4  5  6  7  8  9 10 11 12
 14 15 16 17 18  0  1  2  3  4  5  6  7  8  9 10 11 12 13
 15 16 17 18  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14
 16 17 18  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15
 17 18  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16
 18  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17

On server:
LatinSquares> N=19; time CPandiagonal $N "" | clasp 0 | CP_clasp_first_columns.awk -v N=$N | ./ECSAT1_QueensCubes eo "" secouep
ECSAT1_QC_19_43184_eoprimesecouep.cnf
p cnf 1237565 18523119
real    3m2.071s
user    3m5.514s
sys     0m0.704s
LatinSquares$ cadical -q ECSAT1_QC_19_43184_eoprimesecouep.cnf  | CP_clasp_first_columns.awk -v N=$N | ./CP_clasp_expand.awk
Same solution as above.
LatinSquares> N=19; time CPandiagonal $N "" | clasp 0 | CP_clasp_first_columns.awk -v N=$N | ./ECSAT1_QueensCubes eo secouep secouep
ECSAT1_QC_19_43184_eosecouepsecouep.cnf
p cnf 1240453 18482687
real    3m6.959s
user    3m10.467s
sys     0m0.917s
LatinSquares$ cadical -q ECSAT1_QC_19_43184_eosecouepsecouep.cnf | CP_clasp_first_columns.awk -v N=$N | ./CP_clasp_expand.awk
Same solution as above.


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
        "0.1.3",
        "18.4.2024",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/LatinSquares/ECSAT1_QueensCubes.cpp",
        "GPL v3"};

  const std::string error = "ERROR[" + proginfo.prg + "]: ";
  constexpr int commandline_args = 4;

  using CF = PQOptions::CF;
  using CT = PQOptions::CT;
  using NC = ECOptions::NC;
  using PQOptions::no_output;
  using Algorithms::UInt_t;
  using Algorithms::Cubing_t;

  const std::string prefix = "ECSAT1_QC_", suffix = ".cnf";
  std::string output_filename(const UInt_t N, const UInt_t m,
                              const CF cf1, const CT ct1,
                              const CT ct2, const NC nc) noexcept {
    std::ostringstream res(prefix, std::ios::ate);
    using Environment::W0;
    res << N << "_" << m << "_"
        << W0(cf1) << W0(ct1) << W0(ct2) << W0(nc)
        << suffix;
    return res.str();
  }

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
      "> " << proginfo.prg <<
      " c-form1 c-type1 c-type2 add-constraints\n\n"
      " - constraint-form : " << Environment::WRPO<CF>{} << "\n" <<

      " - constraint-type : " << Environment::WRPO<CT>{} << "\n" <<
      " - add-constraint  : " << Environment::WRPO<NC>{} << "\n\n" <<
      "reads from standard input and establishes N, m:\n\n"
      "  - creates file " << prefix << "N_m_cform1ctype1ctype2atype" <<
        suffix << "\n"
      "   - except for disallowed combination, where it prints \"" <<
        no_output << "\" and the reason\n"
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
        << DWW{"Additional_Constraint"} << enc.nc << "\n"
        << DWW{"   Primary-n-cells"} << enc.N3 << "\n"
        << DWW{"  Primary-n"} << enc.n0 << "\n"
        << DWW{"   Auxilliary-n-cells"} << enc.naux1 << "\n"
        << DWW{"   Auxilliary-n-queens"} << enc.naux2 << "\n"
        << DWW{"n"} << enc.n << "\n"
        << DWW{"  Clauses-cells"} << enc.ceo1 << "\n"
        << DWW{"  Clauses-queens"} << enc.ceo2 << "\n"
        << DWW{"  Connecting-2-clauses"} << enc.cbin << "\n"
        << DWW{"  Non-cyclic-clauses"} << enc.cnoncyclic << "\n"
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
  if (not allowed(cf1, ct1)) {
    std::cout << no_output << "For cf1=" << cf1
              << " one can not have ct1=" << ct1 << ".\n";
    return 0;
  }
  const CT ct2 = read_ct(argv[3]);
  const NC nc = [&argv]{const auto nc0 = Environment::read<NC>(argv[4]);
    if (not nc0) {
      std::cerr << error << "The additional constraint could not be read from"
        " string \"" << argv[4] << "\".\n";
      std::exit(1);
    }
    return nc0.value();}();

  const Cubing_t cubes = Algorithms::read_queens_cubing(std::cin);
  if (cubes.m == 0) {
    std::cout << "Empty input.\n";
    return 0;
  }

  const std::string filename = output_filename(cubes.N,cubes.m,cf1,ct1,ct2,nc);
  std::cout << filename << std::endl;
  std::ofstream file(filename);
  if (not file) {
    std::cerr << error << "Can not open file \"" << filename << "\" for"
      " writing.\n";
    return 1;
  }

  const auto encoding = ECEncoding::EC1Encoding(cubes, cf1, ct1, ct2, nc);

  statistics(file, encoding, argc,argv);
  std::cout << encoding.dp; std::cout.flush();
  ECEncoding::ecsat1(file, encoding);

}
