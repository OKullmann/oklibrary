// Oliver Kullmann, 25.8.2007 (Swansea)
/* Copyright 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Cryptology/docus/Rijndael.hpp
  \brief The docus for computer algebra system implementations of the Rijndael block cipher.
  

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

*/
