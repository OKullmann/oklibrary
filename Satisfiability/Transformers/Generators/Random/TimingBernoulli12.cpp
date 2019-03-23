// Oliver Kullmann, 5.3.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Timing of function bernoulli(g)

Example (annotation on next line):

Random> ./TimingBernoulli12 -v
program name:       TimingBernoulli12
 version:           0.4.2
 last change:       23.3.2019
 git-id:            7c8043676e280f5987d26ffb9aadd02958aaa971
machine name:       csltok.swansea.ac.uk
 bogomips:          4788.21
compiler version:   g++ 8.3.0
 date:              Mar_23_2019 16:29:45
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only

Random> ./TimingBernoulli12 0 3e9 0
# computational level, number N of calls of the generator, and
# number of discards (default values),
# plus sequence of 64-bit seed values (empty by default)
"0,s,min" 3000000000 0
# the arguments (numbers in integer format)
()
# the list of 32-bit seeds (initialising the generator)
1499919941 0.49997331366666666666 0.0034629664979143074932
# the count of results "true", their relative frequency, and the p-value
3e+09 0
# N and discards again, in float80-precision.

Remark: The p-value here is very low: the probability that with 3e9 fair
coin-flips we obtain a deviation of the measured frequency f to the true
probability of at least the given value, i.e.,
  |f-0.5| >= 0.5 - 0.49997331366666666666
holds, is 0.35%, which is a low probability, and one would reject that the
given sequence is "random", due to having too few "true"-results.
This 0/1-sequence is further evaluated in Distributions.hpp.

With one seed-value (a 64-bit 0, yielding two 32-bit 0s):
Random> ./TimingBernoulli12 0 3e9 0 0
"0,s,min" 3000000000 0 0
(0,0)
1500008065 0.50000268833333333336 0.76838185202788818196
3e+09 0

At computational level 2, and with 3 64-bit seed-values:
Random> time ./TimingBernoulli12 2 3e9 0 1 2 3
"2,s,min" 3000000000 0 1 2 3
(1,0,2,0,3,0)
1499999818 0.49999993933333333334 0.99469753731439383899
1500023951 0.38180962301409408641
# number of runs, and p-value
31 30.8151
# longest run, and expected longest run
3e+09 0


Results:

On csltok:

Random> ./RunTime ./TimingBernoulli12
program name:       TimingBernoulli12
 version:           0.3.2
 last change:       19.3.2019
 git-id:            0e33bd127e3ef9e78912f8c8a900413d59382c25
machine name:       csltok.swansea.ac.uk
 bogomips:          4788.21
compiler version:   g++ 8.3.0
 date:              Mar_19_2019 20:37:29
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only
Output program:
3000000000 0
()
1499919941 0.49997331366666666666
3e+09 0
MIN + MAX user times:
15.97 16.25

Roughly 3e9 / 15.97 ~ 188e6 generations per sec, which is
188 / 225 ~ 84% of pure generation speed.
And this is 188 / 207 ~ 91% of the speed of the previous version,
where just trivial changes took place -- the compilation is "all over
the place".

Newer version:
Random> ./RunTime ./TimingBernoulli12
program name:       TimingBernoulli12
 version:           0.4.0
 last change:       21.3.2019
 git-id:            e130527c6a50f5016461088eeffdee7e29961895
machine name:       csltok.swansea.ac.uk
 bogomips:          4788.21
compiler version:   g++ 8.3.0
 date:              Mar_21_2019 19:27:32
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only
Output program:
3000000000 0
()
1499919941 0.49997331366666666666 0.0034629664979143074932
3e+09 0
MIN + MAX user times:
16.08 16.37

Newest version:
Random> ./RunTime ./TimingBernoulli12
program name:       TimingBernoulli12
 version:           0.4.2
 last change:       23.3.2019
 git-id:            7c8043676e280f5987d26ffb9aadd02958aaa971
machine name:       csltok.swansea.ac.uk
 bogomips:          4788.21
