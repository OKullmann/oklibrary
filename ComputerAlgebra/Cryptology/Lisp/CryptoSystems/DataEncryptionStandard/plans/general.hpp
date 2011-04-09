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


  \todo Add variants with reduced number of rounds
  <ul>
   <li> In the literature, such as [Logical cryptanalysis as a SAT problem;
   Massaci and Marraro], variants of DES with smaller numbers of rounds are
   considered. </li>
   <li> We should offer generalised versions of the current functions. </li>
   <li> These generalised functions should take the number r of rounds. </li>
   <li> The functions should then compute the DES encryption with r rounds on
   the other inputs. </li>
   <li> Do variants of the DES with smaller numbers of rounds flip the two
   32-bit outputs? <ul>
    <li> It makes sense to do so. Then applying the cipher with the reversed
    round key list gives the decryption algorithm. This is the same as with 
    16-round DES. </li>
   </ul>
   </li>
   <li> We should have a clear simple function for the full DES.
   Therefore, it is best to have a different function for the smaller
   variants. For instance "des_encryption_reduced". </li>
   <li> With the current system, this is as simple as:
   \verbatim
des_encryption_reduced(plaintext, key, r) :=
 des_template(plaintext, take_elements(r,des_round_keys(key)))$
   \endverbatim
   </li>
   <li> To be able to properly test this, we need test vectors for
   smaller variants; see "Find more test vectors". </li>
  </ul>


  \todo Find more test vectors
  <ul>
   <li> We need test vectors for variants of DES with smaller numbers of
   rounds. </li>
   <li> Full 16 round DES swaps the two 32-bit outputs. Do DES variants
   with less rounds do this? </li>
   <li> Authorative test vectors should answer this question. </li>
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

