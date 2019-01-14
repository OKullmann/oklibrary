// Oliver Kullmann, 2.1.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Analysis of the ltau-computation

USAGE:

> ExploreBts

gives information on input and output.

> ExploreBTs "H"
a b ltau N rp eam1 eb sum ldiff ltau2 N2 rp2 ub

prints the header line for the output:
 - a, b the inputs, sorted in ascending order
 - ltau = ln(tau(a,b))
 - N is the number of iterations
 - rp is the return-place
 - eam1 = exp(-a*ltau)-1
 - eb = exp(-b*ltau)
 - sum = eam1 + eb
 - ldiff = ln(b) - ln(a)
 - ltau2 uses the improved lower bound
 - N2 the number of iterations for that
 - rp2 the return-place for that
 - ub is the upper bound (via Lambert-W0).

> ExploreBTs "L"
la lltau N rp

 - la = ln(a)
 - lltau = -ln(ln(tau(a,b)))
 - N, rp as above

> ExploreBTs N

Creates N random 1 <= a,b <= 2^64, and determines the maximum number of reached
iterations for ltau(a,b,true), ltau(1,a,true), ltau(1,b,true), printing
the maximum reached with a, b, and at the end the average.
N is read as a long double, and then converted to an unsigned 64-bit integer.

> ExploreBTs a b

prints the results of the computation for ln(tau(a,b)).

> ExploreBTs begin end N

where "begin" and "end" are of type floating_t, and N is an UInt_t,
creates the data for ltau(1,x), x running from begin to end in steps of
(begin-end)/N, as left-closed right-open interval, so leaving out end
for begin < end (and creating exactly N numbers).

> Explore begin end N L

now prints the data fitting to "ExploreBTs L" above.

TODOS:

1. Analyse ltau:

   (a) Are there cases where the current implementation does not terminate?
       Apparently not.
   (b) Are there cases where one of the asserts triggers (of course, not
       considering the two asserts for the arguments)?
       Apparently not.
   (c) The computation of fpx0 = a*Am1 + a + b*B could also be formulated as
       fpx0 = a*(Am1 + 1) + b*B, however the current form seems numerically
       better -- is this true? One would assume that the form with "+1"
       is slightly faster.
        (1) Addition +1 introduces an error, and then we multiplied with that
            accumulated error.
        (2) The current form not only avoids this, but is then also able to
            use fma.
   (d) How many interations are used? Where is the maximum reached, and
       how big is it? First the old way of computation:
        (1) A surprisingly good approximation of the number N of iterations is
            ln(b/a) (assuming b >= a), that is, ln(b)-ln(a).
            Below we get for a=1 and 1 <= b <= 10^10 the numerical relation
            N ~ 4.1579086 + 0.9009774 * ln(b).
        (2) Apparently due to the capping of the precision, this is shortcut
            at 11400 iterations.
        (3) How is the relation for float (32 bits) and double (64 bits)?
       Now for the new form, using the new lower bound (ltau_Wlb):
        (4) Maximum reached: 9th iteration, for inputs
            16703907943236820288 10311623104440942929  a/b ~ 1.6199 rp=3
            1066418399878483031  2412617782618577230   b/a ~ 2.2624 rp=3
            16589867186399607565 9683439302147830244   a/b ~ 1.7132 rp=3
            10160342482430904122 4299059295903343351   a/b ~ 2.3634 rp=3

            Experiment on cs-wsok:
> time ./ExploreBTs 1e10
6 15802936068377499781 183160344130469849
7 16791642383913292757 4065916805604311715
8 4218619374332161423 9979003554481616949
9 16589867186399607565 9683439302147830244
N=10000000000, seed=3588235421, mean=5.1369
real    1733m52.295s
user    1732m46.498s
sys     0m0.990s
            On csverify:
$ time ./ExploreBTs 1e10
5 18440059746480114549 6759161357241715963
7 12825578832803872753 6472497884121332112
8 12020428015117746044 3901439736596916880
9 16703907943236820288 10311623104440942929
N=10000000000, seed=197189146, mean=5.1369
real    1802m47.240s
user    1802m47.281s
sys     0m0.000s

