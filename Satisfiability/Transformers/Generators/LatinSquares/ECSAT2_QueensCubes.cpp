// Oliver Kullmann, 14.4.2024 (Swansea)
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
  Using now all the pandiagonal constraints on the cells.

EXAMPLES:

LatinSquares> N=5; CPandiagonal $N "" | clasp 0 | CP_clasp_first_columns.awk -v N=$N | ./ECSAT2_QueensCubes_debug "" "" "" ""
ECSAT2_QC_5_2_amoprimeprime_nc0.cnf
p cnf 135 1315
LatinSquares> clasp 0 -q ECSAT2_QC_5_2_amoprimeprime_nc0.cnf | awk '/Models/'
c Models         : 2

LatinSquares> N=7; CPandiagonal $N "" | clasp 0 | CP_clasp_first_columns.awk -v N=$N | ./ECSAT2_QueensCubes_debug "" "" "" ""
ECSAT2_QC_7_4_amoprimeprime_nc0.cnf
p cnf 371 5397
LatinSquares> clasp 0 -q ECSAT2_QC_7_4_amoprimeprime_nc0.cnf | awk '/Models/'
c Models         : 4

LatinSquares> N=11; CPandiagonal $N "" | clasp 0 | CP_clasp_first_columns.awk -v N=$N | ./ECSAT2_QueensCubes_debug "" "" "" ""
ECSAT2_QC_11_8_amoprimeprime_nc0.cnf
p cnf 1419 34573
LatinSquares> clasp 0 -q ECSAT2_QC_11_8_amoprimeprime_nc0.cnf | awk '/Models/'
c Models         : 8

LatinSquares$ N=13; CPandiagonal $N "" | clasp 0 | CP_clasp_first_columns.awk -v N=$N | ./ECSAT2_QueensCubes_debug eo "" secouep nc
ECSAT2_QC_13_348_eoprimesecouep_nc1.cnf
p cnf 8957 141671
LatinSquares> time cadical -q -n ECSAT2_QC_13_348_eoprimesecouep_nc1.cnf
s SATISFIABLE
real	0m1.531s
user	0m1.526s
sys	0m0.005s
LatinSquares> time cadical -q ECSAT2_QC_13_348_eoprimesecouep_nc1.cnf | passextractpos.awk | Pass2PSquares $N /dev/stdin
 0  1  2  3  4  5  6  7  8  9 10 11 12
 2  6  0  1 11 12  8  5 10  7  4  3  9
 4  7  5 12  8  9 10 11  1  3  0  2  6
 5 12  8  9 10 11  1  3  0  2  6  4  7
 8  9  7  4  2  3  0 12  6  1 11  5 10
 3 10  1  6  0  4  2  9  7  5 12  8 11
 1 11  3  2  5  6  7  4 12  8  9 10  0
12  0  6 11  7 10  5  8  9  4  2  1  3
 9  4 12  5  1  8  3 10 11  6  7  0  2
 7  2  9  8 12  0 11  1  3 10  5  6  4
 6  8  4 10  9  1 12  0  2 11  3  7  5
10  5 11  7  3  2  9  6  4  0  1 12  8
11  3 10  0  6  7  4  2  5 12  8  9  1


LatinSquares$ N=17; CPandiagonal $N "" | clasp 0 | CP_clasp_first_columns.awk -v N=$N | ./ECSAT2_QueensCubes eo "" secouep nc1
ECSAT2_QC_17_8276_eoprimesecouep_nc1.cnf
p cnf 215917 3090325
LatinSquares> time cadical -q -n ECSAT2_QC_17_8276_eoprimesecouep_nc1.cnf
aborted after 4 min


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
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/LatinSquares/ECSAT2_QueensCubes.cpp",
        "GPL v3"};

  const std::string error = "ERROR[" + proginfo.prg + "]: ";
  constexpr int commandline_args = 4;

  using CF = PQOptions::CF; using CT = PQOptions::CT;
  using NC = ECOptions::NC; using CC = ECOptions::CC; using DQ = ECOptions::DQ;
  using PQOptions::no_output;
  using Algorithms::UInt_t;
  using Algorithms::Cubing_t;

  const std::string prefix = "ECSAT2_QC_", suffix = ".cnf";
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

  void statistics(std::ostream& out, const ECEncoding::EC2Encoding& enc,
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

  const auto encoding = ECEncoding::EC2Encoding(cubes, cf1,ct1,ct2, nc,cc,dq);

  statistics(file, encoding, argc,argv);
  std::cout << encoding.dp; std::cout.flush();
  ECEncoding::ecsat2(file, encoding);

}
