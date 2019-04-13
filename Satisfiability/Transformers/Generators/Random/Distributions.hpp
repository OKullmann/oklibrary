// Oliver Kullmann, 15.3.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Distributions for random numbers

   Bernoulli distributions:

    - Function bernoulli for a random bool, taking a randgen_t or
      a RandGen_t by reference.

    - Class Bernoulli2 for random bool with dyadic probability (here we
      can still guarantee well-definedness, and this with exactly one
      generator-call).

    - Class Bernoulli for arbitrary probabilities p = nom/den, with
      nom, den unsigned 64-bit integers: again well-defined, and coinciding
      for nom, which are powers of 2, with Bernoulli2 (thus here exactly
      one generator-call per generation).

      Either bernoulli or Bernoulli should normally be used

    Uniform distributions:

    - Class UniformRange for generation of uniform random numbers from
      0, ..., n-1. Also well-defined, but for n which are not powers of
      two, possibly more than one generator-call is needed.

TODOS:

1. Create new class Bernoulli
    - Based on 64-bit fractions. DONE
    - Taking a RandGen_t by & or &&.

2. Make UniformRange accept RandGen_t
    - Unclear in general whether we should accept also a randgen_t.
    - The only point here might be the default-constructed randgen_t,
      which is well-behaved, and is constructed faster than via a
      seed-sequence.

The statistics below need to go to subdirectory data.

3. Testing the function bernoulli
    - Running experiments:

4 experiments on csverify:

.1 Empty initialisaton-sequence:

Random$ /usr/bin/time --output=Out2e13_e --append ./TimingBernoulli12 2e13 > Out2e13_e &
0.2.1 14.3.2019 TimingBernoulli12 8e4bd32eb285402856d475e6235515b94f51bcd7
csverify g++ 8.2.0 Mar 14 2019 21:41:23
--std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static -fno-finite-math-only
20000000000000
()
10000003316339 0.50000016581694999999
2e+13
39780.27user 0.00system 11:03:00elapsed 99%CPU (0avgtext+0avgdata 1516maxresident)k
0inputs+8outputs (0major+54minor)pagefaults 0swaps

> confprop(99.9, 0.50000016581694999999, 2e13)
[1] 0.4999998 0.5000005
> confprop(69, 0.50000016581694999999, 2e13)
[1] 0.5000001 0.5000003
> confprop(70, 0.50000016581694999999, 2e13)
[1] 0.5000000 0.5000003

So from this point of view, the probability for 1/2 being inconsistent with
the observed frequency is around 69%.

Such confidence intervals seem the right thing to do. The above computation
also uses the precise binomial distribution, not an approximation.

The more conventional "p-value tests" (now using the approximation by the
normal distribution, helped by the "Yates continuity correction" and by
the "Wilson score interval"):

> prop.test(10000003316339, 2e13, 0.5)
        1-sample proportions test with continuity correction
data:  10000003316339 out of 2e+13, null probability 0.5
X-squared = 2.1996, df = 1, p-value = 0.138
alternative hypothesis: true p is not equal to 0.5
95 percent confidence interval:
 0.4999999 0.5000004
sample estimates:
        p
0.5000002

Reproduction in Maxima of prop.test (without the "continuity correction"),
according to the "Monobit Test":
monobit(m,n) := erfc(abs(2*m-n)/sqrt(n)/sqrt(2));

float(monobit(10000003316339, 2e13));
Evaluation took 0.0010 seconds (0.0010 elapsed)
(%o11) .13804468601177527

This is quite a high deviation; so we run it longer:
Random$ /usr/bin/time --output=Out1e14_e --append ./TimingBernoulli12 1e14 > Out1e14_e &
TimingBernoulli12 0.2.3 15.3.2019 e36bfa11535ab14a2bcf597751e46a3747ff5038
csverify 7183.87
g++ 8.2.0 Mar_15_2019 10:27:52
--std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static -fno-finite-math-only
100000000000000
()
50000007708303 0.50000007708303
1e+14
190633.26user 0.00system 52:57:13elapsed 99%CPU (0avgtext+0avgdata 1516maxresident)k
0inputs+8outputs (0major+53minor)pagefaults 0swaps

