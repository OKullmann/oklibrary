// Matthew Gwynne, 8.10.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Cryptology/Lisp/CryptoSystems/Rijndael/docus/AdvancedEncryptionStandard.hpp
  \brief The docus for the maxima computer algebra system implementation of the AES block cipher.
  
  <h1> Overview </h1>

  <ul>
   <li> Throughout the implementation, particular parameters maintain the same shortened names for brevity, and ease of use
   <ul>
    <li>p - plaintext block (of block size length) </li>
    <li>k - k block (of block size length) </li>
    <li>c - ciphertext block (of block size length) </li>
   </ul>
   </li>
   <li> There are several helper functions which allow one to use the AES encryption and decryption functions with preset parameter functions 
   <ul>
    <li>"aes_encrypt_l(p,k)" and "aes_decrypt_l(c,k)" are the functions used to encrypt a given plaintext block "p" of size 16 (of integers - see Data Types), with a given key block "k" of size 16 (of integers - see Data Types), using lookup functions for all relevant field operations. This method is generally much less computationally expensive. </li>
    <li>"aes_encrypt_f(p,k)" and "aes_decrypt_f(c,k)" are the functions used to encrypt a given plaintext block "p" of size 16 (of integers - see Data Types), with a given key block "k" of size 16 (of integers - see Data Types), using functions which use the available field operations within the maxima systems, namely those in the "gf" package. This method is generally much more computationally expensive, and can take a considerable amount of time. </li>
   </ul>
   </li>
  </ul>  

  <h1> Data Types </h1>

  <ul>
   <li>AES bytes are represented as integers between 0 and 255, with the byte 
   being interpreted in a "big endian" manner (most significant bit to the 
   left). </li>
   <li> AES blocks are then simply lists of bytes, where each set of 4 
   consequtive integers form a row in the AES block matrix. </li>
   <li> There is no interface within this implementation to the bit-level of
   the AES, even those the Sbox does implement certain AES functionality at 
   this level. </li>
  </ul>

  <h1> Examples </h1>
  
  <ul>
   <li> AES is simply an encryption scheme and doesn't necessarily represent
   any encoding from a particular data type (i.e string) into an AES block or 
   more importantly - back again (since for instance most things are already in
   a binary format but the block output from AES might not be a valid block for
   that data type - e.g null values in the middle of a string), therefore one 
   must devise their own encoding of a given data type into a list of integers
   or use various functions provided by maxima. </li>
   <li> Encrypting a list of integers 
   \verbatim
key :  [202,200,168,53,202,187,250,252,46,206,107,75,12,120,233,226]$
plaintext : create_list(i,i,0,15)$
ciphertext : aes_encrypt_l(plaintext,key);
[244,189,246,179,85,160,222,126,158,248,201,206,115,222,96,177]
aes_decrypt_l(ciphertext,key);
[0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15]
   \endverbatim
   </li>
  </ul>
*/
