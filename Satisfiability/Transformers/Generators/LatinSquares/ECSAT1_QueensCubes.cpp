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
ECSAT1_QC_5_2_amoprimeprime_nc0.cnf
p cnf 135 310
LatinSquares> ctawSolver ECSAT1_QC_5_2_amoprimeprime_nc0.cnf | awk '/solutions/'
c number_of_solutions                   2

LatinSquares> N=13; CPandiagonal $N "" | clasp 0 | CP_clasp_first_columns.awk -v N=$N | ./ECSAT1_QueensCubes_debug "" "" "" ""
ECSAT1_QC_13_348_amoprimeprime_nc0.cnf
p cnf 6721 856921
LatinSquares> time clasp 0 -q ECSAT1_QC_13_348_amoprimeprime_nc0.cnf
XXX running server2 XXX
LatinSquares> N=13; CPandiagonal $N "" | clasp 0 | CP_clasp_first_columns.awk -v N=$N | ./ECSAT1_QueensCubes_debug "" "" secouep ""
ECSAT1_QC_13_348_amoprimesecouep_nc0.cnf
p cnf 8957 87737
LatinSquares> time clasp 0 -q ECSAT1_QC_13_348_amoprimesecouep_nc0.cnf
XXX running server2 XXX

On server3:
LatinSquares> N=17; time CPandiagonal $N "" | clasp 0 | CP_clasp_first_columns.awk -v N=$N | ./ECSAT1_QueensCubes "" "" "" ""
ECSAT1_QC_17_8276_amoprimeprime_nc0.cnf
p cnf 145605 584544235
real    1m17.329s
user    1m6.445s
sys     0m11.247s
LatinSquares> ls -l ECSAT1_QC_17_8276_amoprimeprime_nc0.cnf
9684162076
LatinSquares> time cadical -q -n ECSAT1_QC_17_8276_amoprimeprime_nc0.cnf
s SATISFIABLE
real    1m49.018s
user    1m17.416s
sys     0m31.554s

LatinSquares> N=17; time CPandiagonal $N "" | clasp 0 | CP_clasp_first_columns.awk -v N=$N | ./ECSAT1_QueensCubes "" "" seco ""
ECSAT1_QC_17_8276_amoprimeseco_nc0.cnf
p cnf 215917 2853059
real    0m5.069s
user    0m5.377s
sys     0m0.110s
LatinSquares> time cadical -q -n ECSAT1_QC_17_8276_amoprimeseco_nc0.cnf
s SATISFIABLE
real    0m0.661s
user    0m0.480s
sys     0m0.180s
With solution:
LatinSquares> N=17; cadical -q ECSAT1_QC_17_8276_amoprimeseco_nc0.cnf | passextractpos.awk | Pass2PSquares $N /dev/stdin
 0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16
 2  3  4  5  6  7  8  9 10 11 12 13 14 15 16  0  1
 8  9 10 11 12 13 14 15 16  0  1  2  3  4  5  6  7
 6  7  8  9 10 11 12 13 14 15 16  0  1  2  3  4  5
 9 10 11 12 13 14 15 16  0  1  2  3  4  5  6  7  8
 1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16  0
10 11 12 13 14 15 16  0  1  2  3  4  5  6  7  8  9
14 15 16  0  1  2  3  4  5  6  7  8  9 10 11 12 13
16  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15
 3  4  5  6  7  8  9 10 11 12 13 14 15 16  0  1  2
12 13 14 15 16  0  1  2  3  4  5  6  7  8  9 10 11
 4  5  6  7  8  9 10 11 12 13 14 15 16  0  1  2  3
 7  8  9 10 11 12 13 14 15 16  0  1  2  3  4  5  6
 5  6  7  8  9 10 11 12 13 14 15 16  0  1  2  3  4
11 12 13 14 15 16  0  1  2  3  4  5  6  7  8  9 10
13 14 15 16  0  1  2  3  4  5  6  7  8  9 10 11 12
15 16  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14

Non-cyclic:
LatinSquares> N=17; time CPandiagonal $N "" | clasp 0 | CP_clasp_first_columns.awk -v N=$N | ./ECSAT1_QueensCubes "" "" seco nc1
ECSAT1_QC_17_8276_amoprimeseco_nc1.cnf
p cnf 215917 2861335
real    0m5.418s
user    0m5.980s
sys     0m0.059s
LatinSquares> time cadical -q -n ECSAT1_QC_17_8276_amoprimeseco_nc1.cnf
aborted after 5min


