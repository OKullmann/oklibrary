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
   where the first 64-p entries of the 64-bit vector key are replaced by
   the value "und" (while by hexstr2binv(key) we obtain the key as boolean
   vector). </li>
   <li> Their benchmarks only consider 13 <= 64-p <= 34, d.h.,
   30 <= p <= 51, however we can consider the whole range. </li>
   <li> We consider then all translations, and also many solvers. </li>
   <li> The OKsolver takes at least four hours on any instance (still
   running). </li>
   <li> Solving time (ranked best to worst) on gss-X-s100.cnf:
    <ul>
     <li> X = 13 : glucose (0.61s), precosat-570.1 (3.5s), minisat-2.2.0
     (3.72s), cryptominisat (7s), precosat236 (10.2s). </li>
     <li> X = 14 : precosat-570.1 (2s), minisat-2.2.0 (12.35s), cryptominisat
     (22.33s), precosat236 (32s), glucose (58.53s). </li>
     <li> X = 15 : minisat-2.2.0 (5.43s), cryptominisat (10.71s),
     precosat-570.1 (44.7s), precosat236 (53.8s), glucose (68.75). </li>
     <li> X = 16 : minisat-2.2.0 (6.76s), precosat236 (32.1s), precosat-570.1
     (53.8s), cryptominisat (104.38s), glucose (133.32s) . </li>
     <li> X = 17 : minisat-2.2.0 (31.77s), precosat-570.1 (70.8s),
     precosat236 (109.8s), cryptominisat (144.73s), glucose (896.53s).
     </li>
     <li> X = 19 : minisat-2.2.0 (321.1s). </li>
     <li> X = 20 : minisat-2.2.0 (1376.75s). </li>
     <li> X = 21 : minisat-2.2.0 (6181.81s). </li>
     <li> X = 22 : minisat-2.2.0 (17361.05s). </li>
     <li> X = 23 : minisat-2.2.0 (2166.59s). </li>
     <li> X = 24 : minisat-2.2.0 (39297.07s). </li>
     <li> X = 25 : minisat-2.2.0 (239.77s). </li>
     <li> X = 26 : minisat-2.2.0 (>36000s - timed out). </li>
    </ul>
   </li>
   <li> Why are these times so erratic?
    <ul>
     <li> Randomly shuffling clause-list for X = 25: minisat-2.2.0 (>25h and
     still running).
     \verbatim
shell> cat gss-25-s100.cnf | RandomShuffleDimacs-O3-DNDEBUG > 25-shuffled_test.cnf
shell> minisat-2.2.0 25-shuffled_test.cnf
     \endverbatim
     </li>
     <li> It seems the solver was just "lucky" with the original clause
     order. </li>
    </ul>
   </li>
   <li> We should consider other plaintext-ciphertext pairs so we
   can get average runtimes. </li>
   <li> The ArgoSAT DES instances have already applied some UCP.
   This is after applying the plaintext-ciphertext assignment.
   This makes it difficult to apply different plaintext-ciphertext
   assignments. </li>
   <li> The ArgoSAT DES instances all have 64 unit-clauses. What are
   these unit-clauses? </li>
   <li> We should contact the ArgoSAT guys and ask for the generators.
   </li>
  </ul>


  \todo Summarise results
  <ul>
   <li> We need the results from each todo summarised in a highly
   visible todo/place. </li>
   <li> We also need to know which are the best solvers in each instance.
   </li>
  </ul>


  \todo Canonical+ translation comparison to Argosat-desgen example
  <ul>
   <li> Generating the instances for 16 rounds using the canonical translation:
??? this contradicts the title ???
??? what is translated ???
   \verbatim
unknown_bits : 13$
sbox_fcl_l : create_list(dualtsplus_fcl([listify(setn(10)), des_sbox_fulldnf_cl(i)]), i, 1, 8)$
F : des2fcl(sbox_fcl_l)$
P : des_plain2fcl(hexstr2binv("038E596D4841D03B"))$
C : des_cipher2fcl(hexstr2binv("A2FB6032638EC79D"))$
K : des_key2fcl(append(create_list(und,i,1,unknown_bits), rest(hexstr2binv("15FBC08D31B0D521"),unknown_bits)))$
F_std : standardise_fcs([F[1],append(F[2],P[2],K[2],C[2])])$
output_fcs_v(sconcat("DES ArgoSat comparison over 16 rounds with the first ", unknown_bits, " key bits undefined."), F_std[1] , sconcat("des_argocomp_b",unknown_bits,".cnf"), F_std[2]);

??? what are the sizes etc. ???

