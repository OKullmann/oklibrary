// Matthew Gwynne, 27.3.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/DataEncryptionStandard/plans/KeyDiscovery.hpp
  \brief On investigations into the Data Encryption Standard key discovery


  \todo Transferring the Argosat-desgen example
  <ul>
   <li> Exactly as with argosat-desgen-sat09.tar.bz2, we shall consider
   key 15FBC08D31B0D521, plaintext 038E596D4841D03B and ciphertext
   A2FB6032638EC79D, and providing the examples for all 0 <= p <= 64,
   where the last p keybits are provided (as unit-clauses). </li>
   <li> This can be easily achieved by using function des_key2fcl(key),
   where the first 64-p entries of the 64-bit vector key' are replaced by
   the value "und" (while by hexstr2binv(key) we obtain the key as boolean
   vector). </li>
   <li> Their benchmarks only consider 13 <= 64-p <= 34, d.h.,
   30 <= p <= 51, however we can consider the whole range. </li>
   <li> We consider then all translations, and also many solvers. </li>
   <li> Running solvers on "gss-13-s100.cnf" (first 13 key bits unknown):
    <ul>
     <li> CryptoMiniSat solves it in 7s with 5423 conflicts and 6126
     decisions. </li>
     <li> MiniSat-2.2.0 solves it in 3.72s with 4163 conflicts and 4782 
     decisions. </li>
     <li> Glucose solves it in 0.61s with 246 conflicts and 796 decisions.
     </li>
     <li> precosat236 solves it in 10.2s with 7197 conflicts and 10406
     decisions. </li>
     <li> precosat570 solves it in 3.5s with 1530 conflicts and 2862
     decisions. </li>
     <li> OKsolver doesn't terminate in over 2 hours. </li>
    </ul>
   </li>
   <li> Running solvers on "gss-16-s100.cnf" (first 16 key bits unknown):
    <ul>
     <li> CryptoMiniSat solves it in 104.38s with 115187 conflicts and 
     130480 decisions. </li>
     <li> MiniSat-2.2.0 solves it in 6.76s with 6260 conflicts and 7575
     decisions. </li>
     <li> Glucose solves it in 133.32s with 63860 conflicts and 72619
     decisions. </li>
     <li> precosat236 solves it in 32.1s with 25158 conflicts and 34364
     decisions. </li>
     <li> precosat570 solves it in 53.8s with 29307 conflicts and 42929
     decisions. </li>
     <li> OKsolver doesn't terminate in over 2 hours. </li>
    </ul>
   </li>
  </ul>


  \todo Canonical translation comparison to Argosat-desgen example
  <ul>
   <li> Generating the instances for 16 rounds using the canonical translation:
   \verbatim
unknown_bits : 13$
sbox_fcl_l : create_list(dualts_fcl([listn(10), des_sbox_fulldnf_cl(i)]), i, 1, 8)$
F : des2fcl(sbox_fcl_l)$
P : des_plain2fcl(hexstr2binv("038E596D4841D03B"))$
C : des_cipher2fcl(hexstr2binv("A2FB6032638EC79D"))$
K : des_key2fcl(append(create_list(und,i,1,unknown_bits), rest(hexstr2binv("15FBC08D31B0D521"),unknown_bits)))$
F_std : standardise_fcs([F[1],append(F[2],P[2],K[2],C[2])])$
output_fcs_v(sconcat("DES ArgoSat comparison over 16 rounds with the first ", unknown_bits, " key bits undefined."), F_std , sconcat("des_argocomp_b",unknown_bits,".cnf"), F_std[2]);
   \endverbatim
   </li>
   <li> Instance with first key bit unknown is trivial. All solvers solve
   it in under a second and with 0,..,2 decisions. </li>
   <li> Instance with first 2 key bits unknown is trivial. All solvers solve
   it in under a second and with 0,..,3 decisions. </li>
   <li> Running solvers on instance with first 4 key bits unknown:
    <ul>
     <li> CryptoMiniSat solves it in 0.12s with 0 conflicts and 1 decision. 
     </li>
     <li> MiniSat-2.2.0 solves it in 0.05s with 11 conflicts and 34 
     decisions. </li>
     <li> Glucose solves it in 0.03s with 8 conflicts and 17 decisions.
     </li>
     <li> precosat236 solves it in 0.1s with 0 conflicts and 0
     decisions. </li>
     <li> precosat570 solves it in 0.3s with 12 conflicts and 13
     decisions. </li>
     <li> OKsolver solvers it in 0.1s with 1 nodes and max tree depth
     0. </li>
    </ul>
   </li>
   <li> Running solvers on instance with first 8 key bits unknown:
    <ul>
     <li> CryptoMiniSat solves it in 0.5s with 180 conflicts and 211 
     decisions. </li>
     <li> MiniSat-2.2.0 solves it in 0.09s with 104 conflicts and 409 
     decisions. </li>
     <li> Glucose solves it in 0.14s with 132 conflicts and 170 decisions.
     </li>
     <li> precosat236 solves it in 0.4s with 11 conflicts and 16
     decisions. </li>
     <li> precosat570 solves it in 0.4s with 105 conflicts and 123
     decisions. </li>
     <li> OKsolver solvers it in 1s with 2 nodes and max tree depth
     1. </li>
    </ul>
   </li>
   <li> Running solvers on instance with first 13 key bits unknown:
    <ul>
     <li> CryptoMiniSat solves it in 0.52s with 189 conflicts and 223
     decisions. </li>
     <li> MiniSat-2.2.0 solves it in 2.06s with 3619 conflicts and 3967 
     decisions. </li>
     <li> Glucose solves it in 3.63s with 6005 conflicts and 7229 decisions.
     </li>
     <li> precosat236 solves it in 3.5s with 3755 conflicts and 4391
     decisions. </li>
     <li> precosat570 solves it in 2.4s with 2269 conflicts and 3071
     decisions. </li>
     <li> OKsolver solvers it in 73.5s with 203 nodes and max tree depth
     7. </li>
    </ul>
   </li>
   <li> Running solvers on instance with first 16 key bits unknown:
    <ul>
     <li> CryptoMiniSat solves it in 2.83s with 4748 conflicts and 5217
     decisions. </li>
     <li> MiniSat-2.2.0 solves it in 6.78s with 12121 conflicts and 13630
     decisions. </li>
     <li> Glucose solves it in 23.49s with 36659 conflicts and 43833 
     decisions. </li>
     <li> precosat236 solves it in 1.1s with 894 conflicts and 1173
     decisions. </li>
     <li> precosat570 solves it in 0.4s with 70 conflicts and 140
     decisions. </li>
     <li> OKsolver solvers it in 270.8s with 752 nodes and max tree depth
     10. </li>
    </ul>
   </li>
  </ul>


  \todo Move into separate sub-module

*/
