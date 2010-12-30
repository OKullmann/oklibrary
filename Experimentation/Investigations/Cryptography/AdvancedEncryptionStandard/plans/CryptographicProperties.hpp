// Matthew Gwynne, 30.12.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
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
  </ul>

*/
