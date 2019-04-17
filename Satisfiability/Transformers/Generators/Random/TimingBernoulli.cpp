// Oliver Kullmann, 13.4.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Timing and counting of class Bernoulli2

Examples (annotations on following lines):

Version information:

Random> ./TimingBernoulli -v

Profiling-run (for version 0.4.1):
Random> ./TimingBernoulli -p
level 0, p = 1/8:
125000509 0.125000509 0.96118251599523360463
level 0, p = 1/3:
333337111 0.33333711100000000001 0.79994863174657725299
level 1, p = 1/8:
125004168 0.125004168 0.69023436991634035142
218762718 0.70610407807180843704
level 1, p = 1/3:
333328877 0.33332887700000000001 0.76498588121572307856
444416713 0.15029692986467186576

Default version:
Random> ./TimingBernoulli

is equivalent to
Random> ./TimingBernoulli 0,s 1e9 1/8
# computational level and output-type, number N of calls of the generator, and
# number of discards (default values),
# plus sequence of 64-bit seed values (empty by default)
"0,s" 1000000000 1/8 ()
# the arguments (numbers in integer format),
# concluded by the list of 32-bit seeds (initialising the generator)
124997746 0.124997746 0.82935909643158182823
# the count of results "true", their relative frequency, and the p-value
1e+09 0.125
# N and the probability, in float80-precision.

Run-level 1, still simple outout (the default), and the current timestamp
as seed:
Random> ./TimingBernoulli 1 1e9 1/3 $(date "+%s%N")
"1,s" 1000000000 1/3 (120744458,362165035)
333319059 0.333319059 0.33828902096377324641
444460479 0.13771832967296769587
1e+09 0.33333333333333333334

In order to reproduce this call, one has to reconstruct the 64-bit seed:
120744458 + 2^32 * 362165035;
  1555486981200439818
For illustration now in R-data format:
Random> ./TimingBernoulli 1,rd 1e9 1/3 1555486981200439818
1000000000 0.33333333333333333334 "(120744458,362165035)" 333319059 0.333319059 0.33828902096377324641 444460479 0.13771832967296769587

With "rf" (including the header) and "e" (explained) one gets more
information.

   Floating-point values are computed in float80-type.
   When for their output "=" is used, then this means the output is at
   full (decimal) precision, while when "~" is used, then this means the
   standard output-precision is used.

*/

#include <tuple>
#include <iostream>
#include <array>

#include <cassert>

#include <Numerics/FloatingPoint.hpp>
#include <ProgramOptions/Environment.hpp>

#include "Distributions.hpp"
#include "Tests.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.4.2",
        "17.4.2019",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/TimingBernoulli.cpp",
        "GPL v3"};

  using namespace RandGen;

  constexpr gen_uint_t N_default = 1e9L;
  constexpr Prob64 p_default{1,8};


  // Policy classes OP, CL

  using OP = Environment::OP;

  // Computation-level:
  enum class CL {basic=0, runs=1}; // 0, 1
  std::ostream& operator <<(std::ostream& out, const CL l) {
    return out << int(l);
  }

}
namespace Environment {
  template <>
  struct RegistrationPolicies<CL> {
   static constexpr int size = int(CL::runs) + 1;
   static constexpr std::array<const char*, size> string
      {"0", "1"};
  };
}
namespace RandGen {

  // The output specification:
  typedef std::tuple<CL,OP> output_t;
  constexpr char sep = ',';

  std::ostream& operator <<(std::ostream& out, const output_t o) {
    return out << "\"" << std::get<CL>(o) << sep << std::get<OP>(o) << "\"";
  }


  // Outputting the parameters:

  using FloatingPoint::Wrap;
  using FloatingPoint::float80;
  using Environment::DWW;
  using Environment::DHW;

