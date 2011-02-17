// Oliver Kullmann, 11.2.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Representations/MixColumns.hpp
  \brief Studying good representations of the AES (small-scale) MixColumns operations


  \todo Sizes (and structure)
  <ul>
   <li> To start, we need precise and explained data on the sizes of the
   MixColumns-permutations for AES and variations. </li>
  </ul>


  \todo Direct linear representation
  <ul>
   <li> Every linear map over ZZ_2 from {0,1}^n to {0,1}^m can be described by
   m sums over the input variables. </li>
   <li> We need to determine this representation, and we need to find out the
   relation to our sum-of-boxes representation. </li>
  </ul>


  \todo Computation of r_k-bases
  <ul>
   <li> See Experimentation/Investigations/BooleanFunctions/plans/Permutations/Linear.hpp
   for general investigations. </li>
   <li> For very small sizes we can directly compute r_0- and r_1-bases. </li>
   <li> Can we learn something from that for the bigger cases? </li>
  </ul>

*/
