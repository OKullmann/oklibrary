// Matthew Gwynne, 1.4.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/DataEncryptionStandard/plans/Sboxes/general.hpp
  \brief On investigations into the Data Encryption Standard


  \todo Links
  <ul>
   <li> Investigations into specific Sboxes:
    <ol>
     <li> Sbox 1: Investigations/Cryptography/DataEncryptionStandard/plans/Sboxes/Sbox_1.hpp.
     </li>
     <li> Sbox 2: Investigations/Cryptography/DataEncryptionStandard/plans/Sboxes/Sbox_2.hpp.
     </li>
     <li> Sbox 3: Investigations/Cryptography/DataEncryptionStandard/plans/Sboxes/Sbox_3.hpp.
     </li>
     <li> Sbox 4: Investigations/Cryptography/DataEncryptionStandard/plans/Sboxes/Sbox_4.hpp.
     </li>
     <li> Sbox 5: Investigations/Cryptography/DataEncryptionStandard/plans/Sboxes/Sbox_5.hpp.
     </li>
     <li> Sbox 6: Investigations/Cryptography/DataEncryptionStandard/plans/Sboxes/Sbox_6.hpp.
     </li>
     <li> Sbox 7: Investigations/Cryptography/DataEncryptionStandard/plans/Sboxes/Sbox_7.hpp.
     </li>
     <li> Sbox 8: Investigations/Cryptography/DataEncryptionStandard/plans/Sboxes/Sbox_8.hpp.
     </li>
    </ol>
   </li>
  </ul>


  \todo Analysing the S-boxes
  <ul>
   <li> Most urgent is to use all our instruments to analyse the 8 S-boxes.
   </li>
   <li> Of course, starting with defining them at Maxima-level. </li>
   <li> Considering them as one 10-bit boolean function, or as 4 6-bit
   boolean functions. Likely better the first view, but we need to consider
   all possibilities. </li>
   <li> We need to investigate: what are the defining properties of the DES
   S-boxes?
    <ul>
     <li> Are there properties of the DES S-boxes such that we can define
     each S-box without simply specifying the complete truth table? </li>
     <li> This is useful for understanding the S-boxes and also for testing.
     </li>
     <li> See "Improve tests" in
     ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/DataEncryptionStandard/plans/general.hpp.
     </li>
    </ul>
   </li>
   <li> See "Add 6-to-1 bit Sbox functions" in
   ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/DataEncryptionStandard/plans/general.hpp.
   </li>
   <li> Determining the number of prime implicates, data on the subsumption
   hypergraph, and minimum CNFs, and r_0-,r_1-bases. </li>
   <li> Here also minimum DNF-representations are of interest! </li>
   <li> They can be used to yield pseudo-canonical translations. </li>
   <li> Analysis of the prime clauses:
    <ol>
     <li> Creating the CNF/DNF files:
     \verbatim
OKplatform> mkdir EXP_DES
OKplatform> cd EXP_DES/
EXP_DES> oklib --maxima

oklib_load("OKlib/ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/DataEncryptionStandard/Sboxes.mac");
for i : 1 thru 8 do (output_dessbox_fulldnf_stdname(i),output_dessbox_fullcnf_stdname(i));
     \endverbatim
     </li>
     <li> Prime-clause statistics:
     \verbatim
EXP_DES> for F in *.cnf; do QuineMcCluskeySubsumptionHypergraphFullStatistics-n16-O3-DNDEBUG ${F}; done

EXP_DES> for F in DES_Sbox_?_fullDNF.cnf_primes_stats; do cat ${F}; done

EXP_DES> for F in DES_Sbox_?_fullCNF.cnf_primes_stats; do cat ${F}; done

 n non_taut_c red_l taut_c orig_l comment_count finished_bool
10 1624 9554 0 9554 0 1
 length count
5 243
6 1328
7 53

10 1844 10967 0 10967 0 1
5 154
6 1633
7 57

10 1767 10458 0 10458 0 1
5 219
6 1473
7 75

10 1881 11197 0 11197 0 1
5 153
6 1664
7 64

10 1812 10768 0 10768 0 1
5 174
6 1568
7 70

10 1705 10115 0 10115 0 1
5 204
6 1412
7 89

10 1673 9891 0 9891 0 1
5 228
6 1364
7 81

