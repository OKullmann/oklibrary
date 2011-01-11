// Matthew Gwynne, 7.1.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Representations/Mul_9_8.hpp
  \brief Investigations into AES field multiplication by 09 for the 8-bit field.



  \todo Overview
  <ul>
   <li> XXX </li>
  </ul>

  
  \todo Prime implicates
  <ul>
   <li> Computing the prime implicates:
   \verbatim
maxima> output_rijnmult_fullcnf_stdname(9);
   \endverbatim
   and then
   \verbatim
shell> QuineMcCluskey-n16-O3-DNDEBUG AES_byte_field_mul_full_9.cnf > AES_byte_field_mul_pi_9.cnf
   \endverbatim
   yields a CNF with:
   \verbatim
shell> ExtendedDimacsFullStatistics-O3-DNDEBUG < AES_byte_field_mul_pi_9.cnf 
c's = 1, n = 16, c = 7840, tc = 0, ntc = 7840, tl = 60352, l = 60352, finished = 1
3 : 16
4 : 64
5 : 208
6 : 768
7 : 1920
8 : 2816
9 : 2048
   \endverbatim
   </li>
  </ul>

*/
