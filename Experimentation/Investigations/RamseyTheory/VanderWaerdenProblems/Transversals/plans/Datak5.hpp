// Oliver Kullmann, 18.3.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/Transversals/plans/Datak5.hpp
  \brief Investigating the transversal hypergraph of van-der-Waerden hypergraphs for k = 5 (length of arithmetic progressions)


  \todo Elementary statistics for k=5
  <ul>
   <li> The sequence r_5(n) (starting with n=1) is A003004:
   \verbatim
1, 2, 3, 4, 4, 5, 6, 7, 8, 8,
9, 10, 11, 12, 12, 13, 14, 15, 16, 16,
16, 16, 16, 17, 18, 18, 19, 20, 21, 21,
22, 22, 23, 24, 24, 25, 26, 27, 28, 28,
29, 30, 31, 32, 32, 32, 32, 32, 33, 33
   \endverbatim
   (covering the range 1 <= n <= 50). </li>
   <li>
   \verbatim
G5 : lambda([n], arithprog_hg(5,n))$
oklib_monitor:true;
L_5_20 : transversals_mongen_rs(20,G5)$
for i : 0 thru 20 do print(i, statistics_fcs(G5(i)), rest(extended_statistics_fcs(cs2fcs(L_5_20[i+1])),-2));
4 [4,0,0,-1,inf] [0,1,0,0,0,[[0,1]],inf,minf]
5 [5,1,5,5,5] [5,5,5,1,1,[[1,5]],1,1]
6 [6,2,10,5,5] [6,5,6,1,2,[[1,4],[2,1]],1,1]
7 [7,3,15,5,5] [7,6,9,1,2,[[1,3],[2,3]],1,2]
8 [8,4,20,5,5] [8,8,14,1,2,[[1,2],[2,6]],1,3]
9 [9,6,30,5,5] [9,10,21,1,3,[[1,1],[2,7],[3,2]],1,3]
10 [10,8,40,5,5] [10,13,30,2,3,[[2,9],[3,4]],2,4]
11 [11,10,50,5,5] [11,20,55,2,4,[[2,6],[3,13],[4,1]],3,11]
12 [12,12,60,5,5] [12,28,88,2,4,[[2,4],[3,16],[4,8]],6,14]
13 [13,15,75,5,5] [13,57,217,2,5,[[2,2],[3,10],[4,42],[5,3]],11,27]
14 [14,18,90,5,5] [14,85,358,2,5,[[2,1],[3,6],[4,52],[5,26]],15,38]
15 [15,21,105,5,5] [15,124,563,3,6,[[3,5],[4,49],[5,68],[6,2]],24,53]
16 [16,24,120,5,5] [16,180,886,3,6,[[3,4],[4,27],[5,128],[6,21]],33,87]
17 [17,28,140,5,5] [17,306,1697,3,7,[[3,3],[4,12],[5,110],[6,177],[7,4]],72,161]
18 [18,32,160,5,5] [18,483,2920,3,8,[[3,2],[4,4],[5,84],[6,274],[7,118],[8,1]],122,228]
19 [19,36,180,5,5] [19,761,4934,3,8,[[3,1],[4,4],[5,33],[6,359],[7,316],[8,48]],177,332]
20 [20,40,200,5,5] [20,1199,8346,4,9,[[4,5],[5,24],[6,208],[7,740],[8,220],[9,2]],295,549]
   \endverbatim
   </li>
   <li> So the sequence tau_arithprog_hg(5,n) (starting with n=0) begins with
   0,0,0,0,0,1,1,1,1,1,2,2,2,2,2,3,3,3,3,3,4. </li>
   <li> It follows that the sequence vanderwaerden_{k+1}([2]_k, 5) (starting
   with k=0) begins with
   5,10,15,20. </li>
   <li> The sequence n - tau_arithprog_hg(5,n) = r_5(n):
   0,1,2,3,4,4,5,6,7,8,8,9,10,11,12,12,13,14,15,16,16. </li>
   <li> The sequence length(arithprog_hg(5,n)[2]) begins with
   1,1,1,1,1,5,5,6,8,10,13,20,28,57,85,124,180,306,483,761,1199. </li>
   <li> And the max-rank sequence begins with
   0,0,0,0,0,1,2,2,2,3,3,4,4,5,5,6,6,7,8,8,9. </li>
   <li> Computing the number of minimum transversals:
   \verbatim
