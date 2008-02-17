// Oliver Kullmann, 20.12.2007 (Swansea)
/* Copyright 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Cryptology/Lisp/plans/general.hpp
  \brief Plans for cryptological tools realised in Maxima/Lisp

  \todo Conversion Functions
  <ul>
    <li> Needs tidying. </li>
    <li> Conversion functions with suitable inverses working on lists
    of the appropriate elements to allow movement between various 
    different representations of data in Cryptology (for AES and others) :
    <ol>
     <li> hexToGF2 (with GF2ToHex) for conversion of a hexidecimal string
     in the same format as in the Design of Rijndael (discuss here). 
     </li>
     <li> GF2ToGF2t4 (with GF2t4ToGF2) for conversion of a list of binary bit 
     values representing GF(2) elements to a list of GF(2^4) elements.
     </li>
     <li> GF4ToGF2t8 (with GF2t8ToGF2t4)) for a conversion from a list of 
     elements in GF(2^4) to elements in GF(2^8).
     </li>
     <li> GF2t8ToPID (with PIDToGF2t8) for a conversion of a list of binary
     bit values representing GF(2) elements to a list of 4-Byte PID column elements.
     </li>
  </ul>

  \todo Create milestones. DONE

*/

