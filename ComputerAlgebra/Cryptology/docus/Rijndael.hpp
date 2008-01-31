// Oliver Kullmann, 25.8.2007 (Swansea)
/* Copyright 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Cryptology/docus/Rijndael.hpp
  \brief The docus for Computer Algebra System Implementations of the Rijndael block cipher.
  
  <h1> Terminology </h1>

  <ul>
   <li> A bit may be called 
    <ul>
     <li> an element in GF(2) </li>
     <li> an element in the "bit field". </li>
    </ul>
   <li> An 8-bit element, representing a polynomial over GF(2) or an element
   in GF(2^8) can be called
    <ul>
     <li> an element in GF(2^8) </li>
     <li> an element in the "byte field". </li>
     <li> an element in the "rijndael byte field" </li>
    </ul>
   The most significant bit represents the highest order term in the polynomial,
   and values are usually written in hexadecimal notation, so, for example, 
   A2 is 10100010 which represents the polynomial x^7 + x^5 + x.
   </li>
   <li> A 4-byte element formed by taking a column of GF(2^8) elements in the 
   Rijndael block might be called
    <ul>
     <li> a 4-byte column </li>
     <li> an element in the 4-Byte PID (Principal Ideal Domain) </li>
     <li> an element in the QR (Quotient Ring) </li>
    </ul>
   as this is not actually a field (some elements don't have an inverse as the 
   modulus x^4 + 1 isn't irreducible over GF(2^8)) but since this is only used
   for multiplication by a constant, the constant can be carefully chosen such
   that it does have an inverse. </li>
   <li> A block in Rijndael is set of input data supplied as plaintext to the
   algorithm. This can be of size 128 to 256 bit (in increments of 32) and is,
   in general, modelled as a (4xn) matrix of bytes (hence the requirement for 
   block size to be in increments of 32 bits).</li>
  </ul>

  <h1> Overview of the Sage implementation </h1>

  <h2> Helper Functions </h2>

  The sage implemenation has several helper functions to convert between various representations
  of input and key blocks in Rijndael, the two most helpful when using Rijndael being
  <ul>
   <li> hexToGF2t8Mat - which takes a hexidecimal string and converts it to a matrix of GF(2^8)
   elements for use in functions such as rijndael_en etc. 
   </li>
   <li> GF2t8MatToHex - which takes a matrix of GF(2^8) elements and returns a hexidecimal string.
   </li>
  </ul>

  <h2> Main Functions </h2>
  
  The sage implementation offers several main functions to encrypt various block 
  and key sizes with the Rijndael block cipher and these are also made up 
  of several functions.
  <ul>
   <li> rijndael_en(pm,km,r = -1)  - where 
    <ul>
     <li> pm is a matrix of GF(2^8) field elements (see Helper Functions) 
     representing the plaintext block </li>
     <li> km is the key represented as a matrix of GF(2^8) field elements </li>
     <li> r is an optional argument specifying the number of rounds, where if 
     unspecified will deduce the recommended number of rounds for the given 
     block and key sizes. </li>
     <li> The result of this function is the encrypted ciphertext block 
     represented as a matrix of GF(2^8) elements. </li>
    </ul>
   </li>
   <li> rijndael_de(cm,km,r = -1)  - where
    <ul>
     <li> cm is a matrix of GF(2^8) field elements (see Helper Functions) 
     representing the ciphertext block <li>
     <li> km is the key represented as a matrix of GF(2^8) field elements </li>
     <li> r is an optional argument specifying the number of rounds, where if 
     unspecified it will deduce the recommended number of rounds for the given 
     block and key sizes. </li>
     <li> The result of this function is the decrypted plaintext block 
     represented as a matrix of GF(2^8) elements. </li>
   </li>
  </ul>

  <h2> Example </h2>
  \verbatim 
~ # attach $OKplatform/OKsystem/Transitional/ComputerAlgebra/Cryptology/Rijndael.sage 

sage: k = hexToGF2t8Mat('00000000000000000000000000000000')                
sage: p = hexToGF2t8Mat('00000000000000000000000000000000')                
sage: c = rijndael_en(p,k)                                                 
sage: GF2t8MatToHex(c)                                                     
66E94BD4EF8A2C3B884CFA59CA342B2E
sage: GF2t8MatToHex( rijndael_de(c,k) )                                    
00000000000000000000000000000000
sage: k = hexToGF2t8Mat('000000000000000000000000000000000000000000000000')
sage: GF2t8MatToHex( rijndael_en(p,k) )                                    
AAE06992ACBF52A3E8F4A96EC9300BD7

    \endverbatim
    where $OKPLATFORM has to be replaced by the path to the OKplatform directory. 
    The hex notation used above is the same as used in the text Design of Rijndael,
    a columnwise byte representation in hex (2 digits per byte).

*/
