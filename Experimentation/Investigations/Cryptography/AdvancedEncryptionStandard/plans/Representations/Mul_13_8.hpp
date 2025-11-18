// Matthew Gwynne, 7.1.2011 (Swansea)
/* Copyright 2011, 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/Representations/Mul_13_8.hpp
  \brief Investigations into AES field multiplication by 13 for the 8-bit field


  \todo Basic data
  <ul>
   <li> The CNF-file "AES_byte_field_mul_full_13.cnf" is created by the
   Maxima-function output_rijnmult_fullcnf_stdname(13); in
   ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/FieldOperationsAnalysis.mac,
   which is a full clause-set with 16
   variables and 2^16 - 2^8 = 65280 clauses:
   \verbatim
> cat AES_byte_field_mul_full_13.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
16 65280 1044480 0 1044480 1 1
 length count
16 65280
   \endverbatim
   </li>
   <li> Computing the prime implicates:
   \verbatim
maxima> output_rijnmult_fullcnf_stdname(13);
   \endverbatim
   and then
   \verbatim
shell> QuineMcCluskey-n16-O3-DNDEBUG AES_byte_field_mul_full_13.cnf > AES_byte_field_mul_pi_13.cnf
   \endverbatim
   yields a CNF with:
   \verbatim
shell> ExtendedDimacsFullStatistics-O3-DNDEBUG < AES_byte_field_mul_pi_13.cnf
c's = 1, n = 16, c = 15312, tc = 0, ntc = 15312, tl = 122816, l = 122816, finished = 1
4 : 80
5 : 192
6 : 1152
7 : 2368
8 : 5632
9 : 5888
   \endverbatim
   </li>
   <li> The smallest known CNF representation is of size 60 (see
   "Using weighted MaxSAT to compute small CNFs"). </li>
   <li> The minimum size CNF representation is *not* known. </li>
  </ul>


  \todo r_1-bases : mincl_r1 <= 1040
  <ul>
   <li> The 1-bases below need to be checked to ensure they are actually
   1-bases; see "Computing r_1-bases for a set of prime implicates" in
   Satisfiability/Reductions/Bases/plans/UcpBase.hpp. </li>
   <li> Computing an r_1-base:
   \verbatim
shell> QuineMcCluskey-n16-O3-DNDEBUG AES_byte_field_mul_full_13.cnf > AES_byte_field_mul_pi_13.cnf
shell> RandomShuffleDimacs-O3-DNDEBUG 1 < AES_byte_field_mul_pi_13.cnf | SortByClauseLength-O3-DNDEBUG > AES_byte_field_mul_13_sortedpi.cnf
shell> RUcpGen-O3-DNDEBUG AES_byte_field_mul_13_sortedpi.cnf > AES_byte_field_mul_13_gen.cnf
shell> RandomShuffleDimacs-O3-DNDEBUG 1 < AES_byte_field_mul_13_gen.cnf | SortByClauseLengthDescending-O3-DNDEBUG | RUcpBase-O3-DNDEBUG > AES_byte_field_mul_13_base.cnf
shell> cat AES_byte_field_mul_13_base.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
16 1040 6016 0 6016 0 1
 length count
4 80
5 192
6 640
7 128
   \endverbatim
   </li>
  </ul>


  \todo Using weighted MaxSAT to compute small CNFs : mincl_rinf <= 60
  <ul>
   <li> Computing the weighted MaxSAT problem:
   \verbatim
shell> QuineMcCluskeySubsumptionHypergraph-n16-O3-DNDEBUG AES_byte_field_mul_full_13.cnf > AES_byte_field_mul_shg_13.cnf
shell> cat AES_byte_field_mul_shg_13.cnf | MinOnes2WeightedMaxSAT-O3-DNDEBUG > AES_byte_field_mul_13_shg.wcnf
   \endverbatim
   </li>
   <li> Running then:
   \verbatim
shell> ubcsat-okl -alg gsat -w -cutoff 1000000 -runs 100 -i AES_byte_field_mul_13_shg.wcnf
   \endverbatim
   yields:
   \verbatim
       sat  min     osteps     msteps       seed
      1 0    60               286294              1000000  673057735
<snip>
TotalLiterals = 5323728
FlipsPerSecond = 25097
BestStep_Mean = 336474.79
Steps_Mean = 1000000
Steps_Max = 1000000
PercentSuccess = 0.00
BestSolution_Mean = 64.99
BestSolution_Median = 66
BestSolution_Min = 60
BestSolution_Max = 73
   \endverbatim
   </li>
  </ul>

*/
