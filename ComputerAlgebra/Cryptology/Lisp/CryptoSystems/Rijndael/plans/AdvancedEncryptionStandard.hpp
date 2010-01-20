// Oliver Kullmann, 20.7.2008 (Swansea)
/* Copyright 2008, 2009, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Cryptology/Lisp/CryptoSystems/Rijndael/plans/AdvancedEncryptionStandard.hpp
  \brief Plans on the AES implementation


  \todo Alter internal AES functions to use polynomials and matrices
  <ul>
   <li> The functions used internally within the AES encryption
   implementation should use polynomials as the basic representation
   as then
    <ul>
     <li> there is no need for conversions to and from the 
     polynomial representations within each of these internal functions and 
     <li> the addition and multiplications within the byte field are
     simply the standard polynomials multiplications, modulo the rijndael
     polynomial (and such a standardisation can be applied at higher
     levels, although initially it should still be applied within
     each of these functions). </li>
    </ul>
   </li>
   <li> Additionally, using matrices of polynomials as a standard
   representations makes complete sense, as then the operations
   such as rijn_mixcolumns are just standard matrix multiplications,
   and there is again no need for unnecessary conversions. </li>
   <li> If natural, such as in the case of the sbox (where
   for experimentation etc, sometimes the different
   variations of the sbox can be used) the old versions
   (such as rijn_sbox_nat) can still be made available and 
   used in old functions (such as aes_key_expansion) until those
   functions have been updated to use polynomials as
   well. </li>
   <li> Functions to translate (including lookup versions of any of the below):
    <ul>
     <li> aes_encrypt_l </li>
     <li> aes_decrypt_l </li>
     <li> aes_encrypt_f </li>
     <li> aes_decrypt_f </li>
     <li> aes_round </li>
     <li> aes_inv_round </li>
     <li> aes_key_expansion : DONE </li>
     <li> rijn_mixcolumns : DONE </li>
     <li> rijn_mixcolumn : DONE </li>
     <li> rijn_sbox : DONE </li>
    </ul>
   </li>
   <li> Such a translation should occur piecewise, first the translation
   to using polynomials, and then to using matrices. </li>
  </ul>

  \todo DONE Generating randomised AES parameters
  <ul>
   <li> There is no need to generate a random seed. Simply providing the 
   seed is enough, and randomly generating one gains nothing, and simply
   makes things more complicated as then this seed must always be recorded. 
   </li>
   <li> Otherwise, the same functionality can be had by simply doing:
   \verbatim
random_block : map(random, create_list(256,i,1,16))$
   \endverbatim
   </li>
   <li> For both testing and experimentation (see
   ComputerAlgebra/Cryptology/Lisp/CryptoSystems/Rijndael/plans/general.hpp), 
   we need some way of generating random plaintext, key and ciphertext blocks. 
   </li>
   <li> As plaintext, key and ciphertext blocks are of the same form, one
   function which generates a random block of a given size seems sensible. 
   </li>
   <li> Such a parameter generator needs:
   <ul>
    <li> to be able to take the number of bits for the block </li> 
    <li> To somehow return the random seed that it uses to generate this
    block. </li>
   </ul>
   </li>
   <li> Two functions are proposed:
   <ul>
    <li> One function takes a seed, and an integer "n" and returns a block 
    "p" where "n" is the number of block bits (in multiples of 32), and 
    "p" is the randomly generated block (as an integer list, as in the AES 
    functions themselves) given the random seed. 
    </li>
    <li> The other function takes an integer "n" and returns a list of the form
    [p,r_s] where "p" is as in the first function, given the randomly selected 
    seed "r_s". </li>
   </ul>
   </li>
  </ul>


  \todo Add todos.


  \todo DONE Truncate AES for reduced rounds
  <ul>
   <li> See "Truncate AES for reduced rounds" in 
   ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/plans/Translations.hpp
   . </li>
  </ul>


*/

