// Oliver Kullmann, 25.7.2007 (Swansea)
/* Copyright 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Cryptology/plans/Rijndael.hpp
  \brief Plans for cryptological tools regarding the Rijndael cipher

  
  \todo Expand docus
  <ul>
   <li> Add basic overview of AES/Rijndael.</li>
   <li> Add additional terminology and refine current terminology</li>
  </ul>

  
  \todo Migrate the mupad-code.
  <ul>
   <li> What is to be achieved here? A movement of the relevant AES code from
   OKlib/ComputerAlgebra/Mupad/TestenFormelGenAES.mup to a seperate mupad
   AES implementation in ComputerAlgebra/Cryptology/ with docus and demos attached?</li>
  </ul>


  \todo Terminology (DONE Moved to 
  ComputerAlgebra/Cryptology/docus/Rijndael.hpp)
  <ul>
   <li> A bit could be called 
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
   </li>
   <li> The most significant bit represents the highest order term in the polynomial,
   and values are usually written in hexadecimal notation, so, for example, 
   A2 is 10100010. Some clarification may be needed on the implementation details
   for the specific GF(2^8) field. </li>
   <li> A 4-byte element formed by taking a column of GF(2^8) elements in the 
   Rijndael block might be called
    <ul>
     <li> a 4-byte column </li>
     <li> an element in the 4-Byte PID (Principal Ideal Domain) </li>
     <li> an element in the QR (Quotient Ring) </li>
    </ul>
   as this is not actually a field (some elements don't have an inverse as the 
   modulus x^4 + 1 isn't irreducible over GF(2^8)). </li>
   <li> Which of these is most appropriate? It seems "Quotient Ring" but further
   reading is necessary and perhaps a more elegant and more specific name can be 
   found. Again, the most significant byte of the 4-bytes represents the coefficient
   of the highest order term in the polynomial. </li>
  </ul>

*/

