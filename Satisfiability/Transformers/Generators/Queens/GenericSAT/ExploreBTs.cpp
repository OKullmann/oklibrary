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

> ExploreBTs x
a b ltau N rp eam1 eb sum ldiff

(where "x" can be any string) prints the header line for the output:
 - a, b the inputs, sorted in ascending order
 - ltau = ln(tau(a,b))
 - N is the number of iterations
 - rp is the return-place
 - eam1 = exp(-a*ltau)-1
 - eb = exp(-b*ltau)
 - sum = eam1 + eb
 - ldff = ln(b) - ln(a)

> ExploreBTs L
la lltau N rp

 - la = ln(a)
 - lltau = -ln(ln(tau(a,b)))
 - N, rp as above

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
       how big is it?
        (1) A surprisingly good approximation of the number N of iterations is
            ln(b/a) (assuming b >= a), that is, ln(b)-ln(a).
            Below we get for a=1 and 1 <= b <= 10^10 the numerical relation
            N ~ 4.1579086 + 0.9009774 * ln(b).
        (2) Apparently due to the capping of the precision, this is shortcut
            at 11400 iterations.
        (3) How is the relation for float (32 bits) and double (64 bits)?
   (e) Some approximations of the error, perhaps in dependency of ln(b/a),
       are needed.
        (1) We should check whether we have quadratic convergence (and
            what that means), as predicted by the general results for
            Newton's method. Perhaps the convergence speed should be
            measured.
            https://en.wikipedia.org/wiki/Rate_of_convergence

(2) Alternative methods
   (a) When we start below the tau-value, we have strictly monotonically
       increasing convergence. And above -- is it now strictly monotonically
       decreasing?
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

> ./ExploreBTs x > Data.txt
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

> ./ExploreBTs 1 > Data.txt
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
   g(10^1000) ~ -7.738, g(10^10000) ~ -7.743.
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

*/

#include <iostream>
#include <string>

#include "FloatingPoint.hpp"
#include "BranchingTuples.hpp"

namespace {

  const std::string version = "0.3.0";
  const std::string date = "10.1.2019";
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

  // Version with counting iterations:
  inline constexpr Result_t ltau(FP::floating_t a, FP::floating_t b) noexcept {
    assert(a > 0);
    assert(b > 0);
    if (a == b) return {FP::log(2)/a, 0, 1};
    if (a > b) {const auto t=a; a=b; b=t;}
    assert(a < b);
    if (FP::isinf(b)) return {0, 0, 2};
    FP::floating_t x0 = FP::log(4) / (a+b);
    FP::uint_t rounds = 0;
    while (true) {
      ++rounds;
      const FP::floating_t Am1 = FP::expm1(-a*x0), B = FP::exp(-b*x0);
      const FP::floating_t fx0 = Am1 + B;
      if (fx0 < 0) return {x0, rounds, 3};
      if (fx0 == 0) return {x0, rounds, 4};
      const FP::floating_t fpx0 = FP::fma(b,B,FP::fma(a,Am1,a));
      assert(fpx0 > 0);
      const FP::floating_t x1 = x0 + fx0/fpx0;
      assert(x1 >= x0);
      if (x1 == x0) return {x0, rounds, 5};
      x0 = x1;
    }
  }

  void output_header(std::ostream& out) {
    out << "a b ltau N rp eam1 eb sum ldiff\n";
  }
  void output_single(std::ostream& out, const FP::floating_t a0, const FP::floating_t b0) {
    const FP::floating_t a = FP::min(a0,b0), b = FP::max(a0,b0);
    const auto res = ltau(a,b);
    const FP::floating_t lt = res.t,
      Am1 = FP::expm1(-a*lt), B = FP::exp(-b*lt),
      sum = Am1 + B, pred_num_its = FP::log(b) - FP::log(a);
    out << FP::Wrap(a) << " " << FP::Wrap(b) << " " << res << " " << FP::Wrap(Am1) << " " << FP::Wrap(B) << " " << FP::Wrap(sum) << " " << FP::Wrap(pred_num_its) << "\n";
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
    std::cout << " Output: a b ln(tau(a,b)) N ret-p exp(-a*t)-1 exp(-b*t) sum ln(b)-ln(a).\n";
    std::cout << "Three arguments: begin, end, number of items.\n";
    std::cout << "A fourth argument \"L\": now output the log-form.\n";
    return 0;
  }
  if (argc == 2) {
    if (std::string(argv[1]) == "L") output_1xheader(std::cout);
    else output_header(std::cout);
    return 0;
  }
  if (argc == 3) {
    const FP::floating_t a0 = std::stold(argv[1]), b0 = std::stold(argv[2]);
    output_single(std::cout,a0,b0);
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
  }
}
