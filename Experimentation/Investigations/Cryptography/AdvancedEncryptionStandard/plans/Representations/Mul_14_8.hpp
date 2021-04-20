// Matthew Gwynne, 7.1.2011 (Swansea)
/* Copyright 2011, 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/Representations/Mul_14_8.hpp
  \brief Investigations into AES field multiplication by 14 for the 8-bit field


  \todo Basic data
  <ul>
   <li> The CNF-file "AES_byte_field_mul_full_14.cnf" is created by the
   Maxima-function output_rijnmult_fullcnf_stdname(14); in
   ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/FieldOperationsAnalysis.mac,
   which is a full clause-set with 16
   variables and 2^16 - 2^8 = 65280 clauses:
   \verbatim
> cat AES_byte_field_mul_full_14.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
16 65280 1044480 0 1044480 1 1
 length count
16 65280
   \endverbatim
   </li>
   <li> Computing the prime implicates:
   \verbatim
maxima> output_rijnmult_fullcnf_stdname(14);
   \endverbatim
   and then
   \verbatim
shell> QuineMcCluskey-n16-O3-DNDEBUG AES_byte_field_mul_full_14.cnf > AES_byte_field_mul_pi_14.cnf
   \endverbatim
   yields a CNF with:
   \verbatim
shell> ExtendedDimacsFullStatistics-O3-DNDEBUG < AES_byte_field_mul_pi_14.cnf
ExtendedDimacsFullStatistics-O3-DNDEBUG < AES_byte_field_mul_pi_14.cnf
c's = 1, n = 16, c = 14300, tc = 0, ntc = 14300, tl = 114252, l = 114252, finished = 1
3 : 4
4 : 56
5 : 288
6 : 960
7 : 2496
8 : 5120
9 : 5376
   \endverbatim
   </li>
   <li> The smallest known CNF representation is of size 56 (see
   "Using weighted MaxSAT to compute small CNFs"). </li>
   <li> The minimum size CNF representation is *not* known. </li>
  </ul>


  \todo r_1-bases : mincl_r1 <= 1052
  <ul>
   <li> The 1-bases below need to be checked to ensure they are actually
   1-bases; see "Computing r_1-bases for a set of prime implicates" in
   Satisfiability/Reductions/Bases/plans/UcpBase.hpp. </li>
   <li> Computing an r_1-base:
   \verbatim
shell> QuineMcCluskey-n16-O3-DNDEBUG AES_byte_field_mul_full_14.cnf > AES_byte_field_mul_pi_14.cnf
shell> RandomShuffleDimacs-O3-DNDEBUG 1 < AES_byte_field_mul_pi_14.cnf | SortByClauseLength-O3-DNDEBUG > AES_byte_field_mul_14_sortedpi.cnf
shell> RUcpGen-O3-DNDEBUG AES_byte_field_mul_14_sortedpi.cnf > AES_byte_field_mul_14_gen.cnf
shell> RandomShuffleDimacs-O3-DNDEBUG 1 < AES_byte_field_mul_14_gen.cnf | SortByClauseLengthDescending-O3-DNDEBUG | RUcpBase-O3-DNDEBUG > AES_byte_field_mul_14_base.cnf
shell> cat AES_byte_field_mul_14_base.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
16 1052 6348 0 6348 0 1
 length count
3 4
4 56
5 224
6 384
7 384
   \endverbatim
   </li>
  </ul>


  \todo Using weighted MaxSAT to compute small CNFs : mincl_rinf <= 56
  <ul>
   <li> Computing the weighted MaxSAT problem:
   \verbatim
shell> QuineMcCluskeySubsumptionHypergraph-n16-O3-DNDEBUG AES_byte_field_mul_full_14.cnf > AES_byte_field_mul_shg_14.cnf
shell> cat AES_byte_field_mul_shg_14.cnf | MinOnes2WeightedMaxSAT-O3-DNDEBUG > AES_byte_field_mul_14_shg.wcnf
   \endverbatim
   </li>
   <li> Running then:
   \verbatim
shell> ubcsat-okl -alg gsat -w -cutoff 1000000 -runs 100 -i AES_byte_field_mul_14_shg.wcnf
   \endverbatim
   yields:
   \verbatim
<snip>
     27 0    64               500386              1000000 2630623820
     28 0    62               184936              1000000  978348806
     29 0    64               432908              1000000 1970436541
     30 0    56               237214              1000000 3535520173
     31 0    62               388161              1000000 2840264849
<snip>
TotalLiterals = 5126108
FlipsPerSecond = 25499
BestStep_Mean = 346536.54
Steps_Mean = 1000000
Steps_Max = 1000000
PercentSuccess = 0.00
BestSolution_Mean = 62.38
BestSolution_Median = 62
BestSolution_Min = 56
BestSolution_Max = 72
   \endverbatim
   </li>
  </ul>

*/
