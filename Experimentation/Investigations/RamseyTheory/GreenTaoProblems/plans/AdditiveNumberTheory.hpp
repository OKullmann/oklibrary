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
   The main starting point is [Grosswald, Hagis, 1979, Arithmetic
   progressions consisting only of primes, Mathematics of Computation,
   33(148):1343-1352]. </li>
   <li> The conjecture is that nhyp_arithprog_primes_hg(k,n) is asymptotically
   equal to C_k / (2*(k-1)) * n^2 / (log n)^(k-2). This is proven for k <= 4.
    <ol>
     <li> We have C_2 = 1. </li>
     <li> The following data has been computed by
     \verbatim
> GrosswaldHagisFormula-O3-DNDEBUG 3,20 1000000000
Precision in bits: 416
The first prime number not taken into account: 1000000007
     \endverbatim
     </li>
     <li> Computation of C_3:
     \verbatim
C_3 = 1.3203236317546366339
Finite and the infinite part: 1.5, 0.88021575450309108924
1 - first remaining factor: 9.99999988000000108e-19
     \endverbatim
     while GH give the value 1.32032; a guess is C_3 = 1.320323631... . </li>
     <li> Computation of C_4:
     \verbatim
C_4 = 2.8582485961147147258
Finite and the infinite part: 4.5, 0.63516635469215882796
1 - first remaining factor: 2.999999966000000288e-18
     \endverbatim
     while GH give the value 2.85825; let's guess C_4 = 2.85824859... .  </li>
     <li> Computation of C_5:
     \verbatim
C_5 = 4.1511808643862090045
Finite and the infinite part: 6.591796875, 0.62974951187133007712
1 - first remaining factor: 5.999999936000000507e-18
     \endverbatim
     while GH give the value 4.15118; let's guess C_5 = 4.1511808... . </li>
     <li> Computation of C_6:
     \verbatim
C_6 = 10.131794954669182916
Finite and the infinite part: 24.71923828125, 0.40987488527728368616
1 - first remaining factor: 9.999999900000000735e-18
     \endverbatim
     while GH give the value 10.1318; let's guess C_6 = 10.1317949... . </li>
     <li> Computation of C_7:
     \verbatim
C_7 = 17.298612323552886198
Finite and the infinite part: 33.392508824666341146, 0.51803871384394953143
1 - first remaining factor: 1.4999999860000000945e-17
     \endverbatim
     while GH give the value 17.2986; let's guess C_7 = 17.2986123... . </li>
     <li> Computation of C_8:
     \verbatim
C_8 = 53.971948352406135059
Finite and the infinite part: 146.09222610791524251, 0.36943751074433075237
1 - first remaining factor: 2.0999999818000001113e-17
     \endverbatim
     while GH give the value 53.9720; let's guess C_8 = 53.9719483... . </li>
     <li> Computation of C_9:
     \verbatim
C_9 = 148.55162885563210856
Finite and the infinite part: 639.15348922212918599, 0.23241933488687408369
1 - first remaining factor: 2.7999999776000001218e-17
     \endverbatim
     while GH give the value 148.552; let's guess C_9 = 148.551628... . </li>
     <li> Computation of C_10:
     \verbatim
C_10 = 336.034327307194497
Finite and the infinite part: 2796.2965153468151887, 0.12017120697427801113
1 - first remaining factor: 3.5999999736000001242e-17
     \endverbatim
     while GH give the value 336.034; let's guess C_10 = 336.034327... . </li>
     <li> Computation of C_11:
     \verbatim
C_11 = 511.42228312047417728
     \endverbatim
     while GH give the value 511.422; let's guess C_11 = 511.422283... . </li>
     <li> Computation of C_12:
     \verbatim
C_12 = 1312.3197146277008806
Finite and the infinite part: 13882.452232084007257, 0.094530828753350440844
1 - first remaining factor: 5.499999967000000099e-17
     \endverbatim
     while GH give the value 1312.32; let's guess C_12 = 1312.31971... . </li>
     <li> Computation of C_13:
     \verbatim