Strange that this is slower than on cs-wsok.

The computations on csltok+cs-wsok seem more precise than those on csverify
(fewer iterations are needed)?!? On csverify we use gcc-7.3, on csltok and
cs-wsok gcc-7.4?!?

            Indeed these values are lower for the debug-version, and when
            option -fno-unsafe-math-optimizations is used. One needs to see
            the timing difference.

            Perhaps the upper bound could help here (see below)? Yes, that
            indeed further reduces the number of iterations.
   (e) Some approximations of the error, perhaps in dependency of ln(b/a),
       are needed.
        (1) We should check whether we have quadratic convergence (and
            what that means), as predicted by the general results for
            Newton's method. Perhaps the convergence speed should be
            measured.
            https://en.wikipedia.org/wiki/Rate_of_convergence

(2) Alternative methods
   (a) When we start below the tau-value, we have strictly monotonically
       increasing convergence.
       Indeed, for any start value x0 # lt := ltau(a,b), due to
       strict convexity of the lchi-function, the next Newton-value x1
       will yield a value x1 < lt, and if x0 < lt, so x1 > x0.
       So from the upper-bound via one Newton-step we get a lower bound --
       is is worth it?
       It seems that this lower bound, obtained by ltau_down(a,b), is always
       better than ltau_Wlb(a,b),
   (b) The Newton-Fourier method
       https://en.wikipedia.org/wiki/Newton%27s_method#Newton–Fourier_method
       yields lower and upper bounds at the same time, using a somewhat
       changed second strand -- how does this fit?
   (c) Via Halley's method
       https://en.wikipedia.org/wiki/Halley%27s_method
       we should obtain cubic convergence.

(3) Better exproximation for ltau(1,a) for a >= 1.

    Question asked at
    https://mathoverflow.net/questions/320584/a-certain-generalisation-of-the-golden-ratio
    --- this yielded a very good upper and lower bounds, both asymptotically
   precise (BranchingTuples:: ltau_Wlb and ltau_Wub).
   Numerically this seems very good (and we have asymptotic equality):
     ltau(a,b) >= 1/b * (q - log(q) + 1/2 log(q) / q)
     for q := log(b/a) = log(b) - log(a).
   This yields
     ltau(1,a) >= 1/a * (log(a) - log(log(a)) + 1/2 log(log(a)) / log(a)),
   which asymptotically is log(a) / a.
   Thus ltau(a,b) (always assuming a <= b) is asymptotically
     log(b/a) / b, for b/a going to infinity.

   (a) In principle, handling of this case is enough, via
       ltau(a,b) = 1/a ltau(1,b/a) for a <= b.
   (b) While having good lower bounds (for some a) would enable faster general
       computations, which currently just uses the start value ltau(1,1)
       (scaled).
       On should start curve-fitting, to see the general shape (growth rate)
       of f(x) = ltau(1,x), x >= 1.
   (b) ltau(1,1) = ln(2), ltau(1,2) = ln(1+sqrt(5)) - ln(2).
       At Maxima we see formulas via radicals for tau(1,3), tau(1,4), while
       the method fails for integers a >= 5 -- is this provably?
       tau(1,5): x^-1 + x^-5 = 1, <=> x^4 + 1 = x^5 <=> x^5 - x^4 - 1 = 0 ---
       is it provable that this polynomial has no solutions via radicals?
       Algorithms (implemented) should exist to decide this question.
   (c) The general polynomial is x^a - x^(a-1) - 1 = 0. Having more information
       on this for natural numbers a would be helpful.
       The related reccurrences are
       f(n) = f(n-1) + f(n-a). In Maxima, for a=5:
       f5(n) := if n<5 then n else f5(n-1) + f5(n-5)
       for n >= 0. It seems that this sequence is not in the OEIS.
         create_list(f5(n),n,0,20);
         [0,1,2,3,4,4,5,7,10,14,18,23,30,40,54,72,95,125,165,219,291]
   (d) One would assume that considering e.g.
         exp(-x) + exp(-5*x) = 1
       wouldn't have analytical advantages (only numerical)?
       All that should be raised at MathOverflow.
   (e) The general bounds we have are,
       using the general bounds
         mtau(a,b) := ln(2) / ltau(a,b)
         sqrt(a*b) <= mtau(a,b) <= (a+b)/2,
       which yields
         ltau(1,a) >= 2ln(2) / (a+1)
         ltau(1,a) <= ln(2) / sqrt(a).

       One would guess that the upper bound is closer to the truth?
       Considering the quotients, that's only true for parameter a not too big
       (and then it changes).

       For the curve-fitting as above, one would guess an approach
         f(x) ~ alpha / (x+gamma)^beta
       for 1/2 < gamma < 1, should yield a reasonable fit.
       That makes
         ln(f(x)) ~ ln(alpha) - beta * ln(x+gamma).
       Numerical evaluation below yields
         f(x) ~ exp(1.024) / (x+7.29)^0.9065
       but other models (also with three parameters) are better.

