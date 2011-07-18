// Matthew Gwynne, 20.5.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/KeyDiscovery/016/2_1_8/general.hpp
  \brief Investigations into small scale AES key discovery for AES with a 2x1 plaintext matrix and 8-bit field elements


  \todo Problem specification
  <ul>
  <li> We consider the small scale AES with 2 row, 1 column, using the 8-bit
   field size for rounds 1 to 20. </li>
   <li> We denote this AES instance by aes(r,2,1,8) for r in 1,...,20. </li>
   <li> We investigate translations of the key discovery problem for
   aes(r,2,1,8) into SAT. </li>
   <li> aes(r,2,1,8) takes a 16-bit plaintext and 16-bit key and outputs a
   16-bit ciphertext. </li>
   <li> aes(r,2,1,8) applies the following operations:
    <ol>
     <li> Key schedule which takes the key and generates r+1 16-bit round
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
   <li> The key schedule computes the round key i, K_(i,j), from round key
   i-1, K_(i-1), by:
   \verbatim
K_(i,j) := S-box(K_(i-1,j)) + C_i
   \endverbatim
   where
    <ul>
     <li> C_i is the round constant for round i; </li>
     <li> K_(i,j) is the j-th 4-bit word of the i-th round-key. </li>
    </ul>
   </li>
   <li> The S-box is a permutation from {0,1}^8 to {0,1}^8 which we consider
   as either:
    <ul>
     <li> a 16-bit to 1-bit boolean function; see ss_sbox_bf in
     ComputerAlgebra/Cryptology/Lisp/CryptoSystems/Rijndael/AdvancedEncryptionStandard.mac;
     </li>
     <li> 4 8-bit to 1-bit boolean functions. </li>
    </ul>
   </li>
   <li> The MixColumns operation is a permutation from ({0,1}^8)^2 to
   ({0,1}^8)^2, which we consider to be defined as:
   \verbatim
MixColumns(I_1) := Mul02(I_1) + Mul03(I_2)
MixColumns(I_2) := Mul03(I_1) + Mul02(I_2)
   \endverbatim
   where
    <ul>
     <li> I_i is the i-th 8-bit word in the input; </li>
     <li> Mul02 is a permutation over {0,1}^8 representing multiplication
     by 02 in the Rijndael byte field; </li>
     <li> Mul03 is a permutation over {0,1}^8 representing multiplication
     by 03 in the Rijndael byte field. </li>
    </ul>
   </li>
   <li> The decompositions and translations are listed in "Investigating
   dimensions" in
   Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Experimentation.hpp.
   </li>
   <li> The plaintext and ciphertext variables are then set, and the SAT
   SAT solver is run on this instance to deduce the key variables. </li>
  </ul>


  \todo Comparisons
  <ul>
   <li> Comparing the different translations. </li>
   <li> For 10 round instances, the canonical translation
   seems to perform best by a factor of 20 or more. </li>
   <li> See:
    <ul>
     <li> 'The canonical box translation'. </li>
     <li> 'The "minimum" box translation'. </li>
    </ul>
   </li>
  </ul>


  \todo The canonical box translation
  <ul>
   <li> Up to 14 rounds solved in < 200s. </li>
   <li> Minimum times to solve with best solvers:
    <ul>
     <li> 1 round; 0.11s; minisat-2.2.0. </li>
     <li> 10 rounds; 6.06s; cryptominisat. </li>
     <li> 14 rounds; 181.9s; precosat236. </li>
    </ul>
   </li>
   <li> Note that for 14 rounds, there is only 1 second difference between
   precosat236 and cryptminisat.
   </li>
   <li> See:
    <ul>
     <li> Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/KeyDiscovery/016/2_1_8/1_13.hpp. </li>
     <li> Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/KeyDiscovery/016/2_1_8/10_13.hpp. </li>
     <li> Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/KeyDiscovery/016/2_1_8/14_13.hpp. </li>
    </ul>
   </li>
  </ul>


  \todo The "minimum" box translation
  <ul>
   <li> Up to 1 rounds solved in ~ 0s. </li>
   <li> Minimum times to solve with best solvers:
    <ul>
     <li> 1 round; 0.0s; precosat236. </li>
     <li> 10 rounds; 619.39s; glucose. </li>
     <li> 14 rounds; 432.36s; cryptominisat. </li>
    </ul>
   </li>
   <li> See:
    <ul>
     <li> Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/KeyDiscovery/016/2_1_8/1_13.hpp. </li>
     <li> Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/KeyDiscovery/016/2_1_8/10_13.hpp. </li>
    </ul>
   </li>
   <li> Add data. </li>
  </ul>


  \todo The 1-base box translation
  <ul>
   <li> Add data. </li>
  </ul>

*/
