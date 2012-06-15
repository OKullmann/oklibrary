// Matthew Gwynne, 7.1.2011 (Swansea)
/* Copyright 2011, 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/Representations/Mul_3_8.hpp
  \brief Investigations into AES field multiplication by 03 for the 8-bit field


  \todo Basic data
  <ul>
   <li> The CNF-file "AES_byte_field_mul_full_3.cnf" is created by the
   Maxima-function output_rijnmult_fullcnf_stdname(3); in
   ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/FieldOperationsAnalysis.mac,
   which is a full clause-set with 16
   variables and 2^16 - 2^8 = 65280 clauses:
   \verbatim
> cat AES_byte_field_mul_full_3.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
16 65280 1044480 0 1044480 1 1
 length count
16 65280
   \endverbatim
   <li> Computing the prime implicates:
   \verbatim
maxima> output_rijnmult_fullcnf_stdname(3);
   \endverbatim
   and then
   \verbatim
shell> QuineMcCluskey-n16-O3-DNDEBUG AES_byte_field_mul_full_3.cnf > AES_byte_field_mul_pi_3.cnf
   \endverbatim
   yields a CNF with:
   \verbatim
shell> ExtendedDimacsFullStatistics-O3-DNDEBUG < AES_byte_field_mul_pi_3.cnf
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
16 5048 39592 0 39592 1 1
 length count
3 24
4 64
5 224
6 512
7 768
8 1408
9 2048
   \endverbatim
   </li>
   <li> The smallest known CNF representation is of size 36 (see
   "Using weighted MaxSAT to compute small CNFs"). </li>
   <li> The minimum size CNF representation is *not* known. </li>
  </ul>


  \todo Using weighted MaxSAT to compute small CNFs : mincl_rinf <= 36
  <ul>
   <li> Computing the weighted MaxSAT problem:
   \verbatim
shell> QuineMcCluskeySubsumptionHypergraph-n16-O3-DNDEBUG AES_byte_field_mul_full_3.cnf > AES_byte_field_mul_shg_3.cnf
shell> cat AES_byte_field_mul_shg_3.cnf | MinOnes2WeightedMaxSAT-O3-DNDEBUG > AES_byte_field_mul_3_shg.wcnf
   \endverbatim
   <li>
   <li> Running then:
   \verbatim
shell> ubcsat-okl  -alg gsat -w -runs 100 -cutoff 5000000 -i AES_byte_field_mul_3_shg.wcnf -r model AES_byte_field_mul_3_36.ass -wtarget 36 -solve
shell> cat AES_byte_field_mul_full_3.cnf_primes | FilterDimacs AES_byte_field_mul_3_36.ass > AES_byte_field_mul_3_s36.cnf
   \endverbatim
   so far yields:
   \verbatim
       sat  min     osteps     msteps       seed
      1 0    36                68505              5000000  102207734
      2 0    40                84667              5000000 1261221746
      3 0    42                46042              5000000 1485516236
      4 0    40                86429              5000000 4111246975
      5 0    36                44904              5000000 2868932619
      6 0    37                40070              5000000 1205421272
      7 0    37                27961              5000000 1581091370
      8 0    36                54726              5000000 3554355522
      9 0    36                59709              5000000 3915062434
   \endverbatim
   </li>
   <li> The hardness of this "minimum" representation for the multiplication
   by 03 is 3:
    <ul>
     <li> See "Hardness of boolean function representations" in
     Experimentation/Investigations/BooleanFunctions/plans/general.hpp
     for a description of the notion of hardness, and method of computation.
     </li>
     <li> Computing the hardness:
     \verbatim
maxima> output_ssmult_fullcnf_stdname(3,2,8,ss_polynomial_2_8);
shell> QuineMcCluskey-n16-O3-DNDEBUG ss_byte2_8_field_mul_full_3.cnf > ss_byte2_8_field_mul_full_3.cnf_primes
maxima> Mul3_F_primes : read_fcl_f("ss_byte2_8_field_mul_full_3.cnf_primes")$
maxima> hardness_wpi_cs(setify(ev_hm(ss_field_cnfs,[8,3])[2]),Mul3_F_primes[2]);
3
     \endverbatim
     </li>
    </ul>
   </li>
  </ul>


  \todo r_1-bases : mincl_r1 <= 80
  <ul>
   <li> The 1-bases below need to be checked to ensure they are actually
   1-bases; see "Computing r_1-bases for a set of prime implicates" in
   Satisfiability/Reductions/Bases/plans/UcpBase.hpp. </li>
   <li> Computing three r_1-bases:
   \verbatim
> seed=1; RandomShuffleDimacs-O3-DNDEBUG ${seed} < AES_byte_field_mul_pi_3.cnf | RUcpBase-O3-DNDEBUG | tee AES_byte_field_mul_bases_${seed}.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
16 80 328 0 328 1 1
 length count
3 24
4 24
5 32
> seed=2; RandomShuffleDimacs-O3-DNDEBUG ${seed} < AES_byte_field_mul_pi_3.cnf | RUcpBase-O3-DNDEBUG | tee AES_byte_field_mul_bases_${seed}.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
16 80 328 0 328 1 1
 length count
3 24
4 24
5 32
> seed=3; RandomShuffleDimacs-O3-DNDEBUG ${seed} < AES_byte_field_mul_pi_3.cnf | RUcpBase-O3-DNDEBUG | tee AES_byte_field_mul_bases_${seed}.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
16 80 328 0 328 1 1
 length count
3 24
4 24
5 32
   \endverbatim
   </li>
   <li> Using the method "from below":
   \verbatim
> RUcpGen-O3-DNDEBUG AES_byte_field_mul_pi_3.cnf | RandomShuffleDimacs-O3-DNDEBUG | RUcpBase-O3-DNDEBUG | ExtendedDimacsFullStatistics-O3-DNDEBUG
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
16 80 328 0 328 1 1
 length count
3 24
4 24
5 32
   \endverbatim
   </li>
   <li> Each of these runs yields the same r_1 base:
   \verbatim
> sort AES_byte_field_mul_bases_1.cnf > AES_byte_field_mul_bases_1_sorted
> sort AES_byte_field_mul_bases_2.cnf > AES_byte_field_mul_bases_2_sorted
> sort AES_byte_field_mul_bases_3.cnf > AES_byte_field_mul_bases_3_sorted
> diff AES_byte_field_mul_bases_1_sorted AES_byte_field_mul_bases_2_sorted
> diff AES_byte_field_mul_bases_3_sorted AES_byte_field_mul_bases_2_sorted
> diff AES_byte_field_mul_bases_3_sorted AES_byte_field_mul_bases_1_sorted
   \endverbatim
   </li>
   <li> Is this r_1-base unique?!?! </li>
  </ul>

*/
