// Oliver Kullmann, 26.4.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Timing and counting for function choose_kn

Examples (annotations on following lines):

The arguments and their default values:
Random> ./TimingChoose N=1e8 M=7 K=3 T=3

Call "choose K out of M" N times, printing p-values for
 - the frequencies of occurrences of 0, ..., M-1 in the resulting K-sets,
 - the corresponding runs;
in both cases as Bernoulli-experiments with probability K/M.
As main seed the current timestamp is chosen, plus a running-seed 0,...,T-1.

Running the profiling-version, with fixed main seed:
Random> ./TimingChoose -p
# Timestamp: 11.05.2019 21:30:29_+0100 1557606629212056904
# Producing program: https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/TimingUniformRange.cpp
# program name:       TimingChoose
#  version:           0.1.0
#  last change:       26.4.2019
#  git-id:            a172780b12ce378524a6ef0aa6294f8cf3156873
# machine name:       csltok.swansea.ac.uk
#  bogomips:          4787.85
# compiler version:   g++ 8.3.0
#  compilation date:  May_11_2019 20:23:22
#  used options:      --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static -fno-finite-math-only
# N = 100000000, M = 7, K = 3, T = 3
# Main seed: 1234567890
seeds pf0 pr0 pf1 pr1 pf2 pr2 pf3 pr3 pf4 pr4 pf5 pr5 pf6 pr6
"(1234567890,0,0,0)" 0.0088687882676492076313 0.76328560210518616358 0.91569567498133841308 0.63118173795505439432 0.99663720023423714233 0.44902717061920336327 0.94246758741441921735 0.77177941478743925972 0.44170382194347914857 0.81994931900991758534 0.11886214191701749761 0.26898080759380585766 0.073627731080569508008 0.62742068466779591615
"(1234567890,0,1,0)" 0.56397585100887408883 0.40428579365738926763 0.75890471985975176345 0.73932277065889117468 0.67236160346861019004 0.066015965159483194445 0.9753126251717901143 0.97845844984617926854 0.71231138016110937683 0.94672382391044207164 0.51293001636781487921 0.21541978033729140262 0.25205329846650812857 0.42541972688234132812
"(1234567890,0,2,0)" 0.61667893058050510279 0.44418796813412235176 0.24640838810750408284 0.65616922079271960169 0.27928561658518618721 0.14011450107248946736 0.41337946041138197553 0.92150441376563747294 0.6665791584047657248 0.61133082840009187961 0.66104658248104434864 0.87814063717529595261 0.055439312593558619672 0.084974896287724261841


*/

#include <iostream>
#include <vector>

#include <cassert>

#include <Numerics/FloatingPoint.hpp>
#include <ProgramOptions/Environment.hpp>

#include "Distributions.hpp"
#include "Algorithms.hpp"
#include "Tests.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.2",
        "11.5.2019",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/TimingChoose.cpp",
        "GPL v3"};

  using namespace RandGen;

  constexpr gen_uint_t N_default = 1e8L;
  constexpr gen_uint_t M_default = 7;
  constexpr gen_uint_t K_default = 3;
  constexpr gen_uint_t T_default = 3;
  constexpr gen_uint_t seed_default = 1234567890;

  using FloatingPoint::Wrap;
  using FloatingPoint::float80;

  void out_header(std::ostream& out, const gen_uint_t M) {
    out << "seeds";
    for (gen_uint_t i = 0; i < M; ++i) out << " pf" << i << " pr" << i;
    out << "\n";
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
  const gen_uint_t K = (argc <= index) ? K_default : FloatingPoint::toUInt(argv[index++]);
  assert(K >= 1 and  K <= M);
  const gen_uint_t T = (argc <= index) ? T_default : FloatingPoint::toUInt(argv[index++]);
  index.deactivate();

  const auto seed_main = profiling ? seed_default : Environment::CurrentTime::timestamp();
  vec_seed_t seeds = transform({gen_uint_t(seed_main),0});
  assert(seeds.size() == 4);

  // Header info:
  std::cout << Environment::Wrap(proginfo, Environment::OP::rf);
  std::cout << "# N = " << N << ", M = " << M << ", K = " << K << ", T = " << T << "\n"
            << "# Main seed: " << seed_main << "\n";
  out_header(std::cout, M);
  std::cout.flush();

  std::vector<unsigned> found(M);
  for (gen_uint_t i = 0; i < T; ++i) {
    RandGen_t g(seeds);
    std::cout << "\"" << SW{seeds} << "\"";

    std::vector<CountRuns> rM; rM.reserve(M);
    const auto first = choose_kn(K, M, g);
    for (const auto i : first) found[i] = true;
    for (gen_uint_t k = 0; k < M; ++k) rM.emplace_back(CountRuns(found[k]));

    for (gen_uint_t j = 1; j < N; ++j) {
      const auto x = choose_kn(K, M, g);
      for (auto& x : found) x = false; // std::copy XXX
      for (const auto i : x) found[i] = true;
      for (gen_uint_t k = 0; k < M; ++k) rM[k](found[k]);
    }

    for (gen_uint_t k = 0; k < M; ++k) {
      const auto [cr, ct] = *rM[k];
      std::cout << " " << Wrap(monobit(ct, N, Prob64{K,M}))
                << " " << Wrap(runstest_gen(N, cr, Prob64{K,M}));
    }

    std::cout << std::endl;
    inc(seeds[2],seeds[3]);
  }


}