> confprop(99.9, 0.50000007708303, 1e14)
[1] 0.4999999 0.5000002
> confprop(42, 0.50000007708303, 1e14)
[1] 0.5000000 0.5000001
> confprop(41, 0.50000007708303, 1e14)
[1] 0.5000001 0.5000001

> prop.test(50000007708303, 1e14, 0.5)
        1-sample proportions test with continuity correction
data:  50000007708303 out of 1e+14, null probability 0.5
X-squared = 2.3767, df = 1, p-value = 0.1232
alternative hypothesis: true p is not equal to 0.5
95 percent confidence interval:
 0.5000000 0.5000002
sample estimates:
        p
0.5000001
float(monobit(50000007708303, 1e14));
.12315609106877996

Let's run it even longer, using discard to jump over the first 1e14
generations:
Random$ file="Out5e14_e"; /usr/bin/time --output=${file} --append ./TimingBernoulli12 4e14 1e14 > ${file} &
Random$ cat Out5e14_e
TimingBernoulli12 0.3.0 17.3.2019 1e8a59b0765fac3968f91869e1f2d3a31e031167
csverify 7183.87
g++ 8.2.0 Mar_17_2019 18:19:37
--std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static -fno-finite-math-only
400000000000000 100000000000000
()
199999986038128 0.49999996509532000001
4e+14 1e+14
850779.50user 0.00system 236:19:40elapsed 99%CPU (0avgtext+0avgdata 1516maxresident)k
0inputs+8outputs (0major+53minor)pagefaults 0swaps

50000007708303 + 199999986038128;
249999993746431

prop.test(249999993746431, 5e14, 0.5)
X-squared = 0.3129, df = 1, p-value = 0.5759
95 percent confidence interval:
 0.4999999 0.5000000
sample estimates:
  p
0.5

No problem here.


.2 Initialisation-sequence {0,0}

Random$ /usr/bin/time --output=Out2e13_0 --append ./TimingBernoulli12 2e13 0 > Out2e13_0 &
0.2.1 14.3.2019 TimingBernoulli12 8e4bd32eb285402856d475e6235515b94f51bcd7
csverify g++ 8.2.0 Mar 14 2019 21:41:23
--std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static -fno-finite-math-only
20000000000000 0
(0,0)
9999999968549 0.49999999842745000001
2e+13
39897.78user 0.00system 11:04:57elapsed 99%CPU (0avgtext+0avgdata 1516maxresident)k
0inputs+8outputs (0major+54minor)pagefaults 0swaps

float(monobit(9999999968549, 2e13));
.9887778725038041

> confprop(99.9, 0.49999999842745000001, 2e13)
[1] 0.4999996 0.5000004
Within R-precision, not possible to deviate from 0.5:
> confprop(1e-10, 0.49999999842745000001, 2e13)
[1] 0.5 0.5


.3 Initialisation-sequence {1,0}

Random$ /usr/bin/time --output=Out2e13_1 --append ./TimingBernoulli12 2e13 1 > Out2e13_1 &
0.2.1 14.3.2019 TimingBernoulli12 8e4bd32eb285402856d475e6235515b94f51bcd7
csverify g++ 8.2.0 Mar 14 2019 21:41:23
--std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static -fno-finite-math-only
20000000000000 1
(1,0)
9999999846301 0.49999999231504999999
2e+13
40098.29user 0.00system 11:08:18elapsed 99%CPU (0avgtext+0avgdata 1516maxresident)k
0inputs+8outputs (0major+53minor)pagefaults 0swaps

float(monobit(9999999846301, 2e13));
Evaluation took 0.0010 seconds (0.0010 elapsed)
(%o4) .9451995372980065

> confprop(99.9, 0.49999999231504999999, 2e13)
[1] 0.4999996 0.5000004
> confprop(1e-10, 0.49999999231504999999, 2e13)
[1] 0.5 0.5


.4 Initialisation-sequence {2,0}

