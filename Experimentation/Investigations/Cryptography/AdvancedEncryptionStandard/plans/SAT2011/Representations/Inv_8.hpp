// Matthew Gwynne, 6.1.2011 (Swansea)
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
> E=read.table("ss_byte2_8_field_mul_full.cnf_shg_stats",skip=2,header=TRUE)
> summary(E[E["count"]!=0,])
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
   <li> See "Basic data" in 
   Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Representations/Sbox_8.hpp.
   </li>
   <li> See "First considerations of random permutation" in
   Experimentation/Investigations/BooleanFunctions/plans/Permutations.hpp for
   random permutations. </li>
  </ul>


*/
