// Matthew Gwynne, 16.5.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/DataEncryptionStandard/plans/Sboxes/Sbox_5.hpp
  \brief On investigations into Sbox five of the Data Encryption Standard


  \todo Basic data
  <ul>
   <li> Generating the full CNF representation:
    <ol>
     <li> The CNF-file "DES_Sbox_5_fullCNF.cnf" is created by the Maxima-function
     output_dessbox_fullcnf_stdname(5) in
     ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/DataEncryptionStandard/Sboxes.mac,
     which is a full clause-set with 10
     variables and 2^10 - 2^6 = 960 clauses:
     \verbatim
> cat DES_Sbox_5_fullCNF.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
10 960 9600 0 9600 1 1
 length count
10 960
     \endverbatim
     </li>
     <li> This clause-set is also computed by
     bf2relation_fullcnf_fcs(des_sbox_bf(5),6). </li>
    </ol>
   </li>
   <li> The minimum CNF representation has at most 67 clauses. See
   "Using weighted MaxSAT to compute small CNFs". </li>
  </ul>


  \todo Using weighted MaxSAT to compute small CNFs (mincl_rinf <= 67)
  <ul>
   <li> Computing the weighted MaxSAT problem:
   \verbatim
shell> QuineMcCluskeySubsumptionHypergraph-n16-O3-DNDEBUG DES_Sbox_5_fullCNF.cnf > DES_Sbox_5_shg.cnf
shell> cat DES_Sbox_5_shg.cnf | MinOnes2WeightedMaxSAT-O3-DNDEBUG > DES_Sbox_5_shg.wcnf
   \endverbatim
   </li>
   <li> Running then:
   \verbatim
shell> ubcsat-okl  -alg gsat -w -runs 100 -cutoff 400000 -wtarget 67 -solve 1 -seed 1876503362 -i DES_Sbox_5_shg.wcnf -r model DES_Sbox_5_s67.ass;
shell> cat DES_Sbox_5_fullCNF.cnf_primes | FilterDimacs DES_Sbox_5_s67.ass > DES_Sbox_5_s67.cnf
shell> cat DES_Sbox_5_s67.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
10 67 375 0 375 1 1
 length count
5 29
6 36
7 2
   \endverbatim
   </li>
   <li> The hardness of this "minimum" representation is 2:
    <ul>
     <li> See "Hardness of boolean function representations" in
     Experimentation/Investigations/BooleanFunctions/plans/general.hpp
     for a description of the notion of hardness, and method of computation.
     </li>
     <li> Computing the hardness:
     \verbatim
maxima> Sbox_min_F : read_fcl_f("DES_Sbox_5_s67.cnf")$
maxima> Sbox_primes_F : read_fcl_f("DES_Sbox_5_fullCNF.cnf_primes")$
maxima> hardness_wpi(setify(Sbox_min_F[2]), setify(Sbox_primes_F[2]));
2
     \endverbatim
     </li>
    </ul>
   </li>
  </ul>


  \todo 1-base : mincl_r1 <= 134
  <ul>
   <li> Computing an 1-base
   \verbatim
shell> QuineMcCluskey-n16-O3-DNDEBUG DES_Sbox_5_fullCNF.cnf > DES_Sbox_5_pi.cnf
shell> RandomShuffleDimacs-O3-DNDEBUG 67 < DES_Sbox_5_pi.cnf | SortByClauseLength-O3-DNDEBUG > DES_Sbox_5_sortedpi.cnf
shell> RUcpGen-O3-DNDEBUG DES_Sbox_5_sortedpi.cnf > DES_Sbox_5_gen.cnf
shell> RandomShuffleDimacs-O3-DNDEBUG 2 < DES_Sbox_5_gen.cnf | SortByClauseLengthDescending-O3-DNDEBUG | RUcpBase-O3-DNDEBUG > DES_Sbox_5_1base.cnf
shell> cat DES_Sbox_5_1base.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
10 134 726 0 726 0 1
 length count
5 78
6 56
   \endverbatim
   </li>
  </ul>


  \todo Move Sbox-1-specific investigations here

*/
