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
ECSAT2_QC_5_2_amoprimeprime0.cnf
p cnf 135 1315
LatinSquares> clasp 0 -q ECSAT2_QC_5_2_amoprimeprime0.cnf | awk '/Models/'
c Models         : 2

LatinSquares> N=7; CPandiagonal $N "" | clasp 0 | CP_clasp_first_columns.awk -v N=$N | ./ECSAT2_QueensCubes_debug "" "" "" ""
ECSAT2_QC_7_4_amoprimeprime0.cnf
p cnf 371 5397
LatinSquares> clasp 0 -q ECSAT2_QC_7_4_amoprimeprime0.cnf | awk '/Models/'
c Models         : 4

LatinSquares> N=11; CPandiagonal $N "" | clasp 0 | CP_clasp_first_columns.awk -v N=$N | ./ECSAT2_QueensCubes_debug "" "" "" ""
ECSAT2_QC_11_8_amoprimeprime0.cnf
p cnf 1419 34573
LatinSquares> clasp 0 -q ECSAT2_QC_11_8_amoprimeprime0.cnf | awk '/Models/'
c Models         : 8

LatinSquares$ N=13; CPandiagonal $N "" | clasp 0 | CP_clasp_first_columns.awk -v N=$N | ./ECSAT2_QueensCubes_debug eo "" secouep nc
ECSAT2_QC_13_348_eoprimesecouepnc.cnf
p cnf 8957 141671
LatinSquares> time cadical -q -n ECSAT2_QC_13_348_eoprimesecouepnc.cnf
s SATISFIABLE
real	0m1.531s
user	0m1.526s
sys	0m0.005s

LatinSquares$ N=17; CPandiagonal $N "" | clasp 0 | CP_clasp_first_columns.awk -v N=$N | ./ECSAT2_QueensCubes eo "" secouep nc
ECSAT2_QC_17_8276_eoprimesecouepnc.cnf
p cnf 215917 3090325
LatinSquares> time cadical -q -n ECSAT2_QC_17_8276_eoprimesecouepnc.cnf
aborted



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
        "0.1.0",
        "14.4.2024",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/LatinSquares/ECSAT2_QueensCubes.cpp",
        "GPL v3"};

  const std::string error = "ERROR[" + proginfo.prg + "]: ";
  constexpr int commandline_args = 4;

  using CF = PQOptions::CF;
  using CT = PQOptions::CT;
  using AC = ECOptions::AC;
  using Algorithms::UInt_t;
  using Algorithms::Cubing_t;

  const std::string prefix = "ECSAT2_QC_", suffix = ".cnf";
  std::string output_filename(const UInt_t N, const UInt_t m,
                              const CF cf1, const CT ct1,
                              const CT ct2, const AC ac) noexcept {
    std::ostringstream res(prefix, std::ios::ate);
    using Environment::W0;
    res << N << "_" << m << "_"
        << W0(cf1) << W0(ct1) << W0(ct2) << W0(ac)
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
      " - add-constraint  : " << Environment::WRPO<AC>{} << "\n\n" <<
      "reads from standard input and establishes N, m:\n\n"
      "  - creates file " << prefix << "N_m_cform1ctype1ctype2atype" <<
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
        << DWW{"Additional_Constraint"} << enc.ac << "\n"
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
  const CT ct2 = read_ct(argv[3]);
  const AC ac = [&argv]{const auto ac0 = Environment::read<AC>(argv[4]);
    if (not ac0) {
      std::cerr << error << "The additional constraint could not be read from"
        " string \"" << argv[4] << "\".\n";
      std::exit(1);
    }
    return ac0.value();}();

  const Cubing_t cubes = Algorithms::read_queens_cubing(std::cin);
  if (cubes.m == 0) {
    std::cout << "Empty input.\n";
    return 0;
  }

  const std::string filename = output_filename(cubes.N,cubes.m,cf1,ct1,ct2,ac);
  std::cout << filename << std::endl;
  std::ofstream file(filename);
  if (not file) {
    std::cerr << error << "Can not open file \"" << filename << "\" for"
      " writing.\n";
    return 1;
  }

  const auto encoding = ECEncoding::EC2Encoding(cubes, cf1, ct1, ct2, ac);

  statistics(file, encoding, argc,argv);
  std::cout << encoding.dp; std::cout.flush();
  ECEncoding::ecsat2(file, encoding);

}
