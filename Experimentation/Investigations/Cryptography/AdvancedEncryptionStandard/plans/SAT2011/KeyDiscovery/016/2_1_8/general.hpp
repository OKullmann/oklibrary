// Matthew Gwynne, 20.5.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/KeyDiscovery/016/2_1_8/general.hpp
  \brief Investigations into small scale AES key discovery for AES with a 2x1 plaintext matrix and 8-bit field elements


  \todo Comparisons
  <ul>
   <li> Comparing the different translations. </li>
   <li> See:
    <ul>
     <li> "The canonical box translation". </li>
    </ul>
   </li>
  </ul>


  \todo The canonical box translation
  <ul>
   <li> Up to 14 rounds solved in < 200s. </li>
   <li> Minimum times to solve with best solvers:
    <ul>
     <li> 1 round; 0.11s; minisat-2.2.0. </li>
     <li> 14 rounds; 181.9s; precosat236. </li>
    </ul>
   </li>
   <li> See:
    <ul>
     <li> Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/KeyDiscovery/016/2_1_8/1_13.hpp. </li>
     <li> Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/KeyDiscovery/016/2_1_8/14_13.hpp. </li>
    </ul>
   </li>
  </ul>


  \todo The 1-base box translation
  <ul>
   <li> Add data. </li>
  </ul>


  \todo The "minimum" box translation
  <ul>
   <li> Add data. </li>
  </ul>

*/
