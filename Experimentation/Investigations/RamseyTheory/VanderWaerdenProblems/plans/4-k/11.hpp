// Oliver Kullmann, 6.3.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/4-k/11.hpp
  \brief On investigations into vdw_2(4,11)


  \todo Best ubcsat-solver
  <ul>
   <li> We need to find the best algorithm from the ubcsat-1-2-0 suite. </li>
   <li> For vdw_2(4,12) it seems adaptg2wsat is best, so we take this as the
   starting point (see
   Investigations/RamseyTheory/VanderWaerdenProblems/plans/4-k/12.hpp). </li>
   <li>
   \verbatim
E=run_ubcsat("VanDerWaerden_2-4-11_358.cnf",runs=100,cutoff=10000000)
> eval_ubcsat_dataframe(E,FALSE)
1. ag2wsat:
 1  2 10 11 12 13 14 15
 2  3  6 14 22 38 14  1
fps: 192249
2. ddfw:
 1  2  3 10 11 12 13 14
 1  1  1  7 33 43 13  1
fps: 47823
3. anovp:
 1  8  9 10 11 12 13
 1  1  1  6 35 43 13
fps: 149054
4. novp_p:
 1 21 22 24 25 26 27 28 29 30 31 32 33
 1  1  1  1  1  5  4 11 27 22 19  5  2
fps: 104163
5. dano:
 2  3  4  9 10 11 12 13 14
 2  1  1  3  6 29 39 17  2
fps: 154413
6. anovpp:
 2  3  9 10 11 12 13
 2  1  1  6 34 39 17
fps: 151746
7. rnov:
 2  3 10 11 12 13 14 15 16
 1  1  2 10 16 38 23  7  2
fps: 148764
8. paws:
 2 14 15 16 17 18 19 20
 1  1  1  5 11 27 47  7
fps: 174008
   \endverbatim
   seems to confirm that adaptg2wsat is best here. </li>
  </ul>


  \todo vanderwaerden_2(4,11) > 358
  <ul>
   <li> Certificate for n=358:
   \verbatim
7,15,20,23,32,34,38,41,45,46,
55,59,61,66,69,70,72,76,78,80,
81,87,90,94,103,104,113,117,120,126,
127,129,131,135,137,138,141,143,146,148,
152,156,158,161,162,163,167,169,173,175,
177,178,184,187,191,200,201,210,214,223,
224,226,228,232,234,235,238,240,243,245,
253,259,260,263,264,266,270,272,274,275,
281,284,288,297,307,310,314,320,321,331,
333,335,336,342,352,355,356
   \endverbatim
   </li>
   <li> Start with the palindromic solution for n=347:
   \verbatim
C : create_certificate2c_pdvdw([
8,13,14,20,30,31,32,35,43,47,
50,55,57,58,59,69,76,77,78,85,
89,95,96,97,99,104,105,107,110,119,
122,123,124,127,128,129,132,140,141,144,
146,147,152,154,155,156,162,166,173], 347);
certificate_pdvdw_p([4,11],347,C);
  true
output_certificate2_vdw(reverse(C), "VanDerWaerden_2-4-11_347.cnf_sol");

RunVdWk1k2 4 11 347 adaptg2wsat 200 20000000 VanDerWaerden_2-4-11_347.cnf_sol
UNSAT for n=359
> E=read_ubcsat("VanDerWaerden_2-4-11_359.cnf_OUT",nrows=200)
 1  2  3  4 10 11 12 13 14
 2  9  5  1  7 29 60 72 15
200
 1  2  3  4  9 10 11 12 13 14
 7  8  3  3  1  5 27 71 69  6
200
   \endverbatim
   </li>
   <li> Starting from scratch (cstriton):
   \verbatim
> k1=4 k2=11 n0=12 alg="adaptg2wsat" runs=100 cutoff=100000000; nohup RunVdWk1k2 ${k1} ${k2} ${n0} "${alg}" ${runs} ${cutoff} &
   \endverbatim
   n=358 found satisfiable (a solution was found in 37 runs), with certificate
   \verbatim
7,15,20,23,32,34,38,41,45,46,
55,59,61,66,69,70,72,76,78,80,
81,87,90,94,103,104,113,117,120,126,
127,129,131,135,137,138,141,143,146,148,
156,158,161,162,163,167,169,173,175,177,
178,184,187,191,200,201,210,214,223,224,
226,228,232,234,235,238,239,240,243,245,
249,253,259,260,263,264,266,270,272,274,
281,284,288,297,307,310,314,320,321,331,
333,335,336,342,352,355,356
   \endverbatim
   For n=359
   \verbatim
 1  2  3  4 10 11 12 13
13 20  5  3  6 29 23  1
100
   \endverbatim
   </li>
   <li> n=359:
    <ol>
     <li> Starting with the solution for n=358:
     \verbatim
> k1=4 k2=11 n0=358 alg="adaptg2wsat" runs=400 cutoff=400000000 oldsol="VanDerWaerden_2-4-11_358.cnf_sol"; nohup RunVdWk1k2 ${k1} ${k2} ${n0} "${alg}" ${runs} ${cutoff} ${oldsol} &
  1   2   3   4   8   9  10  11
134 148  38  16   2   6  31  25
400
     \endverbatim
     </li>
    </ol>
   </li>
  </ul>


  \todo vdw_2^pd(4,11) = (343,348)
  <ul>
   <li> Certificates:
    <ol>
     <li> n=342:
     \verbatim
11,12,17,22,32,35,36,37,40,41,
43,47,49,51,52,58,61,65,74,75,
84,88,97,98,100,102,106,108,109,112,
113,114,117,119,127,129,133,134,140,144,
146,148,149,155,158,162,171
     \endverbatim
     </li>
     <li> n=347:
     \verbatim
8,13,14,20,30,31,32,35,43,47,
50,55,57,58,59,69,76,77,78,85,
89,95,96,97,99,104,105,107,110,119,
122,123,124,127,128,129,132,140,141,144,
146,147,152,154,155,156,162,166,173
     \endverbatim
     </li>
    </ol>
   </li>
   <li> Established by "CRunPdVdWk1k2 4 11 minisat-2.2.0". </li>
   <li> Evaluation:
   \verbatim
> E=read_crunpdvdw_minisat()
$t:
     Min.   1st Qu.    Median      Mean   3rd Qu.      Max.
0.000e+00 1.999e-03 7.998e-03 1.994e+02 9.798e-02 4.997e+04
sd= 2740.204
       95%        96%        97%        98%        99%       100%
  133.8438   204.2265   430.8473   599.1797  1389.0116 49973.5000
sum= 67213.92
$cfs:
     Min.   1st Qu.    Median      Mean   3rd Qu.      Max.
        0         0         3   2989000      4071 660500000
sd= 36422253
      95%       96%       97%       98%       99%      100%
  3210628   4874701   9388303  12944174  27372790 660465016
sum= 1007135497
$t ~ $cfs:
               Estimate  Std. Error  t value  Pr(>|t|)
(Intercept) -2.5174e+01  6.6100e+00  -3.8085 0.0001663 ***
E$cfs        7.5161e-05  1.8114e-07 414.9318 < 2.2e-16 ***
R-squared: 0.9981
   \endverbatim
   The log-plot shows that even problems (i.e., even v) are harder for v > 300,
   where the hardest problems are, while for 200 < v < 300 (problems with
   intermediate hardness) odd problems seem harder. </li>
   <li> "RunPdVdWk1k2 4 11 adaptg2wsat 100 1000000" determines satisfiability
   very easily. </li>
  </ul>

*/
