// Matthew Gwynne, 18.7.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/KeyDiscovery/032/2_4_4/general.hpp
  \brief Investigations into small-scale AES key discovery for AES with a 2x4 plaintext matrix and 4-bit field elements


  \todo Problem specification
  <ul>
  <li> We consider the small-scale AES with 2 row, 4 column, using the 4-bit
   field size for rounds 1 to 20. </li>
   <li> We denote this AES instance by aes(r,2,4,4) for r in 1,...,20. </li>
   <li> We investigate translations of the key discovery problem for
   aes(r,2,4,4) into SAT. </li>
   <li> aes(r,2,4,4) takes a 32-bit plaintext and 32-bit key and outputs a
   32-bit ciphertext. </li>
   <li> aes(r,2,4,4) applies the following operations:
    <ol>
     <li> Key schedule which takes the key and generates r+1 32-bit round
     keys. </li>
     <li> Application of the following operation (the "round") r times:
      <ol>
       <li> Addition of round key n-1. </li>
       <li> Application of Sbox operation to each byte. </li>
       <li> Application of the MixColumns operation. </li>
      </ol>
     </li>
     <li> Addition of round key r+1. </li>
     <li> The result of the last round key addition is the ciphertext. </li>
    </ol>
   </li>
   <li> Round key 0 is the input key. </li>
   <li> The key schedule computes the round key i from round key i-1 by:
   \verbatim
K_(i,1,k) := S-box(K_(i-1,2,4)) + C_i + sum(K_(i-1,j,l),l,1,j)
K_(i,2,k) := S-box(K_(i-1,1,4)) + sum(K_(i-1,j,l),l,1,j)
   \endverbatim
   where
    <ul>
     <li> C_i is the round constant for round i; </li>
     <li> K_(i,j,k) is the 4-bit word in the j-th row, k-th column of the i-th
     round-key considered as a 2x2 matrix. </li>
    </ul>
   </li>
   <li> The S-box is a permutation from {0,1}^4 to {0,1}^4 which we consider
   as either:
    <ul>
     <li> an 8x1 boolean function; see ss_sbox_bf in
     ComputerAlgebra/Cryptology/Lisp/CryptoSystems/Rijndael/AdvancedEncryptionStandard.mac;
     </li>
     <li> 4 4x1 boolean functions. </li>
    </ul>
   </li>
   <li> The MixColumns operation is a permutation from ({0,1}^4)^2 to
   ({0,1}^4)^2, which we consider to be defined as:
   \verbatim
MixColumns(I_1) := Mul02(I_1) + Mul03(I_2)
MixColumns(I_2) := Mul03(I_1) + Mul02(I_2)
MixColumns(I_3) := Mul02(I_3) + Mul03(I_4)
MixColumns(I_4) := Mul03(I_3) + Mul02(I_4)
   \endverbatim
   where
    <ul>
     <li> I_i is the i-th 4-bit word in the input; </li>
     <li> Mul02 is a permutation over {0,1}^4 representing multiplication
     by 02 in the Rijndael byte field; </li>
     <li> Mul03 is a permutation over {0,1}^4 representing multiplication
     by 03 in the Rijndael byte field. </li>
    </ul>
   </li>
   <li> The decompositions and translations are listed in "Investigating
   dimensions" in
   Cryptography/AdvancedEncryptionStandard/plans/Experimentation.hpp.
   </li>
   <li> The plaintext and ciphertext variables are then set, and the SAT
   SAT solver is run on this instance to deduce the key variables. </li>
  </ul>

*/
