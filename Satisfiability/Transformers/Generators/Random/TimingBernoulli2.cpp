// Oliver Kullmann, 7.3.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Timing of class Bernoulli2

Example (annotations on following lines):

Version information:



Random> ./TimingBernoulli2 -v
** Information on the program:

author:             "Oliver Kullmann"
 url:               "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/TimingBernoulli2.cpp"
 license:           "GPL v3"
program name:       TimingBernoulli2
 version:           0.4.0
 last change:       13.4.2019
 git-id:            394a7e8103777304c2d903158dd5e2b8860c5c28
machine name:       csltok.swansea.ac.uk
 bogomips:          4787.68
compiler version:   g++ 8.3.0
 date:              Apr_13_2019 16:20:57
 options:           "--std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static -fno-finite-math-only"

Profiling run (same as default-run, i.e., no arguments):
Random> ./TimingBernoulli2 -p
s 1000000000 3 1 ()
124997746 0.124997746 0.82935909643158182823
1e+09 0.125

Random> ./TimingBernoulli2

is equivalent to

Random> ./TimingBernoulli2 s 1e9 3 1
# output-type, number N of calls of the generator, exponent e of denominator, nominator x (default values)
# plus sequence of 64-bit seed values (empty by default)
s 1000000000 3 1 ()
# the arguments (in integer-format)
# concluded by the list of 32-bit seeds (initialising the generator)
124997746 0.124997746 0.82935909643158182823
# the count of results "true", their relative frequency, and the p-value
1e+09 0.125
# N again, in float80-precision, and p = x/2^e.

Concerning the first argument, the output-types are
 - "s" (simple)
 - "e" (explained)
 - "d" (Dimacs)
 - "rh" (R, header-only)
 - "rd" (R, data-only)
 " "rf" (R, both).

Run with "e" instead of "s" to get explanations.
Run with "rh", to see the header-information for R-data-files.
Run with "d" to get all results in a system form ("Dimacs", like "rd"),
but still readable.


*/

#include <iostream>

#include <cassert>

#include <Numerics/FloatingPoint.hpp>
#include <ProgramOptions/Environment.hpp>

#include "Distributions.hpp"
#include "Tests.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.4.1",
        "13.4.2019",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/TimingBernoulli2.cpp",
        "GPL v3"};

  using namespace RandGen;

  constexpr gen_uint_t N_default = 1e9L;
  constexpr gen_uint_t e_default = 3;

  using OP = Environment::OP;

  constexpr char sep = ',';


  // Outputting the parameters:

  using FloatingPoint::Wrap;
  using FloatingPoint::float80;
  using Environment::DWW;
  using Environment::DHW;

  void output_parameters(std::ostream& out, const OP op, const gen_uint_t N, const gen_uint_t e, const gen_uint_t x, const float80 p, const vec_seed_t& seeds) {
    assert(op != OP::rh);
    using RandGen::SW;
    if (op == OP::rd or op == OP::rf) {
      out << N << " " << x << " " << e << " " << p << " \"" << SW{seeds} << "\" ";
      out.flush();
    }
    else if (op == OP::dimacs) {
      out << DHW{"Parameters"}
          << DWW{"N"} << N << "\n"
          << DWW{"nominator"} << x << "\n"
          << DWW{"log2_denominator"} << e << "\n"
          << DWW{"probability"} << Wrap(p) << "\n"
          << DWW{"seeds"} << SW{seeds} << "\n"
          << DHW{"Results"};
      out.flush();
    }
    else if (op == OP::explained) {
      out << "\n** The parameters, obtained from the command-line, and possibly using default values:\n\n"
             "0. The output-type is \"e\" as \"explained\".\n"
             "1. The number N of runs is, as precise integer and in floating-point (with restricted precision):\n"
             "   N = " << N << ", approx = " << float80(N) << "\n"
             "2. The binary logarithm and the denominator itself are:\n"
             "   " << e << " " << FloatingPoint::exp2(e) << "\n"
             "3. The nominator is:\n"
             "   " << x << "\n"
             "4. The resulting probability nominator/denominator is:\n"
             "   p = " << Wrap(p) << "\n"
             "5. The sequence of 32-bit seeds used is:\n   "
          << SW{seeds} << "\n"
          << "\n** The results of the computation are:\n" << std::endl;
    }
    else
      out << op << " " << N << " " << e << " " << x << " " << SW{seeds} << std::endl;
  }

  void reminder_parameters(std::ostream& out, const gen_uint_t N, const float80 p, const OP op) {
    assert(op != OP::rh);
    if (op == OP::rd or op == OP::rf or op == OP::dimacs or op == OP::explained) return;
    out << float80(N) << " " << Wrap(p) << "\n";
  }


  // The computations and their output:

  Count_true frequency(const gen_uint_t N, const gen_uint_t x, const gen_uint_t e, const vec_seed_t& seeds) noexcept {
    Count_true ct;
    randgen_t g{init(seeds)};
    Bernoulli2 b(g,x,e);
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
      out << "1. Count of true's and relative frequency:\n  "
          << ct << " " << freq << "\n"
             "The value (observed - expected) / standard-deviation and the corresponding p-value are:\n  "
          << Wrap(dev) << " " << pval;
    }
    else out << ct << " " << freq << " " << pval;
    out << std::endl;
  }


  void out_header(std::ostream& out) {
    out << " N x e p seeds count freq pfreq\n";
  }


}

int main(int argc0, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc0, argv)) return 0;

  const int argc = (Environment::profiling(argc0, argv)) ? 1 : argc0;

  Environment::Index index;

  const OP op = (argc <= index) ? OP{} : std::get<OP>(Environment::translate<OP>()(argv[index++], sep));

  const gen_uint_t N = (argc <= index) ? N_default : FloatingPoint::toUInt(argv[index++]);

  const gen_uint_t e = (argc <= index) ? e_default : FloatingPoint::toUInt(argv[index++]);
  const gen_uint_t size = FloatingPoint::exp2(e);

  const gen_uint_t x = (argc <= index) ? 1 : FloatingPoint::toUInt(argv[index++]);
  assert(x <= size);
  const float80 p = float80(x) / size;

  // Header info:
  if (op != OP::simple and op != OP::rd) {
    std::cout << Environment::Wrap(proginfo, op);
    if (op == OP::rh or op == OP::rf) {
      std::cout << "# Expected values for N=" << float80(N) << " and p=" << p << ":\n"
                << "#  number true:             " << mean_Binomial(N,p) << "\n"
                << "#   sigma:                  " << sigma_Binomial(N,p) << "\n";
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

  output_parameters(std::cout, op, N, e, x, p, seeds);

  out(std::cout, N, frequency(N, x, e, seeds), p, op);

  reminder_parameters(std::cout, N, p, op);

}
