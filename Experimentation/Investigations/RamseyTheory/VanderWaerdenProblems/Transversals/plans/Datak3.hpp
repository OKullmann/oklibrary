// Oliver Kullmann, 18.3.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/Transversals/plans/Datak3.hpp
  \brief Investigating the transversal hypergraph of van-der-Waerden hypergraphs for k = 3 (length of arithmetic progressions)


  \todo Elementary statistics
  <ul>
   <li> Just using clause-set statistics-functions:
   \verbatim
G3 : lambda([n], arithprog_hg(3,n))$
oklib_monitor:true;
L_3_20 : transversals_mongen_rs(20,G3)$
for i : 0 thru 20 do print(i, statistics_fcs(G3(i)), rest(extended_statistics_fcs(cs2fcs(L_3_20[i+1])),-2));
   \endverbatim
   we obtain
   \verbatim
0 [0,0,0,-1,inf] [0,1,0,0,0,[[0,1]],inf,minf]
1 [1,0,0,-1,inf] [0,1,0,0,0,[[0,1]],inf,minf]
2 [2,0,0,-1,inf] [0,1,0,0,0,[[0,1]],inf,minf]
3 [3,1,3,3,3] [3,3,3,1,1,[[1,3]],1,1]
4 [4,2,6,3,3] [4,3,4,1,2,[[1,2],[2,1]],1,1]
5 [5,4,12,3,3] [5,3,5,1,2,[[1,1],[2,2]],1,1]
6 [6,6,18,3,3] [6,4,8,2,2,[[2,4]],1,2]
7 [7,9,27,3,3] [7,10,30,3,3,[[3,10]],3,6]
8 [8,12,36,3,3] [8,25,100,4,4,[[4,25]],10,15]
9 [9,16,48,3,3] [9,38,186,4,5,[[4,4],[5,34]],19,22]
10 [10,20,60,3,3] [10,48,264,5,6,[[5,24],[6,24]],24,28]
11 [11,25,75,3,3] [11,58,362,5,7,[[5,7],[6,30],[7,21]],30,35]
12 [12,30,90,3,3] [12,73,498,6,8,[[6,25],[7,36],[8,12]],37,44]
13 [13,36,108,3,3] [13,86,642,6,9,[[6,6],[7,41],[8,32],[9,7]],44,52]
14 [14,42,126,3,3] [14,109,878,6,10,[[6,1],[7,14],[8,76],[9,14],[10,4]],52,70]
15 [15,49,147,3,3] [15,146,1284,7,11,[[7,4],[8,32],[9,102],[10,6],[11,2]],73,95]
16 [16,56,168,3,3] [16,210,2018,8,12,[[8,14],[9,60],[10,131],[11,4],[12,1]],110,141]
17 [17,64,192,3,3] [17,285,2959,9,12,[[9,43],[10,92],[11,148],[12,2]],154,192]
18 [18,72,216,3,3] [18,411,4578,10,12,[[10,97],[11,160],[12,154]],209,286]
19 [19,81,243,3,3] [19,614,7294,11,13,[[11,220],[12,248],[13,146]],326,438]
20 [20,90,270,3,3] [20,957,12090,11,14,[[11,2],[12,451],[13,400],[14,104]],502,700]
   \endverbatim
   </li>
   <li> So the sequence tau_arithprog_hg(3,n) (starting with n=0) begins with
   0,0,0,1,1,1,2,3,4,4,5,5,6,6,6,7,8,9,10,11,11. </li>
   <li> It follows that the sequence vanderwaerden_{k+1}([2]_k, 3) (starting
   with k=0) begins with
   3,6,7,8,10,12,15,16,17,18,19. </li>
   <li> The sequence n - tau_arithprog_hg(3,n) = alpha_arithprog_hg(3,n) =
   r_3(n):
   0,1,2,2,3,4,4,4,4,5,5,6,6,7,8,8,8,8,8,8,9. </li>
   <li> Excluding the first term, this is sequence A003002;
   see [S. S. Wagstaff, Jr., On k-free sequences of integers, Math. Comp., 26
   (1972), 767-771]:
   \verbatim