C_13 = 2364.598970504069348
Finite and the infinite part: 13428.850937459748114, 0.17608349228957677085
1 - first remaining factor: 6.5999999648000000693e-17
     \endverbatim
     while GH give the value 2364.60; let's guess C_13 = 2364.59897... . </li>
     <li> Computation of C_14:
     \verbatim
C_14 = 7820.6000583800047652
Finite and the infinite part: 70011.873897902124282, 0.11170390996511158496
1 - first remaining factor: 7.7999999636000000273e-17
     \endverbatim
     while GH give the value 7820.61; let's guess C_14 = 7820.60005... .
     So here we have a descrepancy. </li>
     <li> Computation of C_15:
     \verbatim
C_15 = 22938.908728604769022
Finite and the infinite part: 365009.82172812513753, 0.062844634207379343627
1 - first remaining factor: 9.0999999635999999727e-17
     \endverbatim
     while GH give the value 22939; let's guess C_15 = 22938.9087... . </li>
     <li> Computation of C_16:
     \verbatim
C_16 = 55651.462823015330355
Finite and the infinite part: 1902993.9143221524098, 0.029244162266718764193
1 - first remaining factor: 1.0499999964999999906e-16
     \endverbatim
     while GH give the value 55651; let's guess C_16 = 55651.462... . </li>
     <li> Computation of C_17:
     \verbatim
C_17 = 91555.111732881423894
Finite and the infinite part: 1539516.4947250383578, 0.059470042735224739834
1 - first remaining factor: 1.1999999967999999826e-16
     \endverbatim
     while GH give the value 91555; let's guess C_17 = 91555.111.... </li>
     <li> Computation of C_18:
     \verbatim
C_18 = 256474.86146297656364
Finite and the infinite part: 8527979.2287552010855, 0.030074517606489678389
1 - first remaining factor: 1.3599999972799999735e-16
     \endverbatim
     while GH give the value 256480; let's guess C_18 = 256474.861... .
     So here we have a descrepancy. </li>
     <li> Computation of C_19:
     \verbatim
C_19 = 510992.01391519899563
Finite and the infinite part: 6579820.4950027289514, 0.077660479385917816586
1 - first remaining factor: 1.5299999979599999633e-16
     \endverbatim
     while GH give the value 510990; let's guess C_19 = 510992.01... . </li>
     <li> Computation of C_20:
     \verbatim
C_20 = 1900972.5998672649484
Finite and the infinite part: 38473077.653099090942, 0.049410463519653916442
1 - first remaining factor: 1.7099999988599999521e-16
     \endverbatim
     while GH give the value 1901000; let's guess C_19 = 1900972.5... . </li>
    </ol>
   </li>
   <li> The role model for curve-fitting is implemented by "fit_greentao"
   in OKlib/Statistics/R/GreenTao.R. </li>
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
Coefficients: 17.11290 -2.962008 21.49815 -57.41431 ;  0.3300809
Residual range: -37.03781 45.99563

> f(100)
  578.0767

> f = fit_greentao(3,10000)
Number of observations (changes) =  9995
Max nhyp =  3091531
Coefficients: 321.6195 -3.338598 30.12935 -101.7892 ;  0.3300809
Residual range: -715.0495 930.5577

f(100)
  790.0139
   \endverbatim
   (where the correct value for f(100) is 579). </li>
   <li> So f_3(n) = 321.6195 + 0.3300809*n^2/log(n) * (1 + 
   -3.338598/log(n) + 30.12935/log(n)^2 - 101.7892/log(n)^3) is a good model.
   </li>
   <li> For N=30000 we obtain:
   \verbatim
