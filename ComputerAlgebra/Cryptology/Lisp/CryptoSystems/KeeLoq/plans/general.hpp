// Matthew Gwynne, 27.2.2012 (Swansea)
/* Copyright 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Cryptology/Lisp/CryptoSystems/KeeLoq/plans/general.hpp
  \brief Plans for the KeeLoq crypto-system in Maxima/Lisp


  \todo Good definition
  <ul>
   <li> A definition is provided for the KeeLoq cipher in
   ComputerAlgebra/Cryptology/Lisp/CryptoSystems/KeeLoq/Cipher.mac. </li>
   <li> Two problems with this definition:
    <ol>
     <li> 0-index arrays are used, whereas the maxima system uses 1-indexed
     arrays, and </li>
     <li> the inputs and outputs are indexed in reverse. </li>
    </ol>
   </li>
   <li> The definition comes from [Algebraic and Slide attacks on KeeLoq;
   Gregory Bard, Nicholas Courtois and David Wagner]. </li>
   <li> We should derive an equivalent definition which doesn't
   use 0-index arrays and doesn't assume the inputs and outputs
   are reversed. </li>
  </ul>


  \todo Add todos


  \todo Create milestones

*/