  void output_parameters(std::ostream& out, const output_t choices, const gen_uint_t N, const Prob64 p, const vec_seed_t& seeds) {
    const OP op = std::get<OP>(choices);
    assert(op != OP::rh);
    using RandGen::SW;
    if (op == OP::rd or op == OP::rf) {
      out << N << " " << Wrap(p) << " \"" << SW{seeds} << "\" ";
      out.flush();
    }
    else if (op == OP::dimacs) {
      out << DHW{"Parameters"}
          << DWW{"N"} << N << "\n"
          << DWW{"nominator"} << p.nom << "\n"
          << DWW{"denominator"} << p.den << "\n"
          << DWW{"probability"} << Wrap(p) << "\n"
          << DWW{"seeds"} << SW{seeds} << "\n"
          << DWW{"choices"} << choices << "\n"
          << DHW{"Results"};
      out.flush();
    }
    else if (op == OP::explained) {
      const auto size_broken = randgen_max - Bernoulli::set_l(p, Bernoulli::set_S(p));
      out << "\n** The parameters, obtained from the command-line, and possibly using default values:\n\n"
             "0. The choices for computation-level and output-style are " << choices << ".\n"
             "1. The number N of runs is, as precise integer and in floating-point (with restricted precision):\n"
             "   N = " << N << ", approx = " << float80(N) << ".\n"
             "2. The nominator is " << p.nom << ".\n"
             "3. The denominator is " << p.den << ".\n"
             "4. The sequence of 32-bit seeds used is " << SW{seeds} << ".\n"
             "\n** Derived values:\n"
             " - The resulting probability nominator/denominator is p = " << Wrap(p) << ".\n"

             " - Thus the expected value for the number of true's is " << Wrap(mean_Binomial(N,p))
          << ",\n    with standard deviation " << Wrap(sigma_Binomial(N,p)) << ".\n";
      if (std::get<CL>(choices) != CL::basic) {
        out <<
             " - The expected value for the number of runs is " << Wrap(mean_numruns(N,p))
          << ",\n    with standard deviation " << Wrap(sigma_numruns(N,p)) << ".\n";
      }
      out << " - The probability of a discarded generator-call is " << size_broken <<" / 2^64 = " << Wrap(FloatingPoint::ldexp(size_broken, -64)) << ".\n"
             "\n** The results of the computation are:\n" << std::endl;
    }
    else
      out << choices << " " << N << " " << p << " " << SW{seeds} << std::endl;
  }

  void reminder_parameters(std::ostream& out, const gen_uint_t N, const float80 p, const OP op) {
    assert(op != OP::rh);
    if (op == OP::rd or op == OP::rf or op == OP::dimacs or op == OP::explained) return;
    out << float80(N) << " " << Wrap(p) << "\n";
  }


  // The computations and their output:

  Count_true frequency(const gen_uint_t N, const Prob64 p, const vec_seed_t& seeds) noexcept {
    Count_true ct;
    BernoulliS b(p, seeds);
    for (gen_uint_t i = 0; i < N; ++i) ct(b());
    return ct;
  }
  void out_freq(std::ostream& out, const gen_uint_t N, const Prob64 p, const gen_uint_t ct, const OP op) {
    const auto freq = Wrap(float80(ct) / N);
    const auto pval = Wrap(monobit(ct, N, p));
    if (op == OP::dimacs) {
      out << DWW{"count_true"} << ct << "\n"
          << DWW{"  freq_true"} << freq << "\n"
          << DWW{"  pval_true"} << pval;
    }
    else if (op == OP::explained) {
      const float80 mu = mean_Binomial(N,p);
      const float80 sigma = sigma_Binomial(N,p);
      const float80 dev = (ct - mu) / sigma;
      out << "Count of true's and derived relative frequency:\n  "
          << ct << " " << freq << "\n"
             "Thus the value (observed - expected) / standard-deviation and the corresponding p-value are:\n  "
          << Wrap(dev) << " " << pval;
    }
    else out << ct << " " << freq << " " << pval;
  }
  void out(std::ostream& out, const gen_uint_t N, Count_true&& c, const Prob64 p, const OP op) {
    const auto ct = *c;
    out_freq(out, N, p, ct, op);
    if (op == OP::rd or op == OP::rf) out << " NA NA";
    out << std::endl;
  }

