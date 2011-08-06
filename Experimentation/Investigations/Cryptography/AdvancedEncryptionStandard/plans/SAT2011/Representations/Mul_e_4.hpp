// Matthew Gwynne, 20.6.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Representations/Mul_e_4.hpp
  \brief Investigations into AES field multiplication by arbitary elements (e) in the 4-bit field


  \todo Basic data
  <ul>
   <li> The CNF-files " ss_byte2_4_field_mul_full_${e}.cnf" are created by the
   Maxima-function output_rijnmult_fullcnf_stdname(e); in
   ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/FieldOperationsAnalysis.mac,
   which are full clause-sets with 16 variables and 2^8 - 2^4 = 240
   clauses:
   \verbatim
maxima> for e : 1 thru 15 do output_ssmult_fullcnf_stdname(e,2,4,ss_polynomial_2_4);
   \endverbatim
   Note we investigate the multiplications which are permutations (i.e., not
   00) here.
   </li>
   <li> Computing the prime implicates statistics:
   \verbatim
maxima> for i : 1 thru 15 do output_ssmult_fullcnf_stdname(i, 2, 4, ss_polynomial_2_4);
shell> for i in $(seq 1 15); do QuineMcCluskey-n16-O3-DNDEBUG ss_byte2_4_field_mul_full_${i}.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG; done
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
8 8 16 0 16 1 1
 length count
2 8
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
8 14 36 0 36 1 1
 length count
2 6
3 8
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
8 120 528 0 528 1 1
 length count
3 16
4 40
5 64
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
8 32 108 0 108 1 1
 length count
2 4
3 12
4 16
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
8 120 528 0 528 1 1
 length count
3 16
4 40
5 64
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
8 132 580 0 580 1 1
 length count
3 12
4 56
5 64
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
8 132 580 0 580 1 1
 length count
3 12
4 56
5 64
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
8 74 308 0 308 1 1
 length count
2 2
3 16
4 24
5 32
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
8 14 36 0 36 1 1
 length count
2 6
3 8
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
8 120 528 0 528 1 1
 length count
3 16
4 40
5 64
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
8 120 528 0 528 1 1
 length count
3 16
4 40
5 64
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
8 120 528 0 528 1 1
 length count
3 16
4 40
5 64
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
8 32 108 0 108 1 1
 length count
2 4
3 12
4 16
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
8 120 528 0 528 1 1
 length count
3 16
4 40
5 64
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
8 74 308 0 308 1 1
 length count
2 2
3 16
4 24
5 32
   \endverbatim
   </li>
   <li> Computing minimum representations:
    <ul>
     <li> Computed using
     Satisfiability/Optimisation/minimise_cnf_cryptominisat. </li>
     <li> Statistics:
     \verbatim
maxima> for i : 2 thru 15 do print(statistics_fcs(ev_hm(ss_field_cnfs,[4,i])));
[8,9,22,3,2]
[8,16,52,4,3]
[8,11,30,3,2]
[8,16,56,4,3]
[8,16,60,4,3]
[8,16,60,4,3]
[8,13,42,4,2]
[8,9,22,3,2]
[8,16,52,4,3]
[8,16,52,4,3]
[8,16,52,4,3]
[8,11,30,3,2]
[8,16,52,4,3]
[8,13,42,4,2]
     \endverbatim
     </li>
     <li> The hardness of all minimum representations of the AES 4-bit field
     multiplications is always <=3:
      <ul>
       <li> For the definition and computation of hardness in general, see
       "Hardness of boolean function representations" in
       Experimentation/Investigations/BooleanFunctions/plans/general.hpp.
       </li>
       <li> Computing the hardness:
       \verbatim
for i : 2 thru 15 do print(sconcat("hd(F_mul",i, ") = ", hardness_cs(setify(ev_hm(ss_field_cnfs,[4,i])[2]))));
hd(F_mul2) = 2
hd(F_mul3) = 2
hd(F_mul4) = 2
hd(F_mul5) = 2
hd(F_mul6) = 3
hd(F_mul7) = 3
hd(F_mul8) = 2
hd(F_mul9) = 2
hd(F_mul10) = 2
hd(F_mul11) = 2
hd(F_mul12) = 2
hd(F_mul13) = 2
hd(F_mul14) = 2
hd(F_mul15) = 2
       \endverbatim
       </li>
       <li> In all cases we consider so far (2 and 3), the hardness of the
       minimum translation is 2 for the 4-bit AES field multiplications.
       </li>
      </ul>
     </li>
  </ul>

*/
