// Matthew Gwynne, 7.1.2011 (Swansea)
/* Copyright 2011, 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/Representations/Mul_11_8.hpp
  \brief Investigations into AES field multiplication by 11 for the 8-bit field


  \todo Basic data
  <ul>
   <li> The CNF-file "AES_byte_field_mul_full_11.cnf" is created by the
   Maxima-function output_rijnmult_fullcnf_stdname(11); in
   ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/FieldOperationsAnalysis.mac,
   which is a full clause-set with 16
   variables and 2^16 - 2^8 = 65280 clauses:
   \verbatim
> cat AES_byte_field_mul_full_11.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
16 65280 1044480 0 1044480 1 1
 length count
16 65280
   \endverbatim
   </li>
   <li> Computing the prime implicates:
   \verbatim
maxima> output_rijnmult_fullcnf_stdname(11);
   \endverbatim
   and then
   \verbatim
shell> QuineMcCluskey-n16-O3-DNDEBUG AES_byte_field_mul_full_11.cnf > AES_byte_field_mul_pi_11.cnf
   \endverbatim
   yields a CNF with:
   \verbatim
shell> ExtendedDimacsFullStatistics-O3-DNDEBUG < AES_byte_field_mul_pi_11.cnf
c's = 1, n = 16, c = 15008, tc = 0, ntc = 15008, tl = 119824, l = 119824, finished = 1
4 : 48
5 : 336
6 : 1056
7 : 2560
8 : 5376
9 : 5632
   \endverbatim
   </li>
   <li> The smallest known CNF representation is of size 66 (see
   "Using weighted MaxSAT to compute small CNFs"). </li>
   <li> The minimum size CNF representation is *not* known. </li>
  </ul>


  \todo r_1-bases : mincl_r1 <= 1376
  <ul>
   <li> The 1-bases below need to be checked to ensure they are actually
   1-bases; see "Computing r_1-bases for a set of prime implicates" in
   Satisfiability/Reductions/Bases/plans/UcpBase.hpp. </li>
   <li> Computing an r_1-base:
   \verbatim
shell> QuineMcCluskey-n16-O3-DNDEBUG AES_byte_field_mul_full_11.cnf > AES_byte_field_mul_pi_11.cnf
shell> RandomShuffleDimacs-O3-DNDEBUG 1 < AES_byte_field_mul_pi_11.cnf | SortByClauseLength-O3-DNDEBUG > AES_byte_field_mul_11_sortedpi.cnf
shell> RUcpGen-O3-DNDEBUG AES_byte_field_mul_11_sortedpi.cnf > AES_byte_field_mul_11_gen.cnf
shell> RandomShuffleDimacs-O3-DNDEBUG 1 < AES_byte_field_mul_11_gen.cnf | SortByClauseLengthDescending-O3-DNDEBUG | RUcpBase-O3-DNDEBUG > AES_byte_field_mul_11_base.cnf
shell> cat AES_byte_field_mul_11_base.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
16 1376 8016 0 8016 0 1
 length count
4 48
5 336
6 800
7 192
   \endverbatim
   </li>
  </ul>


  \todo Using weighted MaxSAT to compute small CNFs : mincl_rinf <= 66
  <ul>
   <li> Computing the weighted MaxSAT problem:
   \verbatim
shell> QuineMcCluskeySubsumptionHypergraph-n16-O3-DNDEBUG AES_byte_field_mul_full_11.cnf > AES_byte_field_mul_shg_11.cnf
shell> cat AES_byte_field_mul_shg_11.cnf | MinOnes2WeightedMaxSAT-O3-DNDEBUG > AES_byte_field_mul_11_shg.wcnf
   \endverbatim
   </li>
   <li> Running then:
   \verbatim
shell> ubcsat-okl -alg gsat -w -cutoff 1000000 -runs 100 -i AES_byte_field_mul_11_shg.wcnf
   \endverbatim
   yields:
   \verbatim
       sat  min     osteps     msteps       seed
      1 0    85               690970              1000000  651138378
      2 0    72               353973              1000000 1635869598
      3 0    74               589507              1000000 3503387899
      4 0    72               334496              1000000 2193890246
      5 0    68               684981              1000000 3037183790
      6 0    74               400412              1000000 2310997959
      7 0    76               455696              1000000 1697964535
      8 0    71               893641              1000000  372929467
      9 0    73               369444              1000000 4221454731
     10 0    68               530949              1000000 1754477525
     11 0    79               395268              1000000 2300315436
     12 0    72               745484              1000000 3254974086
     13 0    74               269095              1000000 1993743598
     14 0    66               295210              1000000 3873470822
<snip>
TotalLiterals = 5388960
FlipsPerSecond = 25671
BestStep_Mean = 571227.38
Steps_Mean = 1000000
Steps_Max = 1000000
PercentSuccess = 0.00
BestSolution_Mean = 74.53
BestSolution_Median = 74
BestSolution_Min = 66
BestSolution_Max = 87
   \endverbatim
   </li>
  </ul>

*/
