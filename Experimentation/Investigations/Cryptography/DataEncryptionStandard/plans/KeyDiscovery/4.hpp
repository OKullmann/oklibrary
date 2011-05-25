// Matthew Gwynne, 25.5.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/DataEncryptionStandard/plans/KeyDiscovery/4.hpp
  \brief On investigations into the one-round Data Encryption Standard key discovery

  \todo Overview
  <ul>
   <li> For initial experiments we use the Argosat-desgen plaintext-ciphertext
   pairs. See "Transferring the Argosat-desgen example" in
   Investigations/Cryptography/DataEncryptionStandard/plans/KeyDiscovery/KnownKeyBits.hpp.
   </li>
   <li> All translations are solved in < 0.1s. </li>
   <li> Using the:
    <ul>
     <li> canonical translation; fastest solver solves in 31s, all in less
     than 330s. See "Using the canonical translation". </li>
   </li>
  </ul>


  \todo Using the canonical translation
  <ul>
   <li> Generating the instance:
   \verbatim
rounds : 4$
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
   <li> Solvers (t:time,c:conflicts,n:nodes): glucose (t:39.31s,c:178662),
   minisat-2.2.0 (t:131s,c:555383), cryptominisat (t:195s,c:676495),
   precosat236 (t:306s,c:1144952), OKsolver_2002 (t:327s,n:5601). </li>
   </li>
  </ul>

*/