  CountRuns runs(const gen_uint_t N, const Prob64 p, const vec_seed_t& seeds) noexcept {
    assert(N >= 1);
    BernoulliS b(p, seeds);
    CountRuns ct(b());
    for (gen_uint_t i = 1; i < N; ++i) ct(b());
    return ct;
  }
  void out_runs(std::ostream& out, const gen_uint_t N, const Prob64 p, const gen_uint_t ct, const gen_uint_t cr, const OP op) {
    const auto pval = Wrap(runstest(ct, N, cr, p));
    if (op == OP::dimacs) {
      out << DWW{"count_runs"} << cr << "\n";
      out << DWW{"  pval_runs"} << pval;
    }
    else if (op == OP::explained) {
      const Prob64 p{ct,N};
      const float80 mu = mean_numruns(N,p);
      const float80 sigma = sigma_numruns(N,p);
      const float80 dev = (cr - mu) / sigma;
      out << "2. Count of runs of true's and false's (i.e., maximal constant intervals):\n  "
          << cr << " " "\n"
             "The value (X - mu) / sigma and the corresponding p-value are:\n  "
          << Wrap(dev) << " " << pval;
    }
    else out << cr << " " << pval;
  }
  void out(std::ostream& out, const gen_uint_t N, CountRuns&& c, const Prob64 p, const OP op) {
    const auto [cr, ct] = *c;
    out_freq(out, N, p, ct, op);
    if (op == OP::rd or op == OP::rf) out << " ";
    else out << "\n";
    out_runs(out, N, p, ct, cr, op);
    out << std::endl;
  }


  void out_header(std::ostream& out) {
    out << " N p seeds count freq pfreq runs pruns\n";
  }


}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;

  if (Environment::profiling(argc, argv)) {
    const OP op{};
    const gen_uint_t N = N_default;
    {const Prob64 p = p_default;
     const vec_seed_t seeds = transform({1});
     std::cout << "level 0, p = 1/8:" << std::endl;
     out(std::cout, N, frequency(N, p, seeds), p, op);
    }
    {const Prob64 p{1,3};
     const vec_seed_t seeds = transform({2});
     std::cout << "level 0, p = 1/3:" << std::endl;
     out(std::cout, N, frequency(N, p, seeds), p, op);
    }
    {const Prob64 p = p_default;
     const vec_seed_t seeds = transform({3});
     std::cout << "level 1, p = 1/8:" << std::endl;
     out(std::cout, N, runs(N, p, seeds), p, op);
    }
    {const Prob64 p{1,3};
     const vec_seed_t seeds = transform({4});
     std::cout << "level 1, p = 1/3:" << std::endl;
     out(std::cout, N, runs(N, p, seeds), p, op);
    }
    return 0;
  }

  Environment::Index index;

  const output_t choices = (argc <= index) ? output_t{} : Environment::translate<output_t>()(argv[index++], sep);
  const OP op = std::get<OP>(choices);

  const gen_uint_t N = (argc <= index) ? N_default : FloatingPoint::toUInt(argv[index++]);

  const Prob64 p = (argc <= index) ? p_default : toProb64(argv[index++]).value();

  // Header info:
  if (op != OP::simple and op != OP::rd) {
    std::cout << Environment::Wrap(proginfo, op);
    if (op == OP::rh or op == OP::rf) {
      const auto size_broken = randgen_max - Bernoulli::set_l(p, Bernoulli::set_S(p));
      std::cout << "# Expected values for N ~ " << float80(N) << " and p = " << p << " ~ " << float(p) << ":\n"
                   "# number true             " << Wrap(mean_Binomial(N,p)) << "\n"
                   "#   standard deviation    " << Wrap(sigma_Binomial(N,p)) << "\n"
                   "# runs                    " << Wrap(mean_numruns(N,p)) << "\n"
                   "#   standard deviation    " << Wrap(sigma_numruns(N,p)) << "\n"
                   "# The probability of a discarded generator-call is " << size_broken <<" / 2^64 = " << Wrap(FloatingPoint::ldexp(size_broken, -64)) << ".\n";
      out_header(std::cout);
      if (op == OP::rh) return 0;
    }
  }

  vec_eseed_t seeds64;
  assert(index <= argc);
  seeds64.reserve(argc-index);
  for (int i = index; i < argc; ++i)
    seeds64.push_back(FloatingPoint::toUInt(argv[i]));
  index.deactivate();
  // Reading of command-line parameters completed.

  const vec_seed_t seeds = transform(seeds64);

  output_parameters(std::cout, choices, N, p, seeds);

  switch (std::get<CL>(choices)) {
  case CL::basic : out(std::cout, N, frequency(N, p, seeds), p, op); break;
  default : out(std::cout, N, runs(N, p, seeds), p, op);
  }

  reminder_parameters(std::cout, N, p, op);

}
