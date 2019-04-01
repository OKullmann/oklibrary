// Oliver Kullmann, 5.3.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Timing and counting of function bernoulli(g)

Examples (annotations on following lines):

Version information:

Random> ./TimingBernoulli12 -v
author:             "Oliver Kullmann"
 url:               "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/TimingBernoulli12.cpp"
 license:           "GPL v3"
program name:       TimingBernoulli12
 version:           0.5.2
 last change:       31.3.2019
 git-id:            ba0485bb341d2185d778be13952b87a1a04e3960
machine name:       csltok.swansea.ac.uk
 bogomips:          4788.21
compiler version:   g++ 8.3.0
 date:              Mar_31_2019 16:16:35
 options:           "--std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static -fno-finite-math-only -fno-fast-math"

Profiling run (calls all three main functions, with seeds {1},{2},{3} resp.),
with outputs of just the main results, in "simple" format (just numbers):
Random> ./TimingBernoulli12 -p
Level 0:
1500065160 0.50002171999999999998 0.017345233310718022048
Level 1:
1500059458 0.50001981933333333334 0.029923675397408930482
1500005778 0.83283359437941609921
Level 2:
1499977528 0.49999250933333333333 0.41189613015665212586
1500006180 0.8214549916662288995
30 30

Random> .TimingBernoulli12

is equivalent to

Random> ./TimingBernoulli12 0,s 3e9 0
# computational level and output-type, number N of calls of the generator, and
# number of discards (default values),
# plus sequence of 64-bit seed values (empty by default)
"0,s" 3000000000 0 ()
# the arguments (numbers in integer format),
# concluded by the list of 32-bit seeds (initialising the generator)
1499919941 0.49997331366666666666 0.0034629664979143074965
# the count of results "true", their relative frequency, and the p-value
3e+09 0
# N and discards again, in float80-precision.

Concerning the first argument:
An arbitrary (possibly empty) list of strings is possible, which will
be evaluated from left to right, and if recognised (otherwise they are
ignored) will set the computational level resp. the output-type:
 - levels are "0", "1", "2"
 - output-types are
  - "s" (simple)
  - "e" (explained)
  - "d" (Dimacs)
  - "rh" (R, header-only)
  - "rd" (R, data-only)
  " "rf" (R, both).

With one seed-value (a 64-bit 0, yielding two 32-bit 0s), explained:
Random> ./TimingBernoulli12 0 3e9 0 0
"0,s,min" 3000000000 0 0 (0,0)
1500008065 0.50000268833333333336 0.76838185202788818196
3e+09 0

At computational level 2, and with 3 64-bit seed-values:
Random> time ./TimingBernoulli12 2 3e9 0 1 2 3
"2,s,min" 3000000000 0 1 2 3 (1,0,2,0,3,0)
1499999818 0.49999993933333333334 0.99469753731439383899
1500023951 0.38180962301409408641
# number of runs, and p-value
31 30
# longest run of "true" resp. "false"
3e+09 0

Run with "e" instead of "s" to get explanations.
Run with "rh", to see the header-information for R-data-files.
Run with "d" to get all results in a system form (like "rd"),
but still readable.

*/

#include <tuple>
#include <iostream>
#include <string>
#include <array>
#include <chrono>

#include <cassert>

#include <Numerics/FloatingPoint.hpp>
#include <ProgramOptions/Environment.hpp>

#include "Distributions.hpp"
#include "Tests.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.5.5",
        "1.4.2019",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/TimingBernoulli12.cpp",
        "GPL v3"};

  using namespace RandGen;

  constexpr gen_uint_t N_default = 3e9L;
  constexpr gen_uint_t discard_default = 0;


  // Policy classes OP, CL

  using Environment::read;
  template <typename P>
  using RegistrationPolicies = Environment::RegistrationPolicies<P>;

  using OP = Environment::OP;

  // Computation-level:
  enum class CL {basic=0, runs=1, max=2}; // 0, 1, 2
  std::ostream& operator <<(std::ostream& out, const CL l) {
    return out << int(l);
  }

}
namespace Environment {
  template <>
  struct RegistrationPolicies<CL> {
   static constexpr int size = int(CL::max) + 1;
   static constexpr std::array<const char*, size> string
      {"0", "1", "2"};
  };
}
namespace RandGen {

  // The output specification:
  typedef std::tuple<CL,OP> output_t;
  constexpr char sep = ',';

  std::ostream& operator <<(std::ostream& out, const output_t o) {
    return out << "\"" << std::get<CL>(o) << sep << std::get<OP>(o) << "\"";
  }

  output_t translate(const std::string& arg) noexcept {
    output_t res;
    for (const std::string& item : Environment::split(arg,sep)) {
      if (item.empty()) continue;
      {const auto cl = read<CL>(item);
       if (cl) { std::get<CL>(res) = *cl; continue; }}
      {const auto ot = read<OP>(item);
       if (ot) { std::get<OP>(res) = *ot; continue; }}
    }
    return res;
  }