Random$ /usr/bin/time --output=Out2e13_2 --append ./TimingBernoulli12 2e13 2 > Out2e13_2 &
0.2.1 14.3.2019 TimingBernoulli12 8e4bd32eb285402856d475e6235515b94f51bcd7
csverify g++ 8.2.0 Mar 14 2019 21:41:23
--std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static -fno-finite-math-only
20000000000000 2
(2,0)
10000000144468 0.5000000072234
2e+13
39775.64user 0.00system 11:02:55elapsed 99%CPU (0avgtext+0avgdata 1516maxresident)k
0inputs+8outputs (0major+53minor)pagefaults 0swaps

float(monobit(10000000144468, 2e13));
.9484860721728519

> confprop(99.9, 0.5000000072234, 2e13)
[1] 0.4999996 0.5000004
> confprop(1e-10, 0.5000000072234, 2e13)
[1] 0.5 0.5


Running a fuller approach, considering the distribution of p-values:

> make p-TimingBernoulli12
> mkdir ExpB12_10000; mv TimingBernoulli12 ExpB12_10000; cd ExpB12_10000
> ./TimingBernoulli12 rh > data
> for (( seed=0; seed<10000; ++seed )); do ./TimingBernoulli12 "2,rd" 3e9 0 ${seed} >> data; done

On csverify:

ExpB12_10000> head -20 data
# Timestamp: 26.03.2019 15:26:27_+0000 1553613987803232429
# Producing program: https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/TimingBernoulli12.cpp
# program name:       TimingBernoulli12
#  version:           0.4.10
#  last change:       26.3.2019
#  git-id:            4cda9f7bf436742fb247cc46d8737582572f2e53
# machine name:       csverify
#  bogomips:          7183.87
# compiler version:   g++ 8.2.0
#  compilation date:  Mar_26_2019 15:26:07
#  used options:      --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static -fno-finite-math-only -fprofile-use
# Expected values for N=3e+09:
#  number true:             1.5e+09
#   sigma:                  27386.1
#  runs:                    1.5e+09
#   sigma:                  27386.1
#  longest run true(asymp): 30.8151
#   sigma:                  1.87271
 N discard seeds count freq pfreq runs pruns lt lf
3000000000 0 "(0,0)" 1500008065 0.50000268833333333336 0.76838185202788818196 1500022098 0.41972037952230002966 31 32

> E=read.table("ExpB12_10000", header=TRUE)
> length(E$N)
[1] 10000
> summary(E)
     pfreq                runs             pruns            lt
 Min.   :0.0000015   Min.   :1.5e+09   Min.   :-Inf   Min.   :26.00
 1st Qu.:0.2528771   1st Qu.:1.5e+09   1st Qu.:   0   1st Qu.:30.00
 Median :0.5061971   Median :1.5e+09   Median :   0   Median :31.00
 Mean   :0.5047669   Mean   :1.5e+09   Mean   :-Inf   Mean   :30.82
 3rd Qu.:0.7581706   3rd Qu.:1.5e+09   3rd Qu.:   1   3rd Qu.:32.00
 Max.   :0.9998835   Max.   :1.5e+09   Max.   :   1   Max.   :46.00
       lf
 Min.   :27.00
 1st Qu.:30.00
 Median :31.00
 Mean   :30.81
 3rd Qu.:32.00
 Max.   :46.00
> ks.test(E$pfreq, "punif", exact=TRUE)
D = 0.012, p-value = 0.1127
  ties should not be present for the Kolmogorov-Smirnov test
> length(E$pfreq) - length(unique(E$pfreq))
[1] 959
> x = E$count - 1.5e9
> plot(x)
> hist(x)
> ks.test(x, "pnorm", 0, 27386.1, exact=TRUE)
D = 0.0083, p-value = 0.4877
  ties should not be present for the Kolmogorov-Smirnov test
> min(x)
[1] -111906
> max(x)
[1] 131614

The frequencies seem "normal".

> E[E$pruns==-Inf,]
1515 3e+09       0 (1514,0) 1499888094 0.4999627 4.384404e-05 1500041198  -Inf
1669 3e+09       0 (1668,0) 1500131614 0.5000439 1.540848e-06 1499930904  -Inf
     lt lf