Numerical data:

First the direct data (non-logarithmic):

> ./ExploreBTs H > Data.txt
> for (( e=0; e<=9; )); do a1="1e${e}"; ((++e)); a2=" 1e${e}"; ./ExploreBTs ${a1} ${a2} 10000 >> Data.txt; done

> E=read.table("Data.txt", header=TRUE)
> x=E$b
> y=E$ltau
> plot(log(x), log(y))
> M=nls(log(y) ~ alpha * (log(x+gamma))^beta, start=list(alpha=-0.6,beta=1.1,gamma=1))
> summary(M)
Parameters:
        Estimate Std. Error  t value Pr(>|t|)
alpha -6.174e-01  3.839e-05 -16081.9   <2e-16 ***
beta   1.110e+00  2.173e-05  51062.5   <2e-16 ***
gamma  1.462e+00  1.813e-03    806.2   <2e-16 ***
Residual standard error: 0.02384 on 99997 degrees of freedom
Number of iterations to convergence: 4
Achieved convergence tolerance: 2.901e-06

So f(x) ~ exp(-0.6174 * log(x + 1.462)^1.11).

Alternatively (it seems nicest to consider log(x) versus -log(y)):

> M2=nls(-log(y) ~ alpha * log(x)^beta + gamma, start=list(alpha=1,beta=1,gamma=0))
> summary(M2)
Parameters:
       Estimate Std. Error t value Pr(>|t|)
alpha 5.567e-01  1.333e-04  4176.9   <2e-16 ***
beta  1.139e+00  7.262e-05 15686.2   <2e-16 ***
gamma 2.629e-01  4.793e-04   548.5   <2e-16 ***
Residual standard error: 0.03879 on 99997 degrees of freedom
Number of iterations to convergence: 4
Achieved convergence tolerance: 4.07e-07
> plot(log(x), -log(y))
> lines(log(x), predict(M2))

So f(x) ~ exp(-0.5567 * log(x)^1.139 + 0.2629).

Considering

> plot(log(log(x)), log(-log(y)))

it seems that the function becomes linear asymptotically.
What kind of role model is appropriate here?

Considering the above guess:
> M3=nls(log(y) ~ alpha - beta * log(x+gamma), start=list(alpha=1,beta=1,gamma=0))
> summary(M3)
Parameters:
       Estimate Std. Error t value Pr(>|t|)
alpha 1.024e+00  1.060e-03   966.3   <2e-16 ***
beta  9.065e-01  7.193e-05 12603.4   <2e-16 ***
gamma 7.290e+00  2.024e-02   360.2   <2e-16 ***
Residual standard error: 0.119 on 99997 degrees of freedom
Number of iterations to convergence: 8
Achieved convergence tolerance: 3.179e-06

In all cases we have the initial behaviour, closer to the upper bound,
which then turns to being close to the lower bound.


Concerning the number of iterations:

> summary(log(x)-E$ldiff)
      Min.    1st Qu.     Median       Mean    3rd Qu.       Max.
