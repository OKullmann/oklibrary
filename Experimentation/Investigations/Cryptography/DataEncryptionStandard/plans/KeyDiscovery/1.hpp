// Matthew Gwynne, 25.5.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/DataEncryptionStandard/plans/KeyDiscovery/1.hpp
  \brief On investigations into the one-round Data Encryption Standard key discovery

  \todo Overview
  <ul>
   <li> For initial experiments we use the Argosat-desgen plaintext-ciphertext
   pairs. See "Transferring the Argosat-desgen example" in
   Investigations/Cryptography/DataEncryptionStandard/plans/KeyDiscovery/KnownKeyBits.hpp.
   </li>
   <li> Using the:
    <ul>
     <li> canonical translation; most (but not all) solvers solve with no
     conflicts. See "Using the canonical translation". </li>
     <li> 1-base translation; most (but not all) solvers solve with no
     conflicts. See "Using the 1-base translation". OKsolver needs some
     backtracking. </li>
    </ul>
   </li>
  </ul>


  \todo Using the canonical translation
  <ul>
   <li> Generating the instance:
   \verbatim
rounds : 1$
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
  sconcat("des_argocomp_r",rounds,"_b",unknown_bits,".cnf"),
  F_std[2])$
print("DONE!");
   \endverbatim
   </li>
   <li> Most solvers solve without any conflicts/backtracks (cryptominisat,
   precosat236, precosat-570.1, minisat-2.2.0, minisat2, glucose, satz215,
   sat-grasp). </li>
   <li> Some solvers do take some conflicts/nodes (picosat:9, march_pl:25).
   </li>
   <li> OKsolver_2002 also solves without any decisions:
   \verbatim
c number_of_initial_unit-eliminations   608
c running_time(sec)                     0.0
c number_of_nodes                       1
c number_of_2-reductions                7
c number_of_autarkies                   8
   \endverbatim
   </li>
   <li> Are we just lucky that some solvers happen to pick the right
   variables? </li>
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
rounds : 1$
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
  sconcat("des_argocomp_r",rounds,"_b",unknown_bits,".cnf"),
  F_std[2])$
print("DONE!");
   \endverbatim
   </li>
   <li> Most solvers solve without any conflicts/backtracks (cryptominisat,
   precosat236, precosat-570.1, minisat-2.2.0, minisat2, satz215, sat-grasp).
   </li>
   <li> Some solvers do take some conflicts/nodes (picosat:3, march_pl:17,
   glucose:2).
   </li>
   <li> OKsolver_2002 now needs some decisions to solve:
   \verbatim
c running_time(sec)                     0.0
c number_of_nodes                       7
c number_of_quasi_single_nodes          1
c number_of_2-reductions                3
c number_of_autarkies                   8
c number_of_1-autarkies                 24
   \endverbatim
   </li>
   <li> Are we just lucky that some solvers happen to pick the right
   variables? </li>
  </ul>

*/
