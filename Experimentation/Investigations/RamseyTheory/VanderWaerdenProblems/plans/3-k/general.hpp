// Oliver Kullmann, 10.4.2009 (Swansea)
/* Copyright 2009, 2010, 2011, 2013 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/3-k/general.hpp
  \brief On investigations into vdW-numbers vdw_2(3,k)

  Instances created by output_vanderwaerden2nd_stdname(3,k,n) at Maxima-level,
  or by "VanderWaerden-O3-DNDEBUG 3 k n" at C++ level (instances differ by
  the order of negated literals in clauses).
  An ubcsat-experiment is run by "RunVdW3k k n0 alg runs cutoff".

  The palindromic versions are created by
  output_pd_vanderwaerden2nd_stdname(3,k,n) at Maxima-level, or by
  "PdVanderWaerdenCNF-O3-DNDEBUG 3 k n" at C++ level (instances differ by
  the order of negated literals in clauses).
  An ubcsat-experiment is run by "RunPdVdWk1k2 3 k n0 alg runs cutoff".
  And running a complete solver by "CRunPdVdWk1k2 3 k solver".


  \todo Connections
  <ul>
   <li> This is sequence A007783 at OEIS https://oeis.org/A007783 . </li>
  </ul>


  \todo Predictions of vdw_2(3,k)
  <ul>
   <li> A conjecture is vdw_2(3,k) <= k^2 for k >= 3.
    <ol>
     <li> The first counterexample is vdw_2(3,24) > 592. </li>
     <li> It seems that this is not just an outlier, but that growth is
     more than quadratic. </li>
    </ol>
   </li>
   <li> The known values with k=1, ..., 19 are available via
   vanderwaerden3k(k):
   \verbatim
create_list(vanderwaerden3k(k),k,1,19);
 [3,6,9,18,22,32,46,58,77,97,114,135,160,186,218,238,279,312,349]
   \endverbatim
   </li>
   <li> Log-log regression (in R) does not yield a good model, as can be
   seen inspecting the plot:
   \verbatim
d0 = c(3,6,9,18,22,32,46,58,77,97,114,135,160,186,218,238,279,312,349)
plot(d0)
lines(d0)
x0 = log((1:length(d0)))
y0 = log(d0)
plot(x0,y0)
L0 = lm(y0 ~ x0)
summary(L0)

Coefficients:
            Estimate Std. Error t value Pr(>|t|)
(Intercept)  0.58155    0.12319   4.721 0.000197 ***
x0           1.73669    0.05563  31.220  < 2e-16 ***
Residual standard error: 0.1906 on 17 degrees of freedom
Multiple R-squared: 0.9829,     Adjusted R-squared: 0.9818

lines(x0,predict(L0))
   \endverbatim
   </li>
   <li> Using the conjectured values 389,416,464,516,593,656,727,770,827,
   868 (see below):
   \verbatim
d = append(d0, c(389,416,464,516,593,656,727,770,827,868))
plot(d)
lines(d)
lines((1:length(d))^2)
x = log((1:length(d)))
y = log(d)
L = lm(y ~ x)
plot(x,y)
lines(x,predict(L))
   \endverbatim
   Again this doesn't seem to be a good model (larger k-values are not taken
   enough into account).
   </li>
   <li> Non-linear regression yields a better model:
   \verbatim
X = (1:length(d))
Y = d
plot(X,Y)
NL = nls(Y ~ a*(X^b), start = c(a = 0.8, b = 2))
lines(predict(NL))
summary(NL)

Parameters:
  Estimate Std. Error t value Pr(>|t|)
a  0.58471    0.05241   11.16 1.29e-11 ***
b  2.17488    0.02799   77.70  < 2e-16 ***
Residual standard error: 11.28 on 27 degrees of freedom

Cnl = coefficients(NL)
Cnl
        a         b
0.5847057 2.1748818
fnl = function(k){Cnl[1] * k^Cnl[2]}
round(fnl(1:35))
 1 3 6 12 19 29 40 54 70 87
 108 130 155 182 211 243 277 314 353 395
 439 486 535 587 642 699 759 821 886 954
 1024 1098 1174 1252 1334

d - round(fnl(1:29))
 2 3 3 6 3 3 6 4 7 10
 6 5 5 4 7 -5 2 -2 -4 -6
 -23 -22 -19 6 14 28 11 6 -18
   \endverbatim
   </li>
   <li> Finally using a quadratic model with non-linear regression and
   all data:
   \verbatim
NLq = nls(d ~ a + b*X + c * X^2, start = c(a=0, b = 0, c = 1))
lines(predict(NLq))
summary(NLq)

Parameters:
  Estimate Std. Error t value Pr(>|t|)
a 17.38095    7.21230   2.410   0.0233 *
b -5.09519    1.10817  -4.598 9.72e-05 ***
c  1.20367    0.03584  33.580  < 2e-16 ***
Residual standard error: 12.06 on 26 degrees of freedom

Cnlq = coefficients(NLq)
Cnlq
        a         b         c
17.380952 -5.095185  1.203665
fnlq = function(k){Cnlq[1] + Cnlq[2]*k +Cnlq[3] * k^2}
round(fnlq(1:35))
 13 12 13 16 22 30 41 54 69 87
 107 130 155 182 212 244 279 316 355 397
 441 488 537 588 642 699 757 818 882 948
 1016 1087 1160 1236 1314

d - round(fnlq(X))
 -10 -6 -4 2 0 2 5 4 8 10
 7 5 5 4 6 -6 0 -4 -6 -8
 -25 -24 -21 5 14 28 13 9 -14
   \endverbatim
   Hard to distinguish from the above model. </li>
   <li> Refined models:
    <ol>
     <li> It seems that vdw_2(3,30) is quite below the predicted value. </li>
     <li> Looking at the plots, there seems to be an oscillation around some
     core curve, and with k=29 it seems a down-phase (with negative
     differences in the above "d - predicted" tables) is entered. </li>
     <li> How to model that? </li>
    </ol>
   </li>
  </ul>


  \todo Analysing certificates
  <ul>
   <li> See "Analysing certificates" in
   ComputerAlgebra/RamseyTheory/Lisp/VanderWaerden/plans/Certificates.hpp.
   </li>
   <li> It seems that solutions at least for the harder problems come in
   small clusters, where the clusters have large Hamming distances, while
   within the clusters we have rather small Hamming distances. </li>
  </ul>


  \todo SAT 2011 competition
  <ul>
   <li> See
   http://www.cril.univ-artois.fr/SAT11/results/globalbybench.php?idev=46&idcat=63
   </li>
   <li> clasp 2.0-R4092-crafted performed best on ordinary vdW-instances.
   </li>
   <li> Also on palindromic instances performance was sometimes best, but also
   MPhaseSAT 2011-02-15, claspmt 2.0-R4095-patched, Coach-SAT-Portfolio
   2011-03-18, ppfolio par, clasp 1.2.0-SAT09-32, sattime+ 2011-32-02,
   sathys 2011-04-01, and glucose 2 need to be considered. </li>
  </ul>


  \todo Local search for the satisfiable instances
  <ul>
   <li> The first impression is that ubcsat::samd is best. </li>
   <li> For k=16, n=237 a local minimum of 1 is easily found (even with just
   100000 steps), while to find a solution one should use, say, 10^7 steps
   (and then solutions seem to be found often, e.g., seed=370833091). </li>
   <li> But with vanderwaerden_2(3,17) we see that adaptnovelty+
   is better; perhaps, as with vdw_2(4,k), getting stronger with higher
   cutoff. </li>
   <li> However, for vanderwaerden_2(3,18) samd again seems clearly better,
   while for 19 <= k <= 23 gsat-tabu seems best, and for k >= 24 rots
   seems best --- this needs to be investigated more thoroughly. </li>
  </ul>

*/
