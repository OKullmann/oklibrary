// Oliver Kullmann, 6.1.2021 (Swansea)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

> ./TimingLSRG_debug
c Output_time 10.01.2021 20:07:43_+0000 1610309263904863701
c ** Program information **
c program_name                          "TimingLSRG_debug"
c version                               "0.2.1"
c date                                  "10.1.2021"
c gid_id                                "f7cc22befe9aec7f3c186e966f4452caade0ac58"
c author                                "Oliver Kullmann"
c url                                   "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/TimingLSRG.cpp"
c ** Machine information **
c machine_name                          "csltok2"
c bogomips                              5799.77
c ** Compilation information **
c compiler_version                      "g++ 10.1.0"
c compilation_date                      "Jan_10_2021 20:07:07"
c compilation_options                   "--std=c++20 -pedantic -fmax-errors=5 -Wall -Wextra -Wno-missing-field-initializers -Wno-dangling-else -g -D_GLIBCXX_DEBUG"
c NDEBUG                                0
c OPTIMIZE                              0
c ** Parameters **
c command-line                          "./TimingLSRG_debug"
c N                                     4
c T                                     10000
c gen-option                            ma+jm
c num_e-seeds                           15
c  e-seeds                              1609092523835210350 1 0 1609092786237186306 0 3 0 0 4 4 0 0 0 10000 0

N=4, reduced=0, L(N)=576
T=10000, found=575
exact prob=L(N)/T = 0.0017361111111111111111
frequency statistics:
N=575
                        0.0001      0.0017391304347826086962                        0.0035
                        median                        0.0019
                            sd     0.00080608483530132999749
p-value statistics:
N=575
     2.2651313323446083143e-05        0.29913738042966844056        0.93087659402422740743
                        median         0.1755738419849071785
                            sd        0.32087393100424178611
ks:        0.36052909021725858496     1.9208612998914688192e-67

Comparison with uniform generator for the exact prob:
Frequencies:
N=576
     0.00060000000000000000001      0.0017361111111111111111      0.0030999999999999999999
                        median                        0.0017
                            sd     0.00043187938913924223522
P-values:
N=576
      0.0010522204506551148561        0.48962399297574945756        0.93087659402422740743
                        median         0.5261566464333386553
                            sd        0.29252033388278899745
ks:        0.08115532070576386112     0.00095237591060431378348


TODOS:

1. Use exact computation for the p-values.

*/

#include <iostream>
#include <fstream>
#include <string>

#include <ProgramOptions/Environment.hpp>
#include <Numerics/FloatingPoint.hpp>

#include "LSRG.hpp"
#include "Numbers.hpp"
#include "Distributions.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.3.0",
        "11.1.2021",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/TimingLSRG.cpp",
        "GPL v3"};

  using namespace LSRG;

  namespace RG = RandGen;
  namespace LS = LatinSquares;

  constexpr LS::ls_dim_t N_default = 4;
  typedef RG::gen_uint_t count_t;
  constexpr count_t T_default = 10'000;

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg << " [N] [T] [option] [seeds]\n\n"
    " N       : default = " << N_default << "\n"
    " T       : default = " << T_default << "\n"
    " options : " << Environment::WRP<LS::StRLS>{} << "\n"
    "           " << Environment::WRP<GenO>{} << "\n"
    " seeds   : ";
    RG::explanation_seeds(std::cout, 11);
    std::cout << "\n"
    " generates T random Latin square of order N and tests them.\n"
;
    return true;
  }

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  Environment::Index index;

  using LS::ls_dim_t;
  const ls_dim_t N = argc <= index ? N_default :
    std::string_view(argv[index]).empty() ? index++,N_default :
    FloatingPoint::touint(argv[index++]);

  const count_t T = argc <= index ? T_default :
    std::string_view(argv[index]).empty() ? index++,T_default :
    FloatingPoint::toUInt(argv[index++]);

  const option_t options = argc <= index ? option_t{} :
    Environment::translate<option_t>()(argv[index++], sep);
  const LS::StRLS sto = std::get<LS::StRLS>(options);
  const GenO geo = std::get<GenO>(options);

  const std::string ss = argc <= index ? "" : argv[index++];

  index.deactivate();

  RG::vec_eseed_t seeds = basic_seeds(N, LS::Selection(N), geo, sto);
  seeds.push_back(T);
  SO::add_user_seeds(seeds, ss);
  seeds.push_back(0);

  std::cout << Environment::Wrap(proginfo, Environment::OP::dimacs);
  using Environment::DHW;
  using Environment::DWW;
  using Environment::qu;
  std::cout << DHW{"Parameters"}
            << DWW{"command-line"};
  Environment::args_output(std::cout, argc, argv);
  std::cout << "\n"
            << DWW{"N"} << N << "\n"
            << DWW{"T"} << T << "\n"
            << DWW{"std-option"} << sto << "\n"
            << DWW{"gen-option"} << geo << "\n"
            << DWW{"num_e-seeds"} << seeds.size() << "\n"
            << DWW{" e-seeds"} << RG::ESW{seeds} << "\n" << std::endl;

  Count_ls experiment(N, sto);
  RG::RandGen_t g_comp(seeds);
  RG::UniformRange u_comp(g_comp, experiment.total);
  std::vector<RG::gen_uint_t> counts(experiment.total);
  for (; seeds.back() < T; ++seeds.back()) {
    experiment.add(random_ls(N, LS::Selection(N), geo, sto, seeds));
    ++counts[u_comp()];
  }
  assert(experiment.count_all == T);

  FloatingPoint::fullprec_float80(std::cout);
  std::cout << experiment;

  std::cout << "\nComparison with uniform generator for the exact prob:\n";
  LSRG::Count_ls::Statistics f_comp, p_comp;
  for (const auto x : counts) {
    if (x == 0) continue;
    f_comp += FloatingPoint::float80(x) / T;
    p_comp += RG::monobit(x, T, experiment.p);
  }
  std::cout << "Frequencies:\n";
  f_comp.simple_output(std::cout);
  std::cout.flush();
  if (f_comp.N == experiment.total) {
    std::cout << "P-values:\n";
    p_comp.simple_output(std::cout, true);
    std::cout << "\n";
  }
}
