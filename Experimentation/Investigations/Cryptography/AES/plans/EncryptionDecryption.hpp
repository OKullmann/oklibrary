// Matthew Gwynne, 4.8.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/Cryptography/AES/plans/EncryptionDecryption.hpp
  \brief On investigating encryption and decryption using AES SAT-translation


  \todo Encryption
  <ul>
   <li> Given a CNF representing AES over "n" rounds called "AES_rn.cnf", a CNF
   with unit clauses, "setting up" the plaintext and key variables with values
   given in hexidecimal form, can be generated in the following way:
   \verbatim
plaintext : "00000000000000000000000000000000"$
key : "00000000000000000000000000000000"$
ciphertext : il2hex(aes_encrypt_l(hex2il(plaintext),hex2il(key)))$
aes_num_rounds : 10$
aes_intl2varl(il,vl) := binl2dnf_l(flatten(map(lambda([a],int2binlist(a,8)),transpose_l(il,4))),vl)$
pa : setify(append(
  aes_intl2varl(hex2il(plaintext),create_list(i,i,1,128)), 
  aes_intl2varl(hex2il(key), create_list(i,i,129,256)), 
  aes_intl2varl(hex2il(ciphertext), create_list(i,i,257,384))))$
output_fcs(
  sconcat("AES - ",aes_num_rounds," rounds - Plaintext=", plaintext, "Key=",key, 
    "Ciphertext=", ciphertext),
  cs2fcs(map(set,pa)),
  sconcat("AES_R10_P",plaintext,"_K",key,"_C",ciphertext,"_UC.cnf"))$
   \endverbatim
   This CNF can then simply be added to the "AES_rn.cnf" to produce a SAT 
   problem which encodes the encryption problem, given the plaintext "P" and
   key "K", where the satisfying assignment for this CNF should be one
   where the ciphertext variables are set according to the value of the 
   ciphertext produced by AES given "P" and "K". </li>
   </li>
  </ul>


  \todo Decryption
  <ul>
   <li> Given a CNF representing AES over "n" rounds called "AES_rn.cnf", a CNF
   with unit clauses, "setting up" the plaintext and key variables with values
   given in hexidecimal form, can be generated in the following way:
   \verbatim
key : "00000000000000000000000000000000"$
ciphertext : "00000000000000000000000000000000"$
plaintext : il2hex(aes_decrypt_l(hex2il(ciphertext),hex2il(key)))$
aes_num_rounds : 10$
aes_intl2varl(il,vl) := binl2dnf_l(flatten(map(lambda([a],int2binlist(a,8)),transpose_l(il,4))),vl)$
pa : setify(append(
  aes_intl2varl(hex2il(plaintext),create_list(i,i,1,128)), 
  aes_intl2varl(hex2il(key), create_list(i,i,129,256)), 
  aes_intl2varl(hex2il(ciphertext), create_list(i,i,257,384))))$
output_fcs(
  sconcat("AES - Decryption - ",aes_num_rounds," rounds - Plaintext=", 
    plaintext, "Key=",key, "Ciphertext=", ciphertext),
  cs2fcs(map(set,pa)),
  sconcat("AES_R10_P",plaintext,"_K",key,"_C",ciphertext,"_UC.cnf"))$
   \endverbatim
   This CNF can then simply be added to the "AES_rn.cnf" to produce a SAT 
   problem which encodes the decryption problem, given the plaintext "P" and
   key "K", where the satisfying assignment for this CNF should be one
   where the ciphertext variables are set according to the value of the 
   ciphertext produced by AES given "P" and "K". </li>
   </li>
  </ul>

*/