-3.553e-15  0.000e+00  0.000e+00  1.500e-19  0.000e+00  3.553e-15
> L=lm(E$N ~ E$ldiff)
> summary(L)
Residuals:
    Min      1Q  Median      3Q     Max
-4.1579 -0.3915 -0.1040  0.2898  2.9700
Coefficients:
             Estimate Std. Error t value Pr(>|t|)
(Intercept) 4.1579086  0.0034782    1195   <2e-16 ***
E$ldiff     0.9009774  0.0002549    3534   <2e-16 ***
Residual standard error: 0.5352 on 99998 degrees of freedom
Multiple R-squared:  0.9921,    Adjusted R-squared:  0.9921
F-statistic: 1.249e+07 on 1 and 99998 DF,  p-value: < 2.2e-16


Now the logarithmic data (large):

> ./ExploreBTs L > Data.txt
> for (( e=0; e<=999; )); do a1="1e${e}"; ((++e)); a2=" 1e${e}"; ./ExploreBTs ${a1} ${a2} 1000 L >> Data.txt; done

> E=read.table("Data.txt", header=TRUE)
> L=lm(E$lltau ~ E$la)
> summary(L)
Residuals:
    Min      1Q  Median      3Q     Max
-0.4101 -0.3388 -0.1315  0.1946  5.5797
Coefficients:
              Estimate Std. Error t value Pr(>|t|)
(Intercept) -5.213e+00  1.018e-03   -5122   <2e-16 ***
E$la         9.987e-01  7.654e-07 1304780   <2e-16 ***
Residual standard error: 0.5088 on 999998 degrees of freedom
Multiple R-squared:      1,     Adjusted R-squared:      1
F-statistic: 1.702e+12 on 1 and 999998 DF,  p-value: < 2.2e-16

> L2=lm(E$N ~ E$la)
> summary(L2)
Residuals:
    Min      1Q  Median      3Q     Max
-1.1350 -0.4728 -0.1217  0.3320  6.3422
Coefficients:
              Estimate Std. Error  t value Pr(>|t|)
(Intercept) -1.761e-01  1.394e-03   -126.4   <2e-16 ***
E$la         9.986e-01  1.048e-06 952722.6   <2e-16 ***
Residual standard error: 0.6967 on 999998 degrees of freedom
Multiple R-squared:      1,     Adjusted R-squared:      1
F-statistic: 9.077e+11 on 1 and 999998 DF,  p-value: < 2.2e-16

> L3=lm(E$lltau ~ E$N)
> summary(L3)
Residuals:
    Min      1Q  Median      3Q     Max
-2.3077 -0.2354  0.0666  0.3674  5.4029
Coefficients:
              Estimate Std. Error t value Pr(>|t|)
(Intercept) -5.036e+00  9.514e-04   -5294   <2e-16 ***
E$N          1.000e+00  7.165e-07 1395739   <2e-16 ***
Residual standard error: 0.4756 on 999998 degrees of freedom
Multiple R-squared:      1,     Adjusted R-squared:      1
F-statistic: 1.948e+12 on 1 and 999998 DF,  p-value: < 2.2e-16

Considering g(x) := -ln(ltau(1,x)) - ln(x) for x >= 1:

 - g(1) > 0
 - g is strictly decreasing, with g(x)=0 for
   x0 ~ 2.18019225601615510012.
 - g(x) goes to -infinity, but very slowly, e.g.
   g(10^1000) ~ -7.738421, g(10^10000) ~ -10.04.
 - So for x >= x0 we have
     -ln(ltau(1,x)) <= ln(x),
     <=> ltau(1,x) >= 1/x
   which is easy to prove (below).
   The quotient ltau(1,x) / (1/x) goes to infinity (slowly).

   2 ln(2) / (x+1) = 1/x <=> 2 ln(2) x = x+1 <=> x = 1/(2 ln(2) - 1)
   <=> x = 2.58..., so for x >= 2.58... the standard lower bound
   is a better aproximation. There isn't much information here -- the rather
   poor lower bound 1/x is rendered "good" due to taking the logarithm of
   the result (a double-logarithm in effect).

   We know via the W-bounds that ltau(1,x) is asymptotically equal to
   log(x) / x, so
     g(x) ~ -log(log(x))
   indeed verifying the above numerical computations.
   E.g. -log(log(10^1000)) = -7.741..., -log(log(10^10000)) = -10.04...

