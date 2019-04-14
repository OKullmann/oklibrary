// Oliver Kullmann, 13.4.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Timing of class Bernoulli2

   Floating-point values are computed in float80-type.
   When for their output "=" is used, then this means the output is at
   full (decimal) precision, while when "~" is used, then this means the
   standard output-precision is used.

*/

#include <iostream>

#include <cassert>

#include <Numerics/FloatingPoint.hpp>
#include <ProgramOptions/Environment.hpp>

#include "Distributions.hpp"
#include "Tests.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.3.1",
        "14.4.2019",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/TimingBernoulli.cpp",
        "GPL v3"};

  using namespace RandGen;

  constexpr gen_uint_t N_default = 1e9L;
  constexpr Prob64 p_default{1,8};

  using OP = Environment::OP;

  constexpr char sep = ',';


  // Outputting the parameters:

  using FloatingPoint::Wrap;
  using FloatingPoint::float80;
  using Environment::DWW;
  using Environment::DHW;

  void output_parameters(std::ostream& out, const OP op, const gen_uint_t N, const Prob64 p, const vec_seed_t& seeds) {
    assert(op != OP::rh);
    using RandGen::SW;
    if (op == OP::rd or op == OP::rf) {
      out << N << " " << float80(p) << " \"" << SW{seeds} << "\" ";
      out.flush();
    }
    else if (op == OP::dimacs) {
      out << DHW{"Parameters"}
          << DWW{"N"} << N << "\n"
          << DWW{"nominator"} << p.nom << "\n"
          << DWW{"denominator"} << p.den << "\n"
          << DWW{"probability"} << Wrap(p) << "\n"
          << DWW{"seeds"} << SW{seeds} << "\n"
          << DHW{"Results"};
      out.flush();
    }
    else if (op == OP::explained) {
      const auto size_broken = randgen_max - Bernoulli::set_l(p, Bernoulli::set_S(p));
      out << "\n** The parameters, obtained from the command-line, and possibly using default values:\n\n"
             "0. The output-type is \"e\" as \"explained\".\n"
             "1. The number N of runs is, as precise integer and in floating-point (with restricted precision):\n"
             "   N = " << N << ", approx = " << float80(N) << ".\n"
             "2. The nominator is " << p.nom << ".\n"
             "3. The denominator is " << p.den << ".\n"
             "4. The sequence of 32-bit seeds used is " << SW{seeds} << ".\n"
             "\n** Derived values:\n"
             " - The resulting probability nominator/denominator is p = " << Wrap(p) << ".\n"

             " - Thus the expected value for the number of true's is approx. " << mean_Binomial(N,p)
          << ",\n    with standard deviation approx. " << sigma_Binomial(N,p) << ".\n"
             " - The probability of a discarded generator-call is " << size_broken <<" / 2^64 = " << Wrap(FloatingPoint::ldexp(size_broken, -64)) << ".\n"
             "\n** The results of the computation are:\n" << std::endl;
    }
    else
      out << op << " " << N << " " << p << " " << SW{seeds} << std::endl;
  }

  void reminder_parameters(std::ostream& out, const gen_uint_t N, const float80 p, const OP op) {
    assert(op != OP::rh);
    if (op == OP::rd or op == OP::rf or op == OP::dimacs or op == OP::explained) return;
    out << float80(N) << " " << Wrap(p) << "\n";
  }


  // The computations and their output:

  Count_true frequency(const gen_uint_t N, const Prob64 p, const vec_seed_t& seeds) noexcept {
    Count_true ct;
    RandGen_t g(seeds);
    Bernoulli b(g,p);
    for (gen_uint_t i = 0; i < N; ++i) ct(b());
    return ct;
  }
  void out(std::ostream& out, const gen_uint_t N, Count_true&& c, const float80 p, const OP op) {
    const auto ct = *c;
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
    out << std::endl;
  }


  void out_header(std::ostream& out) {
    out << " N p seeds count freq pfreq\n";
  }


}

int main(int argc0, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc0, argv)) return 0;

  const int argc = (Environment::profiling(argc0, argv)) ? 1 : argc0;

  Environment::Index index;

  const OP op = (argc <= index) ? OP{} : std::get<OP>(Environment::translate<OP>()(argv[index++], sep));

  const gen_uint_t N = (argc <= index) ? N_default : FloatingPoint::toUInt(argv[index++]);

  const Prob64 p = (argc <= index) ? p_default : toProb64(argv[index++]).value();

  // Header info:
  if (op != OP::simple and op != OP::rd) {
    std::cout << Environment::Wrap(proginfo, op);
    if (op == OP::rh or op == OP::rf) {
      const auto size_broken = randgen_max - Bernoulli::set_l(p, Bernoulli::set_S(p));
      std::cout << "# Expected value for N ~ " << float80(N) << " and p = " << p << " ~ " << float(p) << ":\n"
                   "#  number true ~            " << mean_Binomial(N,p) << "\n"
                   "#  standard deviation ~     " << sigma_Binomial(N,p) << "\n"
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

  output_parameters(std::cout, op, N, p, seeds);

  out(std::cout, N, frequency(N, p, seeds), p, op);

  reminder_parameters(std::cout, N, p, op);

}
