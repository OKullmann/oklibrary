// Matthew Gwynne, 7.1.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Representations/Mul_13_8.hpp
  \brief Investigations into AES field multiplication by 13 for the 8-bit field.



  \todo Overview
  <ul>
   <li> XXX </li>
  </ul>

  
  \todo Prime implicates
  <ul>
   <li> Computing the prime implicates:
   \verbatim
maxima> output_rijnmult_fullcnf_stdname(13);
   \endverbatim
   and then
   \verbatim
shell> QuineMcCluskey-n16-O3-DNDEBUG AES_byte_field_mul_full_13.cnf > AES_byte_field_mul_pi_13.cnf
   \endverbatim
   yields a CNF with:
   \verbatim
shell> ExtendedDimacsFullStatistics-O3-DNDEBUG < AES_byte_field_mul_pi_13.cnf
c's = 1, n = 16, c = 15312, tc = 0, ntc = 15312, tl = 122816, l = 122816, finished = 1
4 : 80
5 : 192
6 : 1152
7 : 2368
8 : 5632
9 : 5888
   \endverbatim
   </li>
  </ul>

*/