1515 31 28
1669 31 34
# Reminder: the -inf-cases are those where the prerequisite-test on frequency
# fails.
> y = E$pruns[-c(1515,1669)]
> summary(y)
     Min.   1st Qu.    Median      Mean   3rd Qu.      Max.
0.0000347 0.2432000 0.4912000 0.4956000 0.7452000 0.9999000
> ks.test(y, "punif", exact=TRUE)
D = 0.0104, p-value = 0.2267
> z=E$runs - 1.5e9
> hist(z)
> ks.test(z,"pnorm",0,27386.1)
D = 0.0088, p-value = 0.4187
  ties should not be present for the Kolmogorov-Smirnov test

Also the runs seem "normal".
-Inf occurs for a deviation greater 4*sigma, which is 0.006%, which from 10000
is 0.6.


4. Testing class Bernoulli2

Four experiments on csverify:

.1

Random$ /usr/bin/time --output=Out31_e --append ./TimingBernoulli2 2e13 3 1 > Out31_e &
TimingBernoulli2 0.2.0 16.3.2019 954cfb6d774b9f562424d99234bbd1df6e75bc52
csverify 7183.87
g++ 8.2.0 Mar_16_2019 21:27:17
--std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static -fno-finite-math-only
20000000000000 3 1
()
2499999915548 0.1249999957774
2e+13 0.125
49385.85user 0.00system 13:43:05elapsed 99%CPU (0avgtext+0avgdata 1516maxresident)k
0inputs+8outputs (0major+53minor)pagefaults 0swaps

Roughly 405e6 generations per sec (lower than reported in TimingBernoulli2).

> confprop(99.9, 0.1249999957774, 2e13)
[1] 0.1249998 0.1250002
> confprop(1e-7, 0.1249999957774, 2e13)
[1] 0.125 0.125


gmonobit(m,n,p):=erfc(abs(m-n*p)/(sqrt(n)*sqrt(2)*sqrt(p)*sqrt(1-p)))

float(gmonobit(2499999915548, 2e13, 1/8));
(%o22) .9544655573781293

> prop.test(2499999915548, 2e13, 1/8)
x-squared = 0.0033, df = 1, p-value = 0.9545


.2

Random$ /usr/bin/time --output=Out31_0 --append ./TimingBernoulli2 2e13 3 1 0 > Out31_0 &
TimingBernoulli2 0.2.0 16.3.2019 954cfb6d774b9f562424d99234bbd1df6e75bc52
csverify 7183.87
g++ 8.2.0 Mar_16_2019 21:27:17
--std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static -fno-finite-math-only
20000000000000 3 1 0
(0,0)
2500000106276 0.12500000531380000001
2e+13 0.125
49273.68user 0.00system 13:41:13elapsed 99%CPU (0avgtext+0avgdata 1520maxresident)k
0inputs+8outputs (0major+54minor)pagefaults 0swaps

Roughly 406e6 generations per sec.

> confprop(99.9, 0.12500000531380000001, 2e13)
[1] 0.1249998 0.1250002
> confprop(1e-7, 0.12500000531380000001, 2e13)
[1] 0.125 0.125

float(gmonobit(2500000106276, 2e13, 1/8));
.9427167524217229

> prop.test(2500000106276, 2e13, 1/8)
X-squared = 0.0052, df = 1, p-value = 0.9427


.3

Random$ /usr/bin/time --output=Out101023_e --append ./TimingBernoulli2 2e13 10 1023 > Out101023_e &
TimingBernoulli2 0.2.0 16.3.2019 954cfb6d774b9f562424d99234bbd1df6e75bc52
csverify 7183.87
g++ 8.2.0 Mar_16_2019 21:27:17
--std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static -fno-finite-math-only
20000000000000 10 1023
()
19980468567160 0.99902342835799999999
2e+13 0.9990234375
49282.38user 0.00system 13:41:22elapsed 99%CPU (0avgtext+0avgdata 1516maxresident)k
0inputs+8outputs (0major+55minor)pagefaults 0swaps

Roughly 406e6 generations per sec.

> confprop(99.9, 0.99902342835799999999, 2e13)
[1] 0.9990234 0.9990235

We need confprop with higher precision.

float(gmonobit(19980468567160, 2e13, 1023/1024));
.19055608982711367

