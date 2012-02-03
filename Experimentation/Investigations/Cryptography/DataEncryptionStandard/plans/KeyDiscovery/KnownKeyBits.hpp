// Matthew Gwynne, 25.5.2011 (Swansea)
/* Copyright 2011, 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/DataEncryptionStandard/plans/KeyDiscovery/KnownKeyBits.hpp
  \brief On investigations into the full Data Encryption Standard key discovery where some key bits are known


  \todo Translations
  <ul>
   <li> We must compare the following translations of DES into SAT:
    <ul>
     <li> Massacci translation, see "DES generator" in
     Buildsystem/ExternalSources/SpecialBuilds/plans/SAT.hpp. </li>
     <li> Translations using:
      <ol>
       <li> Prime implicates (0-bases). </li>
       <li> 1-bases for all Sboxes (without new variables). </li>
       <li> "minimum" representations (inf-soft representations). </li>
       <li> DONE The canonical translation (1-soft representation). </li>
       <li> DONE The canonical+ translation (1-soft representation). </li>
      </ol>
     </li>
     <li> DONE Argosat-desgen translation, see
     "Transferring the Argosat-desgen example". </li>
    </ul>
   </li>
   <li> Solving key discovery with last q key bits unknown:
    <ul>
     <li> Argosat-desgen, solves up to q = 23 in ~35 minutes.
     q=24 takes > 10 hours. </li>
     <li> canonical translation, solves up to q = 27 in ~5.5 hours.
     q = 28 takes > 10 hours. </li>
     <li> canonical+ translation, solves up to q = 26 in ~30 minutes.
     q = 27 takes > 10 hours. </li>
    </ul>
   </li>
   <li> Note that in some cases higher q can be solved but
   randomising the order of the clause-set means that this isn't
   always the case. A study of many different randomised examples
   is necessary. </li>
   <li> For discussions on the variable order and standardisation of variable
   names, see "Variable ordering and standardisation" in
   ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/DataEncryptionStandard/plans/general.hpp.
   </li>
  </ul>


  \todo Overview
  <ul>
   <li> We consider the full 16 round DES given by the encryption function
   des_encryption in
   ComputerAlgebra/Cryptology/Lisp/CryptoSystems/DataEncryptionStandard/Cipher.mac.
   </li>
   <li> The DES(P,K,C) is a 192-bit boolean function where the first
   64 bits are the plaintext bits P, the next 64 bits are the key
   bits K and the final 64 bits are the ciphertext C. </li>
   <li> DES(P,K,C) is true iff DES encrypts P to C using K. </li>
   <li> The translation of DES(P,K,C) to SAT is given at the Maxima level by
   des_fcl in
   ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/DataEncryptionStandard/ConstraintTranslation.mac.
   </li>
   <li> The DES consists of certain rewiring of the bits, additions (XOR) and
   the application of 8 S-boxes (substitution boxes) for each round. </li>
   <li> We consider the DES S-boxes as 6-bit to 4-bit boolean functions,
   given by des_sbox_bf in
   ComputerAlgebra/Cryptology/Lisp/CryptoSystems/DataEncryptionStandard/Sboxes.mac.
   </li>
   <li> We should also consider the DES S-boxes as 4 6-bit to 1-bit functions.
   See "Basic translation" in
   Investigations/Cryptography/DataEncryptionStandard/plans/general.hpp. </li>
   <li> We translate the DES by treating the additions and S-boxes as the
   boolean functions, which we consider our units of translation. </li>
   <li> The additions are translated by the set of their prime implicates.
   </li>
   <li> The S-boxes are translated using each of the following CNF
   representations:
    <ul>
     <li> canonical(+) representation, see dualts_fcl and dualtsplus_fcl in
     ComputerAlgebra/Satisfiability/Lisp/FiniteFunctions/TseitinTranslation.mac;
     </li>
     <li> 1-base translations, see
     Investigations/Cryptography/DataEncryptionStandard/plans/Sboxes/general.hpp;
     </li>
     <li> minimum translations, see
     Investigations/Cryptography/DataEncryptionStandard/plans/Sboxes/general.hpp;
     </li>
     <li> their prime implicates; </li>
     <li> their canonical CNF representations. </li>
    </ul>
   All such translations apply to both the 6-bit to 4-bit S-box functions and
   the 4 decomposed 6-bit to 1-bit functions.
   </li>
   <li> Summary of results:
    <ul>
     <li> Determining X unknown key bits for 16-round DES(P,?,C) using
     the following translations for the S-boxes
     (X; best solver time; best solver):
      <ul>
       <li> Canonical+ (26; 1681s; precosat-570.1). </li>
       <li> Canonical (27; 19557s; minisat-2.2.0). </li>
      </ul>
     </li>
     <li> SplittingViaOKsolver finds 27 unknown key bits for the 16-round DES
     using the canonical translation for the S-boxes in ~35m. This is far
     better than solvers on any other translation so far. See "Applying
     SplittingViaOKsolver". </li>
    </ul>
   </li>
  </ul>


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
     <li> Randomly shuffling clause-list for X = 25: minisat-2.2.0 (>25h).
     \verbatim
shell> cat gss-25-s100.cnf | RandomShuffleDimacs-O3-DNDEBUG > 25-shuffled_test.cnf
shell> minisat-2.2.0 25-shuffled_test.cnf
     \endverbatim
     </li>
     <li> It seems the solver was just "lucky" with the original clause
     order. </li>
    </ul>
   </li>
   <li> We should consider other plaintext-ciphertext-pairs so we
   can get average runtimes. </li>
   <li> The ArgoSAT DES instances have already applied some UCP.
   This is after applying the plaintext-ciphertext-pair assignment.
   This makes it difficult to apply different plaintext-ciphertext-pair
   assignments. </li>
   <li> The ArgoSAT DES instances all have 64 unit-clauses. What are
   these unit-clauses? </li>
   <li> We should contact the ArgoSAT guys and ask for the generators.
   </li>
  </ul>


  \todo Canonical+ translation for the S-box (6-to-4)
  <ul>
   <li> Translating the full 16 round DES key discovery problem using the
   canonical+ translation for the DES Sboxes, as 6-to-4 bit boolean functions.
   </li>
   <li> The canonical+ translation is implemented by dualtsplus_fcl in
   ComputerAlgebra/Satisfiability/Lisp/FiniteFunctions/TseitinTranslation.mac
   at the Maxima level. </li>
   <li> Generating the CNF:
   \verbatim
unknown_bits : 13$
sbox_fcl_l : create_list(dualtsplus_fcl([listify(setn(10)), des_sbox_fulldnf_cl(i)]), i, 1, 8)$
F : des2fcl(sbox_fcl_l)$
P : des_plain2fcl(hexstr2binv("038E596D4841D03B"))$
C : des_cipher2fcl(hexstr2binv("A2FB6032638EC79D"))$
K : des_key2fcl(append(create_list(und,i,1,unknown_bits), rest(hexstr2binv("15FBC08D31B0D521"),unknown_bits)))$
Fs : standardise_fcl([F[1],append(F[2],P[2],K[2],C[2])])$
output_fcl_v(sconcat("DES over 16 rounds with the first ", unknown_bits, " key bits undefined; translated using the canonical+ translation for the S-boxes (6-to-4)."), Fs[1], sconcat("des_6t4_canonp_b",unknown_bits,".cnf"), Fs[2]);
   \endverbatim
   </li>
   <li> Basic statistics are
   \verbatim
> UB=13
> cat des_6t4_canonp_b${UB}.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
10112 98099 362163 0 362163 10113 1
 length count
1 179
2 81920
3 5120
11 8192
33 2560
64 128
> cat des_6t4_canonp_b${UB}.cnf | UnitClausePropagationW-O3-DNDEBUG > des_6t4_canonp_ucp_b${UB}.cnf
> cat des_6t4_canonp_ucp_b${UB}.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
10104 77910 286532 0 286532 10118 1
 length count
2 66927
3 2016
4 59
5 56
6 8
7 20
8 63
9 58
10 227
11 6272
15 2
16 16
17 102
18 10
19 2
32 14
33 1960
64 98
   \endverbatim
   </li>
   <li> S-box statistics (canonical+ translation):
   \verbatim
ncl_list_fcl(dualtsplus_fcl([listify(setn(10)), des_sbox_fulldnf_cl(1)]));
[[2,640],[11,64],[33,20],[64,1]]
   \endverbatim
   </li>
   <li> We have the following number of clauses of the following sizes:
    <ul>
     <li> 179 unit-clauses (setting plaintext + key + ciphertext); </li>
     <li> 81920 binary clauses (8*16 = 128 S-boxes); </li>
     <li> 5120 ternary clauses (80 * 16 = 1280 binary additions); </li>
     <li> 8192 clauses of length eleven (8 * 16 = 128 S-boxes); </li>
     <li> 2560 clauses of length 33 (8 * 16 = 128 S-boxes); </li>
     <li> 128 clauses of length 64 (8 * 16 = 128 S-boxes). </li>
    </ul>
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


  \todo Canonical translation for the S-box (6-to-4)
  <ul>
   <li> Translating the full 16 round DES key discovery problem using the
   canonical translation for the DES Sboxes, as 6-to-4 bit boolean functions.
   </li>
   <li> The canonical translation is implemented by dualts_fcl in
   ComputerAlgebra/Satisfiability/Lisp/FiniteFunctions/TseitinTranslation.mac
   at the Maxima level. </li>
   <li> Generating the CNF:
   \verbatim
unknown_bits : 13$
sbox_fcl_l : create_list(dualts_fcl([listify(setn(10)), des_sbox_fulldnf_cl(i)]), i, 1, 8)$
F : des2fcl(sbox_fcl_l)$
P : des_plain2fcl(hexstr2binv("038E596D4841D03B"))$
C : des_cipher2fcl(hexstr2binv("A2FB6032638EC79D"))$
K : des_key2fcl(append(create_list(und,i,1,unknown_bits), rest(hexstr2binv("15FBC08D31B0D521"),unknown_bits)))$
Fs : standardise_fcl([F[1],append(F[2],P[2],K[2],C[2])])$
output_fcl_v(sconcat("DES over 16 rounds with the first ", unknown_bits, " key bits undefined; translated using the canonical translation for the S-boxes (6-to-4)."), Fs[1], sconcat("des_6t4_canon_b",unknown_bits,".cnf"), Fs[2]);
   \endverbatim
   </li>
   <li> Basic statistics are
   \verbatim
> UB=13
> cat des_6t4_canon_b${UB}.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
10112 95539 277683 0 277683 10113 1
 length count
1 179
2 81920
3 5120
11 8192
64 128
> cat des_6t4_canon_b${UB}.cnf | UnitClausePropagationW-O3-DNDEBUG > des_6t4_canon_ucp_b${UB}.cnf
> cat des_6t4_canon_ucp_b${UB}.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
10104 81135 234596 0 234596 10118 1
 length count
2 71850
3 2088
4 5
6 20
7 20
10 128
11 6912
32 4
64 108
   \endverbatim
   These statistics need explanation. </li>
   <li> S-box statistics (canonical translation):
   \verbatim
ncl_list_fcl(dualts_fcl([listify(setn(10)), des_sbox_fulldnf_cl(1)]));
[[2,640],[11,64],[64,1]]
   \endverbatim
   </li>
   <li> We have the following number of clauses of the following sizes:
    <ul>
     <li> 179 unit-clauses (setting plaintext + key + ciphertext); </li>
     <li> 81920 binary clauses (8*16 = 128 S-boxes); </li>
     <li> 5120 ternary clauses (80 * 16 = 1280 binary additions); </li>
     <li> 8192 clauses of length eleven (8 * 16 = 128 S-boxes); </li>
     <li> 128 clauses of length 64 (8 * 16 = 128 S-boxes). </li>
    </ul>
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


  \todo Applying SplittingViaOKsolver
  <ul>
   <li> The following processing of the SplittingViaOKsolver-result can now
   be done much more convenient by using ProcessSplitViaOKsolver. </li>
   <li> First what apparently should be the canonical translation for the
   boxes as 10-bit functions, corrected in the most obvious places:
   \verbatim
unknown_bits : 27$
sbox_fcl_l : create_list(dualts_fcl([listify(setn(10)), des_sbox_fulldnf_cl(i)]), i, 1, 8)$
F : des2fcl(sbox_fcl_l)$
P : des_plain2fcl(hexstr2binv("038E596D4841D03B"))$
C : des_cipher2fcl(hexstr2binv("A2FB6032638EC79D"))$
K : des_key2fcl(append(create_list(und,i,1,unknown_bits), rest(hexstr2binv("15FBC08D31B0D521"),unknown_bits)))$
Fs : standardise_fcl([F[1],append(F[2],P[2],K[2],C[2])])$
output_fcl_v(sconcat("DES over 16 rounds with the first ", unknown_bits, " key bits undefined; translated using the canonical translation for the S-boxes (6-to-4)."), Fs[1], sconcat("des_6t4_canon_b",unknown_bits,".cnf"), Fs[2]);
   \endverbatim
   </li>
   <li> Basic statistics are (27 unknown bits):
   \verbatim
> UB=27
> cat des_6t4_canon_b${UB}.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
10112 95525 277669 0 277669 10113 1
 length count
1 165
2 81920
3 5120
11 8192
64 128

> cat des_6t4_canon_b${UB}.cnf | UnitClausePropagationW-O3-DNDEBUG > des_6t4_canon_ucp_b${UB}.cnf
> cat des_6t4_canon_ucp_b${UB}.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
10112 83526 241826 0 241826 10118 1
 length count
2 73441
3 2692
4 6
6 2
7 24
8 81
11 7168
64 112
   \endverbatim
   These statistics need explanation. </li>
   <li> Determining a reasonable splitting-depth:
   \verbatim
> SplittingViaOKsolver -D0 des_ucp_b${UB}.cnf
> more SplitViaOKsolver_D0des_ucp_b${UB}cnf_2011-06-05-111558/Statistics
37
 1

> SplittingViaOKsolver -D40 des_6t4_canon_ucp_b${UB}.cnf
233
  2

> SplittingViaOKsolver -D240 des_6t4_canon_ucp_b${UB}.cnf
${UB}4
  4

> SplittingViaOKsolver -D300 des_6t4_canon_ucp_b${UB}.cnf
406 436
  4   8

> SplittingViaOKsolver -D410 des_6t4_canon_ucp_b${UB}.cnf
436
 16

> SplittingViaOKsolver -D450 des_6t4_canon_ucp_b${UB}.cnf
463
 32

> SplittingViaOKsolver -D480 des_6t4_canon_ucp_b${UB}.cnf
573 633
 64  64

> SplittingViaOKsolver -D580 des_6t4_canon_ucp_b${UB}.cnf
633 680 688
 64  64  64

> SplittingViaOKsolver -D640 des_6t4_canon_ucp_b${UB}.cnf
680 688
128 128

> SplittingViaOKsolver -D685 des_6t4_canon_ucp_b${UB}.cnf
686 688
256 128

> SplittingViaOKsolver -D688 des_6t4_canon_ucp_b${UB}.cnf
688 736 746 756
128 128 256 128
   \endverbatim
   Clearly something systematic is going in, which needs to be explored. </li>
   <li>
   \verbatim
> cd SplitViaOKsolver_D688des_6t4_canon_ucp_b${UB}cnf_2011-06-05-114707/
> more Statistics
> E=read.table("Data")
> summary(E$n)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  688.0   736.0   746.0   734.4   746.0   756.0
> table(E$n)
688 736 746 756
128 128 256 128
> more Result
s UNKNOWN
c sat_status                            2
c initial_maximal_clause_length         64
c initial_number_of_variables           8962
c initial_number_of_clauses             83526
c initial_number_of_literal_occurrences 241826
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   73441
c running_time(sec)                     720.5
c number_of_nodes                       1279
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                9646
c number_of_pure_literals               0
c number_of_autarkies                   0
c number_of_missed_single_nodes         0
c max_tree_depth                        10
c number_of_table_enlargements          0
c number_of_1-autarkies                 0
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             des_6t4_canon_ucp_b27.cnf
c splitting_directory                   SplitViaOKsolver_D688des_6t4_canon_ucp_b27cnf_2011-06-05-114707/Instances
c splitting_cases                       640

> cd Instances
> OKP=~/OKplatform; I="../$(cat ../F)"; echo " i n t sat cfs dec rts r1 mem ptime stime cfl" > Stats; time tail -n +2 ../Data | while read C F N; do cat $I | ApplyPass-O3-DNDEBUG $F Temp.cnf; minisat-2.2.0 Temp.cnf >Temp.out 2>&1; S=$?; if [[ $S != 20 ]]; then echo -e "UNEXPECTED RETURN VALUE ${S}\!"; break; else echo -n "$C " >> Stats; awk -f ${OKP}/OKsystem/OKlib/Experimentation/ExperimentSystem/SolverMonitoring/ExtractMinisat.awk Temp.out >> Stats; echo -n "$C "; fi; done
1 ... 73 UNEXPECTED RETURN VALUE 10\!
real    19m28.193s
user    19m24.485s
sys     0m1.344s

# Monitoring in R via
#> E=read.table("Stats",header=TRUE,colClasses=c(rep("integer",3),"numeric","integer",rep("numeric",8))); plot(E$t); cat(sprintf("%d: %.2fh, sum-cfs=%e, mean-t=%.3fs, mean-cfs=%.0f",length(E$t),sum(E$t)/60/60,sum(E$cfs),mean(E$t),mean(E$cfs)),"\n")
73: 0.32h, sum-cfs=1.459788e+06, mean-t=15.697s, mean-cfs=19997
   \endverbatim
   As one can see, this approach holds great promisses. </li>
   <li> Now with 35 unknown bits (continuing the above Maxima-computation):
   \verbatim
unknown_bits : 35$
K : des_key2fcl(append(create_list(und,i,1,unknown_bits), rest(hexstr2binv("15FBC08D31B0D521"),unknown_bits)))$
Fs : standardise_fcl([F[1],append(F[2],P[2],K[2],C[2])])$
output_fcl_v(sconcat("DES over 16 rounds with the first ", unknown_bits, " key bits undefined."), Fs[1], sconcat("des_b",unknown_bits,".cnf"), Fs[2]);

> UB=35
> cat des_6t4_canon_b${UB}.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
10112 95517 277661 0 277661 10113 1
 length count
1 157
2 81920
3 5120
11 8192
64 128
> cat des_6t4_canon_b${UB}.cnf | UnitClausePropagationW-O3-DNDEBUG > des_6t4_canon_ucp_b${UB}.cnf
> cat des_6t4_canon_ucp_b${UB}.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
10112 84608 245120 0 245120 10118 1
 length count
2 74100
3 3024
4 1
7 4
8 81
9 80
10 32
11 7168
16 5
32 1
64 112

> SplittingViaOKsolver -D0 des_6t4_canon_ucp_b${UB}.cnf
0
1

> SplittingViaOKsolver -D5 des_6t4_canon_ucp_b${UB}.cnf
46 77
 1  1
> SplittingViaOKsolver -D100 des_6t4_canon_ucp_b${UB}.cnf
193
  4
> SplittingViaOKsolver -D200 des_6t4_canon_ucp_b${UB}.cnf
234
  8
> SplittingViaOKsolver -D300 des_6t4_canon_ucp_b${UB}.cnf
305 373
 64  64
> SplittingViaOKsolver -D400 des_6t4_canon_ucp_b${UB}.cnf
418 434
128 128
> SplittingViaOKsolver -D500 des_6t4_canon_ucp_b${UB}.cnf
> cd SplitViaOKsolver_D500des_6t4_canon_ucp_b35cnf_2011-06-05-142032/
> more Md5sum
b6448203fbb62d3c864c08e89ea5b0cd
> more Statistics
> E=read.table("Data")
> summary(E$n)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  505.0   519.0   577.0   564.4   598.0   620.0
> table(E$n)
505 513 521 561 569 577 585 596 604 610 612 618 620
512 256 256 256 128 384 256 256 128 256 128 128 128
> more Result
s UNKNOWN
c sat_status                            2
c initial_maximal_clause_length         64
c initial_number_of_variables           9073
c initial_number_of_clauses             84608
c initial_number_of_literal_occurrences 245120
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   74100
c running_time(sec)                     4293.6
c number_of_nodes                       6143
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                47788
c number_of_pure_literals               0
c number_of_autarkies                   0
c number_of_missed_single_nodes         0
c max_tree_depth                        12
c number_of_table_enlargements          0
c number_of_1-autarkies                 0
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             des_6t4_canon_ucp_b35.cnf
c splitting_directory                   SplitViaOKsolver_D500des_6t4_canon_ucp_b35cnf_2011-06-05-142032/Instances
c splitting_cases                       3072

> cd Instances
> OKP=~/OKplatform; I="../$(cat ../F)"; echo " i n t sat cfs dec rts r1 mem ptime stime cfl" > Stats; time tail -n +2 ../Data | while read C F N; do cat $I | ApplyPass-O3-DNDEBUG $F Temp.cnf; minisat-2.2.0 Temp.cnf >Temp.out 2>&1; S=$?; if [[ $S != 20 ]]; then echo -e "UNEXPECTED RETURN VALUE ${S}\!"; break; else echo -n "$C " >> Stats; awk -f ${OKP}/OKsystem/OKlib/Experimentation/ExperimentSystem/SolverMonitoring/ExtractMinisat.awk Temp.out >> Stats; echo -n "$C "; fi; done
# Aborted
Instances> tail Stats
 i n t sat cfs dec rts r1 mem ptime stime cfl
1 10112 78088 11198.5 0 5438716 6201208 8464 18222770660 173.00 0.03 0.04 288629608

# Monitoring in R via
#> E=read.table("Stats",header=TRUE,colClasses=c(rep("integer",3),"numeric","integer",rep("numeric",8))); plot(E$t); cat(sprintf("%d: %.2fh, sum-cfs=%e, mean-t=%.3fs, mean-cfs=%.0f",length(E$t),sum(E$t)/60/60,sum(E$cfs),mean(E$t),mean(E$cfs)),"\n")

> SplittingViaOKsolver -D600 des_6t4_canon_ucp_b${UB}.cnf
> more Md5sum
80ea472f725f825a7fdc01ee265884f9
> more Statistics
> E=read.table("Data")
> summary(E$n)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  601.0   624.5   723.0   694.0   737.0   745.0
> table(E$n)
 601  604  610  612  618  620  626  634  715  723  737  745
 256  256  256  256  256  256  256  256  768  512 1792 1024
> more Result
c running_time(sec)                     10094.9
c number_of_nodes                       12287
c number_of_2-reductions                62412
c max_tree_depth                        13
c splitting_cases                       6144

> cd Instances
> OKP=~/OKplatform; I="../$(cat ../F)"; echo " i n t sat cfs dec rts r1 mem ptime stime cfl" > Stats; time tail -n +2 ../Data | while read C F N; do cat $I | ApplyPass-O3-DNDEBUG $F Temp.cnf; minisat-2.2.0 Temp.cnf >Temp.out 2>&1; S=$?; if [[ $S != 20 ]]; then echo -e "UNEXPECTED RETURN VALUE ${S}\!"; break; else echo -n "$C " >> Stats; awk -f ${OKP}/OKsystem/OKlib/Experimentation/ExperimentSystem/SolverMonitoring/ExtractMinisat.awk Temp.out >> Stats; echo -n "$C "; fi; done
# Aborted
> cat Stats
 i n t sat cfs dec rts r1 mem ptime stime cfl
1 10112 76731 1004.16 0 847267 976918 1796 3553672413 107.00 0.03 0.04 28968805
2 10112 76731 764.748 0 735776 808505 1534 3241826510 84.00 0.03 0.04 20171828
3 10112 76731 740.549 0 769142 882894 1627 2958263481 114.00 0.03 0.04 28677199
4 10112 76731 1744.42 0 1215076 1401107 2301 4156482006 91.00 0.03 0.04 48238569
5 10112 76731 1279.79 0 721716 793579 1534 3640664306 84.00 0.04 0.07 16922471
6 10112 76731 1243.12 0 731168 816242 1534 2735598762 132.00 0.05 0.06 29142121
7 10112 76731 1227.7 0 738869 833943 1534 3171143916 92.00 0.04 0.07 23929650
8 10112 76731 1902.28 0 1133544 1299501 2077 4429735974 91.00 0.04 0.06 40980923
9 10112 76731 1175.01 0 751187 833601 1565 2975769953 87.00 0.05 0.07 23208699
10 10112 76731 1039.99 0 626715 682545 1326 2702304667 95.00 0.04 0.07 19029727
11 10112 76731 1221.4 0 807618 895448 1724 2873258239 84.00 0.04 0.07 29330746
12 10112 76731 1009.75 0 675993 742704 1467 2799526491 96.00 0.05 0.07 20747437
13 10112 76731 937.177 0 584729 632928 1244 2599219882 54.00 0.04 0.07 14297574
# Monitoring in R via
#> E=read.table("Stats",header=TRUE,colClasses=c(rep("integer",3),"numeric","integer",rep("numeric",8))); plot(E$t); cat(sprintf("%d: %.2fh, sum-cfs=%e, mean-t=%.3fs, mean-cfs=%.0f",length(E$t),sum(E$t)/60/60,sum(E$cfs),mean(E$t),mean(E$cfs)),"\n")
13: 4.25h, sum-cfs=1.033880e+07, mean-t=1176.161s, mean-cfs=795292

> SplittingViaOKsolver -D800 des_6t4_canon_ucp_b${UB}.cnf
> more Md5sum
3ab10361c7312e3f8c95e374cad26a65
> more Statistics
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  800.0   828.0   832.0   849.4   850.0   929.0
 800  826  828  830  832  842  845  849  850  878  882  929
1024 1024 3072 1024 3072 1024 1024 1024 1024 1024 1024 2048
> more Result
c running_time(sec)                     24976.9
c number_of_nodes                       34815
c number_of_2-reductions                102604
c max_tree_depth                        15
c splitting_cases                       17408

> cd Instances
> OKP=~/SAT-Algorithmen/OKplatform; I="../$(cat ../F)"; echo " i n t sat cfs dec rts r1 mem ptime stime cfl" > Stats; time tail -n +2 ../Data | while read C F N; do cat $I | ApplyPass-O3-DNDEBUG $F Temp.cnf; minisat-2.2.0 Temp.cnf >Temp.out 2>&1; S=$?; if [[ $S != 20 ]]; then echo -e "UNEXPECTED RETURN VALUE ${S}\!"; break; else echo -n "$C " >> Stats; awk -f ${OKP}/OKsystem/OKlib/Experimentation/ExperimentSystem/SolverMonitoring/ExtractMinisat.awk Temp.out >> Stats; echo -n "$C "; fi; done
# Solution found

# Monitoring in R via
#> E=read.table("Stats",header=TRUE,colClasses=c(rep("integer",3),"numeric","integer",rep("numeric",8))); plot(E$t); cat(sprintf("%d: %.2fh, sum-cfs=%e, mean-t=%.3fs, mean-cfs=%.0f",length(E$t),sum(E$t)/60/60,sum(E$cfs),mean(E$t),mean(E$cfs)),"\n")
> summary(E)
       i               n               t               sat         cfs
 Min.   :10104   Min.   :74861   Min.   : 140.6   Min.   :0   Min.   : 69610
 1st Qu.:10104   1st Qu.:74861   1st Qu.: 178.0   1st Qu.:0   1st Qu.: 94226
 Median :10104   Median :75340   Median : 382.4   Median :0   Median :189122
 Mean   :10108   Mean   :75319   Mean   : 354.5   Mean   :0   Mean   :184186
 3rd Qu.:10112   3rd Qu.:75630   3rd Qu.: 430.1   3rd Qu.:0   3rd Qu.:218465
 Max.   :10112   Max.   :75726   Max.   :1390.5   Max.   :0   Max.   :964029
      dec               rts               r1                 mem
 Min.   :  73663   Min.   : 219.0   Min.   :3.963e+08   Min.   : 28.00
 1st Qu.: 103266   1st Qu.: 255.0   1st Qu.:4.369e+08   1st Qu.: 40.00
 Median : 203532   Median : 509.0   Median :9.437e+08   Median : 47.00
 Mean   : 201539   Mean   : 468.6   Mean   :8.518e+08   Mean   : 49.81
 3rd Qu.: 237944   3rd Qu.: 511.0   3rd Qu.:1.025e+09   3rd Qu.: 60.00
 Max.   :1228755   Max.   :2045.0   Max.   :2.601e+09   Max.   :136.00
     ptime             stime              cfl
 Min.   :0.02000   Min.   :0.05000   Min.   :  839163
 1st Qu.:0.04000   1st Qu.:0.07000   1st Qu.: 2058694
 Median :0.04000   Median :0.07000   Median : 3652514
 Mean   :0.04264   Mean   :0.07127   Mean   : 4294789
 3rd Qu.:0.04000   3rd Qu.:0.07000   3rd Qu.: 5207808
 Max.   :0.10000   Max.   :0.15000   Max.   :47672816
> sum(E$t)/60/60/24
[1] 27.4666


> SplittingViaOKsolver -D900 des_6t4_canon_ucp_b${UB}.cnf
> more Md5sum
0748e5b828e2bd6b33494da204085d75
> more Statistics
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
    912     929     933     936     960     994
  912   914   915   916   929   933   960   962   976   978   994
 4608  3008    32  1536 10240 10240  6084  6052    60    90     2
> more  Result
c running_time(sec)                     61128.2
c number_of_nodes                       83903
c number_of_2-reductions                128620
c max_tree_depth                        16
c splitting_cases                       41952

> cd Instances
> OKP=~/OKplatform; I="../$(cat ../F)"; echo " i n t sat cfs dec rts r1 mem ptime stime cfl" > Stats; time tail -n +2 ../Data | while read C F N; do cat $I | ApplyPass-O3-DNDEBUG $F Temp.cnf; minisat-2.2.0 Temp.cnf >Temp.out 2>&1; S=$?; if [[ $S != 20 ]]; then echo -e "UNEXPECTED RETURN VALUE ${S}\!"; break; else echo -n "$C " >> Stats; awk -f ${OKP}/OKsystem/OKlib/Experimentation/ExperimentSystem/SolverMonitoring/ExtractMinisat.awk Temp.out >> Stats; echo -n "$C "; fi; done

# Monitoring in R via
#> E=read.table("Stats",header=TRUE,colClasses=c(rep("integer",3),"numeric","integer",rep("numeric",8))); plot(E$t); cat(sprintf("%d: %.2fh, sum-cfs=%e, mean-t=%.3fs, mean-cfs=%.0f",length(E$t),sum(E$t)/60/60,sum(E$cfs),mean(E$t),mean(E$cfs)),"\n")
26128: 1069.27h, sum-cfs=1.711826e+09, mean-t=147.327s, mean-cfs=65517

SplitViaOKsolver_D900des_ucp_b35cnf_2011-06-10-185340> grep "26129" Data
9975 26129 960
26129 14817 929
> cat des_ucp_b35.cnf | ApplyPass-O3-DNDEBUG Instances/14817 > inst_26129.cnf
> minisat-2.2.0 inst_26129.cnf
CPU time              : 34.4662 s
SATISFIABLE
   \endverbatim
   Interesting that the 13 sub-instances for D=600 are so much harder than the
   instances for 27 unknown bits. And the instances for D=800 are still much
   harder. </li>
   <li> It seems the SAT solvers "don't understand the instance anymore" (35
   unknown bits is 8 bits more than 27 bits, and 20m multiplied with 2^8=256
   would mean 4 days, not 44.6 days). </li>
  </ul>


  \todo DONE Add todos


  \todo DONE Summarise results
  <ul>
   <li> DONE (see "Overview" and "Translations")
   We need the results from each todo summarised in a highly
   visible todo/place. </li>
   <li> DONE (see "Overview" and "Translations")
   We also need to know which are the best solvers in each instance.
   </li>
  </ul>

*/
