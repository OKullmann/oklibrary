// Matthew Gwynne, 7.1.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Representations/Mul_3_8.hpp
  \brief Investigations into AES field multiplication by 03 for the 8-bit field.



  \todo Overview
  <ul>
   <li> There are 5048 prime implicates (See "Prime implicates"). </li>
   <li> The smallest known CNF representation is of size 36 (see
   "Using weighted MaxSAT to compute small CNFs"). </li>
   <li> The minimum size CNF representation is *not* known. </li>
  </ul>

  
  \todo Prime implicates
  <ul>
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
  </ul>

  
  \todo Using weighted MaxSAT to compute small CNFs
  <ul>
   <li> Computing the weighted MaxSAT problem:
   \verbatim
maxima> output_rijnmult_fullcnf_stdname(3);
   \endverbatim
   and then 
   \verbatim
shell> QuineMcCluskeySubsumptionHypergraph-n16-O3-DNDEBUG AES_byte_field_mul_full_3.cnf > AES_byte_field_mul_shg_3.cnf
shell> cat AES_byte_field_mul_shg_3.cnf | awk --file ${OKPLATFORM}/OKsystem/OKlib/Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/shg2partial_maxsat.awk > AES_byte_field_mul_3_shg.wcnf
   \endverbatim
   <li>
   <li> Running then:
   \verbatim
shell> ubcsat-okl  -alg gsat -w -runs 100 -cutoff 5000000 -i AES_byte_field_mul_3_shg.wcnf
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
  </ul>

*/
