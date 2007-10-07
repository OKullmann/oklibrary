// Oliver Kullmann, 7.10.2007 (Swansea)
/* Copyright 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Applications/Cryptanalysis/docus/general.hpp
  \brief Main docus-file for the module on cryptanalysis


  <h1> Cryptanalysis and SAT </h1>

  <h2> Basic ideas </h2>

  The basic idea is to bring together the "general intelligence"
  of SAT with the "special intelligence" of cryptography ---
  such unifications are one of the main goals of the
  OKlibrary.

  <ul>
   <li> Simple CNF transformation will not break any
   cryptosystem, but it is a start (when performed in
   a modular way, so that also more general decompositions,
   using constraints at various levels, are obtained). </li>
   <li> The obvious decompositions into components, looking for
   "active clauses" stronger than clauses but still efficient,
   likely will still not break anything relevant, however it
   might reveil that in a sense for example block ciphers are in
   fact designed to withstand SAT attacks. </li>
   <li> What is needed are novel decompositions, yielding
   non-trivial active clauses and active clause-sets,
   combined with powerful general SAT techniques. </li>
   <li> Especially regarding Rijdael (AES) the integration of
   SAT and computational commutative algebra becomes interesting.
   </li>
  </ul>

*/

