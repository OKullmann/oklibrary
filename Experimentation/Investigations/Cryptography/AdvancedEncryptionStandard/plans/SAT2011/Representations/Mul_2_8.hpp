// Matthew Gwynne, 7.1.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Representations/Mul_2_8.hpp
  \brief Investigations into AES field multiplication by 02 for the 8-bit field.



  \todo Overview
  <ul>
   <li> We can compute all 102 minimum CNF representations of size 20, see
   "Minimising using hypergraph transversal tools". </li>
  </ul>


  \todo Prime implicates
  <ul>
   <li> Computing the prime implicates:
   \verbatim
maxima> output_rijnmult_fullcnf_stdname(2);
   \endverbatim
   and then
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

  
  \todo Minimising using hypergraph transversal tools
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


*/
