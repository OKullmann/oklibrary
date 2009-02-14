// Oliver Kullmann, 28.12.2008 (Swansea)
/* Copyright 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/LatinSquares/plans/general.hpp
  \brief On investigations regarding latin squares and variations


  \todo Counting the number of n-jective relations
  <ul>
   <li> Let mj_n(m) be the number of n-jective relations on a set with
   m elements ("mj" like "multijection"). </li>
   <li> So mj_0(m) = 2, mj_1(m) = m and mj_2(m) = m!, while mj_3(m) is
   the (total) number of latin squares (quasigroups) of order n. </li>
   <li> Conjecture: mj_k(3) = 3 * 2^(k-1) for k >= 1 (verified for k <= 5).
   </li>
   <li> mj_k(4) : 2,4,24,576 </li>
  </ul>

*/