compiler version:   g++ 8.3.0
 date:              Mar_23_2019 16:29:45
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only
Output program:
"0,s,min" 3000000000 0
()
1499919941 0.49997331366666666666 0.0034629664979143074932
3e+09 0
MIN + MAX user times:
16.31 16.59

One sees that just adding further options (not touching the above computation)
make the program slower.

For comparison, in "full mode":
Random> ./RunTime ./TimingBernoulli12 2
program name:       TimingBernoulli12
 version:           0.4.2
 last change:       23.3.2019
 git-id:            7c8043676e280f5987d26ffb9aadd02958aaa971
machine name:       csltok.swansea.ac.uk
 bogomips:          4788.21
compiler version:   g++ 8.3.0
 date:              Mar_23_2019 16:29:45
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only
Output program:
"2,s,min" 3000000000 0
()
1499919941 0.49997331366666666666 0.0034629664979143074932
1499992345 0.7799646876543496644
30 30.8151
3e+09 0
MIN + MAX user times:
34.82 35.38


On cs-wsok:

Random> ./RunTime ./TimingBernoulli12
program name:       TimingBernoulli12
 version:           0.3.2
 last change:       19.3.2019
 git-id:            0e33bd127e3ef9e78912f8c8a900413d59382c25
machine name:       cs-wsok
 bogomips:          5986.74
compiler version:   g++ 8.3.0
 date:              Mar_19_2019 20:41:28
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only
Output program:
3000000000 0
()
1499919941 0.49997331366666666666
3e+09 0
MIN + MAX user times:
11.90 12.00

Roughly 3e9 / 11.9 ~ 252e6 generations per sec, which is
252 / 317 ~ 79.5% of pure generation-speed.
And this is 252 / 291 ~ 87% of the speed of the previous version,

Newer version:
Random> ./RunTime ./TimingBernoulli12
program name:       TimingBernoulli12
 version:           0.4.0
 last change:       21.3.2019
 git-id:            e130527c6a50f5016461088eeffdee7e29961895
machine name:       cs-wsok
 bogomips:          5986.74
compiler version:   g++ 8.3.0
 date:              Mar_21_2019 19:42:15
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only
Output program:
3000000000 0
()
1499919941 0.49997331366666666666 0.0034629664979143074932
3e+09 0
MIN + MAX user times:
11.87 12.02

Basically the same.

Newest version:
Random> ./RunTime ./TimingBernoulli12
program name:       TimingBernoulli12
 version:           0.4.2
 last change:       23.3.2019
 git-id:            7c8043676e280f5987d26ffb9aadd02958aaa971
machine name:       cs-wsok
 bogomips:          5986.74
compiler version:   g++ 8.3.0
 date:              Mar_23_2019 16:41:40
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only
Output program:
"0,s,min" 3000000000 0
()
1499919941 0.49997331366666666666 0.0034629664979143074932
3e+09 0
MIN + MAX user times:
12.08 12.26

For comparison, in "full mode":
Random> ./RunTime ./TimingBernoulli12 2
program name:       TimingBernoulli12
 version:           0.4.2
 last change:       23.3.2019
 git-id:            7c8043676e280f5987d26ffb9aadd02958aaa971
machine name:       cs-wsok
 bogomips:          5986.74
compiler version:   g++ 8.3.0
 date:              Mar_23_2019 16:41:40
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only
Output program:
"2,s,min" 3000000000 0
()
1499919941 0.49997331366666666666 0.0034629664979143074932
1499992345 0.7799646876543496644
30 30.8151
3e+09 0
MIN + MAX user times:
27.07 27.32


On csverify:

Random$ ./RunTime ./TimingBernoulli12
program name:       TimingBernoulli12
 version:           0.3.2
 last change:       19.3.2019
 git-id:            0e33bd127e3ef9e78912f8c8a900413d59382c25
machine name:       csverify
 bogomips:          7183.87
compiler version:   g++ 8.2.0
 date:              Mar_19_2019 20:42:57
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only
Output program:
3000000000 0
()
1499919941 0.49997331366666666666
3e+09 0
MIN + MAX user times:
6.10 6.39

