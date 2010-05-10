// Oliver Kullmann, 26.9.2008 (Swansea)
/* Copyright 2008, 2009, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/GreenTaoProblems/plans/AdditiveNumberTheory.hpp
  \brief On investigations into additive number theory


  \todo Connections
  <ul>
   <li> See RamseyTheory/GreenTaoProblems/plans/CountingProgressions.hpp. </li>
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

