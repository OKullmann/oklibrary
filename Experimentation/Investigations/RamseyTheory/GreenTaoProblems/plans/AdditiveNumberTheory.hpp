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
   <li> Of interest is also
   http://www.research.att.com/~njas/sequences/Sindx_Pri.html#primes_AP
   which gives an overview. It contains for given k the smallest starting
   term, smallest difference and smallest end term for arithmetical
   progressions of primes of length k. However I do not understand
   these sequences (strange explanations). </li>
  </ul>


  \todo Finding the first arithmetic progression
  <ul>
   <li> Fundamental is to consider k -> how many first primes are needed to
   get an progression of length k; this is greentao_1(k). </li>
   <li> See http://users.cybercity.dk/~dsl522332/math/aprecords.htm for
   current information around this subject; the sequence is available as
   follows (showing p_i instead of i, that is, the (unranked) prime numbers
   themselves):
   \verbatim
greentaod1ur;
 [2,3,7,23,29,157,907,1669,1879,2089,
 249037,262897,725663,36850999,173471351,198793279,4827507229,17010526363,83547839407,572945039351,
 6269243827111]
   \endverbatim
   </li>
   <li> The ranked data is available via:
   \verbatim
block([L:[]],for k:1 thru inf unless not integerp(greentao([k])) do L : endcons(greentao([k]),L), L);
 [1,2,4,9,10,37,155,263,289,316,
  21966,23060,58464,2253121,9686320,11015837,227225515,755752809,3466256932,22009064470,
  220525414079]
   \endverbatim
   </li>
   <li> Additional data (upper bounds):
    <ol>
     <li> At http://users.cybercity.dk/~dsl522332/math/aprecords.htm data
     upper bounds are available also for 22 <= k <= 26 (again, this is the
     unranked data):
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
     <li> See "Better algorithms" in
     Structures/NumberTheory/PrimeNumbers/plans/RankPrimes.hpp for thoughts on
     a better algorithm, and on the ranked data. </li>
    </ol>
   </li>
   <li> Plotting the (ranked, precise) data (using R) suggests that
   (log,log(log))-transformation for (x,y) might be appropriate (that is, a
   model y = exp(a * x^b)):
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
lines(x,exp(predict(m0)),col="blue")

y/predict(m)
 [1]  2.0522259  2.8563990  1.7419151  3.1481344  5.7470262  3.7987529
 [7]  1.4561912  0.4981426  9.7285376  2.5797764  1.4869497 11.7370109
[13]  9.3188835  1.7663568  5.4843977  2.4815698  1.4003467  0.9899872
[19]  1.0000489
   \endverbatim
   Explicitly: the model is greentao_1(k) ~ exp(0.084184*k^1.884508). </li>
  </ul>


  \todo The conjecture from [Granville 2006]
  <ul>
   <li> The conjecture is implemented by approxgv_grt1ur(k), and it yields
   quite good approximations:
   \verbatim
for k : 1 thru 21 do print(k, float(greentaod1ur[k]/approxgv_grt1ur(k)));

1 2.289488896354126
2 1.965659777448311
3 2.020885909330544
4 2.468550619322147
5 1.019801665249097
6 1.635675606605423
7 2.574684204425131
8 1.201613382006138
9 0.322264451025868
10 0.08072794540477299
11 2.062519736581183
12 0.445862252087117
13 0.2417209471089714
14 2.319918131851896
15 1.991459630418673
16 0.4025117578670004
17 1.670907399187739
18 0.9772800505078132
19 0.7748870219193731
20 0.8355720914865566
21 1.402142105791055
   \endverbatim
   </li>
   <li> For ranked data:
   \verbatim
for k : 3 thru 21 do print(k, round_fdd(greentaod1(k)/approxgv_grt1Li_hp(k,30), 3));

