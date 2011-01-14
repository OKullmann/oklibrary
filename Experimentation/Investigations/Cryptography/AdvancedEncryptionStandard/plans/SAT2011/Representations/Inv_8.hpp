// Matthew Gwynne, 12.1.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Representations/Inv_8.hpp
  \brief Investigations into the AES field inversion (8-bit)


  \todo Basic data
  <ul>
   <li> The CNF is created by the Maxima-function
   output_ssinv_fullcnf_stdname(2,8,ss_polynomial_2_8), which is a full 
   clause-set with 16 variables and 2^16 - 2^8 = 65280 clauses. </li>
   <li> Prime implicates:
    <ol>
     <li> There are 136647 prime implicates, with 1004920 literals in total, 
     and with clause-length-distribution as follows:
     \verbatim
> cat ss_byte2_8_field_mul_full_.cnf_primes_stats
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
16 136647 1004920 0 1004920 0 1
 length count
5 20
6 4071
7 80960
8 50692
9 902
10 2
     \endverbatim
     </li>
     <li> As with 
     Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Representations/Sbox_8.hpp 
     what are these small number of clauses of length 5?</li>
     <li> We have the following clauses of size 5:
     \verbatim
-11 -6 -1 9 10 0
-12 -8 5 10 11 0
-10 6 8 9 12 0
-14 -9 -3 1 2 0
-14 -6 1 2 10 0
-14 -6 2 9 10 0
-14 -9 1 7 11 0
-15 -10 -2 1 8 0
-15 -4 1 7 11 0
-15 -5 1 8 11 0
-15 -10 6 8 12 0
-6 -1 3 9 15 0
-1 3 8 9 15 0
-12 -7 3 9 15 0
-16 -4 2 3 13 0
-10 -7 -2 9 16 0
-9 1 7 11 16 0
-13 -7 3 9 16 0
-2 1 4 14 16 0
-7 -2 4 14 16 0
     \endverbatim
     All of which are 3 input variables and 2 output, or vice versa.
     </li>
     <li> Also, what are these 2 clauses of size 10? 
     \verbatim
-15 -12 -11 -9 -8 -6 -3 -2 1 10 0
-16 -14 -11 -10 -7 -4 -3 -1 2 9 0
     \endverbatim
     </li>
    </ol>
   </li>
   <li> The subsumption-hypergraph of the prime-clauses:
    <ol>
     <li> Creation:
     \verbatim
> QuineMcCluskeySubsumptionHypergraphFullStatistics-n16-O3-DNDEBUG ss_byte2_8_field_mul_full.cnf
> cat AES_S.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n > AES_S_stat

> head -n 2 ss_byte2_8_field_mul_full.cnf_shg_stats 
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
136647 65280 58753920 0 58753920 0 1
     \endverbatim
     </li>
     <li> All original prime-clauses occur as 136647 variables, and all
     original clauses occur as 65280 clauses (no contractions). </li>
     <li> Curve k -> nr (clause-length to number of occurrences) looks like a
     nice relatively symmetric curve, with maximum around 950, and konvex on
     both sides. </li>
     <li> This is very similar to the full 8-bit Sbox. </li>
     <li> R-summary:
     \verbatim
R> E=read.table("ss_byte2_8_field_mul_full.cnf_shg_stats",skip=2,header=TRUE)
R> summary(E[E["count"]!=0,])
     length           count       
 Min.   : 172.0   Min.   :  1.00  
 1st Qu.: 563.8   1st Qu.: 12.00  
 Median : 837.5   Median : 41.00  
 Mean   : 837.4   Mean   : 59.56  
 3rd Qu.:1111.2   3rd Qu.:107.25  
 Max.   :1466.0   Max.   :194.00
> plot(E)
     \endverbatim
     </li>
     <li> One should try to fit this curve. </li>
     <li> Has the single shortest clause (of length 172) a special meaning?
     What is its original clause? </li>
     </li>
    </ol>
   </li>
   <li> So far, we have the following smallest representation:
   \verbatim
R> subset(E, min == 359)
   sat min osteps  msteps       seed
80   0 359 970714 1000000 3833417812
   \endverbatim
   </li>
   <li> See "Basic data" in 
   Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Representations/Sbox_8.hpp.
   </li>
   <li> See "First considerations of random permutation" in
   Experimentation/Investigations/BooleanFunctions/plans/Permutations.hpp for
   random permutations. </li>
  </ul>


  \todo Using weighted MaxSAT to compute small CNFs
  <ul>
   <li> Computing the weighted MaxSAT problem:
   \verbatim
maxima> output_ssinv_fullcnf_stdname(2,8);
   \endverbatim
   and then 
   \verbatim
shell> QuineMcCluskeySubsumptionHypergraph-n16-O3-DNDEBUG ss_byte2_8_field_mul_full.cnf > ss_byte2_8_field_mul_shg.cnf
shell> cat ss_byte2_8_field_mul_full.cnf_shg | awk --file ${OKPLATFORM}/OKsystem/OKlib/Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/shg2partial_maxsat.awk > ss_byte2_8_field_mul_shg.wcnf
   \endverbatim
   <li>
   <li> Running then:
   \verbatim
shell> new-ubcsat-okl -alg gsat -w -runs 100 -cutoff 1000000 -i ss_byte2_8_field_mul_full_.cnf_shg.wcnf > ubcsat_agsat_r100_c1000000.runs
   \endverbatim
   yields:
   \verbatim
R> E = read.table("ubcsat_agsat_r100_c1000000.runs", header=TRUE)
R> summary(E)
      sat         min            osteps           msteps     
 Min.   :0   Min.   :359.0   Min.   :884154   Min.   :1e+06  
 1st Qu.:0   1st Qu.:368.8   1st Qu.:957724   1st Qu.:1e+06  
 Median :0   Median :372.5   Median :975228   Median :1e+06  
 Mean   :0   Mean   :372.0   Mean   :970125   Mean   :1e+06  
 3rd Qu.:0   3rd Qu.:375.0   3rd Qu.:989484   3rd Qu.:1e+06  
 Max.   :0   Max.   :383.0   Max.   :999082   Max.   :1e+06  
      seed          
 Min.   :5.025e+07  
 1st Qu.:1.371e+09  
 Median :2.106e+09  
 Mean   :2.169e+09  
 3rd Qu.:2.975e+09  
 Max.   :4.280e+09
   \endverbatim
   which seems similar in size to the currently known minimum size for the 
   Sbox.
   </li>
  </ul>


*/