1, 2, 2, 3, 4, 4, 4, 4, 5, 5, 
6, 6, 7, 8, 8, 8, 8, 8, 8, 9, 
9, 9, 9, 10, 10, 11, 11, 11, 11, 12,
12, 13, 13, 13, 13, 14, 14, 14, 14, 15,
16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
17, 17, 17
   \endverbatim
   (covering the range 1 <= n <= 53, see below for extended data).
   </li>
   <li> So the sequence tau_arithprog_hg(3,n) (starting with n=0) begins with
   \verbatim
0,
0,0,1,1,1,2,3,4,4,5,
5,6,6,6,7,8,9,10,11,11,
12,13,14,14,15,15,16,17,18,18,
19,19,20,21,22,22,23,24,25,25,
25,26,27,28,29,30,31,32,33,34,
34,35,36
   \endverbatim
   </li>
   <li> It follows that the sequence vanderwaerden_{k+1}([2]_k, 3) (starting
   with k=0) begins with
   \verbatim
3,
6,7,8,10,12,15,16,17,18,19,
21,22,23,25,27,28,29,31,33,34,
35,37,38,39,42,43,44,45,46,47,
48,49,50,52,53
   \endverbatim
   (covering the range 0 <= k <= 35. </li>
   <li> Another sequence of interest is the number of hyperedges:
   1,1,1,3,3,3,4,10,25,38,48,58,73,86,109,146,210,285,411,614,957. </li>
   <li> Doesn't seem to be in the eis-database (also after excluding the first
   terms). </li>
   <li> Perhaps also the maximum size of a hyperedge is of interest:
   0,0,0,1,2,2,2,3,4,5,6,7,8,9,10,11,12,12,12,13,14. </li>
   <li> Doesn't seem to be in the eis-database (also after excluding the first
   terms). </li>
   <li> Most natural (and relatively easiest to compute) amongst additional
   data on the transversal hypergraph seems to be the number of minimum
   hyperedges:
   \verbatim
A3(n):=arithprog_hg(3,n)$
L60_3 : minimum_transversals_mongen(60,A3,[{}])$
1 0 1
2 0 1
3 1 3
4 1 2
5 1 1
6 2 4
7 3 10
8 4 25
9 4 4
10 5 24
11 5 7
12 6 25
13 6 6
14 6 1
15 7 4
16 8 14
17 9 43
18 10 97
19 11 220
20 11 2
21 12 18
22 13 62
23 14 232
24 14 2
25 15 33
26 15 2
27 16 12
28 17 36
29 18 106
30 18 1
31 19 11
32 19 2
33 20 4
34 21 14
35 22 40
36 22 2
37 23 4
38 24 86
39 25 307
40 25 20
41 25 1
42 26 4
43 27 14
44 28 41
45 29 99
46 30 266
47 31 674
48 32 1505
49 33 3510
50 34 7726
51 34 14
52 35 50
53 36 156
54 36 2
55 37 8
56 38 26
57 39 56
58 39 2
59 40 4
   \endverbatim
   </li>
   <li> With the simple C++ implementation this can be extended further:
   \verbatim
> MinimumTransversals_VanderWaerden-O3-DNDEBUG 100 3
...
60 41 6
61 42 14
62 43 48
63 43 2
64 44 4
65 45 8
66 46 16
67 47 28
68 48 108
69 49 319
70 50 1046
71 50 4
   \endverbatim
   (though an exponential running-time explosions happens here, so we don't
   get that much further). </li>
  </ul>


  \todo Only computing the transversal numbers
  <ul>
   <li> Just computing the transversal numbers, using minisat and the
   direct translation (as provided by "VdWTransversalsInc 3 1 0 OutputFile";
   see above):
   \verbatim
