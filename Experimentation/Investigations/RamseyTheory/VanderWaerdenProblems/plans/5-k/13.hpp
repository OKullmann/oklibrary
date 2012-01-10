// Oliver Kullmann, 23.3.2011 (Swansea)
/* Copyright 2011, 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/5-k/13.hpp
  \brief On investigations into vdw_2(5,13)


  \todo vanderwaerden_2(5,13) > 1204
  <ul>
   <li> Find best ubcsat-solver. </li>
   <li> Start with the palindromic solution for n=1204:
   \verbatim
C : create_certificate2c_pdvdw([
2,4,7,10,11,14,19,23,24,26,
33,41,47,52,59,60,62,64,75,77,
78,81,83,84,93,94,96,97,104,105,
108,109,113,114,119,120,127,128,133,137,
138,140,141,144,147,156,158,165,170,172,
175,176,181,185,189,191,195,198,199,202,
211,218,222,226,228,231,233,234,239,241,
243,244,245,246,251,256,262,267,272,280,
283,284,286,292,293,294,302,305,307,309,
320,322,324,328,331,332,335,347,348,349,
352,356,357,360,362,363,368,369,376,377,
378,379,390,396,400,402,403,405,409,415,
416,420,422,423,428,432,433,434,437,441,
443,449,451,452,455,458,462,469,470,473,
476,477,478,482,486,489,493,496,504,507,
509,510,517,522,530,531,532,535,543,545,
549,550,551,554,555,558,565,568,569,571,
574,579,586,594,595,597], 1204);
certificate_pdvdw_p([5,13],1204,C);
  true
output_certificate2_vdw(reverse(C), "VanDerWaerden_2-5-13_1204.cnf_sol");

RunVdWk1k2 5 13 1204 adaptg2wsat 200 200000000 VanDerWaerden_2-5-13_1204.cnf_sol
UNSAT for n=1205
> E=read_ubcsat("VanDerWaerden_2-5-13_1205.cnf_OUT",nrows=200)
 3  4  5  6  7  8
 2 13 65 73 45  2
200
   \endverbatim
   </li>
  </ul>


  \todo Best local-search solver for palindromic problems
  <ul>
   <li>
   \verbatim
E=run_ubcsat("VanDerWaerden_pd_2-5-13_1204.cnf",runs=100,cutoff=20000000)
# considering only the first 10 algorithm (aborted after that):
hwsat
 4  6  7  8  9 10 11 12 13 14 15 17 18
 2  2  3  7  7 15 16 14 11 15  4  2  2
FlipsPerSecond = 113746
gwsat
 5  9 10 11 12 13 14 15 16
 1  2  2  1 17 25 30 17  5
FlipsPerSecond = 84911
# continuing:
E=run_ubcsat("VanDerWaerden_pd_2-5-13_1204.cnf",runs=100,cutoff=20000000,include_algs=names(tail(run_ubcsat_cnf_algs,-10)))
rnovp
 1  2  3  4  5  6
 1 11 35 37 13  3
anovp
 1  2  3  4  5
 1  4 15 50 30
rnov
 2  3  4  5  6
 6 29 52 10  3
# aborted; next novpc
E=run_ubcsat("VanDerWaerden_pd_2-5-13_1204.cnf",runs=100,cutoff=20000000,include_algs=names(tail(run_ubcsat_cnf_algs,-20)))
ddfw
 2  3  4  5  6
 8 21 40 20  1
anovpp
 2  3  4  5  6
 4 19 56 20  1
# continuing with g2wsat
E=run_ubcsat("VanDerWaerden_pd_2-5-13_1204.cnf",runs=100,cutoff=20000000,include_algs=names(tail(run_ubcsat_cnf_algs,-26)))
1. ag2wsat:
 2  3  4  5
 9 41 40 10
fps: 78069
2. dano:
 2  3  4  5  6
 6 20 53 20  1
fps: 39174
   \endverbatim
   So the ranking is: rnovp, anovp, ag2wsat, ddfw, rnov, dano. </li>
  </ul>


  \todo vdw_2^pd(5,13) >= (1176,1205)
  <ul>
   <li> Certificates:
    <ol>
     <li> n=1175:
     \verbatim
13,14,16,17,18,20,27,30,33,37,
43,45,49,56,58,65,75,76,80,82,
86,87,89,91,92,99,101,102,110,118,
119,120,122,123,125,127,129,135,144,145,
146,149,152,156,157,158,160,161,162,171,
174,182,187,188,189,196,198,200,208,209,
210,214,216,222,226,227,232,237,238,239,
247,248,253,260,262,263,265,267,268,270,
271,277,279,280,286,288,290,295,298,306,
317,324,335,337,338,339,341,345,351,352,
354,355,357,359,361,367,369,370,377,379,
382,389,393,395,401,403,404,406,407,410,
414,415,419,422,424,427,434,436,438,443,
453,454,456,457,460,464,465,472,474,476,
479,484,486,489,492,498,501,504,507,508,
512,525,528,531,533,534,535,536,539,543,
545,546,548,552,553,564,565,567,568,572,
585,586
     \endverbatim
     or
     \verbatim
1,3,8,10,12,13,20,22,31,36,
42,44,46,48,53,55,60,61,65,69,
70,76,80,81,82,85,89,90,98,103,
107,108,111,113,119,120,121,127,128,136,
137,139,149,150,152,163,164,167,168,171,
179,185,187,191,194,196,198,199,203,204,
211,218,220,228,230,231,234,236,237,238,
243,249,259,264,266,270,275,276,277,278,
282,285,286,295,298,302,304,308,311,312,
315,317,319,321,322,324,328,330,335,336,
340,343,346,348,357,365,367,370,371,373,
374,375,377,383,387,395,400,403,405,413,
414,417,424,425,431,433,437,439,442,452,
453,456,460,462,464,466,467,472,474,477,
478,485,493,495,497,500,502,506,509,510,
512,513,514,516,521,522,527,528,538,540,
542,553,560,568,571,575,579,581,582,583,
585
     \endverbatim
     </li>
     <li> n=1204:
     \verbatim
2,4,7,10,11,14,19,23,24,26,
33,41,47,52,59,60,62,64,75,77,
78,81,83,84,93,94,96,97,104,105,
108,109,113,114,119,120,127,128,133,137,
138,140,141,144,147,156,158,165,170,172,
175,176,181,185,189,191,195,198,199,202,
211,218,222,226,228,231,233,234,239,241,
243,244,245,246,251,256,262,267,272,280,
283,284,286,292,293,294,302,305,307,309,
320,322,324,328,331,332,335,347,348,349,
352,356,357,360,362,363,368,369,376,377,
378,379,390,396,400,402,403,405,409,415,
416,420,422,423,428,432,433,434,437,441,
443,449,451,452,455,458,462,469,470,473,
476,477,478,482,486,489,493,496,504,507,
509,510,517,522,530,531,532,535,543,545,
549,550,551,554,555,558,565,568,569,571,
574,579,586,594,595,597
     \endverbatim
    </ol>
   </li>
   <li> "RunPdVdWk1k2 5 13 adaptg2wsat 100 80000000" yields
   \verbatim
Break point 1: 1176
Break point 2: 1187
   \endverbatim
   using at most 14 runs.
   </li>
   <li> "RunPdVdWk1k2 5 13 adaptg2wsat 100 160000000" yields
   \verbatim
Break point 1: 1154
   \endverbatim
    before being aborted, using at most 79 runs.
   </li>
    <li> "RunPdVdWk1k2 5 13 adaptg2wsat 200 160000000" yields
    \verbatim
Break point 1: 1176
Break point 2: 1205
    \endverbatim
    with all solution found in the first 171 runs, and with
    \verbatim
> E=read_ubcsat("VanDerWaerden_pd_2-5-13_1177.cnf_OUT",nrows=200)
  1   2   3   4
  1  52 127  20
200
> E=read_ubcsat("VanDerWaerden_pd_2-5-13_1206.cnf_OUT",nrows=200)
  1   2   3   4
  7  88 104   1
200
   \endverbatim
   </li>
   <li> "RunPdVdWk1k2 5 13 rnovelty+ 200 160000000" yields
   \verbatim
> cat Statistics
   1057     2   18121142 3225654453  p
   1083     3   74524416 2466284449  p
   1093     9  117776196 3295983238  r
   1100     2   60067959 3002331435  r
   1108     2   25313249 3290285470  p
   1113     2  102849808  686652270  p
   1115     3   60615631 3446736450  r
   1128     3   39310545 1584923207  r
   1141     1   35717959  871025648  r
   1146     3  137138342 3581168193  p
   1151   122   21602830 3699966708  r
   1158    15   48433189 2897658593  r
   1158    15   48433189 2897658593  r
> E=read_ubcsat("VanDerWaerden_pd_2-5-13_1167.cnf_OUT")
 1  2  3  4
 3 20 16  2
41
# aborted; again:
   1079     2    3298000 3567242502  p
   1092     2    4135958   92697337  p
   1097     7  155815273 3015937386  r
   1106     2  103038469 2081102029  p
   1120     1    3739455 1392575513  r
   1121     3    2263208  425130818  p
   1124     9   23898080 1658053803  r
   1152     1  101599228 1888050702  r
   1154    17  134279811  341060144  r
   1155    34  140348018  953004940  r
> E=read_ubcsat("VanDerWaerden_pd_2-5-13_1163.cnf_OUT")
 1  2  3  4
10 23 22  5
60
# aborted; again:
XXX cs-oksvr
   \endverbatim
   </li>
  </ul>

*/