  // Outputting the parameters:

  using FloatingPoint::Wrap;
  using Environment::DWW;
  using Environment::DHW;
  using Environment::qu;

  void output_parameters(std::ostream& out, const output_t choices, const gen_uint_t N, const gen_uint_t discard, const vec_seed_t& seeds, const OP p) {
    assert(p != OP::rh);
    using RandGen::SW;
    if (p == OP::rd or p == OP::rf) {
      out << N << " " << discard << " \"" << SW{seeds} << "\" ";
      out.flush();
    }
    else if (p == OP::dimacs) {
      out << DWW{"N"} << N << "\n"
          << DWW{"discard"} << discard << "\n"
          << DWW{"seeds"} << SW{seeds} << "\n"
          << DWW{"choices"} << choices << "\n"
          << DHW{"Results"};
      out.flush();
    }
    else if (p == OP::explained) {
      out << "1. The choices for computation-level, output-style and verbosity are:\n"
             "   " << choices << "\n"
             "2. The number N of runs is, as precise integer and in floating-point (with restricted precision):\n"
             "   N = " << N << ", approx = " << float80(N) << "\n"
             "3. The number of initial discard-steps for the generator is:\n   "
          << discard << ", approx = " << float80(discard) << "\n"
             "4. The sequence of 32-bit seeds used is:\n   "
          << SW{seeds} << "\n"
          << "\n** The results of the computation are:\n" << std::endl;
    }
    else {
      out << choices << " " << N << " " << discard << " "
          << SW{seeds} << std::endl;
    }
  }

  void reminder_parameters(std::ostream& out, const gen_uint_t N, const gen_uint_t discard, const OP p) {
    assert(p != OP::rh);
    if (p == OP::rd or p == OP::rf or p == OP::dimacs or p == OP::explained) return;
    out << float80(N) << " " << float80(discard) << "\n";
  }


  // The computations and their output:

  Count_true frequency(const gen_uint_t N, RandGen_t& g) noexcept {
    Count_true count;
    for (gen_uint_t i = 0; i < N; ++i) count(bernoulli(g));
    return count;
  }
  void out_freq(std::ostream& out, const gen_uint_t N, const gen_uint_t ct, const OP p) {
    const auto freq = Wrap(float80(ct) / N);
    const auto pval = Wrap(monobit(ct, N));
    if (p == OP::dimacs) {
      out << DWW{"count_true"} << ct << "\n"
          << DWW{"  freq_true"} << freq << "\n"
          << DWW{"  pval_true"} << pval;
    }
    else if (p == OP::explained) {
      const float80 mu = mean_Binomial(N);
      const float80 sigma = sigma_Binomial(N);
      const float80 dev = (ct - mu) / sigma;
      out << "1. Count of true's and relative frequency:\n  "
          << ct << " " << freq << "\n"
             "The value (observed - expected) / standard-deviation and the corresponding p-value are:\n  "
          << Wrap(dev) << " " << pval;
    }
    else out << ct << " " << freq << " " << pval;
  }
  void out(std::ostream& out, Count_true&& c, const gen_uint_t N, const OP p) {
    const auto ct = *c;
    out_freq(out, N, ct, p);
    if (p == OP::rd or p == OP::rf) out << " NA NA NA NA";
    out << std::endl;
  }

  CountRuns runs(const gen_uint_t N, RandGen_t& g) noexcept {
    CountRuns count(bernoulli(g));
    for (gen_uint_t i = 1; i < N; ++i) count(bernoulli(g));
    return count;
  }
  void out_runs(std::ostream& out, const gen_uint_t N, const gen_uint_t ct, const gen_uint_t cr, const OP p) {
    const auto pval = Wrap(runstest(ct, N, cr));
    if (p == OP::dimacs) {
      out << DWW{"count_runs"} << cr << "\n";
      out << DWW{"  pval_runs"} << pval;
    }
    else if (p == OP::explained) {
      const float80 mu = mean_numruns(N);
      const float80 sigma = sigma_numruns(N);
      const float80 dev = (cr - mu) / sigma;
      out << "2. Count of runs of true's and false's (i.e., maximal constant intervals):\n  "
          << cr << " " "\n"
             "The value (X - mu) / sigma and the corresponding p-value are:\n  "
          << Wrap(dev) << " " << pval;
    }
    else out << cr << " " << pval;
  }
  void out(std::ostream& out, CountRuns&& c, const gen_uint_t N, const OP p) {
    const auto [cr, ct] = *c;
    out_freq(out, N, ct, p);
    if (p == OP::rd or p == OP::rf) out << " ";
    else out << "\n";
    out_runs(out, N, ct, cr, p);
    if (p == OP::rd or p == OP::rf) out << " NA NA";
    out << std::endl;
  }