LatinSquares> N=19; time CPandiagonal $N "" | clasp 0 | CP_clasp_first_columns.awk -v N=$N | ./ECSAT1_QueensCubes "" "" seco ""
ECSAT1_QC_19_43184_amoprimeseco_nc0.cnf
p cnf 1237565 18112548
real    2m15.017s
user    2m17.128s
sys     0m0.780s
LatinSquares> time cadical -q -n ECSAT1_QC_19_43184_amoprimeseco_nc0.cnf
s SATISFIABLE
real    0m4.047s
user    0m3.008s
sys     0m1.037s
With solution:
LatinSquares> N=19; cadical -q ECSAT1_QC_19_43184_amoprimeseco_nc0.cnf | passextractpos.awk | Pass2PSquares $N /dev/stdin
 0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18
 4  5  6  7  8  9 10 11 12 13 14 15 16 17 18  0  1  2  3
 7  8  9 10 11 12 13 14 15 16 17 18  0  1  2  3  4  5  6
 5  6  7  8  9 10 11 12 13 14 15 16 17 18  0  1  2  3  4
 8  9 10 11 12 13 14 15 16 17 18  0  1  2  3  4  5  6  7
12 13 14 15 16 17 18  0  1  2  3  4  5  6  7  8  9 10 11
14 15 16 17 18  0  1  2  3  4  5  6  7  8  9 10 11 12 13
16 17 18  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15
18  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17
 2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18  0  1
11 12 13 14 15 16 17 18  0  1  2  3  4  5  6  7  8  9 10
 3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18  0  1  2
 6  7  8  9 10 11 12 13 14 15 16 17 18  0  1  2  3  4  5
 9 10 11 12 13 14 15 16 17 18  0  1  2  3  4  5  6  7  8
 1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18  0
10 11 12 13 14 15 16 17 18  0  1  2  3  4  5  6  7  8  9
13 14 15 16 17 18  0  1  2  3  4  5  6  7  8  9 10 11 12
15 16 17 18  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14
17 18  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16

LatinSquares> N=19; time CPandiagonal $N "" | clasp 0 | CP_clasp_first_columns.awk -v N=$N | ./ECSAT1_QueensCubes eo "" secouep ""
ECSAT1_QC_19_43184_eoprimesecouep_nc0.cnf
p cnf 1237565 18523119
real	2m32.375s
user	2m36.948s
sys	0m0.510s
LatinSquares> N=19; cadical -q  ECSAT1_QC_19_43184_eoprimesecouep_nc0.cnf | passextractpos.awk | Pass2PSquares $N /dev/stdin
Same solution as above.
LatinSquares> N=19; time CPandiagonal $N "" | clasp 0 | CP_clasp_first_columns.awk -v N=$N | ./ECSAT1_QueensCubes eo secouep secouep ""
ECSAT1_QC_19_43184_eosecouepsecouep_nc0.cnf
p cnf 1240453 18482687
real	2m27.191s
user	2m31.396s
sys	0m0.554s
LatinSquares> N=19; cadical -q ECSAT1_QC_19_43184_eosecouepsecouep_nc0.cnf | passextractpos.awk | Pass2PSquares $N /dev/stdin
 0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18
17 18  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16
 8  9 10 11 12 13 14 15 16 17 18  0  1  2  3  4  5  6  7
13 14 15 16 17 18  0  1  2  3  4  5  6  7  8  9 10 11 12
18  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17
 1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18  0
11 12 13 14 15 16 17 18  0  1  2  3  4  5  6  7  8  9 10
 5  6  7  8  9 10 11 12 13 14 15 16 17 18  0  1  2  3  4
 7  8  9 10 11 12 13 14 15 16 17 18  0  1  2  3  4  5  6
12 13 14 15 16 17 18  0  1  2  3  4  5  6  7  8  9 10 11
 3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18  0  1  2
15 16 17 18  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14
 2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18  0  1
14 15 16 17 18  0  1  2  3  4  5  6  7  8  9 10 11 12 13
16 17 18  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15
 9 10 11 12 13 14 15 16 17 18  0  1  2  3  4  5  6  7  8
 4  5  6  7  8  9 10 11 12 13 14 15 16 17 18  0  1  2  3
 6  7  8  9 10 11 12 13 14 15 16 17 18  0  1  2  3  4  5
10 11 12 13 14 15 16 17 18  0  1  2  3  4  5  6  7  8  9


Statistics only:

LatinSquares> for N in 5 7 11 13 17; do for F in prime seco secouep; do printf "%d %s " $N $F; CPandiagonal $N "" | clasp 0 | CP_clasp_first_columns.awk -v N=$N | ./ECSAT1_QueensCubes "" "" $F "" | awk '/^p/{print; exit 0}'; done; done
5 prime p cnf 135 310
5 seco p cnf 135 310
5 secouep p cnf 135 310
7 prime p cnf 371 1274
7 seco p cnf 371 1274
7 secouep p cnf 371 1274
11 prime p cnf 1419 7942
11 seco p cnf 1441 7832
11 secouep p cnf 1441 7854
13 prime p cnf 6721 856921
13 seco p cnf 8957 85501
13 secouep p cnf 8957 87737
17 prime p cnf 145605 584544235
17 seco p cnf 215917 2853059
17 secouep p cnf 215917 2923371