> prop.test(19980468567160, 2e13, 1023/1024)
X-squared = 1.7133, df = 1, p-value = 0.1906


.4

Random$ /usr/bin/time --output=Out101023_0 --append ./TimingBernoulli2 2e13 10 1023 0 > Out101023_0 &
TimingBernoulli2 0.2.0 16.3.2019 954cfb6d774b9f562424d99234bbd1df6e75bc52
csverify 7183.87
g++ 8.2.0 Mar_16_2019 21:27:17
--std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static -fno-finite-math-only
20000000000000 10 1023 0
(0,0)
19980469097847 0.99902345489234999998
2e+13 0.9990234375
49421.36user 0.00system 13:43:41elapsed 99%CPU (0avgtext+0avgdata 1516maxresident)k
0inputs+8outputs (0major+53minor)pagefaults 0swaps

Roughly 405e6 generations per sec.

> confprop(99.9, 0.99902345489234999998, 2e13)
[1] 0.9990234 0.9990235

float(gmonobit(19980469097847, 2e13, 1023/1024));
.012766880769691985

> prop.test(19980469097847, 2e13, 1023/1024)
X-squared = 6.2011, df = 1, p-value = 0.01277


This is a very low p-value, close to what in the NIST-test would be rejected.
So a larger test is needed:
Random$ ./TimingBernoulli2 -v
program name:       TimingBernoulli2
 version:           0.3.0
 last change:       21.3.2019
 git-id:            efd3a73bdaecf88852ec2e5cbda2dfc5c225d460
machine name:       csverify
 bogomips:          7183.87
compiler version:   g++ 8.2.0
 date:              Mar_21_2019 21:35:46
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only
Random$ file="Out5e14_0"; /usr/bin/time --output=${file} --append ./TimingBernoulli2 5e14 10 1023 0  > ${file} &
500000000000000 10 1023 0
(0,0)
499511719465264 0.99902343893052799999 0.30578677642757254621
5e+14 0.9990234375
1199790.61user 0.00system 333:16:31elapsed 99%CPU (0avgtext+0avgdata 1516maxresident)k
0inputs+8outputs (0major+53minor)pagefaults 0swaps

Roughly 417e6 generations per sec.

bfloat(gmonobit(499511719465264, 5e14, 1023/1024));
0.305786776427572 631650154007611
                 ^
                 from this digit on is the p-value from the program not correct


Running a fuller approach, considering the distribution of p-values:

> make p-TimingBernoulli2
> mkdir ExpB2_10000; mv TimingBernoulli2_-p ExpB2_10000; cd ExpB2_10000
> ./TimingBernoulli2_-p rh 3e9 > data
> for (( seed=0; seed<10000; ++seed )); do ./TimingBernoulli2_-p "rd" 3e9 3 1 ${seed} >> data; done

On csverify:
XXX


> make p-TimingBernoulli2
> mkdir ExpB2_101023_10000; mv TimingBernoulli2_-p ExpB2_101023_10000; cd ExpB2_101023_10000
> ./TimingBernoulli2_-p rh 3e9 10 1023 > data
> for (( seed=0; seed<10000; ++seed )); do ./TimingBernoulli2_-p "rd" 3e9 10 1023 ${seed} >> data; done

On csverify:
XXX



5. Testing class Bernoulli

> make p-TimingBernoulli
> mkdir ExpB_10000; mv TimingBernoulli_-p ExpB_10000; cd ExpB_10000
> ./TimingBernoulli_-p rh 3e9 1/3 > data
> for (( seed=0; seed<10000; ++seed )); do ./TimingBernoulli_-p "rd" 3e9 1/3 ${seed} >> data; done

On csverify:
XXX

*/

#ifndef DISTRIBUTIONS_6S09j6DxLm
#define DISTRIBUTIONS_6S09j6DxLm

#include <ostream>

#include <cassert>

// Guaranteed to be included:
#include "Numbers.hpp"

namespace RandGen {

  // Returns true/false with probability 1/2, using exactly one call of g:
  inline bool bernoulli(randgen_t& g) noexcept { return lessP263(g()); }
  inline bool bernoulli(RandGen_t& g) noexcept { return lessP263(g()); }