??? what is meant here with "DES ArgoSat comparison" ??? first of all, these
instances have nothing to do with "ArgoSat", and the main point is the
form of translation ! ???
   \endverbatim
   </li>
   <li> Instances with unknown key bits up to 18 all take less than a
   minute. This includes the OKsolver_2002. </li>
   <li> Solving time (ranked best to worst):
   <ul>
    <li> unknown_bits = 20: precosat-570.1 (50s), precosat236 (82s),
    minisat-2.2.0 (110s), cryptominisat (276s), . </li>
    <li> unknown_bits = 21: precosat236 (392s), minisat-2.2.0 (765s),
    cryptominisat (624s), OKsolver_2002 (1195s), precosat-570.1 (2771s). </li>
    <li> unknown_bits = 22: precosat-570.1 (2019s), minisat-2.2.0 (2367s),
    cryptominisat (2611s), OKsolver_2002 (6452s), precosat236 (6855s) . </li>
    <li> unknown_bits = 23: minisat-2.2.0 (149s), cryptominisat (1018s),
    precosat-570.1 (3091s), OKsolver_2002(4052s) precosat236 (11680s), . </li>
    <li> unknown_bits = 24: minisat-2.2.0 (148s), precosat-570.1 (3121s),
    OKsolver_2002 (4080s), cryptominisat (6581s), precosat236 (13015s). </li>
    <li> unknown_bits = 25: minisat-2.2.0 (11548s), cryptominisat (12975s),
    precosat236 (17554s), precosat-570.1 (23984s). </li>
    <li> unknown_bits = 26: precosat-570.1 (1681s), precosat236 (33395s),
    minisat-2.2.0 (>36000s), cryptominisat (>36000s). </li>
    </ul>
   </li>
   <li> These times are rather erratic. </li>
   <li> We should investigate with more keys, and also randomly permuting
   the clause-lists. </li>
  </ul>


  \todo Canonical translation comparison to Argosat-desgen example
  <ul>
   <li> Generating the instances for 16 rounds using the canonical translation:
   \verbatim
??? what is translated ???

unknown_bits : 13$
sbox_fcl_l : create_list(dualts_fcl([listify(setn(10)), des_sbox_fulldnf_cl(i)]), i, 1, 8)$
F : des2fcl(sbox_fcl_l)$
P : des_plain2fcl(hexstr2binv("038E596D4841D03B"))$
C : des_cipher2fcl(hexstr2binv("A2FB6032638EC79D"))$
K : des_key2fcl(append(create_list(und,i,1,unknown_bits), rest(hexstr2binv("15FBC08D31B0D521"),unknown_bits)))$
F_std : standardise_fcs([F[1],append(F[2],P[2],K[2],C[2])])$
output_fcs_v(sconcat("DES ArgoSat comparison over 16 rounds with the first ", unknown_bits, " key bits undefined."), F_std[1], sconcat("des_argocomp_b",unknown_bits,".cnf"), F_std[2]);

??? again "DES ArgoSat comparison" is inappropriate, and what are the
parameters ???

   \endverbatim
   </li>
   <li> Instances with unknown key bits up to 13 all take less than 5s,
   except the OKsolver_2002 which takes 73.5s (203 nodes). </li>
   <li> Solving time (ranked best to worst):
   <ul>
    <li> unknown_bits = 14: precosat-570.1 (2.6s), cryptominisat (3.96s),
    minisat-2.2.0 (4.66s), precosat236 (4.9s), glucose (10.1s),
    OKsolver_2002 (114s). </li>
    <li> unknown_bits = 15: precosat236-570.1 (0.4s), precosat236 (1.1s),
    cryptominisat (4.26s), minisat-2.2.0 (7.15s), glucose (12.9s),
    OKsolver_2002 (273.3s). </li>
    <li> unknown_bits = 16: precosat-570.1 (0.4s), precosat236 (1.1s),
    cryptominisat (2.83s), minisat-2.2.0 (6.78s), glucose (23.49s),
    OKsolver_2002 (270.8s). </li>
    <li> unknown_bits = 17: minisat-2.2.0 (6.96s), precosat236 (9s),
    cryptominisat (12.8s), precosat-570.1 (33.3s), glucose (58.94s).
    </li>
    <li> unknown_bits = 18: minisat-2.2.0 (20.57s), precosat236 (33.2s) .
    </li>
    <li> unknown_bits = 19: minisat-2.2.0 (68.14s), precosat236 (81.6s). </li>
    <li> unknown_bits = 20: minisat-2.2.0 (6.87s), precosat236 (160.9s) </li>
    <li> unknown_bits = 21: precosat236 (10.3s), minisat-2.2.0 (1715s).  </li>
    <li> unknown_bits = 22: precosat236 (32.5s), precosat-570.1 (71s),
    cryptominisat (769s), glucose (3088s), minisat-2.2.0 (5832s),
    OKsolver_2002 (16809s). </li>
    <li> unknown_bits = 23: precosat-570.1 (251s), glucose (266s),
    precosat236 (517.6s), cryptominisat (4073s), minisat-2.2.0 (4089s),
    OKsolver_2002 (35652s). </li>
    <li> unknown_bits = 24: precosat-570.1 (251s), precosat236 (517.8s),
    minisat-2.2.0 (4092s), cryptominisat (4477s), glucose (15612s). </li>
    <li> unknown_bits = 25: minisat-2.2.0 (1020s), precosat236 (8682s),
    precosat236 (8911s), cryptominisat (27032s) . </li>
    <li> unknown_bits = 26: precosat-570.1 (12520s), precosat236 (25521s),
    cryptominisat (>10 hours; timed out), glucose (>10 hours;timed out),
    minisat2 (>10 hours; timed out). </li>
    <li> unknown_bits = 27: minisat-2.2.0 (19557s), precosat236 (23822s),
    precosat-570.1 (34226s), cryptominisat (> 10 hours; timed out),
    glucose (> 10 hours; timed out). </li>
    </ul>
   </li>
   <li> These times are rather sporadic, yet nothing else runs on the
   machine. </li>
   <li> We should investigate with more keys, and also randomly permuting
   the clause-lists. </li>
  </ul>


  \todo Move into separate sub-module

*/
