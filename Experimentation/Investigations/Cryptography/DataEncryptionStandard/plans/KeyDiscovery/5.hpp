// Matthew Gwynne, 25.5.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/DataEncryptionStandard/plans/KeyDiscovery/5.hpp
  \brief On investigations into the five-round Data Encryption Standard key discovery

  \todo Overview
  <ul>
   <li> For initial experiments we use the Argosat-desgen plaintext-ciphertext
   pairs. See "Transferring the Argosat-desgen example" in
   Investigations/Cryptography/DataEncryptionStandard/plans/KeyDiscovery/KnownKeyBits.hpp.
   </li>
   <li> Using the:
    <ul>
     <li> 1-base translation; fastest solver solves in 553s.
     See "Using the 1-base translation". </li>
     <li> canonical translation; fastest solver solves in 567s.
     See "Using the canonical translation". </li>
     <li> "minimum" translation; fastest solver solves in 6 days.
     See 'Using the "minimum"  translation'. </li>
   </li>
  </ul>


  \todo Using the 1-base translation
  <ul>
   <li> Generating the 1-bases:
   \verbatim
maxima> for i : 1 thru 8 do output_dessbox_fullcnf_stdname(i)$
shell> gen_seed[1]=7;gen_seed[2]=71;gen_seed[3]=185;gen_seed[4]=346;gen_seed[5]=67;gen_seed[6]=327;gen_seed[7]=148;gen_seed[8]=167;
shell> base_seed[1]=1;base_seed[2]=1;base_seed[3]=2;base_seed[4]=4;base_seed[5]=2;base_seed[6]=1;base_seed[7]=2;base_seed[8]=1;
shell> for i in $(seq 1 8); do
  QuineMcCluskey-n16-O3-DNDEBUG DES_Sbox_${i}_fullCNF.cnf > DES_Sbox_${i}_pi.cnf;
  RandomShuffleDimacs-O3-DNDEBUG ${gen_seed[$i]} < DES_Sbox_${i}_pi.cnf | SortByClauseLength-O3-DNDEBUG > DES_Sbox_${i}_sortedpi.cnf;
  RUcpGen-O3-DNDEBUG DES_Sbox_${i}_sortedpi.cnf > DES_Sbox_${i}_gen.cnf;
  RandomShuffleDimacs-O3-DNDEBUG ${base_seed[$i]}  < DES_Sbox_${i}_gen.cnf | SortByClauseLengthDescending-O3-DNDEBUG | RUcpBase-O3-DNDEBUG > DES_Sbox_${i}_1base.cnf;
done
   </li>
   <li> The numbers of clauses in the 1-bases are 124, 129, 138, 128, 134,
   136, 123, 152 respectively. </li>
   <li> All the 1-bases used have clauses of sizes 5 and 6, except Sbox 4
   which has clauses of size 5 and 6 as well as 2 of size 7. </li>
   <li> Generating the instance:
   \verbatim
rounds : 5$
sbox_fcl_l : create_list(read_fcl_f(sconcat("DES_Sbox_",i,"_1base.cnf")), i, 1, 8)$
P_hex : "038E596D4841D03B"$
K_hex : "15FBC08D31B0D521"$
C_hex : des_encryption_hex_gen(rounds, "038E596D4841D03B","15FBC08D31B0D521")$
P : des_plain2fcl_gen(hexstr2binv(P_hex),rounds)$
C : des_cipher2fcl_gen(hexstr2binv(C_hex),rounds)$
F : des2fcl_gen(sbox_fcl_l,rounds)$
F_std : standardise_fcs([F[1],append(F[2],P[2],C[2])])$
output_fcs_v(
  sconcat("DES ArgoSat comparison over ",rounds," rounds"),
  F_std[1],
  sconcat("des_argocomp_r",rounds,".cnf"),
  F_std[2])$
print("DONE!");
   \endverbatim
   </li>
   <li> Solvers (t:time,c:conflicts,n:nodes): cryptominisat (t:553s,c:3198466),
   minisat-2.2.0 (t:13363s,c:183335114), OKsolver_2002 (t:36760s,n:70636225).
   </li>
   <li> precosat236 and precosat-570.1 are still running after 13 hours. </li>
  </ul>


  \todo Using the "minimum" translation
  <ul>
   <li> Generating the "minimum" CNFs for the Sboxes:
   \verbatim
maxima> for i : 1 thru 8 do output_dessbox_fullcnf_stdname(i)$
shell> for i in $(seq 1 8); do
  QuineMcCluskeySubsumptionHypergraph-n16-O3-DNDEBUG DES_Sbox_${i}_fullCNF.cnf > DES_Sbox_${i}_shg.cnf;
  cat DES_Sbox_${i}_shg.cnf | MinOnes2WeightedMaxSAT-O3-DNDEBUG > DES_Sbox_${i}_shg.wcnf;