A5(n):=arithprog_hg(5,n)$
L40_5 : minimum_transversals_mongen(40,A5,[{}])$
1 0 1
2 0 1
3 0 1
4 0 1
5 1 5
6 1 4
7 1 3
8 1 2
9 1 1
10 2 9
11 2 6
12 2 4
13 2 2
14 2 1
15 3 5
16 3 4
17 3 3
18 3 2
19 3 1
20 4 5
21 5 24
22 6 112
23 7 619
24 7 40
25 7 2
26 8 270
27 8 70
28 8 12
29 8 2
30 9 26
31 9 5
32 10 745
33 10 266
34 10 81
35 11 534
36 11 236
37 11 115
38 11 48
39 11 16
40 12 80
41 12 32
42 12 12
43 12 4
44 12 1
45 13 5
   \endverbatim
   </li>
   <li> Extended:
   \verbatim
> MinimumTransversals_VanderWaerden-O3-DNDEBUG 100 5
46 14 38
47 15 278
48 16 1835
49 16 2
50 17 74
51 17 18
52 17 2
53 18 4000
54 18 1300
55 19 6668
56 19 3508
57 19 1736
58 19 768
59 19 256
60 20 1287
61 20 512
62 20 192
63 20 64
64 20 16
65 21 80
66 21 32
67 21 12
68 21 4
69 21 1
   \endverbatim
   </li>
   <li> Using "VdWTransversalsInc 5 1 0 OutputFile" we obtain
   \verbatim
k n tau
5 1 0
5 2 0
5 3 0
5 4 0
5 5 1
5 6 1
5 7 1
5 8 1
5 9 1
5 10 2
5 11 2
5 12 2
5 13 2
5 14 2
5 15 3
5 16 3
5 17 3
5 18 3
5 19 3
5 20 4
5 21 5
5 22 6
5 23 7
5 24 7
5 25 7
5 26 8
5 27 8
5 28 8
5 29 8
5 30 9
5 31 9
5 32 10
5 33 10
5 34 10
5 35 11
5 36 11
5 37 11
5 38 11
5 39 11
5 40 12
5 41 12
5 42 12
5 43 12
5 44 12
5 45 13
5 46 14
5 47 15
5 48 16
5 49 16
5 50 17
5 51 17
5 52 17
5 53 18
5 54 18
5 55 19
5 56 19
5 57 19
5 58 19
5 59 19
5 60 20
5 61 20
5 62 20
5 63 20
5 64 20
5 65 21
5 66 21
5 67 21
5 68 21
5 69 21
5 70 22
5 71 23
5 72 24
5 73 25
5 74 26
5 75 27
5 76 27
5 77 27
5 78 27
5 79 27
5 80 28
5 81 28
5 82 28
5 83 28
5 84 28
5 85 29
5 86 29
5 87 29
5 88 29
5 89 29
5 90 30
5 91 30
5 92 30
5 93 30
5 94 30
5 95 31
5 96 32
5 97 33
5 98 34
5 99 35
5 100 36
5 101 37
5 102 38
5 103 39
5 104 40
   \endverbatim
   </li>
  </ul>


  \todo Predictions
  <ul>
   <li> The transversal vdW-numbers:
   \verbatim
create_list(vanderwaerdent(m,5),m,0,38);
[
 5,10,15,20,21,22,23,26,30,32,
 35,40,45,46,47,48,50,53,55,60,
 65,70,71,72,73,74,75,80,85,90,
 95,96,97,98,99,100,101,102,103
]
   \endverbatim
   </li>
   <li> Considering the relative independency numbers, obtained by
   float(ralphal_arithprog(5)):
   \verbatim
d = ...
x = log((1:length(d))[-(1:4)])
y = log(1/d[-(1:3)])
L = lm(y ~ x)
summary(L)
Coefficients:
             Estimate Std. Error t value Pr(>|t|)
(Intercept) -0.038725   0.018833  -2.056   0.0424 *
x            0.104165   0.004889  21.305   <2e-16 ***
plot(x,y)
lines(x,predict(L))
C = coefficients(L)
f = function(n){1/exp(C[1]) * n^(-C[2])}
plot(d)
lines(f(1:length(d)))
   \endverbatim
   So f(n) ~  1.039485 * n^(-0.1041649). </li>
  </ul>

*/
