// Matthew Gwynne, 27.3.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/DataEncryptionStandard/plans/KeyDiscovery.hpp
  \brief On investigations into the Data Encryption Standard key discovery


  \todo Transferring the Argosat-desgen example
  <ul>
   <li> Exactly as with argosat-desgen-sat09.tar.bz2, we shall consider
   key 15FBC08D31B0D521, plaintext 038E596D4841D03B and ciphertext
   A2FB6032638EC79D, and providing the examples for all 0 <= p <= 64,
   where the last p keybits are provided (as unit-clauses). </li>
   <li> This can be easily achieved by using function des_key2fcl(key),
   where the first 64-p entries of the 64-bit vector key' are replaced by
   the value "und" (while by hexstr2binv(key) we obtain the key as boolean
   vector). </li>
   <li> Their benchmarks only consider 13 <= 64-p <= 34, d.h.,
   30 <= p <= 51, however we can consider the whole range. </li>
   <li> We consider then all translations, and also many solvers. </li>
   <li> The OKsolver takes at least four hours on any instance (still
   running). </li>
   <li> Solving time (ranked best to worst) on gss-X-s100.cnf:
    <ul>
     <li> X = 13 : glucose (0.61s), precosat-570.1 (3.5s), minisat-2.2.0
     (3.72s), cryptominisat (7s), precosat236 (10.2s). </li>
     <li> X = 14 : precosat-570.1 (2s), minisat-2.2.0 (12.35s), cryptominisat
     (22.33s), precosat236 (32s), glucose (58.53s). </li>
     <li> X = 15 : minisat-2.2.0 (5.43s), cryptominisat (10.71s), 
     precosat-570.1 (44.7s), precosat236 (53.8s), glucose (68.75). </li>
     <li> X = 16 : minisat-2.2.0 (6.76s), precosat236 (32.1s), precosat-570.1 
     (53.8s), cryptominisat (104.38s), glucose (133.32s) . </li>
     <li> X = 17 : minisat-2.2.0 (31.77s), precosat-570.1 (70.8s), 
     precosat236 (109.8s), cryptominisat (144.73s), glucose (896.53s).
     </li>
    </ul>
   </li>
  </ul>


  \todo Move into separate sub-module

*/