done
shell> ubcsat-okl  -alg gsat -w -runs 100 -cutoff 400000 -wtarget 67 -solve 1 -seed 2444475534 -i DES_Sbox_1_shg.wcnf -r model DES_Sbox_1.ass;
shell> ubcsat-okl  -alg gsat -w -runs 100 -cutoff 400000 -wtarget 67 -solve 1 -seed 2521057446 -i DES_Sbox_2_shg.wcnf -r model DES_Sbox_2.ass;
shell> ubcsat-okl  -alg gsat -w -runs 100 -cutoff 400000 -wtarget 68 -solve 1 -seed 3544367510 -i DES_Sbox_3_shg.wcnf -r model DES_Sbox_3.ass;
shell> ubcsat-okl  -alg gsat -w -runs 100 -cutoff 400000 -wtarget 69 -solve 1 -seed 3808694681 -i DES_Sbox_4_shg.wcnf -r model DES_Sbox_4.ass;
shell> ubcsat-okl  -alg gsat -w -runs 100 -cutoff 400000 -wtarget 67 -solve 1 -seed 1876503362 -i DES_Sbox_5_shg.wcnf -r model DES_Sbox_5.ass;
shell> ubcsat-okl  -alg gsat -w -runs 100 -cutoff 400000 -wtarget 66 -solve 1 -seed 68018538 -i DES_Sbox_6_shg.wcnf -r model DES_Sbox_6.ass;
shell> ubcsat-okl  -alg gsat -w -runs 100 -cutoff 400000 -wtarget 67 -solve 1 -seed 1856244582 -i DES_Sbox_7_shg.wcnf -r model DES_Sbox_7.ass;
shell> ubcsat-okl  -alg gsat -w -runs 100 -cutoff 400000 -wtarget 69 -solve 1 -seed 4223500633 -i DES_Sbox_8_shg.wcnf -r model DES_Sbox_8.ass;
shell> for i in $(seq 1 8); do
  cat DES_Sbox_${i}_fullCNF.cnf_primes | FilterDimacs DES_Sbox_${i}.ass > DES_Sbox_${i}_min.cnf;
done
   </li>
   <li> The numbers of clauses in the CNFs are 67, 67, 68, 69, 67, 66, 67, and
   69 respectively. </li>
   <li> Generating the instance:
   \verbatim
rounds : 5$
sbox_fcl_l : create_list(read_fcl_f(sconcat("DES_Sbox_",i,"_min.cnf")), i, 1, 8)$
P_hex : "038E596D4841D03B"$
K_hex : "15FBC08D31B0D521"$
C_hex : des_encryption_hex_gen(rounds, "038E596D4841D03B","15FBC08D31B0D521")$
P : des_plain2fcl_gen(hexstr2binv(P_hex),rounds)$
C : des_cipher2fcl_gen(hexstr2binv(C_hex),rounds)$
F : des2fcl_gen(sbox_fcl_l,rounds)$
F_std : standardise_fcs([F[1],append(F[2],P[2],C[2])])$
output_fcs_v(
  sconcat("DES ArgoSat comparison over ",rounds," rounds"),
  F_std[1],
  sconcat("des_argocomp_r",rounds,".cnf"),
  F_std[2])$
print("DONE!");
   \endverbatim
   </li>
   <li> cryptominisat solves in 520,000s (~6 days):
   \verbatim
c static restarts          : 629
c full restarts            : 8
c total simplify time      : 0.01
c conflicts                : 247428989   (3780713.58 / sec)
c decisions                : 262651549   (0.19      % random)
   \endverbatim
   </li>
   <li> OKsolver doesn't finish in 550,000s (> 6 days). </li>
  </ul>


  \todo Using the canonical translation
  <ul>
   <li> Generating the instance:
   \verbatim
rounds : 5$
sbox_fcl_l : create_list(dualts_fcl([listify(setn(10)), des_sbox_fulldnf_cl(i)]), i, 1, 8)$
P_hex : "038E596D4841D03B"$
K_hex : "15FBC08D31B0D521"$
C_hex : des_encryption_hex_gen(rounds, "038E596D4841D03B","15FBC08D31B0D521")$
P : des_plain2fcl_gen(hexstr2binv(P_hex),rounds)$
C : des_cipher2fcl_gen(hexstr2binv(C_hex),rounds)$
F : des2fcl_gen(sbox_fcl_l,rounds)$
F_std : standardise_fcs([F[1],append(F[2],P[2],C[2])])$
output_fcs_v(
  sconcat("DES ArgoSat comparison over ",rounds," rounds"),
  F_std[1],
  sconcat("des_argocomp_r",rounds,".cnf"),
  F_std[2])$
print("DONE!");
   \endverbatim
   </li>
   <li> precosat236 solves in 567s with 2,423,412 conflicts:
   \verbatim
c 2423412 conflicts, 4652299 decisions, 2044 random
c 0 iterations, 4094 restarts, 0 skipped
c 567.9 seconds, 57 MB max, 1479 MB recycled
   \endverbatim
   </li>
   <li> minisat-2.2.0 solves in 17221s using 40,018,619 conflicts:
   \verbatim
shell> minisat-2.2.0 des_argocomp_r4.cnf
restarts              : 56057
conflicts             : 40018619       (612167 /sec)
decisions             : 82938445       (0.00 % random) (1268714 /sec)
propagations          : 13510152814    (206665548 /sec)
conflict literals     : 7079980812     (48.30 % deleted)
   \endverbatim
   </li>
   <li> cryptominisat, precosat-570.1, and the OKsolver_2002 are
   still running after 13 hours. </li>
  </ul>

*/
