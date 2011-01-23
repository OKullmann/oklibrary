// Matthew Gwynne, 5.1.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Cryptology/Lisp/CryptoSystems/Rijndael/docus/SmallScaleAdvancedEncryptionStandard.hpp
  \brief User documentation for the Maxima computer algebra system implementation of the small-scale AES block cipher


  <h1> Using small-scale AES at the Maxima/Lisp level </h1>

  Functions are presented which implement the AES ("Advanced Encryption
  Standard") block cipher.

  
  <h2> General idea </h2>

  <ul>
   <li> XXX </li>
   <li> See also [Small Scale Variants of the AES; Carlos Cid, Sean Murphy 
   and M.J.B Robshaw]. </li>
  </ul>


  <h2> Overview </h2>

  <ul>
   <li> Throughout the implementation, particular parameters maintain the same
   shortened names for brevity, and ease of use:
   <ul>
    <li> p: plaintext block </li>
    <li> k: key block </li>
    <li> c: ciphertext block. </li>
   </ul>
   In each case, there is the assumption that each block is the appropriate
   dimension and field size (see "General idea").
   </li>
   <li> In all functions the plaintext, key and ciphertext blocks are
   assumed to be the same dimension. </li>
   <li> For small-scale encryption we have the following encryption
   and decryption functions:
   <ul>
    <li> "ss_encrypt(p,k,r,b,e)" and "aes_decrypt(c,k,r,b,e)" are the 
    functions used to encrypt a plaintext block using the given key block
    with r rounds encryption/decryption and using the standard (as defined
    by us) GF(b^e) field for the word field. </li>
    <li> The blocks are given as matrices and the number of rows and number of
    columns are derived from the these matrices. The field elements are given 
    as arbitrary polynomials (see "Data types"). </li>
    <li> "ss_encrypt_gen" and "ss_decrypt_gen" provide generalised versions
    of "ss_encrypt" and "ss_decrypt" which allow the specification of 
    additional parameters, which have been given default values (based
    on the other parameters) by "ss_encrypt" and "ss_decrypt":
    <ul>
     <li> mod_poly: the modulo polynomial used to quotient polynomials for
     the small-scale field elements. Defaults in "ss_encrypt" to 
     ss_polynomial(b,e). </li>
     <li> sbox_f: the sbox function, taking a polynomial and returning the
     polynomial result of the small-scale sbox for the given parameters.
     Defaults in "ss_encrypt" to ss_sbox .</li>
     <li> mixcolumns_matrix: the Mixcolumns matrix used in the MixColumns
     operation. Defaults to ss_mixcolumns_matrix(b,e,n_R) where n_R is the
     number of rows in the given plaintext and ciphertext blocks. </li>
    </ul>
    </li>
   </ul>
   </li>
  </ul>  


  <h2> Data types </h2>

  <ul>
   <li> The elements of the small-scale word-field (the elements of the block)
   are represented as arbitrary polynomials and then are 
   "brought into range" or "standardised" using the given polynomial 
   polynomial (see ss_polynomial in 
   ComputerAlgebra/Cryptology/Lisp/CryptoSystems/Rijndael/ByteField.mac).
   </li>
   <li> AES blocks are then matrices of polynomials, where the dimensions of
   the small-scale AES block is given by the dimension of the matrix. 
   </li>
   <li> As with the AES implementation, there is no interface to the bit
   level operations of the small-scale AES, but similar to the explanation
   given in "Data Types" in
   ComputerAlgebra/Cryptology/Lisp/CryptoSystems/Rijndael/docus/SmallScaleAdvancedEncryptionStandard.hpp
   one may use the ss_stand function in 
   ComputerAlgebra/Cryptology/Lisp/CryptoSystems/Rijndael/SmallScaleWordField.mac
   to standardise the polynomial and then consider the coefficients of the
   polynomial as the e bits of the small-scale word (i.e., the coefficient of 
   x^e becomes the most significant bit of the byte and the coefficient of 1 
   becomes the least - see [Design of Rijndael;John Daemen and Vincent Rijmen]
   and [Small Scale Variants of the AES; Carlos Cid, Sean Murphy and M.J.B 
   Robshaw]). </li>
  </ul>


  <h2> Examples </h2>
  
  <ul>
   <li> Encrypting a 128-bit word represented as a list of integers:
   \verbatim
> key : matrix([x,x^2],[0,1])$
> plaintext : genmatrix(lambda([a,b],0),2,2)$
> num_rounds : 4$
> field_base : 2$
> field_exponent : 4$
> ciphertext : ss_encrypt(plaintext,key,num_rounds,field_base,field_exponent);
matrix([0,x^3+x^2],[x^3+x^2+1,x])
> ss_decrypt(ciphertext,key,num_rounds,field_base,field_exponent);
matrix([0,0],[0,0])
   \endverbatim
   </li>
  </ul>

*/
