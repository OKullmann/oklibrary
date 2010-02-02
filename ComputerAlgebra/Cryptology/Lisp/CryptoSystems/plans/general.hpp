// Oliver Kullmann, 6.3.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Cryptology/Lisp/CryptoSystems/plans/general.hpp
  \brief Plans for crypto-systems in Maxima/Lisp


  \todo Decryption using iterative block ciphers
  <ul>
   <li> How to use the current iterative block cipher scheme
   "ibc_0" to encode decryption? </li>
   <li> One encounters the problem that for both encryption and
   decryption, the key will be the same, but for instance, in the
   AES, the key will be the first round key for encryption but the last
   round key for decryption. </li>
   <li> This becomes a problem as ibc_0 uses the given key as 
   the first round key no matter what. </li>
   <li> Additionally, the key round function for some ciphers
   (including AES) is not invertible, and to decrypt one must 
   simply generate all keys and reverse the order, and so 
   even if one knew the final round key, one could not
   simply pass an inverse key round function to ibc_0 to
   provide decryption, unless such a function was just a
   lookup to pre-generated keys, and ignored the previous round
   key. </li>
  </ul>


  \todo Other cryptosystems
  <ul>
   <li> RSA </li>
   <li> DES </li>
   <li> Look for systems where SAT has been applied. </li>
  </ul>

*/