*/

#include <iostream>
#include <string>
#include <algorithm>

#include "FloatingPoint.hpp"
#include "BranchingTuples.hpp"
#include "RandGen.hpp"
#include "InOut.hpp"

namespace {

  const std::string version = "0.3.7";
  const std::string date = "13.1.2019";
  const std::string program = "ExploreBTs"
#ifndef NDEBUG
  "_debug"
#endif
;
  const std::string error = "ERROR[" + program + "]: ";

  namespace FP = FloatingPoint;

  struct Result_t {
    FP::floating_t t;
    FP::uint_t c;
    int place;
  };
  std::ostream& operator <<(std::ostream& out, const Result_t r) {
    return out << FP::Wrap(r.t) << " " << r.c << " " << r.place;
  }

  inline constexpr FP::floating_t ltau_down(FP::floating_t a, FP::floating_t b) noexcept {
    assert(a > 0);
    assert(b > 0);
    if (a == b) return FP::log(2)/a;
    if (a > b) {const auto t=a; a=b; b=t;}
    assert(a < b);
    const FP::floating_t x0 = BranchingTuples::ltau_Wub(a,b);
    const FP::floating_t Am1 = FP::expm1(-a*x0), B = FP::exp(-b*x0);
    const FP::floating_t fx0 = Am1 + B;
    const FP::floating_t fpx0 = FP::fma(b,B,FP::fma(a,Am1,a));
    assert(fpx0 > 0);
    const FP::floating_t x1 = x0 + fx0/fpx0;
    assert(x1 <= x0);
    return x1;
  }

  enum class LBlevel { ave, Wlb, Wub };
  inline constexpr FP::floating_t lb_choice(const FP::floating_t a, const FP::floating_t b, const LBlevel lv) noexcept {
    switch(lv) {
    case LBlevel::ave : return FP::log(4) / (a+b);
    case LBlevel::Wlb : return BranchingTuples::ltau_Wlb(a,b);
    case LBlevel::Wub : return ltau_down(a,b);
    default : return FP::log(4) / (a+b);
    }
  }
  // Version with counting iterations:
  inline constexpr Result_t ltau(const FP::floating_t a0, const FP::floating_t b0, const LBlevel lb = LBlevel::ave) noexcept {
    assert(a0 > 0);
    assert(b0 > 0);
    if (a0 == b0) return {FP::log(2)/a0, 0, 1};
    const FP::floating_t a = (a0 <= b0) ? a0 : b0, b = (a0 <= b0) ? b0 : a0;
    assert(a < b);
    if (FP::isinf(b)) return {0, 0, 2};
    FP::floating_t x0 = lb_choice(a,b,lb);
    FP::uint_t rounds = 0;
    const FP::floating_t na = -a, nb = -b;
    while (true) {
      ++rounds;
      const FP::floating_t Am1 = FP::expm1(na*x0), B = FP::exp(nb*x0);
      const FP::floating_t fx0 = Am1 + B;
      if (fx0 < 0) return {x0, rounds, 3};
      if (fx0 == 0) return {x0, rounds, 4};
      const FP::floating_t rfpx0 = 1 / FP::fma(b,B,FP::fma(a,Am1,a));
      assert(rfpx0 > 0);
      const FP::floating_t x1 = FP::fma(fx0, rfpx0, x0);
      assert(x1 >= x0);
      if (x1 == x0) return {x0, rounds, 5};
      x0 = x1;
    }
  }

