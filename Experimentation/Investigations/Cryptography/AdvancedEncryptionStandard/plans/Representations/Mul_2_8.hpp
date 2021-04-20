// Matthew Gwynne, 7.1.2011 (Swansea)
/* Copyright 2011, 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/Representations/Mul_2_8.hpp
  \brief Investigations into AES field multiplication by 02 for the 8-bit field


  \todo Basic data
  <ul>
   <li> The CNF-file "AES_byte_field_mul_full_2.cnf" is created by the
   Maxima-function output_rijnmult_fullcnf_stdname(2); in
   ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/FieldOperationsAnalysis.mac,
   which is a full clause-set with 16
   variables and 2^16 - 2^8 = 65280 clauses:
   \verbatim
> cat AES_byte_field_mul_full_2.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
16 65280 1044480 0 1044480 1 1
 length count
16 65280
   \endverbatim
   </li>
   <li> Computing the prime implicates:
   \verbatim
shell> QuineMcCluskey-n16-O3-DNDEBUG AES_byte_field_mul_full_2.cnf > AES_byte_field_mul_pi_2.cnf
   \endverbatim
   yields a CNF with:
   \verbatim
shell> ExtendedDimacsFullStatistics-O3-DNDEBUG < AES_byte_field_mul_pi_2.cnf
c's = 1, n = 16, c = 58, tc = 0, ntc = 58, tl = 188, l = 188, finished = 1
2 : 10
3 : 24
4 : 24
   \endverbatim
   </li>
  </ul>


  \todo Minimising using hypergraph transversal tools :
  mincl_rinf = 20, num_mincl_rinf=102
  <ul>
   <li> Computing all minimum CNFs:
   \verbatim
maxima> output_rijnmult_fullcnf_stdname(2);
   \endverbatim
   then
   \verbatim
shell> ${OKPLATFORM}/OKsystem/OKlib/Satisfiability/Optimisation/all_minimum_cnf AES_byte_field_mul_full_2.cnf
   \endverbatim
   yields
   \verbatim
shell> echo AES_byte_field_mul_full_2.cnf.trans_* | wc -w
102
   \endverbatim
   that is, there are 102 representations of size 20.
   </li>
   <li> Most (90) of the minimum representations contain 8 clauses of size 2,
   and 12 clauses of size 3. There are then a further twelve clause-sets
   where there are only 8 clause of size 3, but then 4 clauses of size 4.
   </li>
   <li> The hardness of the minimum representations are:
    <ul>
     <li> Computing the hardness:
     \verbatim
maxima> Mul2_min_F_l : create_list(read_fcl_f(sconcat("AES_byte_field_mul_full_2.cnf.trans_",i)),i,1,102)$
maxima> Mul2_pi_F : min_resolution_closure_cs(setify(Mul2_min_F_l[1][2]))$
maxima> Mul2_min_F_hd_l : setify(create_list(hardness_wpi_cs(setify(Mul2_min_F_l[i][2]),Mul2_pi_F[1]),i,1,102));
{2,3}
     \endverbatim
     </li>
     <li> For the definition and computation of hardness in general, see
     "Hardness of boolean function representations" in
     Experimentation/Investigations/BooleanFunctions/plans/general.hpp.
     </li>
     <li> Note that most of these minimum representations (including the
     one we use in the library) have hardness 2. </li>
     <li> However, some minimum representations have hardness 3! This is
     a nice simple example of a boolean function with minimum representations
     of different hardnesses. </li>
    </ul>
   </li>
  </ul>


  \todo r_1-bases : mincl_r1 <= 22
  <ul>
   <li> The 1-bases below need to be checked to ensure they are actually
   1-bases; see "Computing r_1-bases for a set of prime implicates" in
   Satisfiability/Reductions/Bases/plans/UcpBase.hpp. </li>
   <li> Current minimum clause-count of an r_1-base: 22. </li>
   <li> For the mincl_rinf representations, see "Minimising using hypergraph
   transversal tools". </li>
   <li> Computing the bases directly:
   \verbatim
> RandomShuffleDimacs-O3-DNDEBUG 1 < AES_byte_field_mul_2_pi.cnf | RUcpBase-O3-DNDEBUG | ExtendedDimacsFullStatistics-O3-DNDEBUG
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
16 22 56 0 56 1 1
 length count
2 10
3 12
   \endverbatim
   </li>
   <li> Again, as in "r_1-bases" in
   Cryptography/AdvancedEncryptionStandard/plans/Representations/Sbox_8.hpp,
   using any of the minimal transversals from "Minimising using hypergraph
   transversal tools" using "RUcpGen" yields larger (> size 26) r_1 bases.
   </li>
  </ul>

*/
