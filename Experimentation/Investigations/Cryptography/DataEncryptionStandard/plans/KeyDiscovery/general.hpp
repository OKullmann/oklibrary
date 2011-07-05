// Matthew Gwynne, 25.5.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/DataEncryptionStandard/plans/KeyDiscovery/general.hpp
  \brief On investigations into the Data Encryption Standard key discovery


  \todo Overview
  <ul>
   <li> Solving the key discovery problem for:
    <ul>
     <li> 1-round DES, see
     Investigations/Cryptography/DataEncryptionStandard/plans/KeyDiscovery/1.hpp.
     </li>
     <li> 3-round DES, see
     Investigations/Cryptography/DataEncryptionStandard/plans/KeyDiscovery/3.hpp.
     </li>
     <li> 4-round DES, see
     Investigations/Cryptography/DataEncryptionStandard/plans/KeyDiscovery/4.hpp.
     </li>
     <li> 5-round DES, see
     Investigations/Cryptography/DataEncryptionStandard/plans/KeyDiscovery/5.hpp.
     </li>
     <li> 16-round DES with some known key bits, see
     Investigations/Cryptography/DataEncryptionStandard/plans/KeyDiscovery/KnownKeyBits.hpp.
     </li>
    </ul>
   </li>
  </ul>


  \todo Add information on specific S-box translations to experiment instances
  <ul>
   <li> At present, if one encounters an experiment instance for a DES
   experiment, then it is not clear exactly how it was produced. </li>
   <li> The ambiguity occurs because, over time, smaller "minimum" and 1-base
   representations for the S-boxes are found. </li>
   <li> Based on these new representations, new translations are generated,
   and experiments rerun. </li>
   <li> This leaves one with several copies of the same translation file,
   and the possibility of uncertainty about which is which. </li>
   <li> All instance generation code for experiments should include
   information, via the DIMACS comments of the instance, on the S-box
   representations used. </li>
   <li> For example, the seeds given by RandomRUcpBases to generate the
   1-base for a DES S-box could be output as comment in the DIMACS FILE as:
   \verbatim
c DES S-box 1 1-base seeds: 1 (gen) 2 (base).
   \endverbatim
   </li>
   <li> Even better, tools and scripts should be written which take
   only parameters such as seeds from RandomRUcpBases or ubcsat etc and
   output the CNFs. </li>
   <li> Then the actual generation instructions could be added to the CNF
   file as comments. </li>
  </ul>


  \todo DONE Move into separate sub-module

*/