10 2047 12227 0 12227 0 1
5 106
6 1890
7 51
     \endverbatim
     </li>
     <li> Subsumption hypergraph statistics:
     \verbatim
> sbox1_df = read.table(paste("DES_Sbox_",1,"_fullCNF.cnf_shg_stats",sep=""), head=TRUE, skip=2)
> summary(sbox1_df)
     length          count
 Min.   :10.00   Min.   : 0.00
 1st Qu.:22.25   1st Qu.: 5.00
 Median :34.50   Median :19.00
 Mean   :34.50   Mean   :19.20
 3rd Qu.:46.75   3rd Qu.:31.75
 Max.   :59.00   Max.   :48.00
> sbox2_df = read.table(paste("DES_Sbox_",2,"_fullCNF.cnf_shg_stats",sep=""), head=TRUE, skip=2)
> summary(sbox2_df)
     length          count
 Min.   :11.00   Min.   : 0.00
 1st Qu.:23.75   1st Qu.: 3.75
 Median :36.50   Median :19.50
 Mean   :36.50   Mean   :18.46
 3rd Qu.:49.25   3rd Qu.:30.00
 Max.   :62.00   Max.   :56.00
> sbox3_df = read.table(paste("DES_Sbox_",3,"_fullCNF.cnf_shg_stats",sep=""), head=TRUE, skip=2)
> summary(sbox3_df)
     length         count
 Min.   : 7.0   Min.   : 0.00
 1st Qu.:20.5   1st Qu.: 3.00
 Median :34.0   Median :17.00
 Mean   :34.0   Mean   :17.45
 3rd Qu.:47.5   3rd Qu.:29.50
 Max.   :61.0   Max.   :49.00
> sbox4_df = read.table(paste("DES_Sbox_",4,"_fullCNF.cnf_shg_stats",sep=""), head=TRUE, skip=2)
> summary(sbox4_df)
     length          count
 Min.   :12.00   Min.   : 0
 1st Qu.:23.75   1st Qu.: 7
 Median :35.50   Median :21
 Mean   :35.50   Mean   :20
 3rd Qu.:47.25   3rd Qu.:32
 Max.   :59.00   Max.   :52
> sbox5_df = read.table(paste("DES_Sbox_",5,"_fullCNF.cnf_shg_stats",sep=""), head=TRUE, skip=2)
> summary(sbox5_df)
     length         count
 Min.   :11.0   Min.   : 0.00
 1st Qu.:23.5   1st Qu.: 5.50
 Median :36.0   Median :19.00
 Mean   :36.0   Mean   :18.82
 3rd Qu.:48.5   3rd Qu.:29.00
 Max.   :61.0   Max.   :43.00
> sbox6_df = read.table(paste("DES_Sbox_",6,"_fullCNF.cnf_shg_stats",sep=""), head=TRUE, skip=2)
> summary(sbox6_df)
     length          count
 Min.   :11.00   Min.   : 0.00
 1st Qu.:23.75   1st Qu.: 3.75
 Median :36.50   Median :15.00
 Mean   :36.50   Mean   :18.46
 3rd Qu.:49.25   3rd Qu.:31.50
 Max.   :62.00   Max.   :56.00
> sbox7_df = read.table(paste("DES_Sbox_",7,"_fullCNF.cnf_shg_stats",sep=""), head=TRUE, skip=2)
> summary(sbox7_df)
     length       count
 Min.   :11   Min.   : 0.00
 1st Qu.:23   1st Qu.: 4.00
 Median :35   Median :21.00
 Mean   :35   Mean   :19.59
 3rd Qu.:47   3rd Qu.:30.00
 Max.   :59   Max.   :53.00