  void output_header(std::ostream& out) {
    out << "a b ltau N rp eam1 eb sum ldiff ltau2 N2 rp2 ub lbd\n";
  }
  void output_single(std::ostream& out, const FP::floating_t a0, const FP::floating_t b0) {
    const FP::floating_t a = FP::min(a0,b0), b = FP::max(a0,b0);
    const auto res = ltau(a,b);
    const FP::floating_t lt = res.t,
      Am1 = FP::expm1(-a*lt), B = FP::exp(-b*lt),
      sum = Am1 + B, pred_num_its = FP::log(b) - FP::log(a);
    const auto res2 = ltau(a,b,LBlevel::Wlb);
    const auto res3 = ltau(a,b,LBlevel::Wub);
    out << FP::Wrap(a) << " " << FP::Wrap(b) << " " << res << " " << FP::Wrap(Am1) << " " << FP::Wrap(B) << " " << FP::Wrap(sum) << " " << FP::Wrap(pred_num_its) << " " << res2 << " " << FP::Wrap(BranchingTuples::ltau_Wub(a,b)) << " " << FP::Wrap(ltau_down(a,b) - BranchingTuples::ltau_Wlb(a,b)) << " " << res3 << "\n";
  }

  void output_1xheader(std::ostream& out) {
    out << "la lltau N rp\n";
  }
  void output_1xsingle(std::ostream& out, const FP::floating_t a) {
    const auto res = ltau(1,a);
    out << FP::Wrap(FP::log(a)) << " " << " " << FP::Wrap(-FP::log(res.t)) << " " << res.c << " " << res.place << "\n";
  }

}

int main(const int argc, const char* const argv[]) {
  if (argc == 1) {
    std::cout << "Usage:\n";
    std::cout << "One argument x: output the header resp. for x=L the log-header.\n";
    std::cout << "Two arguments: a, b > 0:\n";
    std::cout << " Output: a b ltau(a,b,false) N ret-p exp(-a*t)-1 exp(-b*t) sum ln(b)-ln(a) ltau(a,b,true) N2 ret-p2 ub.\n";
    std::cout << "Three arguments: begin, end, number of items.\n";
    std::cout << "A fourth argument \"L\": now output the log-form.\n";
    return 0;
  }
  if (argc == 2) {
    const std::string arg{argv[1]};
    if (arg == "L") output_1xheader(std::cout);
    else if (arg == "H") output_header(std::cout);
    else {
      const FP::UInt_t N = std::stold(arg);
      const RandGen::seed_t seed = InOut::timestamp();
      RandGen::randgen_t g(seed);
      FP::uint_t max = 0;
      FP::floating_t sum = 0;
      for (FP::UInt_t i = 0; i < N; ++i) {
        const FP::floating_t a = FP::floating_t(g())+1, b = FP::floating_t(g())+1;
        const auto res = ltau(a,b,LBlevel::Wlb), res2 = ltau(1,a,LBlevel::Wlb), res3 = ltau(1,b,LBlevel::Wlb);
        sum += res.c + res2.c + res3.c;
        const auto nmax = std::max(std::max(res.c, res2.c), res3.c);
        if (nmax > max) {
          max = nmax;
          std::cout << nmax << " " << FP::UInt_t(a) << " " << FP::UInt_t(b) << "\n";
        }
      }
      std::cout << "N=" << N << ", seed=" << seed << ", mean=" << sum/(FP::floating_t(N)*3) << "\n";
    }
    return 0;
  }
  if (argc == 3) {
    const FP::floating_t a0 = std::stold(argv[1]), b0 = std::stold(argv[2]);
    output_single(std::cout,a0,b0);
    return 0;
  }
  if (argc >= 4) {
    const FP::floating_t begin = std::stold(argv[1]), end = std::stold(argv[2]);
    if (end < begin) return 0;
    const FP::UInt_t N = std::stoull(argv[3]);
    if (N == 0) return 0;
    const FP::floating_t delta = (end - begin) / N;
    const bool logeval = (argc == 5) ? true : false;
    for (FP::UInt_t i = 0; i < N; ++i) {
      const FP::floating_t x = begin + i * delta;
      if (logeval) output_1xsingle(std::cout, x);
      else output_single(std::cout, 1, x);
    }
    return 0;
  }
}
