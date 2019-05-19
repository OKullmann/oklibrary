// Oliver Kullmann, 15.5.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Statistics for class bernoulli_random_device

Examples (annotations on following lines):

Version information:

Profiling version:

TODOS:

1. Add parallelisation.
    - What are we doing then with the output?
    - Just no output? Or to different files, for each thread one?
    - Seems easiest to have no output, and hoping that the run will finish
      soon enough.

2. Add the option to use the precise Bernoulli-distribution instead
   of the approximation (for the analysis of frequencies).
    - One gets always very low p-values ksfreq and ksruns for low N-values
      -- does this come from the approximated calculation of the primary
      p-values for frequencies and runs?

For example (on cs-wsok):
ExpMD_1558155799820561145> time ./MetaDevice 1e5 1e3 1e4 > data
real    1127m27.438s
user    58m20.733s
sys     1068m9.666s
ExpMD_1558155799820561145> cat data
# Timestamp: 18.05.2019 06:04:21_+0100 1558155861050217151
# Producing program: https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/MetaDevice.cpp
# program name:       MetaDevice
#  version:           0.1.0
#  last change:       15.5.2019
#  git-id:            e37f77bbef6be3bd2fa98a910f3f0097bc3f7462
# machine name:       cs-wsok
#  bogomips:          5986.74
# compiler version:   g++ 8.3.0
#  compilation date:  May_18_2019 06:00:51
#  used options:      --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static -fno-signed-zeros -fno-math-errno -fno-trapping-math -fno-unsafe-math-optimizations -fno-associative-math -fno-reciprocal-math -fno-finite-math-only
# N = 100000, M = 1000, T = 10000
 ksfreq lksfreq cksfreq pksfreq ksruns lksruns cksruns pksruns minpfreq minpruns
-4.7921736023859295983e-17 3 14 0.1354261473739373761 -4.7921736023859295983e-17 3 13 0.20834867055478272597 4.9618905220487352376e-05 9.9999500501463112201e-06

These very low ks-p-values always seem to arise from higher T-values.
The values themselves can't be correct, since negative, and their
computation should be improved. But let's assume that they are indeed
very low, so really unlikely.

For comparison with MetaBernoulli (now on csverify):
$ time ./MetaBernoulli 1e5 1e3 1e4 1/2 > data
XXX


Smaller T-values, higher N-value (so the normal approximation is more
precise):
ExpMD_1558229218716428942> time ./MetaDevice 1e6 1e3 1e3 > data
XXX

For comparison with MetaBernoulli:



3. Also make the precise analysis of runs available (without the
   normal approximation).

*/

#include <iostream>

#include <cassert>

#include <Numerics/FloatingPoint.hpp>
#include <ProgramOptions/Environment.hpp>

#include "Distributions.hpp"
#include "Tests.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.0",
        "15.5.2019",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/MetaDevice.cpp",
        "GPL v3"};

  using namespace RandGen;

  constexpr gen_uint_t N_default = 1e4L;
  constexpr gen_uint_t M_default = 1e3L;
  constexpr gen_uint_t T_default = 1e2L;


  // Outputting the parameters:

  using FloatingPoint::Wrap;
  using FloatingPoint::float80;

  void out_header(std::ostream& out) {
    out << " ksfreq lksfreq cksfreq pksfreq ksruns lksruns cksruns pksruns minpfreq minpruns\n";
  }

  void analyse(const gen_uint_t T, const gen_uint_t M, const gen_uint_t N, AnalysePVal_vt& Afreq, AnalysePVal_vt& Aruns) {
    for (gen_uint_t i = 0; i < T; ++i) {
      fvec_t Pfreq, Pruns;
      Pfreq.reserve(M); Pruns.reserve(M);

      for (gen_uint_t j = 0; j < M; ++j) {
        bernoulli_random_device b;
        if (b.entropy() == 0) {
          std::cerr << "[MetaDevice]: entropy = 0.\n";
          std::exit(1);
        }
        CountRuns count(b());
        for (gen_uint_t k = 1; k < N; ++k) count(b());
        const auto [cr, ct] = *count;
        Pfreq.push_back(monobit(ct, N));
        Pruns.push_back(runstest_gen(N, cr, {1,2}));
      }
      assert(Pfreq.size() == M and Pruns.size() == M);

      Afreq[i] = analyse_pvalues(Pfreq);
      Aruns[i] = analyse_pvalues(Pruns);
    }
  }

}

int main(const int argc0, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc0, argv)) return 0;
  const bool profiling = Environment::profiling(argc0, argv);
  const int argc = profiling ? 1 : argc0;

  Environment::Index index;
  const gen_uint_t N = (argc <= index) ? N_default : FloatingPoint::toUInt(argv[index++]);
  assert(N >= 1);
  const gen_uint_t M = (argc <= index) ? M_default : FloatingPoint::toUInt(argv[index++]);
  assert(M >= 1);
  const gen_uint_t T = (argc <= index) ? T_default : FloatingPoint::toUInt(argv[index++]);
  assert(T >= 1);
  index.deactivate();

  // Header info:
  std::cout << Environment::Wrap(proginfo, Environment::OP::rf);
  std::cout << "# N = " << N << ", M = " << M << ", T = " << T << "\n";
  out_header(std::cout);

  AnalysePVal_vt Afreq, Aruns;
  Afreq.resize(T); Aruns.resize(T);
  analyse(T, M, N, Afreq, Aruns);
  const auto anal_freq = analyse_pvalues(Afreq);
  const auto anal_runs = analyse_pvalues(Aruns);
  std::cout << anal_freq << " " << anal_runs;
  const float80 minpfreq = min_pvalue(Afreq);
  const float80 minpruns = min_pvalue(Aruns);
  std::cout << " " << Wrap(minpfreq) << " " << Wrap(minpruns) << "\n";
}
