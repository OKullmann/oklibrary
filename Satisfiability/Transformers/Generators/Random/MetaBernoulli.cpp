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
# Timestamp: 14.05.2019 22:25:33_+0100 1557869133692532855
# Producing program: https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/MetaBernoulli.cpp
# program name:       MetaBernoulli
#  version:           0.1.3
#  last change:       14.5.2019
#  git-id:            49b0e48cb8177980bec9216ba0b22d6662db7f9f
# machine name:       csltok.swansea.ac.uk
#  bogomips:          4787.71
# compiler version:   g++ 8.3.0
#  compilation date:  May_14_2019 22:24:49
#  used options:      --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static -fno-signed-zeros -fno-math-errno -fno-trapping-math -fno-unsafe-math-optimizations -fno-associative-math -fno-reciprocal-math -fno-finite-math-only
# N = 10000, M = 1000, T = 100, p = 1/3
# Main seed: 1234567890
 ksfreq lksfreq cksfreq pksfreq ksruns lksruns cksruns pksruns minpfreq minpruns
0.00040022288742198462892 2 2 0.26423802107704372135 1.0958404648359672956e-06 2 1 0.63396765872677049458 0.00099950066612560780699 0.080209342840201069025

First two blocks of four numbers, analysing the Kolmogorov-Smirnov p-values
of the T times M experiments, regarding frequency and runs:
 - the KS p-value
 - followed by the extreme-value analysis;
 - finally the minimum of pfreq and pruns (from the extreme values of the T
   experiments).

The primary p-values for the one experiment with N calls of Bernoulli(p)
use the normal approximation, the other p-values all use exact computations.

TODOS:

1. Add the option to use the precise Bernoulli-distribution instead
   of the approximation (for the analysis of frequencies).
    - One gets always very low p-values ksfreq and ksruns for low N-values
      -- does this come from the approximated calculation of the primary
      p-values for frequencies and runs?

2. Also make the precise analysis of runs available (without the
   normal approximation).

*/

#include <iostream>
#include <thread>
#include <functional>

#include <cassert>

#include <Numerics/FloatingPoint.hpp>
#include <ProgramOptions/Environment.hpp>

#include "Distributions.hpp"
#include "Tests.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.2.0",
        "19.5.2019",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/MetaBernoulli.cpp",
        "GPL v3"};

  using namespace RandGen;
  typedef unsigned int NumThreads_t;

  constexpr gen_uint_t N_default = 1e4L;
  constexpr gen_uint_t M_default = 1e3L;
  constexpr gen_uint_t T_default = 1e2L;
  constexpr Prob64 p_default{1,3};
  constexpr gen_uint_t seed_default = 1234567890;


  // Outputting the parameters:

  using FloatingPoint::Wrap;
  using FloatingPoint::float80;

  void out_header(std::ostream& out) {
    out << " ksfreq lksfreq cksfreq pksfreq ksruns lksruns cksruns pksruns minpfreq minpruns\n";
    out.flush();
  }

  void analyse(const gen_uint_t T, const gen_uint_t M, const gen_uint_t N, const Prob64 p, vec_seed_t seeds, AnalysePVal_vt& Afreq, AnalysePVal_vt& Aruns, const gen_uint_t mod, const gen_uint_t rem) {
    for (gen_uint_t i = rem; i < T; i += mod) {
      split(i,seeds[2],seeds[3]);
      fvec_t Pfreq, Pruns;
      Pfreq.reserve(M); Pruns.reserve(M);

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

      Afreq[i] = analyse_pvalues(Pfreq);
      Aruns[i] = analyse_pvalues(Pruns);

      seeds[4] = 0; seeds[5] = 0;
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
  const Prob64 p = (argc <= index) ? p_default : toProb64(argv[index++]).value();
  const NumThreads_t num_threads = (argc <= index) ? std::thread::hardware_concurrency() / 2 : std::stoul(argv[index++]);
  assert(num_threads >= 1);
  index.deactivate();

  const gen_uint_t seed_main = profiling ? seed_default : Environment::CurrentTime::timestamp();
  vec_seed_t seeds = transform({gen_uint_t(seed_main),0,0});
  assert(seeds.size() == 6);

  // Header info:
  std::cout << Environment::Wrap(proginfo, Environment::OP::rf);
  std::cout << "# N = " << N << ", M = " << M << ", T = " << T << ", p = " << p << "\n"
            << "# Main seed: " << seed_main << "\n"
            << "# Number threads: " << num_threads << "\n";
  out_header(std::cout);

  AnalysePVal_vt Afreq, Aruns;
  Afreq.resize(T); Aruns.resize(T);

  std::vector<std::thread> pool; pool.reserve(num_threads-1);
  for (NumThreads_t i = 1; i < num_threads; ++i) {
    pool.emplace_back(std::thread(analyse, T, M, N, p, seeds, std::ref(Afreq), std::ref(Aruns), num_threads, i));
  }
  assert(pool.size() == num_threads - 1);
  analyse(T, M, N, p, seeds, Afreq, Aruns, num_threads, 0);
  for (auto& t : pool) t.join();

  const auto anal_freq = analyse_pvalues(Afreq);
  const auto anal_runs = analyse_pvalues(Aruns);
  std::cout << anal_freq << " " << anal_runs;
  const float80 minpfreq = min_pvalue(Afreq);
  const float80 minpruns = min_pvalue(Aruns);
  std::cout << " " << Wrap(minpfreq) << " " << Wrap(minpruns) << "\n";
}