3 2.641
4 1.867
5 0.868
6 1.312
7 2.031
8 1.136
9 0.371
10 0.11
11 1.924
12 0.477
13 0.269
14 2.199
15 1.914
16 0.423
17 1.63
18 0.978
19 0.783
20 0.841
21 1.385
   \endverbatim
   </li>
   <li> What is the corresponding *direct* approximation for the ranked
   numbers? </li>
   <li> This as model for a linear regression of the ranked data, using
   rank(p) ~ p/log(p) resp. p/(log(p)-1) (optimising on the factor which in
   the gv-model is chosen as exp(1-gamma), or using that factor while
   optimising on an optional outer factor):
   \verbatim
y = c(4,9,10,37,155,263,289,316,21966,23060,58464,2253121,9686320,11015837,227225515,755752809,3466256932,22009064470,220525414079)
x = 3:21

s = c(a = 1.5262)
mgvr = nls(y ~ (x/2*a)^(x/2) / (x/2*(log(x/2)+log(a))), start = s)
> summary(mgvr)
Parameters:
  Estimate Std. Error t value Pr(>|t|)
a  1.58024    0.00225   702.4   <2e-16 ***

y/predict(mgvr)
 [1] 1.4189310 2.0736544 1.1071981 1.6212498 2.3319017 1.2150958
 [7] 0.3741266 0.1060549 1.7867252 0.4283643 0.2351793 1.8705521
[13] 1.5884116 0.3427037 1.2919336 0.7584994 0.5944557 0.6255983
[19] 1.0094899

s = c(a = 1.5262)
mgvr2 = nls(y ~ (x/2*a)^(x/2) / (x/2*(log(x/2)+log(a))-1), start = s)
summary(mgvr2)
Parameters:
  Estimate Std. Error t value Pr(>|t|)
a  1.57485    0.00227   693.8   <2e-16 ***

y/predict(mgvr2)
 [1] 0.31887457 1.17447699 0.78879713 1.28351643 1.96101952 1.06252655
 [7] 0.33622554 0.09726638 1.66467365 0.40417970 0.22422941 1.79925882
[13] 1.53953065 0.33437827 1.26804070 0.74845611 0.58944254 0.62310082
[19] 1.00963502

s = c(b = 1)
mgvr3 = nls(y ~ b*(x/2*1.5262051)^(x/2) / (x/2*(log(x/2)+log(1.5262051))-1), start = s)
summary(mgvr3)
Parameters:
  Estimate Std. Error t value Pr(>|t|)
b  1.37258    0.02089   65.72   <2e-16 ***

y/predict(mgvr3)
 [1] 0.20391699 0.86692693 0.60148029 1.00097550 1.55934574 0.86031118
 [7] 0.27700774 0.08150497 1.41837262 0.35010064 0.19742792 1.61013672
[13] 1.40015594 0.30904249 1.19092347 0.71428235 0.57158839 0.61394061
[19] 1.01076035
   \endverbatim
   Not too bad for the larger k-values; looks better than the above
   y ~ exp(a * x^b) model. Perhaps mgvr3 is most sensible. </li>
   <li> But, as asked in the previous point, one should see to adapt the
   Greenville-approach directly to the ranked case. </li>
   <li> Fitting the unranked data:
   \verbatim
y = c(7,23,29,157,907,1669,1879,2089,249037,262897,725663,36850999,173471351,198793279,4827507229,17010526363,83547839407,572945039351,6269243827111)
x = 3:21

s = c(a = 1.5262)
mgvu = nls(y ~ (x/2*a)^(x/2), start = s)
> summary(mgvu)
Parameters:
  Estimate Std. Error t value Pr(>|t|)
a 1.574839   0.002108     747   <2e-16 ***

y/predict(mgvu)
 [1] 1.92799913 2.31843766 0.94288222 1.48876878 2.30697236 1.05991619
 [7] 0.27983853 0.06900926 1.73568105 0.36936935 0.19713457 1.86255671
[13] 1.57397090 0.31317862 1.27983584 0.73690120 0.57519747 0.61059163
[19] 1.00866581
   \endverbatim
   </li>
   <li> None of these fitting-attempts seems to reveal much; so the original
   model is to be preferred. </li>
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

