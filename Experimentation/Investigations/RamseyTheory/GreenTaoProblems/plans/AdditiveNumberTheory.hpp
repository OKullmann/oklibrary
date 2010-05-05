// Oliver Kullmann, 26.9.2008 (Swansea)
/* Copyright 2008, 2009, 2010 Oliver Kullmann
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
   <li> The best internet resource seems to be
   http://primes.utm.edu/top20/page.php?id=14 . </li>
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
   The main starting point should be [Grosswald, Hagis, 1979, Arithmetic
   progressions consisting only of primes, Mathematics of Computation,
   33(148):1343-1352]. </li>
   <li> This is implemented by "fit_greentao" in OKlib/Statistics/R/GreenTao.R.
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
   <li> At the C++-level we have
   Applications/RamseyTheory/CountProgressions_GreenTao.cpp. </li>
  </ul>


  \todo k=3
  <ul>
   <li> Using Applications/RamseyTheory/CountProgressions_GreenTao.cpp
   and linear regression in R:
   \verbatim
> f = fit_greentao(3,1000)
Number of observations (changes) =  995
Max nhyp =  40510
Coefficients: -1.455676 0.2433109 4.530863 -13.22174
Residual range: -46.01184 36.11967

> f(100)
  578.4306

> f = fit_greentao(3,10000)
Number of observations (changes) =  9995
Max nhyp =  3091531
Coefficients: 187.4584 0.2631131 4.522825 -14.97632
Residual range: -912.0556 707.5263

f(100)
  741.4426
   \endverbatim
   (where the correct value for f(100) is 579). </li>
   <li> So f_3(n) = 187.4584 + 0.2631131*x^2/log(x)^3 * 
   (1 + 4.522825/log(x) - 14.97632/log(x)^2) is a good model, where
   x = n*log(n). </li>
   <li> For N=30000 we obtain:
   \verbatim
> f = fit_greentao(3,30000)
Number of observations (changes) =  29995
Max nhyp =  25000740
Coefficients: 393.4117 0.2968310 3.823654 -11.38840
Residual range: -3423.644 3254.2

f(100)
  961.2944
   \endverbatim
   So f_3(n) = 393.4117 + 0.2968310*x^2/log(x)^3 * 
   (1 + 3.823654/log(x) - 11.38840/log(x)^2) is a good model, where
   x = n*log(n). </li>
   <li> For N=100000 we need a C++ program which doesn't store the
   progressions. </li>
  </ul>


  \todo k=4
  <ul>
   <li>
   \verbatim
> f = fit_greentao(4,20000)
Number of observations (changes) =  19975
Max nhyp =  1462656
Coefficients: 145.6985 0.2360898 10.58077 -41.45108
Residual range: -412.8622 501.4584

> f = fit_greentao(4,40000)
Number of observations (changes) =  39975
Max nhyp =  5148933
Coefficients: -497.1373 0.5891194 2.073577 9.843178
Residual range: -929.1254 1103.620
   \endverbatim
   </li>
  </ul>


  \todo k=5
  <ul>
   <li>
   \verbatim
> f = fit_greentao(5,40000)
Number of observations (changes) =  39347
Max nhyp =  462282
Coefficients: -212.7657 0.762841 0.06329398 28.88302
Residual range: -289.5044 227.3657

> f = fit_greentao(5,80000)
Number of observations (changes) =  79347
Max nhyp =  1545857
Coefficients: -89.12968 0.4274986 8.573074 -25.10926
Residual range: -540.5143 447.7293
   \endverbatim
   </li>
  </ul>


  \todo k=6
  <ul>
   <li>
   \verbatim
> f = fit_greentao(6,80000)
Number of observations (changes) =  70976
Max nhyp =  234774
Coefficients: -106.7271 1.150551 9.422184 14.64226
Residual range: -163.4261 167.8231

80000 - 70976
  9024

> f = fit_greentao(6,160000)
Number of observations (changes) =  150810
Max nhyp =  749499
Coefficients: -100.9089 0.8658116 16.82488 -33.42583
Residual range: -366.5206 321.9369

160000 - 150810
  9190
   \endverbatim
   </li>
  </ul>


  \todo k=7
  <ul>
   <li>
   \verbatim
> f = fit_greentao(7,160000)
Number of observations (changes) =  59909
Max nhyp =  78058
Coefficients: -5.022792 0.2405188 54.32041 -247.3408
Residual range: -169.1657 159.1667

160000 -  59909
  100091

> f = fit_greentao(7,500000)
Number of observations (changes) =  298388
Max nhyp =  497046
Coefficients: 593.3755 -5.288303 225.8332 -1575.388
Residual range: -473.103 585.1443

500000 - 298388
  201612

> f = fit_greentao(7,1000000)
Number of observations (changes) =  736449
Max nhyp =  1558942
Coefficients: -981.546 4.110067 -67.06604 709.1288
Residual range: -972.0245 518.9356

1000000 -  736449
  263551
   \endverbatim
   </li>
  </ul>


  \todo k=8
  <ul>
   <li>
   \verbatim
> f = fit_greentao(8,1000000)
Number of observations (changes) =  230866
Max nhyp =  268082
Coefficients: -177.2491 3.526203 53.25977 198.9277
Residual range: -200.5256 189.2639

1000000 - 230866
  769134

> f = fit_greentao(8,2000000)
Number of observations (changes) =  649644
Max nhyp =  812685
Coefficients: 239.008 3.78891 61.47959 -7.395578
Residual range: -534.6951 876.0352

2000000 - 649644
  1350356

> f = fit_greentao(8,4000000)
Number of observations (changes) =  1781803
Max nhyp =  2491439
Coefficients: -675.2275 6.202912 -31.85938 883.982
Residual range: -863.7256 977.2554

4000000 -  1781803
  2218197

> f = fit_greentao(8,8000000)
TO BE UPDATED:
Number of observations (changes) =  4688545
Max nhyp =  7728990
Non-linear model nhyp = a * n^b:
           a            b
4.218958e-05 1.631506e+00
Residual range:  -10123.48 6262.802

8000000 - 4688545
  3311455
   \endverbatim
   </li>
  </ul>


  \todo Some further considerations
  <ul>
   <li> A natural conjecture here is that for every k there exists some
   index i_k >= 1 such that for every j >= i_k there exists an arithmetic
   progression of length k ending with p_j. For k <= 5 we can already
   conjecture the smallest such i_k, and this should also be possible for
   k=6, while then it becomes more difficult. </li>
   <li> One can consider the densities
   ln_arithprog_primes_c(k,n) / create_list(i,i,1,n). </li>
   <li> Hard to believe that there is nothing in the literature / on the
   Internet: We should enter for example ln_arithprog_primes_c(3,30) =
   [0,0,0,1,2,2,3,5,7,9,11,11,13,16,17,20,23,24,26,30,32,36,40,44,46,49,53,56,59,64]
   into that database of integer sequences and see whether there is information
   in it. </li>
   <li> Yes, this sequence is A125505 in
   http://www.research.att.com/~njas/sequences/Seis.html (see
   http://www.research.att.com/~njas/sequences/A125505). </li>
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
   <li> Additional data (upper bounds):
    <ol>
     <li> At http://users.cybercity.dk/~dsl522332/math/aprecords.htm data
     upper bounds are available also for 22 <= k <= 26:
     \verbatim
11410337850553 + 475180·19#·n (108201410428753)
403185216600637 + 9523·23#·n (449924511422857)
515486946529943 + 136831·23#·n (1217585417914253)
6171054912832631 + 366384·23#·n (8132758706802551)
43142746595714191 + 23681770·23#·n (175223597495211691)
     \endverbatim
     (the numbers in brackets are the end-values, in which we are interest;
     however, it is not known that these are the smallest possible end-values).
     </li>
     <li> The task is now to rank these large prime numbers. </li>
     <li> Running
     \verbatim
OKlib/Structures/NumberTheory/PrimeNumbers> ~/OKplatform/bin/RankPrimes-O3-DNDEBUG data/PrimeRanks.txt rank
108201410428753
     \endverbatim
     would takes a very long time. </li>
     <li> In Structures/NumberTheory/PrimeNumbers/plans/RankPrimes.hpp an
     implementation is discussed under "Better algorithms" which can compute
     these ranks relatively easily. </li>
    </ol>
   </li>
   <li> Plotting the data (using R) suggests that (log,log(log))-transformation
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

