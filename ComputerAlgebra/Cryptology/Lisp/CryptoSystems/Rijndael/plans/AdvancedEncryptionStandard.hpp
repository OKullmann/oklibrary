// Oliver Kullmann, 20.7.2008 (Swansea)
/* Copyright 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Cryptology/Lisp/CryptoSystems/Rijndael/plans/AdvancedEncryptionStandard.hpp
  \brief Plans on the AES implementation


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


*/

