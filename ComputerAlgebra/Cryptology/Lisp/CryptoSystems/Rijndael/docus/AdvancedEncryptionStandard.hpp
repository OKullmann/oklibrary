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
    <li> "aes_encrypt(p,k,r)" and "aes_decrypt(c,k,r)" are the functions used
    to encrypt/decrypt a plaintext 4xn (for n = 4,5 or 6) matrix p with a key 
    matrix k of the same dimension, where the elements of the matrices are 
    given as polynomials (see "Data Types"). Here, r is the number of rounds 
    to apply during encryption/decryption. </li>
    <li> "aes_encrypt_nat" and "aes_decrypt_nat" are the same as "aes_encrypt"
    and "aes_decrypt", except they take lists of integers, which represent
    the matrices polynomials column by column from "aes_encrypt" etc via 
    int2poly(n,2) where n is the integer (see "Examples"). </li>
   </ul>
   </li>
   <li> For each of the above functions, there are versions suffixed with
   "_std" which fix the number of rounds to the standard 10 for AES. </li>
  </ul>  


  <h2> Data Types </h2>

  <ul>
   <li> AES bytes are represented as arbitrary polynomials and then are 
   "brought into range" or "standardised" using the Rijndael modulo
   polynomial (see rijn_polynomial in 
   ComputerAlgebra/Cryptology/Lisp/CryptoSystems/Rijndael/ByteField.mac). 
   </li>
   <li> AES blocks are then matrices of polynomials, where the 4 row, n column
   nature of the AES block is given by the dimension of the matrix. </li>
   <li> There is no interface within this implementation to the bit-level of
   the AES, although one can access this level if one standardises
   a polynomial (representing a byte) using rijn_stand (see
   ComputerAlgebra/Cryptology/Lisp/CryptoSystems/Rijndael/ByteField.mac)
   and then considers the coefficient of the polynomial using the same
   correspondence of coefficients to "bits of a byte" as occurs in
   the definition of Rijndael (i.e., the coefficient of x^7 becomes
   the most significant bit of the byte and the coefficient of 1 becomes
   the least - see [Design of Rijndael;John Daemen and Vincent Rijmen]).
   </li>
  </ul>


  <h2> Examples </h2>
  
  <ul>
   <li> Encrypting a 128-bit word represented as a list of integers :
   \verbatim
> key :  [202,200,168,53,202,187,250,252,46,206,107,75,12,120,233,226]$
> plaintext : create_list(i,i,0,15)$
> ciphertext : aes_encrypt_nat_std(plaintext,key);
[223,7,143,218,18,249,134,153,203,218,50,237,88,69,163,149]
> aes_decrypt_nat_std(ciphertext,key);
[0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15]
   \endverbatim
   </li>
   <li> There are also helper functions which allow the encryption or 
   decryption using the hexidecimal notation used for AES/Rijndael:
   \verbatim
> plaintext : "3243F6A8885A308D313198A2E0370734"$
> key       : "2B7E151628AED2A6ABF7158809CF4F3C"$
> aes_encrypt_hex_std(plaintext,key);
"3925841D02DC09FBDC118597196A0B32"
> aes_decrypt_hex_std("3925841D02DC09FBDC118597196A0B32", key);
"2B7E151628AED2A6ABF7158809CF4F3C"
   \endverbatim
   This test vector is taken from [Design of Rijndael;John Daemen and
   Vincent Rijmen].
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