> sbox8_df = read.table(paste("DES_Sbox_",8,"_fullCNF.cnf_shg_stats",sep=""), head=TRUE, skip=2)
> summary(sbox8_df)
     length         count
 Min.   :12.0   Min.   : 1.00
 1st Qu.:24.5   1st Qu.: 5.50
 Median :37.0   Median :17.00
 Mean   :37.0   Mean   :18.82
 3rd Qu.:49.5   3rd Qu.:28.00
 Max.   :62.0   Max.   :45.00
     \endverbatim
     </li>
     <li> All curves k -> nr (clause-length to number of occurrences) look
     like nice relatively symmetric curves. They have maximums around 30.
     Some (1,2,4) look slightly concave and some look slightly convex
     (3,5,6,7,8). </li>
     <li> The DNF-output shows that no resolutions are possible, and thus these
     boolean functions have unique DNF. </li>
     <li> Quite some differences regarding the prime implicates. S-box number
     1 is the easiest, number 8 is the hardest. </li>
     <li> This is also confirmed by r_1-bases: Box 1 has an r_1-base with 124
     clauses, while for box 8 only one with 152 clauses was found. </li>
     <li> The minimum representations seem to be of a similar size (from 66
     to 69 clauses). See "Investigations into specific Sboxes" under "Links".
     </li>
     <li> As a model one can study random 6 x 4 boolean functions. </li>
     <li> DONE Also minimum representations need to be studied. </li>
    </ol>
   </li>
   <li> DONE These considerations need a dedicated sub-module. </li>
  </ul>


  \todo Analysing the 6-to-1 bit Sbox functions
  <ul>
   <li> In general we consider each DES Sboxes as one whole 6-to-4 bit boolean
   function. </li>
   <li> However, the Sbox can be represented as 4 6-to-1 bit boolean
   functions. </li>
   <li> We should investigate the DNF and CNF representations, prime
   implicates, and so on, of these 6-to-1 bit representations. </li>
   <li> Prime implicates/implicants:
    <ul>
     <li> The number of prime implicants range from 50 to 68. All except
     5 (out of the 32) have less than 64 prime implicants. </li>
     <li> The DNFs are not unique. </li>
     <li> Generating the CNFs and prime implicates:
     \verbatim
maxima> for i : 1 thru 8 do for j : 1 thru 4 do output_dessbox_bit_fullcnf_stdname(i,j);
shell> for i in $(seq 1 8); do for j in $(seq 1 4); do QuineMcCluskey-n16-O3-DNDEBUG DES_Sbox_${i}_${j}_fullCNF.cnf > ${i}_${j}_primes; done; done
     \endverbatim
     </li>
     <li> The number of prime implicates for all DES S-box 6-to-1 bit functions
     is exactly the same as the number of prime implicants:
     \verbatim
oklib_load_all()$
prime1_len_l :  create_list(
      length(min_2resolution_closure_cs(setify( des_sbox_bit_fulldnf_cl(i,j) ))),
    i, 1,8,j,1,4)$
prime0_len_l :  create_list(
      length(min_2resolution_closure_cs( des_sbox_bit_fullcnf_fcs(i,j)[2] )),
    i, 1,8,j,1,4);
is(prime1_len_l = prime0_len_l);

[55,60,57,51,66,50,59,53,60,63,57,57,61,68,68,61,63,65,53,60,57,58,63,56,61,56,57,62,67,61,53,61]
true
     \endverbatim
     Why is this? </li>
    </ul>
   </li>
   <li> Minimum representations:
    <ul>
     <li> We can generate the minimum DNF and CNF representations like so:
     \verbatim
maxima> for i : 1 thru 8 do for j : 1 thru 4 do output_dessbox_bit_fulldnf_stdname(i,j)$
maxima> for i : 1 thru 8 do for j : 1 thru 4 do output_dessbox_bit_fullcnf_stdname(i,j)$
shell> for i in $(seq 1 8); do for j in $(seq 1 4); do $OKLIB/Satisfiability/Optimisation/minimise_cnf_cryptominisat DES_Sbox_${i}_${j}_fullDNF.cnf > ${i}_${j}_min_dnf; done; done
shell> for i in $(seq 1 8); do for j in $(seq 1 4); do $OKLIB/Satisfiability/Optimisation/minimise_cnf_cryptominisat DES_Sbox_${i}_${j}_fullCNF.cnf > ${i}_${j}_min_cnf; done; done
     \endverbatim
     </li>
     <li> The minimum DNF representations have between 30 and 46 clauses. </li>
     <li> Every minimum CNF representations for the 6-to-1 bit DES S-box
     functions have exactly the same number of clauses as their respective
     minimum DNF representations. Why is this? </li>
    </ul>
   </li>
   <li> See bf2nm2n1 in
   ComputerAlgebra/Satisfiability/Lisp/FiniteFunctions/Basics.mac.
   </li>
  </ul>

*/
