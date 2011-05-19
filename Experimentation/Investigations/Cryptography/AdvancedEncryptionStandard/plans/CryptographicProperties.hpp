// Matthew Gwynne, 30.12.2010 (Swansea)
/* Copyright 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/plans/CryptographicProperties.hpp
  \brief On investigating the cryptographic properties of AES and small scale variations.


  \todo Cryptographic properties of AES
  <ul>
   <li> Given a correct translation of the AES into a SAT problem, represented
   by the predicate "AES(P,K,C)" where P, K, and C are lists of 128 variables,
   several questions regarding certain cryptographic properties of the AES can
   be formulated as SAT problems. </li>
   <li> Does AES have two distinct keys which maps the same plaintext block
   to the same ciphertext block?
   <ul>
    <li> This can be translated as "AES(P,K1,C) and AES(P,K2,C) and
    NEQ(K1,K2)" where "NEQ" specifies that K1 differs form K2 in at least one
    bit. </li>
   </ul>
   </li>
   <li> Does AES have any key which acts as the identity on one or all
   plaintext blocks?
   <ul>
    <li> This can be translated as "AES(P,K,P)". </li>
    <li> This can also be expanded trivially to find keys where AES algorithm
    acts as the identity on "k" or more plaintext blocks (for reasonable k)
    by simply considering "AES(P1,K,P1) and AES(P2,K,P2) and ... and
    AES(Pk,K,Pk) and NEQ(P1,P2,...,Pk)" where here "NEQ" specifies that every
    argument differs in at least one variable from every other. </li>
    <li> This may also be made more damaging to the AES by considering
    specifically plaintexts of a particular form (plaintext blocks
    representing particular common ASCII sequences). </li>
    <li> We should also consider the use of QBF translations here. </li>
   </ul>
   <li> Does AES have any key which is the inverse of any other for some
   plaintext/ciphertext pair?
   <ul>
    <li> Considering only a single piece of plaintext (that there are two
    keys K1 and K2 for which AES with that K1 maps some plaintext P to
    ciphertext C and AES with K2 maps C to P) can be translated simply as
    "AES(P,K1,C) and AES(C,K2,P)". </li>
    <li> This can be expanded to find keys K1 and K2 where AES using K2 is
    the inverse of AES with K1 for at least "k" plaintext blocks in the
    following way: "AES(P1,K1,C1) and AES(C1,K2,P1) and ... and
    AES(Pk,K(k-1),Ck) and AES(Ck,Kk,Pk) and NEQ(P1,P2,...,Pk)" . </li>
    <li> There is obviously then the question of whether there is key which
    acts as it's own inverse for at least "k" plaintext blocks, where
    K1=K2 etc. </li>
    <li> Again we should consider using QBF solvers here to translate
    this property for all plaintext ciphertext pairs. </li>
   </ul>
   </li>
   <li> Does the AES have any key for which the cipher becomes the
   identity with that key? See "Identity-map keys". </li>
  </ul>


  \todo Identity-map keys
  <ul>
   <li> Does the AES have any key for which the cipher becomes the
   identity with that key?
   <ul>
    <li> Considering only a single plaintext (that there is a key K for which
    AES with that K maps some plaintext P to itself) can be translated simply
    as "AES(P,K,P)". </li>
    <li> This question can then be expanded to whether there is a key which
    makes the ciphertext act identically on all blocks using QBF solvers.
    </li>
    <li> The single plaintext translation of this question into a SAT
    problem should be implemented in the Maxima system. </li>
    <li> See output_ss_fcl_id_p in
    ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/Translations.mac
    for the implemented translation of the single plaintext cryptographic
    question. </li>
   </ul>
   </li>
   <li> Investigating identity-map keys for the 4-bit trivial AES scheme
   (1 row, 1 column, 4-bit field) using the standard canonical box translation
   with bidirectional MixColumns translation:
    <ul>
     <li> Generating the instances for rounds 1 to 10:
     \verbatim
for i : 1 thru 10 do output_ss_fcl_id_p_std(i,1,1,4,false,aes_ts_box,aes_mc_bidirectional);
     \endverbatim
     </li>
     <li> Running solvers via (using cryptominisat as an example):
     \verbatim
for i in $(seq 1 10); do cryptominisat ssaes_id_p_r${i}_c1_rw1_e4_f0.cnf | awk " { print \"[$i/10] \" \$0 }"; done
     \endverbatim
     </li>
     <li> All instances were found satisfiable in <0.1s using very few
     conflicts (<50). </li>
     <li> Comparing the solvers for finding an example of an identity mapping
     key for 10 rounds:
      <ul>
       <li> march_pl (node_count: 2). </li>
       <li> minisat-2.2.0 (conflicts: 3). </li>
       <li> precosat-570.1 (conflicts: 5). </li>
       <li> picosat913 (conflicts: 12). </li>
       <li> precosat236 (conflicts: 12). </li>
       <li> cryptominisat (conflicts: 13). </li>
       <li> minisat2 (conflicts: 45). </li>
       <li> glucose (conflicts: 67). </li>
       <li> satz:
       \verbatim
[10/10] **** The instance is satisfiable. *****
[10/10] NB_MONO= 0, NB_UNIT= 24035, NB_BRANCHE= 403, NB_BACK= 197
[10/10] Program terminated in 0.000 seconds.
[10/10] satz215 -v 0.000 403 197 64768 763 1 568 3436 -200 3028 0
       \endverbatim
       </li>
      </ul>
      march_pl or minisat-2.2.0 seems best here but the instance is
      trivially solved and so there isn't much difference between solvers.
     </li>
    </ul>
   </li>
  <li> Investigating identity-map keys for the 16-bit AES scheme
   (2 row, 2 column, 4-bit field) using the standard canonical box translation
   with bidirectional MixColumns translation:
    <ul>
     <li> Generating the instances for rounds 1 to 10:
     \verbatim
for i : 1 thru 10 do output_ss_fcl_id_p_std(i,1,1,4,false,aes_ts_box,aes_mc_bidirectional);
     \endverbatim
     </li>
     <li> Running solvers via (using cryptominisat as an example):
     \verbatim
for i in $(seq 1 10); do cryptominisat ssaes_id_p_r${i}_c1_rw1_e4_f0.cnf | awk " { print \"[$i/10] \" \$0 }"; done
     \endverbatim
     </li>
     <li> All instances up to round 4 were found satisfiable in <0.1s using
     very few conflicts (<50). </li>
     <li> Experiments still running for rounds > 4. </li>
     <li> Comparing the solvers for finding an example of an identity mapping
     key for 4 rounds:
      <ul>
       <li> cryptominisat (time: 0.53s, conflicts: 6889). </li>
       <li> precosat236 (time: 1.1s, conflicts: 15684). </li>
       <li> minisat-2.2.0 (time: 1.95s, conflicts: 26539). </li>
       <li> glucose (time: 2.59s, conflicts: 23614). </li>
       <li> precosat-570.1 (time: 20.4s, conflicts: 139328). </li>
       <li> picosat913 (time: 68.5s, conflicts: 866125). </li>
       <li> minisat2 (time: 85.72, conflicts: 118350). </li>
      </ul>
      cryptominisat seems best here.
     </li>
    </ul>
   </li>
  </ul>

*/