  /* Class Bernoulli2, generalising bernoulli(g) for dyadic p

     Here the propability is given by p = x / 2^y for returning tree
     (using real-number arithmetic for the mathematical specification,
     and exact handling in the implementation), now employing a functor:
      - y is integer with 0 <= y <= 63
      - x is integer with 0 <= x <= 2^y.
      - Construct by
          Bernoulli2 b(g, x, y);
        (const or not).
      - Call as
          b()
      - This class can be simulated by UniformRange (see Test.cpp), but is
        more efficient.
      - The generator g is called exactly once for each b(), except of the
        cases with probabilities 0 or 1 (where there are no calls).
  */
  class Bernoulli2 {
    randgen_t& g;
  public :
    enum class S {c0, c1, nc }; // constant 0/1, or non-constant
    const S s;
    const gen_uint_t threshold;
    Bernoulli2(randgen_t& g, const gen_uint_t x, const gen_uint_t y) noexcept :
      g(g), s(set_S(x,y)), threshold(set_t(x,y,s)) {}
    Bernoulli2(const Bernoulli2& b) = delete;
    bool operator ()() const noexcept {
      switch (s) {
      case S::c0 : return false;
      case S::c1 : return true;
      default : return g() < threshold;
      }
    }

    static constexpr S set_S(const gen_uint_t x, const gen_uint_t y) noexcept {
      if (x == 0) return S::c0;
      assert(y < 64);
      assert(x <= iexp2(y));
      if (iexp2(y) == x) return S::c1; else return S::nc;
    }
    static constexpr gen_uint_t set_t(const gen_uint_t x, const gen_uint_t y, const S s) noexcept {
      assert(s == set_S(x,y));
      if (s != S::nc) return 0; else return ildexp(x, 64-y);
    }
  };
  static_assert(Bernoulli2::set_S(0,0) == Bernoulli2::S::c0);
  static_assert(Bernoulli2::set_S(1,0) == Bernoulli2::S::c1);
  static_assert(Bernoulli2::set_S(0,1) == Bernoulli2::S::c0);
  static_assert(Bernoulli2::set_S(1,1) == Bernoulli2::S::nc);
  static_assert(Bernoulli2::set_S(2,1) == Bernoulli2::S::c1);
  static_assert(Bernoulli2::set_t(1,1,Bernoulli2::S::nc) == iexp2(63));


  // Generalising Bernoulli2, now allowing arbitrary 64-bit fractions:
  class Bernoulli {
    RandGen_t& g;
  public :
    enum class S {c0=0, c1=1, dy=2, o=3 }; // constant 0/1, dyadic, other
    static constexpr bool constant(const S s) noexcept {
      return s==S::c0 or s==S::c1;
    }

    const S s;
    const gen_uint_t threshold;
    const gen_uint_t last_valid;
    Bernoulli(RandGen_t& g, const Prob64 p) noexcept :
      g(g), s(set_S(p)), threshold(set_t(p,s)), last_valid(set_l(p,s)) {}
    Bernoulli(const Bernoulli& b) = delete;
    bool operator ()() const noexcept {
      switch (s) {
      case S::c0 : return false;
      case S::c1 : return true;
      case S::dy : return g() < threshold;
      default :
        gen_uint_t result;
        do result = g(); while (result > last_valid);
        return result < threshold;
      }
    }

