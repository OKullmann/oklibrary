// Oliver Kullmann, 25.2.2011 (Swansea)
/* Copyright 2011, 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Benchmarks/plans/general.hpp
  \brief Plans for producing benchmarks


  \todo AES Challenge
  <ul>
   <li> One plain-text/cipher-text pair for standard 128-bit is provided,
   as is, and as a SAT formula, just using the minimum translation. </li>
   <li> Later more translation might follow, once we understand them better.
   </li>
   <li> The directory "SAT2011_AES-Challenge" contains the templates for
   the "AES Challenge". </li>
   <li> The package shall be called "AES-Challenge_OKlibrary". </li>
   <li> The key, created by 128 coin flips (one pound, 2001; erased after
   usage):
   \verbatim
key_24022011 : [???]$
   \endverbatim
   </li>
   <li> The input:
   \verbatim
set_random(1);
message_24022011 : create_list(random(2),i,1,128);
[1,1,0,0,1,1,1,1,1,0,0,1,0,1,1,0,0,1,0,0,0,1,0,0,1,0,0,0,1,0,0,0,1,1,1,1,1,
 0,0,0,1,1,1,1,1,1,0,1,1,0,0,1,0,0,1,1,1,0,1,0,0,1,1,0,1,1,1,1,0,0,1,1,0,0,
 0,0,1,1,1,0,1,0,0,1,1,0,1,1,0,1,0,0,1,1,1,0,1,1,0,1,1,1,1,0,0,0,0,0,1,1,1,
 1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,0]
   \endverbatim
   </li>
   <li> The output:
   \verbatim
key_24022011_int : polyadic2int(key_24022011,2)$
integerp(key_24022011_int);
  true
message_24022011_int : polyadic2int(message_24022011,2);
  275930429712199798024509060124983156862
cipher_24022011_int : aes_int_std(message_24022011_int,key_24022011_int);
  82288044290978544244364916044704454498
aes_int_decrypt_std(cipher_24022011_int,key_24022011_int);
  275930429712199798024509060124983156862
cipher_24022011 : int2polyadic(cipher_24022011_int,2);
[1,1,1,1,0,1,1,1,1,0,1,0,0,0,0,0,0,1,1,0,0,1,1,1,0,1,0,0,0,1,1,0,1,1,1,0,1,
 1,0,1,1,0,0,0,1,0,0,1,0,0,1,1,0,1,1,1,0,1,1,0,1,0,0,0,1,1,0,1,0,1,1,1,1,0,
 0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,1,1,1,0,1,0,0,0,1,0,0,1,0,1,1,0,0,0,0,1,1,0,
 0,0,1,0,1,1,1,0,1,1,0,0,0,1,0]
   \endverbatim
    </li>
    <li> Generating the plaintext-ciphertext-pair unit-clauses:
    \verbatim
plaintext_lits : ss_matrix2pa(ss_hex2matrix(int2hex(275930429712199798024509060124983156862), 2,8,ss_polynomial_2_8,4), create_list(i,i,1,128),2,8,ss_polynomial_2_8)$
ciphertext_lits : ss_matrix2pa(ss_hex2matrix(int2hex(82288044290978544244364916044704454498), 2,8,ss_polynomial_2_8,4), create_list(i,i,257,384),2,8,ss_polynomial_2_8)$
comment : sconcat(
      "P :  CF964488F8FD93A6F30E9B4EDE0FE07E ",
      "C : 3DE819D1BB624DDA35E4445D12C31762")$
pc_phi : append(plaintext_lits, ciphertext_lits)$
pc_unit_fcs : [
  setify(create_list(i,i,1, 384)),setify(map(set, pc_phi))]$
output_fcs(comment, pc_unit_fcs, "aes_ass_challenge.cnf")$
    \endverbatim
    </li>
   </ul>
  </ul>

*/

