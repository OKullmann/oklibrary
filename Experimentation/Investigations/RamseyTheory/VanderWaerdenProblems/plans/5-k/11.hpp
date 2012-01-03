// Oliver Kullmann, 14.3.2011 (Swansea)
/* Copyright 2011, 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/5-k/11.hpp
  \brief On investigations into vdw_2(5,11)


  \todo vanderwaerden_2(5,11) > 766
  <ul>
   <li> Certificate for n = 766 (palindromic):
   \verbatim
1,5,6,9,11,12,16,20,29,33,
37,42,43,48,50,51,52,53,55,58,
61,67,74,81,85,86,89,92,93,96,
98,103,108,110,112,119,124,129,132,136,
137,140,142,144,147,149,150,153,154,165,
166,168,169,170,171,175,183,186,193,197,
206,210,211,212,213,215,217,218,220,222,
223,224,229,231,234,236,240,241,242,244,
248,250,251,253,254,255,263,268,277,288,
295,296,298,300,303,304,305,306,308,309,
310,311,313,314,316,319,330,332,333,334,
336,339,347,349,351,358,359,362,363,367,
371,374,380,387,393,396,400,404,405,408,
409,416,418,420,428,431,433,434,435,437,
448,451,453,454,456,457,458,459,461,462,
463,464,467,469,471,472,479,490,499,504,
512,513,514,516,517,519,523,525,526,527,
531,533,536,538,543,544,545,547,549,550,
552,554,555,556,557,561,570,574,581,584,
592,596,597,598,599,601,602,613,614,617,
618,620,623,625,627,630,631,635,638,643,
648,655,657,659,664,669,671,674,675,678,
681,682,686,693,700,706,709,712,714,715,
716,717,719,724,725,730,734,738,747,751,
755,756,758,761,762,766
   \endverbatim
   </li>
   <li> Find best ubcsat-solver. </li>
   <li> Start with the palindromic solution for n=762:
   \verbatim
C : create_certificate2c_pdvdw([
4,12,15,17,20,22,24,27,30,37,
39,42,46,48,53,59,60,64,65,66,
70,72,80,82,83,85,87,90,95,97,
99,101,102,105,108,115,120,122,123,126,
127,131,134,136,138,139,145,148,149,152,
157,162,172,177,178,179,181,182,189,197,
198,200,201,203,207,208,210,211,212,213,
215,220,221,226,228,229,233,244,247,255,
258,265,266,268,271,273,276,286,288,291,
292,293,294,298,299,300,301,304,305,306,
308,309,311,313,315,321,323,328,329,333,
338,341,346,347,350,351,358,359,361,366,
372,380], 762);
certificate_pdvdw_p([5,11],762,C);
  true;
output_certificate2_vdw(reverse(C), "VanDerWaerden_2-5-11_762.cnf_sol");
   \endverbatim
   "RunVdWk1k2 5 11 762 adaptg2wsat 200 200000000 VanDerWaerden_2-5-11_762.cnf_sol" yields "UNSAT for n=763"
   \verbatim
> E=read_ubcsat("VanDerWaerden_2-5-11_763.cnf_OUT",nrows=200)
 1  2  3  4  5
 1 12 92 92  3
200
   \endverbatim
   Perhaps all we can hope for here is to start with a good palindromic
   solution. </li>
   <li> Start with the palindromic solution for n=766:
   \verbatim
C : create_certificate2c_pdvdw([
1,5,6,9,11,12,16,20,29,33,
37,42,43,48,50,51,52,53,55,58,
61,67,74,81,85,86,89,92,93,96,
98,103,108,110,112,119,124,129,132,136,
137,140,142,144,147,149,150,153,154,165,
166,168,169,170,171,175,183,186,193,197,
206,210,211,212,213,215,217,218,220,222,
223,224,229,231,234,236,240,241,242,244,
248,250,251,253,254,255,263,268,277,288,
295,296,298,300,303,304,305,306,308,309,
310,311,313,314,316,319,330,332,333,334,
336,339,347,349,351,358,359,362,363,367,
371,374,380], 766);
certificate_pdvdw_p([5,11],766,C);
  true;
output_certificate2_vdw(reverse(C), "VanDerWaerden_2-5-11_766.cnf_sol");
   \endverbatim
   "RunVdWk1k2 5 11 766 adaptg2wsat 200 200000000 VanDerWaerden_2-5-11_766.cnf_sol"
   yields
   \verbatim
UNSAT for n=767
> E=read_ubcsat("VanDerWaerden_2-5-11_767.cnf_OUT",nrows=200)
  1   2   3   4   5
  1  10  52 118  19
200
   \endverbatim
   "RunVdWk1k2 5 11 766 adaptg2wsat 200 800000000 VanDerWaerden_2-5-11_766.cnf_sol"
   yields
   \verbatim
> E=read_ubcsat("VanDerWaerden_2-5-11_767.cnf_OUT",nrows=200)
  1   2   3   4
  4  21 118  57
200
   \endverbatim
   </li>
  </ul>


  \todo Best complete solver for palindromic problems


  \todo Best local-search solver for palindromic problems
  <ul>
   <li>
   \verbatim
> E=run_ubcsat("VanDerWaerden_pd_2-5-11_756.cnf",runs=100,cutoff=10000000)
# considering the first 25 algorithms, rnovp and anovpp are best:
rnovp
 1  2  3  4
 8 53 37  2
FlipsPerSecond = 74277
anovpp
 1  2  3
 5 58 37
FlipsPerSecond = 73679
# continuing (csoberon):
>  E=run_ubcsat("VanDerWaerden_pd_2-5-11_756.cnf",runs=100,cutoff=10000000,include_algs=names(tail(run_ubcsat_cnf_algs,-25)))
1. dano:
 0  1  2  3
 1 13 48 38
fps: 71931
2. ddfw:
 1  2  3
36 58  6
fps: 28557
3. ag2wsat:
 1  2  3
23 70  7
fps: 130829
4. g2wsat:
 4  5  6  7  8  9
 2  3 12 25 43 15
fps: 121603
   \endverbatim
   Thus altogether the order is: dano, ddfw, ag2wsat, rnovp, anovpp. </li>
  </ul>


  \todo vdw_2^pd(5,11) >= (748,767)
  <ul>
   <li> Certificates:
    <ol>
     <li> n=747:
     \verbatim
5,8,11,13,16,17,21,22,24,31,
32,35,39,45,50,55,56,59,60,61,
63,67,69,74,79,84,86,88,90,93,
94,96,98,99,100,105,108,111,115,122,
124,125,126,127,129,130,132,135,139,148,
154,155,165,173,174,176,177,179,180,187,
193,194,197,199,203,209,211,213,214,216,
221,226,232,235,236,239,240,248,249,256,
266,267,268,269,273,274,280,281,284,293,
295,296,298,301,302,305,306,308,309,310,
312,313,316,321,322,325,326,328,329,330,
331,333,341,346,351,354,358,368,371,373
     \endverbatim
     </li>
     <li> n=766:
     \verbatim
1,5,6,9,11,12,16,20,29,33,
37,42,43,48,50,51,52,53,55,58,
61,67,74,81,85,86,89,92,93,96,
98,103,108,110,112,119,124,129,132,136,
137,140,142,144,147,149,150,153,154,165,
166,168,169,170,171,175,183,186,193,197,
206,210,211,212,213,215,217,218,220,222,
223,224,229,231,234,236,240,241,242,244,
248,250,251,253,254,255,263,268,277,288,
295,296,298,300,303,304,305,306,308,309,
310,311,313,314,316,319,330,332,333,334,
336,339,347,349,351,358,359,362,363,367,
371,374,380
     \endverbatim
    </ol>
   </li>
   <li> "RunPdVdWk1k2 5 11 adaptg2wsat 100 20000000" yields
   \verbatim
Break point 1: 740
Break point 2: 757
   \endverbatim
   </li>
   <li> "RunPdVdWk1k2 5 11 adaptg2wsat 100 40000000" yields
   \verbatim
Break point 1: 742
Break point 2: 755
   \endverbatim
   </li>
   <li> Hard regions starts around n=700, and at most 25 runs needed. </li>
   <li> sapsnr yields segmentation fault (cs-oksvr), and so does rsaps.
   On cscarme "RunPdVdWk1k2 5 11 sapsnr 100 40000000" yields
   \verbatim
Break point 1: 692
Break point 2: 725
   \endverbatim
   using at most one run in random-mode. So adaptg2wsat seems superior here.
   </li>
   <li> "RunPdVdWk1k2 5 11 danov+ 100 20000000" yields
   \verbatim
Break point 1: 730
Break point 2: 763
> cat Statistics
    693     2    5188820   23771175  r
    701     7    2699820 3989538528  r
    719     3   12634923 3256881742  p
    720     2   14804766 3952877916  r
    722     3   17496508 1718934804  r
    729    26    9447735 2018278808  r
    734     1   13341462 1131130439  r
    744     2   16157401  832487773  p
    758    72   16698529 3230957694  r
    760    42    2316105 2527185156  r
> E=read_ubcsat("VanDerWaerden_pd_2-5-11_731.cnf_OUT",nrows=100)
 1  2  3
10 59 31
100
> E=read_ubcsat("VanDerWaerden_pd_2-5-11_764.cnf_OUT",nrows=100)
 1  2  3  4
 5 50 44  1
100
   \endverbatim
   Run "RunPdVdWk1k2 5 11 danov+ 100 60000000":
   \verbatim
Break point 1: 734
# interrupted
> cat Statistics
    728     2   30171162 2645228588  p
    734     2   30326304 1137542626  p
    748    13   34426618 3159649799  r
    754     3   14762804 3149427122  r
> E=read_ubcsat("VanDerWaerden_pd_2-5-11_735.cnf_OUT",nrows=100)
 1  2  3
12 71 17
100
> E=read_ubcsat("VanDerWaerden_pd_2-5-11_756.cnf_OUT",nrows=100)
 1  2
52 47
99
   \endverbatim
   Run "RunPdVdWk1k2 5 11 danov+ 200 100000000":
   \verbatim
Break point 1: 748
Break point 2: 767
> cat Statistics
    701     2   36450203   47662353  p
    711     3   10938027 3202516672  r
    721     5   14539778 1231976988  r
    729     3   52441809 3776644713  r
    731   167   28393502 3947897785  r
    743    32    5628205 2272388180  r
    746    15   49397211 1390146825  r
    747    16   50052904  550120762  r
    760    37   77725816 3817190208  r
> E=read_ubcsat("VanDerWaerden_pd_2-5-11_749.cnf_OUT",nrows=200)
  1   2   3   4
  6  71 114   9
200
> E=read_ubcsat("VanDerWaerden_pd_2-5-11_768.cnf_OUT",nrows=200)
  1   2   3
 38 150  12
200
   \endverbatim
   </li>
  </ul>

*/
