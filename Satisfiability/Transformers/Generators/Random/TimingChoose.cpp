// Oliver Kullmann, 26.4.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Timing and counting for function choose_kn

Examples (annotations on following lines):

Version information:


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
        "0.1.0",
        "26.4.2019",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/TimingUniformRange.cpp",
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
    if (seeds[2] != FloatingPoint::P232m1) ++seeds[2];
    else {seeds[2] = 0; ++seeds[3];}
  }


}
