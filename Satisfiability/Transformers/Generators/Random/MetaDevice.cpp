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
