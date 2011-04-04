// Matthew Gwynne, 10.3.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Cryptology/Lisp/CryptoSystems/DataEncryptionStandard/plans/general.hpp
  \brief Plans for the Data Encryption Standard crypto-system in Maxima/Lisp


  \todo Add todos

  
  \todo Improve key schedule tests
  <ul>
   <li> We should look for tests vectors for the DES key schedule.
   </li>
   <li> These test vectors should then be added to the tests. </li>
   <li> The test vectors should provide good test coverage. </li>
   <li> Before we can properly test the function, we need a specification
   (see "Specification"). </li>
  </ul>


  \todo Specification
  <ul>
   <li> We need full specifications for the following functions:
   <ul>
    <li> des_round_keys. </li>
    <li> des_encryption. </li>
    <li> des_decryption. </li>
    <li> des_template. </li>
   </ul>
   </li>
  </ul>


  \todo Add variants with reduced number of rounds
  <ul>
   <li> In the literature, such as [Logical cryptanalysis as a SAT problem;
   Massaci and Marraro], variants of DES with smaller numbers of rounds are
   considered. </li>
   <li> Do variants of the DES with smaller numbers of rounds flip the two
   32-bit outputs? <ul>
    <li> It makes sense to do so. Then applying the cipher with the reversed
    round key list gives the decryption algorithm. This is the same as with 
    16-round DES. </li>
   </ul>
   </li>
   <li> To be able to properly test this, we need test vectors for
   smaller variants; see "Find more test vectors". </li>
   <li> All functions which can have a variant with a reduced number of rounds
   should have one. For example, des_encryption_hex should have
   des_encryption_reduced_hex. </li>
   <li> DONE We should have a clear simple function for the full DES.
   Therefore, it is best to have a different function for the smaller
   variants. For instance "des_encryption_reduced". </li>
   <li> DONE With the current system, this is as simple as:
   \verbatim
des_encryption_reduced(plaintext, key, r) :=
 des_template(plaintext, take_elements(r,des_round_keys(key)))$
   \endverbatim
   </li>
   <li> DONE We should offer generalised versions of the current functions.
   </li>
   <li> DONE These generalised functions should take the number r of rounds.
   </li>
   <li> DONE The functions should then compute the DES encryption with r
   rounds on the other inputs. </li>
  </ul>


  \todo Find more test vectors
  <ul>
   <li> We need test vectors for variants of DES with smaller numbers of
   rounds. </li>
   <li> Full 16 round DES swaps the two 32-bit outputs. Do DES variants
   with less rounds do this? </li>
   <li> Authorative test vectors should answer this question. </li>
   <li> The tests should then be extended. </li>
  </ul>


  \todo Notion of DES round
  <ul>
   <li> We consider the DES round function where:
   <ul>
    <li> It takes two 32-bit inputs (previous and current) and a 48-bit round
    key. </li>
    <li> The first 32-bit input, "previous", is the output of the
    round two rounds ago. </li>
    <li> The second 32-bit input, "current", is the output of the previous
    round. </li>
    <li> For the first round, previous is the first 32-bits and
    current is the second 32-bits of the 64-bit DES input. </li>
   </ul>
   </li>
   <li> The round function does the following:
   <ul>
    <li> Applies an "expansion map" to current. This rearranges and repeats
    some bits to make 48-bits. </li>
    <li> Adds the result of the expansion to the 48-bit key. </li>
    <li> Applies DES Sbox i, for i in {1,...,6}, to the i-th 6-bit block
    in the result. This yields 32-bits, as the Sboxes are 6-to-4 bit 
    functions. </li>
    <li> Applies a "permutation box", i.e., a rewiring of bits, to the 
    32-bit result of the Sbox operations. </li>
    <li> Adds previous to the result of the "permutation box". </li>
   </ul>
   </li>
   <li> Can the DES encryption scheme be fit into the notion of an
   iterated block cipher (see 
   ComputerAlgebra/Cryptology/Lisp/CryptoSystems/IteratedBlockCipher.mac)?
   </li>
   <li> Can the DES key addition be moved to the beginning of the round? This
   would then fit with our description of AES. </li>
  </ul>

  
  \todo Links
  <ul>
   <li> For cryptanalysis functions for the DES see
   ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/DataEncryptionStandard/plans/general.hpp.
   </li>
   <li> For investigations see
   Experimentation/Investigations/Cryptography/DataEncryptionStandard/plans/general.hpp.
   </li>
  </ul>

*/