> f = fit_greentao(3,30000)
Number of observations (changes) =  29995
Max nhyp =  25000740
Coefficients: 843.0986 -3.324593 30.58643 -107.2370 ;  0.3300809
Residual range: -3259.890 3230.183

f(100)
  1289.139
   \endverbatim
   So f_3(n) = 843.0986 + 0.3300809*n^2/log(n) * (1 +
   -3.324593/log(n) + 30.58643/log(n)^2 - 107.2370/log(n)^3) is a good model.
   </li>
   <li> For N=100000 we need a C++ program which doesn't store the
   progressions. </li>
  </ul>


  \todo k=4
  <ul>
   <li>
   \verbatim
TO BE UPDATED:
> f = fit_greentao(4,20000)
Number of observations (changes) =  19975
Max nhyp =  1462656
Coefficients: 165.4101 0.3400277 0.684767 -4.955831
Residual range: -499.0341 410.999

> f = fit_greentao(4,40000)
Number of observations (changes) =  39975
Max nhyp =  5148933
Coefficients: -549.0156 0.4599897 -1.662170 6.5372
Residual range: -1115.498 923.5618
   \endverbatim
   </li>
  </ul>


  \todo k=5
  <ul>
   <li>
   \verbatim
TO BE UPDATED:
> f = fit_greentao(5,40000)
Number of observations (changes) =  39347
Max nhyp =  462282
Coefficients: -219.7298 0.5031083 -2.683063 10.54289
Residual range: -230.2833 290.1797

> f = fit_greentao(5,80000)
Number of observations (changes) =  79347
Max nhyp =  1545857
Coefficients: -84.72713 0.4143196 -0.8405665 0.9811276
Residual range: -448.2709 539.9275
   \endverbatim
   </li>
  </ul>


  \todo k=6
  <ul>
   <li>
   \verbatim
TO BE UPDATED:
> f = fit_greentao(6,80000)
Number of observations (changes) =  70976
Max nhyp =  234774
Coefficients: -113.8172 0.8496358 -4.188096 14.99478
Residual range: -168.2007 163.1515

80000 - 70976
  9024

> f = fit_greentao(6,160000)
Number of observations (changes) =  150810
Max nhyp =  749499
Coefficients: -102.2112 0.7783336 -2.665443 6.869895
Residual range: -322.2387 365.8506

160000 - 150810
  9190
   \endverbatim
   </li>
  </ul>


  \todo k=7
  <ul>
   <li>
   \verbatim
TO BE UPDATED:
> f = fit_greentao(7,160000)
Number of observations (changes) =  59909
Max nhyp =  78058
Coefficients: -3.696913 0.8696752 -0.3139054 -13.02317
Residual range: -159.1105 169.4958

160000 -  59909
  100091

> f = fit_greentao(7,500000)
Number of observations (changes) =  298388
Max nhyp =  497046
Coefficients: 703.3813 -0.3935023 32.41404 -224.6832
Residual range: -681.7677 457.4785

500000 - 298388
  201612

> f = fit_greentao(7,1000000)
Number of observations (changes) =  736449
Max nhyp =  1558942
Coefficients: -1097.584 1.736234 -23.11872 137.8766
Residual range: -529.4176 1079.641

1000000 -  736449
  263551
   \endverbatim
   </li>
  </ul>


  \todo k=8
  <ul>
   <li>
   \verbatim
TO BE UPDATED:
> f = fit_greentao(8,1000000)
Number of observations (changes) =  230866
Max nhyp =  268082
Coefficients: -170.0498 2.566231 -13.61017 54.1059
Residual range: -189.4731 201.5056

1000000 - 230866
  769134

> f = fit_greentao(8,2000000)
Number of observations (changes) =  649644
Max nhyp =  812685
Coefficients: 186.8815 3.010198 -22.80165 96.40528
Residual range: -882.6667 537.2372

2000000 - 649644
  1350356

> f = fit_greentao(8,4000000)
TO BE UPDATED:
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