Roughly 3e9 / 6.1 ~ 492e6 generations per sec, which is
492 / 568 ~ 86.6% of pure generation-speed.
And this is 492 / 522 ~ 94% of the speed of the previous version,

Newer version:
Random$ ./RunTime ./TimingBernoulli12
program name:       TimingBernoulli12
 version:           0.4.0
 last change:       21.3.2019
 git-id:            e130527c6a50f5016461088eeffdee7e29961895
machine name:       csverify
 bogomips:          7183.87
compiler version:   g++ 8.2.0
 date:              Mar_21_2019 19:45:42
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only
Output program:
3000000000 0
()
1499919941 0.49997331366666666666 0.0034629664979143074932
3e+09 0
MIN + MAX user times:
6.13 6.42

Basically the same.

Newest version:
Random$ ./RunTime ./TimingBernoulli12
program name:       TimingBernoulli12
 version:           0.4.2
 last change:       23.3.2019
 git-id:            7c8043676e280f5987d26ffb9aadd02958aaa971
machine name:       csverify
 bogomips:          7183.87
compiler version:   g++ 8.2.0
 date:              Mar_23_2019 16:42:22
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only
Output program:
"0,s,min" 3000000000 0
()
1499919941 0.49997331366666666666 0.0034629664979143074932
3e+09 0
MIN + MAX user times:
6.24 6.50

For comparison, in "full mode":
Random$ ./RunTime ./TimingBernoulli12 2
program name:       TimingBernoulli12
 version:           0.4.2
 last change:       23.3.2019
 git-id:            7c8043676e280f5987d26ffb9aadd02958aaa971
machine name:       csverify
 bogomips:          7183.87
compiler version:   g++ 8.2.0
 date:              Mar_23_2019 16:42:22
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only
Output program:
"2,s,min" 3000000000 0
()
1499919941 0.49997331366666666666 0.0034629664979143074932
1499992345 0.7799646876543496644
30 30.8151
3e+09 0
MIN + MAX user times:
19.12 19.34

Surprising, how that little addition of counting runs and the maximum length
increases the runtime by 300%.

It is also surprising that adding just one small test to the generation
slows it down by ~ 10%: the generation should be a much more involved
computation, and so the effect of adding this test should be smaller?
Apparently the computation of g() is very fast, so that such a small addition
matters. Or it is the compilation.

*/

#include <optional>
#include <tuple>
#include <vector>
#include <sstream>
#include <iostream>

#include <cassert>

#include <Numerics/FloatingPoint.hpp>
#include <ProgramOptions/Environment.hpp>

