// Matthew Gwynne, 16.5.2011 (Swansea)
/* Copyright 2011, 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/DataEncryptionStandard/plans/Sboxes/Sbox_6.hpp
  \brief On investigations into Sbox six of the Data Encryption Standard


  \todo Basic data
  <ul>
   <li> Generating the full CNF representation:
    <ol>
     <li> The CNF-file "DES_Sbox_6_fullCNF.cnf" is created by the Maxima-function
     output_dessbox_fullcnf_stdname(6) in
     ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/DataEncryptionStandard/Sboxes.mac,
     which is a full clause-set with 10
     variables and 2^10 - 2^6 = 960 clauses:
     \verbatim
> cat DES_Sbox_6_fullCNF.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
10 960 9600 0 9600 1 1
 length count
10 960
     \endverbatim
     </li>
     <li> This clause-set is also computed by
     bf2relation_fullcnf_fcs(des_sbox_bf(6),6). </li>
    </ol>
   </li>
   <li> The minimum CNF representation has at most 66 clauses. See
   "Using weighted MaxSAT to compute small CNFs". </li>
  </ul>


  \todo Using weighted MaxSAT to compute small CNFs (mincl_rinf <= 66)
  <ul>
   <li> Computing the weighted MaxSAT problem:
   \verbatim
shell> QuineMcCluskeySubsumptionHypergraph-n16-O3-DNDEBUG DES_Sbox_6_fullCNF.cnf > DES_Sbox_6_shg.cnf
shell> cat DES_Sbox_6_shg.cnf | MinOnes2WeightedMaxSAT-O3-DNDEBUG > DES_Sbox_6_shg.wcnf
   \endverbatim
   </li>
   <li> Running then:
   \verbatim
shell> ubcsat-okl  -alg gsat -w -runs 100 -cutoff 400000 -wtarget 66 -solve 1 -seed 68018538 -i DES_Sbox_6_shg.wcnf -r model DES_Sbox_6_s66.ass;
shell> cat DES_Sbox_6_fullCNF.cnf_primes | FilterDimacs DES_Sbox_6_s66.ass > DES_Sbox_6_s66.cnf
shell> cat DES_Sbox_6_s66.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
10 66 368 0 368 1 1
 length count
5 28
6 38
   \endverbatim
   </li>
   <li> The hardness of this "minimum" representation is 3:
    <ul>
     <li> See "Hardness of boolean function representations" in
     Experimentation/Investigations/BooleanFunctions/plans/general.hpp
     for a description of the notion of hardness, and method of computation.
     </li>
     <li> Computing the hardness:
     \verbatim
maxima> Sbox_min_F : read_fcl_f("DES_Sbox_6_s66.cnf")$
maxima> Sbox_primes_F : read_fcl_f("DES_Sbox_6_fullCNF.cnf_primes")$
maxima> hardness_wpi_cs(setify(Sbox_min_F[2]), setify(Sbox_primes_F[2]));
3
     \endverbatim
     </li>
    </ul>
   </li>
  </ul>


  \todo 1-base : mincl_r1 <= 136
  <ul>
   <li> The 1-bases below need to be checked to ensure they are actually
   1-bases; see "Computing r_1-bases for a set of prime implicates" in
   Satisfiability/Reductions/Bases/plans/UcpBase.hpp. </li>
   <li> Computing an 1-base
   \verbatim
shell> QuineMcCluskey-n16-O3-DNDEBUG DES_Sbox_6_fullCNF.cnf > DES_Sbox_6_pi.cnf
shell> RandomShuffleDimacs-O3-DNDEBUG 327 < DES_Sbox_6_pi.cnf | SortByClauseLength-O3-DNDEBUG > DES_Sbox_6_sortedpi.cnf
shell> RUcpGen-O3-DNDEBUG DES_Sbox_6_sortedpi.cnf > DES_Sbox_6_gen.cnf
shell> RandomShuffleDimacs-O3-DNDEBUG 1 < DES_Sbox_6_gen.cnf | SortByClauseLengthDescending-O3-DNDEBUG | RUcpBase-O3-DNDEBUG > DES_Sbox_6_1base.cnf
shell> cat DES_Sbox_6_1base.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
10 136 733 0 733 0 1
 length count
5 83
6 53
   \endverbatim
   </li>
  </ul>


  \todo Move Sbox-1-specific investigations here

*/
