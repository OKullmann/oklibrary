// Matthew Gwynne, 4.8.2009 (Swansea)
/* Copyright 2009, 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/EncryptionDecryption.hpp
  \brief On investigating encryption and decryption using AES SAT-translation


  \todo Overview
  <ul>
   <li> Question here is: Given the current translation, how well can a SAT
   solver perform a simple AES computation (encryption/decryption)? </li>
   <li> It should be that through unit clause propagation etc, the SAT solver
   should be very quick, as there should be no decisions, just simple
   computation. </li>
   <li> See "Encryption" and "Decryption". </li>
  </ul>


  \todo Update instructions
  <ul>
   <li> We need instructions here on how to generate the various
   CNFs which represent AES encryption and decryption with all the 
   various parameters. </li>
   <li> See 
   ComputerAlgebra/Cryptography/Lisp/Cryptanalysis/Rijndael/Translations.mac. 
   </li>
  </ul>

*/
