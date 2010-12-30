// Matthew Gwynne, 4.8.2009 (Swansea)
/* Copyright 2009, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/plans/SmallScaleKeyDiscovery.hpp
  \brief On investigating finding the key given plaintext/ciphertext pairs for small scale AES.


  \todo Overview
  <ul>
   <li> The idea here is, given a plaintext P and a key K which with a certain
   set of small scale parameters encrypts P to ciphertext C, can we use
   some small scale AES translation to derive K given P and C. </li>
   <li> See 
   ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/Translations.mac .
   </li>
  </ul>

*/