#include "Distributions.hpp"
#include "Tests.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.4.3",
        "23.3.2019",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/TimingBernoulli12.cpp",
        "GPL v3"};

  using namespace RandGen;

  constexpr gen_uint_t N_default = 3e9L;
  constexpr gen_uint_t discard_default = 0;

  // Computation-level:
  enum class CL {basic=0, runs=1, max=2}; // 0, 1, 2
  std::optional<CL> rCL(const std::string& s) noexcept {
    if (s == "0") return CL::basic;
    else if (s == "1") return CL::runs;
    else if (s == "2") return CL::max;
    else return {};
  }
  std::ostream& operator <<(std::ostream& out, const CL l) {
    return out << int(l);
  }
  // Output-type:
  enum class OT {simple=0, dimacs=1, rh=2, rd=3, rf=4}; // s, d, rh, rd, rf
  std::optional<OT> rOT(const std::string& s) noexcept {
    if (s == "s") return OT::simple;
    else if (s == "d") return OT::dimacs;
    else if (s == "rh") return OT::rh;
    else if (s == "rd") return OT::rd;
    else if (s == "rf") return OT::rf;
    else return {};
  }
  std::ostream& operator <<(std::ostream& out, const OT o) {
    switch (o) {
    case OT::dimacs : return out << "d";
    case OT::rh : return out << "rh";
    case OT::rd : return out << "rd";
    case OT::rf : return out << "rf";
    default : return out << "s";}
  }
  // Output-level:
  enum class OL {min=0, mid=1, max=2}; // min, mid, max
  std::optional<OL> rOL(const std::string& s) noexcept {
    if (s == "min") return OL::min;
    else if (s == "mid") return OL::mid;
    else if (s == "max") return OL::max;
    else return {};
  }
  std::ostream& operator <<(std::ostream& out, const OL o) {
    switch (o) {
    case OL::mid : return out << "mid";
    case OL::max : return out << "max";
    default : return out << "min";}
  }

  typedef std::tuple<CL,OT,OL> output_t;
  const char sep = ',';

  std::ostream& operator <<(std::ostream& out, const output_t o) {
    return out << "\"" << std::get<CL>(o) << sep << std::get<OT>(o) << sep << std::get<OL>(o) << "\"";
  }

  typedef std::vector<std::string> tokens_t;
  tokens_t split(const std::string& s) {
    std::stringstream ss(s);
    tokens_t res;
    std::string item;
    while (std::getline(ss, item, sep)) res.push_back(item);
    return res;
  }
  output_t translate(const std::string& arg) noexcept {
    output_t res;
    for (const std::string& item : split(arg)) {
      if (item.empty()) continue;
      {const auto cl = rCL(item);
       if (cl) { std::get<CL>(res) = *cl; continue; }}
      {const auto ot = rOT(item);
       if (ot) { std::get<OT>(res) = *ot; continue; }}
      {const auto ol = rOL(item);
       if (ol) { std::get<OL>(res) = *ol; continue; }}
    }
    return res;
  }

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  int index = 1;
  const output_t choices = (argc <= index) ? output_t{} : translate(argv[index++]);
  if (std::get<OT>(choices) == OT::rh) {
    std::cout << "TO BE IMPLEMENTED\n";
    return 0;
  }
  const gen_uint_t N = (argc <= index) ? N_default : FloatingPoint::toUInt(argv[index++]);
  assert(N >= 1);
  const gen_uint_t discard = (argc <= index) ? discard_default : FloatingPoint::toUInt(argv[index++]);
  vec_eseed_t seeds64;
  if (argc >= 4) {
    seeds64.reserve(argc-3);
    for (int i = index; i < argc; ++i)
      seeds64.push_back(FloatingPoint::toUInt(argv[i]));
  }


  const vec_seed_t seeds = transform(seeds64);
  RandGen_t g(seeds);
  g.discard(discard);

  using FloatingPoint::float80;
  using FloatingPoint::Wrap;
  std::cout << choices << " " << N << " " << discard;
  for (const auto x : seeds64) std::cout << " " << x;
  std::cout << "\n";
  out_seeds(std::cout, seeds);
  std::cout << std::endl;

  switch (std::get<CL>(choices)) {
  case CL::basic : {
    Count_true count;
    for (gen_uint_t i = 0; i < N; ++i) count(bernoulli(g));
    const auto ct = *count;
    std::cout << ct << " " << Wrap(float80(ct) / N) << " " << Wrap(monobit(ct, N)) << "\n";
    break;
  }
  case CL::runs : {
    CountRuns count(bernoulli(g));
    for (gen_uint_t i = 1; i < N; ++i) count(bernoulli(g));
    const auto [cr, ct] = *count;

    std::cout << ct << " " << Wrap(float80(ct) / N) << " " << Wrap(monobit(ct, N)) << "\n";
    std::cout << cr << " " << Wrap(runstest(ct, N, cr)) << "\n";
    break;
  }
  default : {
    LongestRun count(bernoulli(g));;
    for (gen_uint_t i = 1; i < N; ++i) count(bernoulli(g));
    const auto [lr, cr, ct] = *count;

    std::cout << ct << " " << Wrap(float80(ct) / N) << " " << Wrap(monobit(ct, N)) << "\n";
    std::cout << cr << " " << Wrap(runstest(ct, N, cr)) << "\n";
    std::cout << lr << " " << expectedlongestrun(N) << "\n";
  }}

  std::cout << float80(N) << " " << float80(discard) << "\n";

}
