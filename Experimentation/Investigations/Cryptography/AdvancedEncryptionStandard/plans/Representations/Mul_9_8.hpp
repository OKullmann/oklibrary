// Matthew Gwynne, 7.1.2011 (Swansea)
/* Copyright 2011, 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/Representations/Mul_9_8.hpp
  \brief Investigations into AES field multiplication by 09 for the 8-bit field


  \todo Basic data
  <ul>
   <li> The CNF-file "AES_byte_field_mul_full_9.cnf" is created by the
   Maxima-function output_rijnmult_fullcnf_stdname(9); in
   ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/FieldOperationsAnalysis.mac,
   which is a full clause-set with 16
   variables and 2^16 - 2^8 = 65280 clauses:
   \verbatim
> cat AES_byte_field_mul_full_9.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
16 65280 1044480 0 1044480 1 1
 length count
16 65280
   \endverbatim
   </li>
   <li> Computing the prime implicates:
   \verbatim
maxima> output_rijnmult_fullcnf_stdname(9);
   \endverbatim
   and then
   \verbatim
shell> QuineMcCluskey-n16-O3-DNDEBUG AES_byte_field_mul_full_9.cnf > AES_byte_field_mul_pi_9.cnf
   \endverbatim
   yields a CNF with:
   \verbatim
shell> ExtendedDimacsFullStatistics-O3-DNDEBUG < AES_byte_field_mul_pi_9.cnf
c's = 1, n = 16, c = 7840, tc = 0, ntc = 7840, tl = 60352, l = 60352, finished = 1
3 : 16
4 : 64
5 : 208
6 : 768
7 : 1920
8 : 2816
9 : 2048
   \endverbatim
   </li>
   <li> The smallest known CNF representation is of size 42 (see
   "Using weighted MaxSAT to compute small CNFs"). </li>
   <li> The minimum size CNF representation is *not* known. </li>
  </ul>


  \todo r_1-bases : mincl_r1 <= 272
  <ul>
   <li> The 1-bases below need to be checked to ensure they are actually
   1-bases; see "Computing r_1-bases for a set of prime implicates" in
   Satisfiability/Reductions/Bases/plans/UcpBase.hpp. </li>
   <li> Computing an r_1-base:
   \verbatim
shell> QuineMcCluskey-n16-O3-DNDEBUG AES_byte_field_mul_full_9.cnf > AES_byte_field_mul_pi_9.cnf
shell> RandomShuffleDimacs-O3-DNDEBUG 1 < AES_byte_field_mul_pi_9.cnf | SortByClauseLength-O3-DNDEBUG > AES_byte_field_mul_9_sortedpi.cnf
shell> RUcpGen-O3-DNDEBUG AES_byte_field_mul_9_sortedpi.cnf > AES_byte_field_mul_9_gen.cnf
shell> RandomShuffleDimacs-O3-DNDEBUG 1 < AES_byte_field_mul_9_gen.cnf | SortByClauseLengthDescending-O3-DNDEBUG | RUcpBase-O3-DNDEBUG > AES_byte_field_mul_9_base.cnf
shell> cat AES_byte_field_mul_9_base.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
16 272 1504 0 1504 0 1
 length count
3 16
4 48
5 48
6 96
7 64
   \endverbatim
   </li>
  </ul>


  \todo Using weighted MaxSAT to compute small CNFs : mincl_rinf <= 42
  <ul>
   <li> Computing the weighted MaxSAT problem:
   \verbatim
shell> QuineMcCluskeySubsumptionHypergraph-n16-O3-DNDEBUG AES_byte_field_mul_full_9.cnf > AES_byte_field_mul_shg_9.cnf
shell> cat AES_byte_field_mul_shg_9.cnf | MinOnes2WeightedMaxSAT-O3-DNDEBUG > AES_byte_field_mul_9_shg.wcnf
   \endverbatim
   </li>
   <li> Running then:
   \verbatim
shell> ubcsat-okl -alg gsat -w -cutoff 1000000 -runs 100 -i AES_byte_field_mul_9_shg.wcnf
   \endverbatim
   yields:
   \verbatim
       sat  min     osteps     msteps       seed
      1 0    44                80289              1000000  626575130
      2 0    46               143131              1000000 3777510035
      3 0    44               130040              1000000 3438911282
      4 0    42               192199              1000000  900059187
<snip>
TotalLiterals = 3579552
FlipsPerSecond = 51088
BestStep_Mean = 116674.64
Steps_Mean = 1000000
Steps_Max = 1000000
PercentSuccess = 0.00
BestSolution_Mean = 43.28
BestSolution_Median = 42
BestSolution_Min = 42
BestSolution_Max = 48
   \endverbatim
   </li>
  </ul>

*/
