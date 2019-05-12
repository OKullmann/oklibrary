// Oliver Kullmann, 10.5.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Statistics for class Bernoulli

Examples (annotations on following lines):

Version information:

Profiling version:
Random> ./MetaBernoulli -p
# Timestamp: 12.05.2019 21:12:31_+0100 1557691951574391165
# Producing program: https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/MetaBernoulli.cpp
# program name:       MetaBernoulli
#  version:           0.1.2
#  last change:       12.5.2019
#  git-id:            fa22c9f31fc8c4a5e2e9ce410482a965615409ea
# machine name:       csltok.swansea.ac.uk
#  bogomips:          4787.85
# compiler version:   g++ 8.3.0
#  compilation date:  May_12_2019 21:11:04
#  used options:      --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static -fno-finite-math-only
# N = 10000, M = 1000, T = 100, p = 1/3
# Main seed: 1234567890
 seeds ksfreq lfreq cfreq pfreq ksruns lruns cruns pruns
"(1234567890,0,0,0,0,0)" 0.37474162606823872937 3 2 0.26424108696981269995 0.49426768036452586755 2 7 0.87112337383011164468

There are T such data-lines, each applying analyse_pvalues:
  - first the Kolmogorov-Smirnov p-value is given of the M p-values, based each
    on N runs and their frequency, followed by the extreme-value analysis;
  - then the analogous 4 numbers are given for the runs-analysis

...
"(1234567890,0,99,0,0,0)" 0.9277623167318689389 3 2 0.26424108696981269995 0.93842293504038883344 3 1 0.63230457522903596158
# ksfreq: 0.00040022288742198755627 2 2 0.26423802107704372151
# ksruns: 1.0958404648381899101e-06 2 1 0.63396765872677049458
# pfreq: 0.00099950066612560780699
# pruns: 0.080209342840201069134

The summarising four numbers:
 - the KS-p-values for the T-many values ksfreq
 - similar for the T-many values ksruns
 - finally the minimum of pfreq and pruns.

The primary p-values for the one experiment with N calls of Bernoulli(p)
use the normal approximation (assuming that N is at least 10^6), the
other p-values all use exact computations.

TODOS:

1. Add parallelisation.

2. Add the option to use the precise Bernoulli-distribution instead
   of the approximation (for the analysis of frequencies).

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
        "0.1.2",
        "12.5.2019",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/MetaBernoulli.cpp",
        "GPL v3"};

  using namespace RandGen;

  constexpr gen_uint_t N_default = 1e4L;
  constexpr gen_uint_t M_default = 1e3L;
  constexpr gen_uint_t T_default = 1e2L;
  constexpr Prob64 p_default{1,3};
  constexpr gen_uint_t seed_default = 1234567890;


  // Outputting the parameters:

  using FloatingPoint::Wrap;
  using FloatingPoint::float80;
  using Environment::DWW;
  using Environment::DHW;

  void out_header(std::ostream& out) {
    out << " seeds ksfreq lfreq cfreq pfreq ksruns lruns cruns pruns\n";
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
  const Prob64 p = (argc <= index) ? p_default : toProb64(argv[index++]).value();

  const gen_uint_t seed_main = profiling ? seed_default : Environment::CurrentTime::timestamp();
  vec_seed_t seeds = transform({gen_uint_t(seed_main),0,0});
  assert(seeds.size() == 6);

  // Header info:
  std::cout << Environment::Wrap(proginfo, Environment::OP::rf);
  std::cout << "# N = " << N << ", M = " << M << ", T = " << T << ", p = " << p << "\n"
            << "# Main seed: " << seed_main << "\n";
  out_header(std::cout);

  index.deactivate();
  // Reading of command-line parameters completed.

  std::vector<AnalysePVal> Afreq, Aruns;
  Afreq.reserve(T); Aruns.reserve(T);
  for (gen_uint_t i = 0; i < T; ++i) {
    fvec_t Pfreq, Pruns;
    Pfreq.reserve(M); Pruns.reserve(M);
    std::cout << "\"" << SW{seeds} << "\"";

    for (gen_uint_t j = 0; j < M; ++j) {
      BernoulliS b(p, seeds);
      CountRuns count(b());
      for (gen_uint_t k = 1; k < N; ++k) count(b());
      const auto [cr, ct] = *count;
      Pfreq.push_back(monobit(ct, N, p));
      Pruns.push_back(runstest_gen(N, cr, p));
      inc(seeds[4],seeds[5]);
    }
    assert(Pfreq.size() == M and Pruns.size() == M);

    const auto afreq = analyse_pvalues(Pfreq);
    Afreq.push_back(afreq);
    const auto aruns = analyse_pvalues(Pruns);
    Aruns.push_back(aruns);

    std::cout << " " << afreq << " " << aruns << "\n";
    std::cout.flush();

    seeds[4] = 0; seeds[5] = 0;
    inc(seeds[2],seeds[3]);
  }

  const auto anal_freq = analyse_pvalues(Afreq);
  const auto anal_runs = analyse_pvalues(Aruns);
  std::cout << "# ksfreq: " << anal_freq << "\n"
            << "# ksruns: " << anal_runs << "\n";
  const float80 minpfreq = min_pvalue(Afreq);
  const float80 minpruns = min_pvalue(Aruns);
  std::cout << "# pfreq: " << Wrap(minpfreq) << "\n"
               "# pruns: " << Wrap(minpruns) << "\n";
}
