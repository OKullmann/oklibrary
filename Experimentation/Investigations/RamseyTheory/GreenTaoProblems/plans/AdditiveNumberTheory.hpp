// Oliver Kullmann, 26.9.2008 (Swansea)
/* Copyright 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/GreenTaoProblems/plans/AdditiveNumberTheory.hpp
  \brief On investigations into additive number theory
  
  
  \todo The distribution of arithmetic progressions amongst primes
  <ul>
   <li> The task is to find a nice (thus very likely approximated) law for
   the values in the list ln_arithprog_primes_c(k,n) (see
   ComputerAlgebra/NumberTheory/Lisp/PrimeNumbers.mac) for fixed k >= 1. </li>
   <li> Ploted via %e.g.
   \verbatim
plot2d([discrete,create_list(i,i,1,1000),ln_arithprog_primes_c(3,1000)]);
   \endverbatim
   </li>
   <li> For k = 1,2 this is trivial. </li>
   <li> For k >= 3 regression is to be performed; most powerful is using R,
   but for initial considerations also simple_linear_regression (use
   'load("stats")') can be used. </li>
   <li> There are role models for the regression function in the literature.
   </li>
   <li> One can also consider n_arithprog_primes_nc[k,n] (the non-cumulative
   data, i.e., as list the difference list of the above list):
   \verbatim
plot2d([discrete,create_list(i,i,1,1000),create_list(n_arithprog_primes_nc[3,n],n,1,1000)]);
   \endverbatim
   Though it seems that the accumulated data is easier to handle (since being
   smoother). </li>
   <li> Perhaps it is more appropriate to consider only changes here, that
   is, skipping n-values where no new arithmetic progression is added).
   This is plotted in non-cumulative resp. cumulative form by
   \verbatim
plot2d([discrete, sizes_strata_indmon_ohg(arithprog_primes_ohg(3,1000))]);
plot2d([discrete, sizes_cstrata_indmon_ohg(arithprog_primes_ohg(3,1000))]);
   \endverbatim
   </li>
   <li> Using Applications/RamseyTheory/CountProgressions_GreenTao.cpp
   and linear regression in R:
   \verbatim
RamseyTheory> ~/csoliver/SAT-Algorithmen/OKplatform/system_directories/bin/CountProgressions_GreenTao-O3-DNDEBUG 3 10000 > GT_3_10000

>  E = read.table("OKsystem/OKlib/Applications/RamseyTheory/GT_3_10000", colClasses=c("integer","integer"),header=TRUE)
x = log(E$n)
y = log(E$nhyp)
L = lm(y ~ x)
summary(L)
Coefficients:
              Estimate Std. Error t value Pr(>|t|)
(Intercept) -2.3098350  0.0010763   -2146   <2e-16 ***
x            1.8719533  0.0001301   14389   <2e-16 ***
Residual standard error: 0.01281 on 9993 degrees of freedom
Multiple R-squared:     1,      Adjusted R-squared:     1
F-statistic: 2.07e+08 on 1 and 9993 DF,  p-value: < 2.2e-16

C = coefficients(L)
f = function(x) {exp(C[1]) * x^(C[2])}
plot(E$n,E$nhyp)
lines(E$n,f(E$n))
exp(C[1])
  0.09927763
C[2]
  1.871953
   \endverbatim
   Thus f(n) = 0.09927763 * n^1.871953 is a good approximation of the number
   of 3-progressions in the first n prime numbers (for n <= 10000). </li>
   <li> Using non-linear regression:
   \verbatim
X = E$n
Y = E$nhyp
NL = nls(Y ~ a*(X^b), start = c(a=0.09927763, b=1.871953))
summary(NL)
  Parameters:
   Estimate Std. Error t value Pr(>|t|)
a 8.331e-02  1.883e-05    4424   <2e-16 ***
b 1.892e+00  2.511e-05   75374   <2e-16 ***
Residual standard error: 741.2 on 9993 degrees of freedom
Number of iterations to convergence: 4
Achieved convergence tolerance: 4.871e-08
C2 = coefficients(NL)
C2
         a          b
0.08330711 1.89230541
f2 = function(x) {C2[1] * x^(C2[2])}
plot(E$n,E$nhyp)
lines(E$n,f2(E$n))
   \endverbatim
   So f2(n) = 0.08330711 * n^1.89230541 seems to be an even better model. </li>
   <li> For N=100000 we need a C++ program which doesn't store the
   progressions. For N=30000 by non-linear regression we obtain
   f3(n) = 0.07487019 * n^1.90377210. We should consider larger n. </li>
   <li> Now for k=4:
   \verbatim
RamseyTheory> ~/csoliver/SAT-Algorithmen/OKplatform/system_directories/bin/CountProgressions_GreenTao-O3-DNDEBUG 4 20000 > GT_4_20000

>  E = read.table("OKsystem/OKlib/Applications/RamseyTheory/GT_4_20000", colClasses=c("integer","integer"),header=TRUE)
x = log(E$n)
y = log(E$nhyp)
L = lm(y ~ x)
summary(L)
Coefficients:
              Estimate Std. Error t value Pr(>|t|)
(Intercept) -3.3482256  0.0010711   -3126   <2e-16 ***
x            1.7696832  0.0001195   14812   <2e-16 ***
Residual standard error: 0.01645 on 19973 degrees of freedom
Multiple R-squared: 0.9999,     Adjusted R-squared: 0.9999
F-statistic: 2.194e+08 on 1 and 19973 DF,  p-value: < 2.2e-16

C = coefficients(L)
f = function(x) {exp(C[1]) * x^(C[2])}
plot(E$n,E$nhyp)
lines(E$n,f(E$n))
exp(C[1])
  0.03514666
C[2]
  1.769683

X = E$n
Y = E$nhyp
NL = nls(Y ~ a*(X^b), start = c(a=0.03514666, b=1.769683))
summary(NL)
Parameters:
   Estimate Std. Error t value Pr(>|t|)
a 2.663e-02  7.532e-06    3535   <2e-16 ***
b 1.799e+00  2.920e-05   61627   <2e-16 ***
Residual standard error: 612 on 19973 degrees of freedom
Number of iterations to convergence: 5
Achieved convergence tolerance: 1.020e-07
C2 = coefficients(NL)
C2
         a          b
0.02662577 1.79946138
f2 = function(x) {C2[1] * x^(C2[2])}
plot(E$n,E$nhyp)
lines(E$n,f2(E$n))
   \endverbatim
   So f_4(n) = 0.02662577 * x^1.79946138 is a good model (for n <= 20000).
   </li>
   <li> Now for k=5:
   \verbatim
RamseyTheory> ~/csoliver/SAT-Algorithmen/OKplatform/system_directories/bin/CountProgressions_GreenTao-O3-DNDEBUG 5 40000 > GT_5_40000

>  E = read.table("OKsystem/OKlib/Applications/RamseyTheory/GT_5_40000", colClasses=c("integer","integer"),header=TRUE)
x = log(E$n)
y = log(E$nhyp)
L = lm(y ~ x)
summary(L)
Coefficients:
              Estimate Std. Error t value Pr(>|t|)
(Intercept) -4.8968771  0.0010886   -4498   <2e-16 ***
x            1.6911613  0.0001123   15058   <2e-16 ***
Residual standard error: 0.01988 on 39344 degrees of freedom
Multiple R-squared: 0.9998,     Adjusted R-squared: 0.9998
F-statistic: 2.268e+08 on 1 and 39344 DF,  p-value: < 2.2e-16

C = coefficients(L)
f = function(x) {exp(C[1]) * x^(C[2])}
plot(E$n,E$nhyp)
lines(E$n,f(E$n))
exp(C[1])
  0.007469874
C[2]
  1.691161

X = E$n
Y = E$nhyp
NL = nls(Y ~ a*(X^b), start = c(a=0.007469874, b=1.691161))
summary(NL)
Parameters:
   Estimate Std. Error t value Pr(>|t|)
a 5.413e-03  1.881e-06    2878   <2e-16 ***
b 1.723e+00  3.349e-05   51449   <2e-16 ***
Residual standard error: 329.6 on 39344 degrees of freedom
Number of iterations to convergence: 6
Achieved convergence tolerance: 1.735e-08
C2 = coefficients(NL)
C2
          a           b
0.005413189 1.723282665
f2 = function(x) {C2[1] * x^(C2[2])}
plot(E$n,E$nhyp)
lines(E$n,f2(E$n))
   \endverbatim
   So f_5(n) = 0.005413189 * x^1.723282665 is a good model (for n <= 40000).
   </li>
   <li> And one can consider the densities
   ln_arithprog_primes_c(k,n) / create_list(i,i,1,n). </li>
   <li> Hard to believe that there is nothing in the literature / on the
   Internet: We should enter for example ln_arithprog_primes_c(3,30) =
   [0,0,0,1,2,2,3,5,7,9,11,11,13,16,17,20,23,24,26,30,32,36,40,44,46,49,53,56,59,64]
   into that database of integer sequences and see whether there is information
   in it. </li>
   <li> Yes, this sequence is A125505 in
   http://www.research.att.com/~njas/sequences/Seis.html. </li>
   <li> There it is only listed for n=64; this we can easily extend, and
   perhaps we should do so. </li>
   <li> And apparently for k >= 4 there is nothing entered there --- we
   should change this.
    <ol>
     <li> Say, up to k=10. </li>
     <li> For k=10 for example the first 315 values are 0, and then
     at least until index 3000 the value is constant 1; for such sequences
     we need a compressed representation. </li>
    </ol>
   </li>
   <li> Of interest is also
   http://www.research.att.com/~njas/sequences/Sindx_Pri.html#primes_AP
   which gives an overview. It contains for given k the smallest starting
   term, smallest difference and smallest end term for arithmetical
   progressions of primes of length k. However I do not understand
   these sequences (strange explanations). </li>
  </ul>


  \todo Finding the first arithmetic progression
  <ul>
   <li> What seems very natural to me is k -> how many first primes are
   needed to get an progression of length k; this is greentao_1(k). </li>
   <li> See http://users.cybercity.dk/~dsl522332/math/aprecords.htm for
   current information around this subject; the related sequence is (now
   showing p_i instead of i):
   \verbatim
L : [
2, 3, 7, 23, 29, 157, 907, 1669, 1879, 2089, 
249037, 262897, 725663, 36850999, 173471351, 198793279, 4827507229, 17010526363, 83547839407, 572945039351,
6269243827111
];
   \endverbatim
   </li>
   <li> This is available as A005115. </li>
   <li> From this sequence S via map(rank_primes,S) we obtain the sequence:
   \verbatim
for p in L do print(rank_primes(p));
1,2,4,9,10,37,155,263,289,316,
21966,23060,58464,2253121,9686320,11015837,227225515,755752809,3466256932,22009064470,
220525414079
   \endverbatim
   where starting with k=12 actually "RankPrimes" was used. </li>
   <li> Plot the data (using R) suggests that (log,log(log))-transformation
   for (x,y) might be appropriate (that is, a model y = exp(a * x^b)):
   \verbatim
y = c(4,9,10,37,155,263,289,316,21966,23060,58464,2253121,9686320,11015837,227225515,755752809,3466256932,22009064470,220525414079)
x = 3:21
plot(log(x),log(log(y)))

m0 = lm(log(log(y)) ~ log(x))
summary(m0)
            Estimate Std. Error t value Pr(>|t|)
(Intercept) -1.48793    0.11902  -12.50 5.37e-10 ***
log(x)       1.54569    0.04925   31.38  < 2e-16 ***
Residual standard error: 0.1191 on 17 degrees of freedom
Multiple R-squared: 0.983,      Adjusted R-squared: 0.982
F-statistic: 984.9 on 1 and 17 DF,  p-value: < 2.2e-16

exp(coefficients(m0)[1])
0.2258395
coefficients(m0)[2]
1.545693
s = c(a = 0.2258395, b = 1.545693)
m = nls(y ~ exp(a * x^b), start = s)

summary(m)
  Estimate Std. Error t value Pr(>|t|)
a 0.084184   0.002663   31.61   <2e-16 ***
b 1.884508   0.010393  181.32   <2e-16 ***
Residual standard error: 273300000 on 17 degrees of freedom
Number of iterations to convergence: 18
Achieved convergence tolerance: 7.111e-06

plot(x,log(y))
lines(x,log(predict(m)))
lines(x,exp(predict(m0)))
   \endverbatim
   </li>
  </ul>


  \todo The first arithmetic progression allowing a missing number
  <ul>
   <li> greentao_2(2,k) has still a "mostly number theoretical touch". </li>
   <li> greentao_2(2,0) = 0, greentao_2(2,1) = 2, greentao(2,2) = 3. </li>
   <li> greentao_2(2,3) = 7. </li>
   <li> greentao_2(2,4) = 14. </li>
   <li> greentao_2(2,5) = 31. </li>
   <li> This sequence is apparently not in that "online encyclopedia" (also
   not after applying unrank_primes to it, obtaining [3,5,17,43,127]). </li>
  </ul>

*/

