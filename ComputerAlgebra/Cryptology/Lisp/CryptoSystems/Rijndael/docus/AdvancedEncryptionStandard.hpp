// Matthew Gwynne, 8.10.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Cryptology/Lisp/CryptoSystems/Rijndael/docus/AdvancedEncryptionStandard.hpp
  \brief User documentation for the Maxima computer algebra system implementation of the AES block cipher.


  <h1> Using AES at the Maxima/Lisp level </h1>

  Functions are presented which implement the AES ("Advanced Encryption
  Standard") block cipher.


  <h2> Overview </h2>

  <ul>
   <li> Throughout the implementation, particular parameters maintain the same shortened names for brevity, and ease of use:
   <ul>
    <li> p - plaintext block </li>
    <li> k - key block </li>
    <li> c - ciphertext block. </li>
   </ul>
   </li>
   <li> There are several helper functions which allow one to use the AES
   encryption and decryption functions with preset parameter functions:
   <ul>
    <li> "aes_encrypt_l(p,k)" and "aes_decrypt_l(c,k)" are the functions used
    to encrypt a plaintext block p of size 16 with a key block k of size 16,
    using <em>lookup</em> functions for all relevant field operations.
    This method is relatively fast. </li>
    <li> "aes_encrypt_f(p,k)" and "aes_decrypt_f(c,k)" are the functions used
    to encrypt a plaintext block p with a key block k using functions which
    use the available <em>field operations</em> within the maxima systems,
    namely those in the "gf" package. This method is generally much more
    computationally expensive, and can take a considerable amount of time.
    </li>
   </ul>
   </li>
  </ul>  


  <h2> Data Types </h2>

  <ul>
   <li>AES bytes are represented as integers between 0 and 255, with the byte 
   being interpreted in a "big endian" manner (most significant bit to the 
   left). </li>
   <li> AES blocks are then simply lists of bytes (where each set of 4 
   consequtive integers forms a row in the AES block matrix). </li>
   <li> There is no interface within this implementation to the bit-level of
   the AES (even though the Sbox does implement certain AES functionality at
   this level). </li>
  </ul>


  <h2> Examples </h2>
  
  <ul>
   <li> Encrypting a 128-bit word represented as a list of integers :
   \verbatim
key :  [202,200,168,53,202,187,250,252,46,206,107,75,12,120,233,226]$
plaintext : create_list(i,i,0,15)$
ciphertext : aes_encrypt_l(plaintext,key);
[244,189,246,179,85,160,222,126,158,248,201,206,115,222,96,177]
aes_decrypt_l(ciphertext,key);
[0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15]
   \endverbatim
   </li>
   <li> A general remark:
    <ol>
     <li> AES is a cryptographic primitive, and should not be used directly
     for encryption purposes. </li>
     <li> Care must be taken how to decompose a larger piece of data to be
     encrypted into blocks of the (small) sizes allowed by AES. </li>
     <li> Most useful in this respect is the "CBC mode" (cipher-block chaining
     mode; not implemented yet), which encrypts a sequence of blocks in a
     manner such that the result of the encryption of one block influences
     the encryption of the next block. </li>
    </ol>
   </li>
  </ul>

*/