  LongestRun longest(const gen_uint_t N, RandGen_t& g) noexcept {
    LongestRun count(bernoulli(g));
    for (gen_uint_t i = 1; i < N; ++i) count(bernoulli(g));
    return count;
  }
  void out_longest(std::ostream& out, const gen_uint_t N, const gen_uint_t lt, const gen_uint_t lf, const OP p) {
    if (p == OP::dimacs) {
      out << DWW{"longest_true_false"} << lt << " " << lf;
    }
    else if (p == OP::explained) {
      out << "3. Longest runs of true's resp. false's, and expected value:\n  "
          << lt << " " << lf << " " << meanasym_longestrunheads(N);
    }
    else out << lt << " " << lf;
  }
  void out(std::ostream& out, LongestRun&& c, const gen_uint_t N, const OP p) {
    const auto [lt, lf, cr, ct] = *c;
    out_freq(out, N, ct, p);
    if (p == OP::rd or p == OP::rf) out << " ";
    else out << "\n";
    out_runs(out, N, ct, cr, p);
    if (p == OP::rd or p == OP::rf) out << " ";
    else out << "\n";
    out_longest(out, N, lt, lf, p);
    out << std::endl;
  }


  void out_header(std::ostream& out) {
    out << " N discard seeds count freq pfreq runs pruns lt lf\n";
  }

}


int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;

  if (Environment::profiling(argc, argv)) {
    {const vec_seed_t seeds = transform({1});
     RandGen_t g(seeds);
     g.discard(1);
     std::cout << "Level 0:" << std::endl;
     out(std::cout, frequency(N_default,g), N_default, OP::simple);
    }
    {const vec_seed_t seeds = transform({2});
     RandGen_t g(seeds);
     g.discard(2);
     std::cout << "Level 1:" << std::endl;
     out(std::cout, runs(N_default,g), N_default, OP::simple);
    }
    {const vec_seed_t seeds = transform({3});
     RandGen_t g(seeds);
     g.discard(3);
     std::cout << "Level 2:" << std::endl;
     out(std::cout, longest(N_default,g), N_default, OP::simple);
    }
    return 0;
  }

  int index = 1;
  const output_t choices = (argc <= index) ? output_t{} : translate(argv[index++]);

  const OP cOP = std::get<OP>(choices);
  const gen_uint_t N = (argc <= index) ? N_default : FloatingPoint::toUInt(argv[index++]);
  assert(N >= 1);

  // Header info (in case of R, dimacs, or explained):
  if (cOP == OP::rh or cOP == OP::rf) {
    std::cout << Environment::Wrap(proginfo, OP::rh)
              << "# Expected values for N=" << float80(N) << ":\n"
              << "#  number true:             " << mean_Binomial(N) << "\n"
              << "#   sigma:                  " << sigma_Binomial(N) << "\n"
              << "#  runs:                    " << mean_numruns(N) << "\n"
              << "#   sigma:                  " << sigma_numruns(N) << "\n"
              << "#  longest run true(asymp): " << meanasym_longestrunheads(N) << "\n"
              << "#   sigma:                  " << sigmaasym_longestrunheads() << "\n";
    out_header(std::cout);
    if (cOP == OP::rh) return 0;
  }
  else if (cOP == OP::dimacs) {
    std::cout << Environment::Wrap(proginfo, OP::dimacs)
              << DHW{"Parameters"};
  }
  else if (cOP == OP::explained) {
    std::cout << "** Information on the program:\n\n"
              << Environment::Wrap(proginfo, OP::explained)
              << "\n** Current date, time, and ticks since the Unix epoch (1.1.1970):\n  "
              << Environment::CurrentTime{}
              << "\n  The number of ticks per nanosecond is "
              << Environment::CurrentTime::ns_per_tick()
              << ".\n\n** The parameters, obtained from the command-line, and possibly using default values:\n\n";
  }

  const gen_uint_t discard = (argc <= index) ? discard_default : FloatingPoint::toUInt(argv[index++]);
  vec_eseed_t seeds64;
  assert(index <= argc);
  seeds64.reserve(argc-index);
  for (int i = index; i < argc; ++i)
    seeds64.push_back(FloatingPoint::toUInt(argv[i]));
  // Reading of command-line parameters completed.


  const vec_seed_t seeds = transform(seeds64);

  output_parameters(std::cout, choices, N, discard, seeds, cOP);

  RandGen_t g(seeds);
  g.discard(discard);

  switch (std::get<CL>(choices)) {
  case CL::basic : out(std::cout , frequency(N,g), N, cOP); break;
  case CL::runs : out(std::cout , runs(N, g), N, cOP); break;
  default : out(std::cout, longest(N, g), N, cOP);
  }

  reminder_parameters(std::cout, N, discard, cOP);
}