*/

#include <iostream>
#include <ostream>
#include <sstream>

#include <cstdlib>

#include <ProgramOptions/Environment.hpp>

#include "PQOptions.hpp"
#include "Algorithms.hpp"
#include "ECEncoding.hpp"
#include "ECOptions.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.2.0",
        "20.4.2024",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/LatinSquares/ECSAT1_QueensCubes.cpp",
        "GPL v3"};

  const std::string error = "ERROR[" + proginfo.prg + "]: ";
  constexpr int commandline_args = 4;

  using CF = PQOptions::CF; using CT = PQOptions::CT;
  using NC = ECOptions::NC; using CC = ECOptions::CC; using DQ = ECOptions::DQ;
  using PQOptions::no_output;
  using Algorithms::UInt_t;
  using Algorithms::Cubing_t;

  const std::string prefix = "ECSAT1_QC_", suffix = ".cnf";
  std::string output_filename(const UInt_t N, const UInt_t m,
                              const CF cf1, const CT ct1, const CT ct2,
                              const NC nc, const CC cc, const DQ dq) noexcept {
    std::ostringstream res(prefix, std::ios::ate);
    using Environment::W0;
    res << N << "_" << m << "_"
        << W0(cf1) << W0(ct1) << W0(ct2) << "_"
        << W0(nc) << W0(cc) << W0(dq)
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
      " - add-constraints  : comma-separated list of:\n"
      "                    :  - " << Environment::WRPO<NC>{} << "\n" <<
      "                    :  - " << Environment::WRPO<CC>{} << "\n" <<
      "                    :  - " << Environment::WRPO<DQ>{} << "\n\n" <<
      "reads from standard input and establishes N, m:\n\n"
      "  - creates file " << prefix << "N_m_cform1ctype1ctype2_atype" <<
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
        << DWW{"Additional_constraints"} << "\n"
        << DWW{"  cyclicity"} << enc.nc << "\n"
        << DWW{"  conjunctions->queens"} << enc.cc << "\n"
        << DWW{"  cells->disjunctions"} << enc.dq << "\n"
        << DWW{"Variables"} << "\n"
        << DWW{"   Primary-n-cells"} << enc.nprimecells << "\n"
        << DWW{"   Primary-n-queens"} << enc.nprimequeens << "\n"
        << DWW{"  Primary-n"} << enc.n0 << "\n"
        << DWW{"   Auxilliary-n-cells"} << enc.nauxcells << "\n"
        << DWW{"   Auxilliary-n-queens"} << enc.nauxqueens << "\n"
        << DWW{"n"} << enc.n << "\n"
        << DWW{"Clauses"} << "\n"
        << DWW{"  Clauses-cells"} << enc.ceocells << "\n"
        << DWW{"  Clauses-queens"} << enc.ceoqueens << "\n"
        << DWW{"  Connecting-2-clauses"} << enc.cbin << "\n"
        << DWW{"  Non-cyclic-clauses"} << enc.cnoncyclic << "\n"
        << DWW{"  Cell-conjunctions"} << enc.cconjcells << "\n"
        << DWW{"  Queens-disjunctions"} << enc.cdisjqc << "\n"
        << DWW{"c"} << enc.c << "\n";
  }

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  if (argc != commandline_args + 1) {
    std::cerr << error << "Exactly " << commandline_args << " command-line"
      " arguments needed (c-form1, c-type1, c-type2, add-constraints),"
      " but the real number is " << argc-1 << ".\n";
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
  const auto [nc,cc,dq] =
    Environment::translate<ECOptions::full_ac_t>()(argv[4],',');
  if (not ECEncoding::valid(cf1,ct1,dq)) {
    std::cout << no_output << "For cf1=" << cf1
              << " one can not have dq=" << dq << ".\n";
    return 0;
  }

  const Cubing_t cubes = Algorithms::read_queens_cubing(std::cin);
  if (cubes.m == 0) {
    std::cout << "Empty input.\n";
    return 0;
  }

  const std::string filename = output_filename(cubes.N,cubes.m,
                                               cf1,ct1,ct2,
                                               nc,cc,dq);
  std::cout << filename << std::endl;
  std::ofstream file(filename);
  if (not file) {
    std::cerr << error << "Can not open file \"" << filename << "\" for"
      " writing.\n";
    return 1;
  }

  const auto encoding = ECEncoding::EC1Encoding(cubes, cf1,ct1,ct2, nc,cc,dq);

  statistics(file, encoding, argc,argv);
  std::cout << encoding.dp; std::cout.flush();
  ECEncoding::ecsat1(file, encoding);

}
