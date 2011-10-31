// Matthew Gwynne, 31.10.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/plans/ConstraintTemplateRewriteRules.hpp
  \brief Plans for the AES constraint rewrite rules


  \bug Test failure with okltest_aes_sbox_pi_cst_cl
  <ul>
   <li>
   \verbatim
okltest_aes_sbox_pi_cst_cl(aes_sbox_pi_cst_cl)
ASSERT: Expression " {-10,-9,-7,-6,-5,3,4} = {-16,-15,-14,-13,4,11,12} " does not evaluate to true.
   \endverbatim
   </li>
  </ul>


  \bug Test failure with okltest_aes_mul3_pi_cst_cl
  <ul>
   <li> What is the point of these hardcoded values, which apparently do not
   have a mathematical meaning? </li>
  </ul>

*/

