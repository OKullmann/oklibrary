// Matthew Gwynne, 27.3.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/plans/BreakingAES_R6-10.hpp
  \brief On investigations into breaking reduced round variants of the AES with larger numbers of rounds


  \todo Breaking six round AES using the canonical translation for Sbox and 
  Mul
  <ul>
   <li> See "Breaking one round AES using the canonical translation for Sbox 
   and Mul" in BreakingAES_R1-5.hpp for aims, questions, and how to generate 
   this experiment ($N=6). </li>
   <li> Experiments currently running on "csio", from n=128 (where n is 
   the number of missing key bits) to n=0 in steps of 5, with a timeout of 2
   hours. </li>
  </ul>


  \todo Breaking seven round AES using the canonical translation for Sbox and 
  Mul
  <ul>
   <li> See "Breaking one round AES using the canonical translation for Sbox 
   and Mul" in BreakingAES_R1-5.hpp for aims, questions, and how to generate 
   this experiment ($N=7). </li>
   <li> Experiments currently running on "cshimalia", from n=128 (where n is 
   the number of missing key bits) to n=0 in steps of 5, with a timeout of 2
   hours. </li>
  </ul>


  \todo Breaking eight round AES using the canonical translation for Sbox and 
  Mul
  <ul>
   <li> See "Breaking one round AES using the canonical translation for Sbox 
   and Mul" in BreakingAES_R1-5.hpp for aims, questions, and how to generate 
   this experiment ($N=8). </li>
   <li> Experiments currently running on "cslysithea", from n=128 (where n is 
   the number of missing key bits) to n=0 in steps of 5, with a timeout of 2
   hours. </li>
  </ul>


  \todo Breaking nine round AES using the canonical translation for Sbox and 
  Mul
  <ul>
   <li> See "Breaking one round AES using the canonical translation for Sbox 
   and Mul" in BreakingAES_R1-5.hpp for aims, questions, and how to generate 
   this experiment ($N=9). </li>
   <li> Experiments currently running on "csiapetus", from n=128 (where n is 
   the number of missing key bits) to n=0 in steps of 5, with a timeout of 2
   hours. </li>
  </ul>


  \todo Breaking 10 round AES using the canonical translation for Sbox and 
  Mul
  <ul>
   <li> See "Breaking one round AES using the canonical translation for Sbox 
   and Mul" in BreakingAES_R1-5.hpp for aims, questions, and how to generate 
   this experiment ($N=10). </li>
   <li> Experiments currently running on "csamalthea", from n=128 (where n is 
   the number of missing key bits) to n=0 in steps of 5, with a timeout of 2
   hours. </li>
  </ul>


*/
