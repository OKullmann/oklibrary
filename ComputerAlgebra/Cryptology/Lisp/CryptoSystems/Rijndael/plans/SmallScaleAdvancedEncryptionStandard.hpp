// Matthew Gwynne, 28.1.2010 (Swansea)
/* Copyright 2010, 2011, 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Cryptology/Lisp/CryptoSystems/Rijndael/plans/SmallScaleAdvancedEncryptionStandard.hpp
  \brief Plans on the AES implementation


  \todo Docus
  <ul>
   <li> Add docus on small-scale implementation, discussing the parameters,
   design decisions etc. </li>
   <li> Included in the design decisions should be mention of the fact
   we choose to stick to a byte-matrix representation of the MixColumn
   rather than generalising this to a large bit-matrix to avoid generalising
   too far from the original AES and overcomplicating the code, which
   should be very easy to read. </li>
  </ul>


  \todo Generating test vectors
  <ul>
   <li> We need to see side-by-side, for all the variations, our computation
   and the corresponding computation in the Sage system. </li>
   <li> And this for several inputs. </li>
   <li> And not only w.r.t. encryption and decryption, but also regarding
   the basic building blocks. </li>
   <li> An implementation of the small-scale variants presented in
   [Algebraic Aspects of the Advanced Encryption Standard] and 
   in more detail in [Small Scale Variants of the AES] is available
   in SAGE. </li>
   <li> To run the encryption for AES given a plaintext and key, one
   does the following in the Sage system:
   \verbatim
import sage.crypto
sr = mq.SR(10,4,4,8, star=true, allow_zero_inversions=true)
AES_e = sr(sr.state_array([0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]), sr.state_array([0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]))
AES_e
   \endverbatim
with 
\verbatim
[                        a^7 + a^5 + 1                 a^7 + a^6 + a^5 + a^2                         a^7 + a^2 + a                 a^6 + a^5 + a^4 + a^3]
[      a^6 + a^5 + a^4 + a^3 + a^2 + a             a^7 + a^6 + a^5 + a^4 + a                                   a^5       a^7 + a^6 + a^5 + a^3 + a^2 + a]
[        a^7 + a^4 + a^3 + a^2 + a + 1                   a^6 + a^4 + a^3 + a                 a^7 + a^5 + a^4 + a^2 a^7 + a^6 + a^5 + a^4 + a^3 + a^2 + 1]
[                  a^6 + a^5 + a^3 + 1                     a^7 + a^3 + a + 1         a^7 + a^5 + a^3 + a^2 + a + 1         a^6 + a^5 + a^3 + a^2 + a + 1]
\endverbatim
   as the result, and this can be converted to the hexidecimal format, like so
   \verbatim
sr.hex_str(AES_e, typ="vector")
   \endverbatim
   resulting in 
   \verbatim
'66E94BD4EF8A2C3B884CFA59CA342B2E'
   \endverbatim
   </li>
   <li> Note above that the first four parameters to the "sr" constructor are 
   r,n_R,n_C,e as specified above, and then 
   <ol>
    <li> the "star" parameter specifies that the small-scale AES variant should
    include a different final round (as occurs with the standard AES), </li>
    <li> "allow_zero_inversions" specifies that the field inversion should be 
    extended such that 0^(-1) = 0 (as in the AES). </li>
   </ol>
   </li>
   <li> There also appears to be an "aes_mode" parameter which suggests that 
   the key schedule implemented is somehow different to the AES, but this 
   does not seem to be the case in either the definition or in any basic
   tests so far. This needs to be further considered, or the authors should
   be e-mailed. </li>
   <li> This information should be moved to the docus. </li>
  </ul>


  \todo Removing parts of the AES
  <ul>
   <li> We need the ability to make each of the following
   components the identity (i.e, essentially remove them):
    <ul>
     <li> Key Schedule. </li>
     <li> MixColumns. </li>
     <li> S-boxes. </li>
    </ul>
   </li>
   <li> We must also implement this at the translation
   level, as discussed in "Removing parts of the AES" in
   Cryptology/Lisp/Cryptanalysis/Rijndael/plans/general.hpp. </li>
  </ul>


  \todo DONE (Handled in "Notions and notations" in
  ComputerAlgebra/Cryptology/Lisp/CryptoSystems/Rijndael/plans/general.hpp)
  Datatypes and naming conventions
  <ul> 
   <li> A naming scheme for each of the datatypes used, as well
   as for the whole class of small-scale AES functions must be
   devised. </li>
   <li> DONE 
   For now, functions are prefixed with "ss_"  for "small-scale".
   </li>
  </ul>



  \bug DONE (Added missing final "true" in test)
  Test failure with level "full"
  <ul>
   <li>
   \verbatim
okltest_ss_inv_sbox_gen(ss_inv_sbox_gen)
Evaluation took 2.1840 seconds (2.9790 elapsed)
true
okltest_ss_sbox_bf(ss_sbox_bf)
ASSERT: Expression " done " does not evaluate to true.
ERROR: /home/csoliver/OKplatform/system_directories/aux/tests/maxima/OKlib/ComputerAlgebra/Cryptology/Lisp/CryptoSystems/Rijndael/testobjects/SmallScaleAdvancedEncryptionStandard
   \endverbatim
   </li>
  </ul>


  \todo DONE Add decomposed Sbox boxes
  <ul>
   <li> In "Rearranging linear components of Sbox and MixColumns " in
   ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/plans/Translations.hpp
   we wish to translate the AES by splitting the various parts of the Sbox
   (inversion, linear multiplication) and moving them into the MixColumns by
   combining them with the field multiplications. </li>
   <li> Therefore we need implementations of each of the following
   functions:
   <ol>
    <li> DONE (see ss_inv in 
    ComputerAlgebra/Cryptology/Lisp/CryptoSystems/Rijndael/SmallScaleWordFields.mac)
    Inversion with the field. </li>
    <li> DONE (see ss_sbox_linmap in
    ComputerAlgebra/Cryptology/Lisp/CryptoSystems/Rijndael/SmallScaleAdvancedEncryptionStandard.mac)
    Linear multiplication by a GF(b) matrix. </li>
    <li> DONE (composition of ss_mul and ss_sbox_linmap in 
    ComputerAlgebra/Cryptology/Lisp/CryptoSystems/Rijndael/SmallScaleWordFields.mac
    and
    ComputerAlgebra/Cryptology/Lisp/CryptoSystems/Rijndael/SmallScaleAdvancedEncryptionStandard.mac)
    Linear multiplication by a GF(b) matrix followed by
    multiplication in the field. </li>
   </ol>
   </li>
  </ul>

*/