    static constexpr S set_S(const Prob64 p) noexcept {
      if (p.zero()) return S::c0;
      if (p.one()) return S::c1;
      if (p.dyadic()) return S::dy;
      return S::o;
    }
    static constexpr gen_uint_t set_t(const Prob64 p, const S s) noexcept {
      assert(s == set_S(p));
      if (constant(s)) return 0;
      if (s == S::dy) return ildexp(p.nom, 64 - ilogp2(p.den));
      return p.nom * (randgen_max / p.den);
    }
    static constexpr gen_uint_t set_l(const Prob64 p, const S s) noexcept {
      assert(s == set_S(p));
      if (s != S::o) return randgen_max;
      return p.den * (randgen_max / p.den) - 1;
    }
  };
  // (2^64-1) - last_valid = 2^64 % p.nom :
  static_assert(randgen_max - Bernoulli::set_l({1,3},Bernoulli::S::o) == 1);
  static_assert(randgen_max - Bernoulli::set_l({2,6},Bernoulli::S::o) == 1);
  static_assert(randgen_max - Bernoulli::set_l({1,5},Bernoulli::S::o) == 1);
  static_assert(randgen_max - Bernoulli::set_l({2,10},Bernoulli::S::o) == 1);
  static_assert(randgen_max - Bernoulli::set_l({1,6},Bernoulli::S::o) == 4);
  static_assert(randgen_max - Bernoulli::set_l({77,1007},Bernoulli::S::o) == 492);
  static_assert(randgen_max - Bernoulli::set_l({777212,10000000019ULL},Bernoulli::S::o) == 8660737959ULL);


  /* Replacement of std::uniform_int_distribution;
     while with the C++-standard we have the usage
       std::uniform_int_distribution<result_type> d(a,b);
       result_type r = d(g);
     for a random number in {a,...,b}, where g is the underlying random number
     generator, here now the type of g is standardised to the 64-bit Mersenne
     twister, result_type is gen_uint_t, and the usage is
       UniformRange U(g, n, start);
       gen_uint_t random = U();
     creating random numbers from {start, ..., n+start-1}.

     If using only U(g,n) (with start=0 by default), then the "range"
     (i.e. half-open interval) [0,n) (including 0, excluding n) is sampled;
     this is the usual treatment of "ranges" in C++, but deviates from
     std::uniform_int_distribution.

     Every use of U() advances the state of g at least once except of the
     trivial case n=1, where nothing happens; if n>1 is a power of 2, then
     g is used exactly once.

     Operators: only <<.
  */
  class UniformRange {
    randgen_t& g;
  public :
    const gen_uint_t n; // size of sampling interval
    const gen_uint_t s; // start of sampling interval
    const bool trivial; // whether n == 1
    const bool p2; // whether n is a power of 2
    const gen_uint_t size_region; // how often the sampling-interval fits
    const gen_uint_t last_regions; // before start of trailing broken interval
    /* The numbers 0,...,randgen_max generated by g() are partioned into n
       consecutive maximum- and equal-sized intervals ("regions") for outcomes
         s, s+1, ..., s+n-1,
       with a trailing region of lesser size beginning after last_regions.

       E.g. let randgen_max := 15 (so we have 4 bits). Now for n=3, s=0:
       size_region = 5, last_regions = 14.
       So 0,...,4 -> 0; 5,...,9 -> 1; 10,...,14 -> 2; 15 -> disposed.
       And for n=2: size_region = 8, last_regions = 15, and so
       0,...,7 -> 0; 8,...,15 -> 1; nothing disposed.

       We have size_region = total_size / n (integer arithmetic), where
       total_size = randgen_max + 1.
       If n is not a divisor of total_size, i.e, n is not a power of 2,
       then size_region = randgen_max / n.
       While last_regions = randgen_max - 2^64 % n.
    */
    UniformRange(randgen_t& g, const gen_uint_t n, const gen_uint_t start=0)
      noexcept : g(g), n(n), s(start),
      trivial(n == 1), p2(trivial or powerof2(n)),
      size_region(randgen_max/n + p2),
      last_regions((p2) ? randgen_max : n*size_region - 1) {
      assert(n != 0);
      assert(s <= randgen_max - (n-1));
      assert(not trivial or size_region == 0);
      assert(size_region >= 1 or trivial);
      assert(not p2 or last_regions == randgen_max);
      assert(last_regions != randgen_max or p2);
    }
    UniformRange(const UniformRange& u) = delete;
    gen_uint_t operator ()() const noexcept {
      if (trivial) return s;
      assert(n >= 2);
      gen_uint_t result;
      do result = g(); while (result > last_regions);
      return result/size_region + s;
    }

    friend std::ostream& operator <<(std::ostream& out, const UniformRange& u) {
      return out << u.n << "," << u.s << "," << u.trivial << ","
        << u.p2 << "," << u.size_region << "," << u.last_regions;
    }

  };

}

#endif
