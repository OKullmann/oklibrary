// Matthew Gwynne, 7.1.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Representations/Mul_14_8.hpp
  \brief Investigations into AES field multiplication by 14 for the 8-bit field.

  \todo Overview
  <ul>
   <li> XXX </li>
  </ul>

  
  \todo Prime implicates
  <ul>
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
  </ul>

*/