60 41
61 42
62 43
63 43
64 44
65 45
66 46
67 47
68 48
69 49
70 50
71 50
72 51
73 52
74 52
75 53
76 54
77 55
78 56
79 57
80 58
81 59
82 59
83 60
84 60
85 61
86 62
87 63
88 64
89 65
90 66
91 67
92 67
93 68
94 69
95 69
96 70
97 71
98 72
99 73
100 73
101 74
102 75
   \endverbatim
   </li>
   <li> Actually, the values of tau_arithprog_hg(3,n) are known for n <= 194,
   so there must be some more specialised (and more efficient) computations
   possible.
    <ol>
     <li> This is given by sequence A065825, as explained in "Relations to
     vanderwaerden_k(n_1, ..., n_k)" in
     RamseyTheory/VanderWaerdenProblems/Transversals/plans/general.hpp. </li>
     <li> There only the first 36 terms are mentioned, while at
     http://www.math.uni.wroc.pl/~jwr/non-ave.htm one finds 5 more terms;
     the complete list, containing for n > 41 apparently only upper bounds,
     is in Bestk3.txt. </li>
     <li> One should be careful here, since the various computations
     for n <= 41 seem to be "ad-hoc" computations; but the upper bounds
     should be all correct. </li>
     <li> In Methodsk3.html various methods for establishing bounds are
     given (from http://www.math.uni.wroc.pl/~jwr/non-ave.htm), using
     bounds for the similar problem considering arithmetic progressions of
     size 3 in the cyclic groups of order n (the size of the vertex set). </li>
     <li> The main conjecture at http://www.math.uni.wroc.pl/~jwr/non-ave.htm
     is that for a(n) (that sequence A065825) we have a(n) <= n^{3/2}. </li>
    </ol>
   </li>
  </ul>


  \todo Predictions
  <ul>
   <li> The transversal vdW-numbers seem most suitable for predictions:
   \verbatim
initial_sequence_vdWt(3);
[
 3,6,7,8,10,12,15,16,17,18,
 19,21,22,23,25,27,28,29,31,33,
 34,35,37,38,39,42,43,44,45,46,
 47,48,49,50,52,53,55,56,57,59,
 60,61,62,64,65,66,67,68,69,70,
 72,73,75,76,77,78,79,80,81,83,
 85,86,87,88,89,90,91,93,94,96,
 97,98,99,101,102,103,105,106,107,108,
 109,110,112,113,115,116,117,118,119,120,
 123,124,125,126,127,128,129,130,131,132,
 133,134,135,136,138,139,140,141,142,143,
 144,146,147,148,149,151,152,153,154,155,
 156,158,159,160,161,162,164,166,167,168,
 170,171,172,173,175,176,177,178,179,180,
 181,182,183,184,185,186,187,188,189,190,
 191,192,193
]
   \endverbatim
   </li>
   <li> Plotting this data in R looks "number-theoretical" and
   not "fittable". </li>
   <li> So let's consider the relative independency numbers, obtained by
   float(ralphal_arithprog(3)):
   \verbatim
d = c( ... float(ralphal_arithprog(3)) ... )
x = log((1:length(d))[-(1:10)])
y = log(1/d[-(1:10)])
L = lm(y ~ x)
summary(L)
Coefficients:
             Estimate Std. Error t value Pr(>|t|)
(Intercept) -0.184533   0.013322  -13.85   <2e-16 ***
x            0.327640   0.002966  110.46   <2e-16 ***
Adjusted R-squared: 0.9852
plot(x,y)
lines(x,predict(L))
C = coefficients(L)
f = function(n){1/exp(C[1]) * n^(-C[2])}
plot(d)
lines(f(1:length(d)))
   \endverbatim
   So f(n) ~ 1.202656 * n^(-0.3276399). </li>
   <li> By [Rankin, 1961] it is known that
   ralpha_arithprog(3,n) > c^(-log(n)^(1/2))
   for some suitable c > 1 (this goes back to [Behrend, 1946]). </li>
   <li>
   \verbatim
x = log(log((1:length(d))[-(1:10)]))
y = log(-log(d[-(1:10)]))
L = lm(y ~ x)
summary(L)
Coefficients:
            Estimate Std. Error t value Pr(>|t|)
(Intercept) -1.52020    0.01922  -79.11   <2e-16 ***
x            1.17830    0.01293   91.13   <2e-16 ***
Adjusted R-squared: 0.9784
plot(x,y)
lines(x,predict(L))
C = coefficients(L)
f = function(n){exp(- exp(C[1]) * log(n)^C[2])}
plot(d)
lines(f(1:length(d)))
   \endverbatim
   so f(n) ~ exp(-0.2186679 * log(n)^1.178299) is a more suitable model (at
   least "theoretically", while regarding this data it is not
   distinguishable from the above model for n >= 20). </li>
   <li> The inverse fi of f is given by
   \verbatim
fi = function(v){exp( exp(C[1])^(-1/C[2]) * (-log(v))^(1/C[2]))}
ft = function(m){exp( exp(C[1])^(-1/C[2]) * log(m)^(1/C[2]))}
   \endverbatim
   (where f(m) = fi(1/m)). </li>
   <li> But the regression is still "too optimistic", namely
   C[2] > 1, while it really needs to be < 1 (see "The behaviour of m -> 
   vanderwaerdend(m,3)" in
   Investigations/RamseyTheory/VanderWaerdenProblems/plans/VanderWaerden_4-3-3-3-3.hpp).
   </li>
  </ul>


  \todo Local search
  <ul>
   <li> sapsnr seems to find fastest the near-solutions (min=1). </li>
   <li> However finding a solution is hard (tau_arithprog(3,100)=73):
   \verbatim
ubcsat-okl -alg sapsnr -runs 1000 -cutoff 10000 -i vdw_trans_3_100_73.cnf
Clauses = 4158
Variables = 407
TotalLiterals = 13232
FlipsPerSecond = 683527
BestStep_Mean = 3524.913000
Steps_Mean = 10000.000000
Steps_Max = 10000.000000
PercentSuccess = 0.00
BestSolution_Mean = 1.325000
BestSolution_Median = 1.000000
BestSolution_Min = 1.000000
BestSolution_Max = 2.000000

> ubcsat-okl -alg sapsnr -runs 1000 -cutoff 100000 -i vdw_trans_3_100_73.cnf | tee Aus_3_100
Clauses = 4158
Variables = 407
TotalLiterals = 13232
FlipsPerSecond = 715359
BestStep_Mean = 8514.983000
Steps_Mean = 100000.000000
Steps_Max = 100000.000000
PercentSuccess = 0.00
BestSolution_Mean = 1.000000
BestSolution_Median = 1.000000
BestSolution_Min = 1.000000
BestSolution_Max = 1.000000

> ubcsat-okl -alg sapsnr -runs 1000 -cutoff 1000000 -i vdw_trans_3_100_73.cnf | tee Aus_3_100
Clauses = 4158
Variables = 407
TotalLiterals = 13232
FlipsPerSecond = 708441
BestStep_Mean = 8657.000000
Steps_Mean = 1000000.000000
Steps_Max = 1000000.000000
PercentSuccess = 0.00
BestSolution_Mean = 1.000000
BestSolution_Median = 1.000000
BestSolution_Min = 1.000000
BestSolution_Max = 1.000000
   \endverbatim
   The problem should be the very low number of solutions. </li>
   <li> The minisat2-preprocessor helps a bit here, but not much:
   \verbatim
> ubcsat-okl -alg sapsnr -runs 1000 -cutoff 10000 -i vdw_trans_3_100_73-m2pp.cnf | tee Aus_3_100 
Clauses = 3935
Variables = 407
TotalLiterals = 12294
FlipsPerSecond = 545852
BestStep_Mean = 3360.347000
Steps_Mean = 10000.000000
Steps_Max = 10000.000000
PercentSuccess = 0.00
BestSolution_Mean = 1.291000
BestSolution_Median = 1.000000
BestSolution_Min = 1.000000
BestSolution_Max = 2.000000

> ubcsat-okl -alg sapsnr -runs 1000 -cutoff 100000 -i vdw_trans_3_100_73-m2pp.cnf | tee Aus_3_100
Clauses = 3935
Variables = 407
TotalLiterals = 12294
FlipsPerSecond = 663482
BestStep_Mean = 8509.346000
Steps_Mean = 100000.000000
Steps_Max = 100000.000000
PercentSuccess = 0.00
BestSolution_Mean = 1.000000
BestSolution_Median = 1.000000
BestSolution_Min = 1.000000
BestSolution_Max = 1.000000
   \endverbatim
   </li>
   <li> So it least for this problem-formulation local search doesn't seem to
   be successful. </li>
  </ul>


  \todo Detailed investigations of Tr(ap(3,n_0)) for small n_0
  <ul>
   <li> One important aspect here is that if for some fixed n_0 we know
   Tr(ap(3,n_0)) very well, then we know also the CNF FvdW_m(k,n_0) well,
   which can be used for preprocessing (see "Preprocessing" in
   Investigations/RamseyTheory/VanderWaerdenProblems/plans/general.hpp). </li>
  </ul>

*/
