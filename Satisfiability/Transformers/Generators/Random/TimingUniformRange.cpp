// Oliver Kullmann, 23.4.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Timing and counting of class Uniformrange

Examples (annotations on following lines):

Version information:

Random> ./TimbingUniformRange -v

The profiling-run runs the default-arguments, but using a fixed main seed
(instead of the timestamp):

Random> ./TimingUniformRange -p
# Timestamp: 25.04.2019 03:04:47_+0100 1556157887625989622
# Producing program: https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/TimingUniformRange.cpp
# program name:       TimingUniformRange
#  version:           0.2.3
#  last change:       25.4.2019
#  git-id:            5230553a98c6cf7b3c009582229878da7748d256
# machine name:       csltok.swansea.ac.uk
#  bogomips:          4788.08
# compiler version:   g++ 8.3.0
#  compilation date:  Apr_25_2019 02:58:49
#  used options:      --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static -fno-finite-math-only
# N = 100000000, M = 7, T = 3
# Main seed: 1234567890
# Probabilities mod 2: 4/7, 3/7
# Probabilities mod 3: 3/7, 2/7, 2/7
# The probability of a discarded generator-call is 2 / 2^64 = 1.084202172485504434e-19.
seeds pfm20 prm20 pfm30 prm30 pfm31 prm31 pfm32 prm32 pf0 pr0 pf1 pr1 pf2 pr2 pf3 pr3 pf4 pr4 pf5 pr5 pf6 pr6
"(1234567890,0,0,0)" 0.83667927835325826315 0.38780152595844642966 0.56346892146338303304 0.11250400515479311845 0.628215851493134344 0.32601178339983980328 0.2639624368239430389 0.44767867784364177142 0.63773101722660418351 0.87349388776319429725 0.56845873768738540519 0.22540226510372126498 0.30456004151886826068 0.42193912852443012139 0.48754035312939814618 0.7143253793737358147 0.23190209600720741994 0.75218981424612551601 0.6778246161761530962 0.43260717023466190603 0.55275704287687728978 0.99775345954850360331
"(1234567890,0,1,0)" 0.20953614329180586934 0.018379877470790121513 0.39549946651536215723 0.9847171921556440743 0.15333122840373420605 0.094070695199639818571 0.61913374802932789746 0.55928544625334713645 0.68012691507544923369 0.79038356766081083947 0.70989594496470197324 0.94219047792843089783 0.33656441082984538468 0.48127204840519742884 0.58650395952866243783 0.54997852653495780679 0.14119171347661771573 0.069663745147265504487 0.10900065201511947331 0.071498733934416548434 0.28461086035922347561 0.77286668782569983193
"(1234567890,0,2,0)" 0.91340560272860458975 0.7341458755876736329 0.25662399982228930055 0.68091628019798058576 0.92389195001450955788 0.66578934454865067963 0.1808271232610736136 0.25037719244296580785 0.79497945241377351096 0.54316629115313947633 0.38863363866391374148 0.51221328251994833293 0.74659791055955160354 0.93309474827183825024 0.034617257965466018004 0.037539777744676399558 0.32441303543246408181 0.23621305280067916784 0.16017102530158546463 0.25515571431801464114 0.44227089796227992235 0.62684532706005118089

Default version:
Random> ./TimingUniformRange

is equivalent to

Random> ./TimingUniformRange 1e8 7 3

with N=1e8 the number of trials, m=7 the size of the uniform range, and
T=3 the number of experiments.

Output are only p-values, in pairs: for frequency and runs, using the two p-values
for Bernoulli-trials, casting the generated values in various ways.
First for the occurrence of even (x mod 2 = 0) results, then the three cases
for x mod 3 = 0, 1, 2, and then for each of the outcomes x =0, ..., M-1.

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
