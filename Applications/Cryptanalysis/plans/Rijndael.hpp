// Oliver Kullmann, 16.12.2006 (Swansea)
/* Copyright 2006 - 2007, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Applications/Cryptanalysis/plans/Rijndael.hpp
  \brief Plans for the module on treating Rijndael

  The prototype is discussed in ComputerAlgebra/Cryptology/plans/Rijndael.hpp.


  \todo Connections
  <ul>
   <li> See
   ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/plans/general.hpp
   for the Maxima-level. </li>
   <li> See
   Investigations/Cryptography/AdvancedEncryptionStandard/plans/general.hpp
   for investigations. </li>
  </ul>


  \todo Overview
  <ul>
   <li> There are many other cryptanalytic scenarios which are
   worth investigating --- one needs an overview. </li>
  </ul>


  \todo First tests AES
  <ul>
   <li> Once we have the various translations, the first test is how
   fast F_AES(p, k, ?) and F'_AES(?, k, c) can be solved ---
   this actually should be fast. </li>
   <li> Next is to solve F_AES(?, k, c) and F'_AES(p, k, ?) --- this
   could already be interesting, as a test for SAT solvers how
   good they understand the inversion. </li>
   <li> With
    <center>
      F2_AES(p, k, c) := F_AES(p, k, c) + F'_AES(p, k, c)
    </center>
   then both ways definitely should be fast. </li>
  </ul>


  \todo Further tests
  <ul>
   <li> Key inference: The most obvious next thing to do is to investigate
   key inference, that is, try to solve formulas
   F_AES(p, ?, c), F'_AES(p, ?, c), F2_AES(p, ?, c). </li>
   <li> And for r + s = 10 one can consider
   F_AES^r(p, ?, x) + F'_AES^s(x, ?, c). </li>
   <li> Partial key inference: Instead of investigating the full key inference
   problem, one could try how many bits of the key need to be supplied
   (as unit clauses) so that the problem finally becomes feasible. </li>
  </ul>


*/

