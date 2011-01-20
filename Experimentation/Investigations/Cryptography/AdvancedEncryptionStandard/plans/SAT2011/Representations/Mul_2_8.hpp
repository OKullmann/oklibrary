// Matthew Gwynne, 7.1.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Representations/Mul_2_8.hpp
  \brief Investigations into AES field multiplication by 02 for the 8-bit field.



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
shell> ${OKPLATFORM}/OKsystem/OKlib/Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/all_minimum_cnf AES_byte_field_mul_full_2.cnf
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
  </ul>


  \todo r_1-bases : mincl_r1 <= 22
  <ul>
   <li> Current minimum clause-count of an r_1-base: 22. </li>
   <li> For the mincl_rinf representations, see "Minimising using hypergraph
   transversal tools". </li>
   <li> Starting with a generating set, created from scratch:
   \verbatim
> RUcpBase-O3-DNDEBUG 1 <  AES_byte_field_mul_2_pi.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG 
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
16 22 56 0 56 1 1
 length count
2 10
3 12
   \endverbatim
   </li>
   <li> Again, as in "r_1-bases" in 
   Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Representations/Sbox_8.hpp,
   using any of the minimal transversals from "Minimising using hypergraph 
   transversal tools" using "RUcpGen" yields larger (> size 26) r_1 bases. 
   </li>
  </ul>

*/
