// Matthew Gwynne, 7.1.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Representations/Mul_11_8.hpp
  \brief Investigations into AES field multiplication by 11 for the 8-bit field.



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
  </ul>

*/
