// Oliver Kullmann, 23.4.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Timing and counting of class Uniformrange

Examples (annotations on following lines):

Version information:


*/

#include <iostream>
#include <vector>

#include <cassert>

#include <Numerics/FloatingPoint.hpp>
#include <ProgramOptions/Environment.hpp>

#include "Distributions.hpp"
#include "Tests.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.2.3",
        "25.4.2019",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/TimingUniformRange.cpp",
        "GPL v3"};

  using namespace RandGen;

  constexpr gen_uint_t N_default = 1e8L;
  constexpr gen_uint_t M_default = 7;
  constexpr gen_uint_t T_default = 3;
  constexpr gen_uint_t seed_default = 1234567890;

  using FloatingPoint::Wrap;
  using FloatingPoint::float80;

  void out_header(std::ostream& out, const gen_uint_t M) {
    out << "seeds pfm20 prm20 pfm30 prm30 pfm31 prm31 pfm32 prm32";
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
  const gen_uint_t T = (argc <= index) ? T_default : FloatingPoint::toUInt(argv[index++]);
  index.deactivate();

  const auto seed_main = profiling ? seed_default : Environment::CurrentTime::timestamp();
  vec_seed_t seeds = transform({gen_uint_t(seed_main),0});
  assert(seeds.size() == 4);

  std::vector<Prob64> pm2; pm2.reserve(2);
  if (M % 2 == 0) pm2.assign(2, {1,2});
  else pm2.assign({{M/2+1, M}, {M/2, M}});
  std::vector<Prob64> pm3; pm3.reserve(3);
  if (M % 3 == 0) pm3.assign(3, {1,3});
  else if (M % 3 == 1) pm3.assign({{M/3+1, M}, {M/3, M}, {M/3, M}});
  else pm3.assign({{M/3+1, M}, {M/3+1, M}, {M/3, M}});

  // Header info:
  std::cout << Environment::Wrap(proginfo, Environment::OP::rf);
  std::cout << "# N = " << N << ", M = " << M << ", T = " << T << "\n"
            << "# Main seed: " << seed_main << "\n"
            << "# Probabilities mod 2: " << pm2[0] << ", " << pm2[1] << "\n"
            << "# Probabilities mod 3: " << pm3[0] << ", " << pm3[1] << ", " << pm3[2] << "\n";
  const auto size_broken = randgen_max - UniformRangeS(M, {}).r.last_regions;
  const float80 prop_reject = FloatingPoint::ldexp(size_broken, -64);
  std::cout << "# The probability of a discarded generator-call is " << size_broken <<" / 2^64 = " << Wrap(prop_reject) << ".\n";
  out_header(std::cout, M);
  std::cout.flush();

  for (gen_uint_t i = 0; i < T; ++i) {
    UniformRangeS U(M, seeds);
    std::cout << "\"" << SW{seeds} << "\"";

    const gen_uint_t first = U();
    CountRuns rm2(first % 2 == 0);
    std::vector<CountRuns> rm3; rm3.reserve(3);
    for (gen_uint_t k = 0; k < 3; ++k) rm3.emplace_back(CountRuns{first % 3 == k});
    std::vector<CountRuns> rM; rM.reserve(M);
    for (gen_uint_t k = 0; k < M; ++k) rM.emplace_back(CountRuns{first == k});

    for (gen_uint_t j = 1; j < N; ++j) {
      const gen_uint_t x = U();
      rm2(x % 2 == 0);
      for (gen_uint_t k = 0; k < 3; ++k) rm3[k](x % 3 == k);
      for (gen_uint_t k = 0; k < M; ++k) rM[k](x == k);
    }

    {
      const auto [cr, ct] = *rm2;
      std::cout << " " << Wrap(monobit(ct, N, pm2[0]))
                << " " << Wrap(runstest_gen(N, cr, pm2[0]));
    }
    for (gen_uint_t k = 0; k < 3; ++k) {
      const auto [cr, ct] = *rm3[k];
      std::cout << " " << Wrap(monobit(ct, N, pm3[k]))
                << " " << Wrap(runstest_gen(N, cr, pm3[k]));
    }
    for (gen_uint_t k = 0; k < M; ++k) {
      const auto [cr, ct] = *rM[k];
      std::cout << " " << Wrap(monobit(ct, N, Prob64{1,M}))
                << " " << Wrap(runstest_gen(N, cr, Prob64{1,M}));
    }

    std::cout << std::endl;
    if (seeds[2] != FloatingPoint::P232m1) ++seeds[2];
    else {seeds[2] = 0; ++seeds[3];}
  }


}
