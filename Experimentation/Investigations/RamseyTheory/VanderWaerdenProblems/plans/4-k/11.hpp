// Oliver Kullmann, 6.3.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/4-k/11.hpp
  \brief On investigations into vdw_2(4,11)


  \todo Best ubcsat-solver
  <ul>
   <li> We need to find the best algorithm from the ubcsat-1-2-0 suite. </li>
   <li> For vdw_2(4,12) it seems adaptg2wsat is best, so we take this as the
   starting poin (see
   Investigations/RamseyTheory/VanderWaerdenProblems/plans/4-k/12.hpp). </li>
  </ul>


  \todo vanderwaerden_2(4,11) > 347
  <ul>
   <li> Start with the palindromic solution for n=347.
    <ol>
     <li> First we need to extract the solution. </li>
    </ol>
   </li>
   <li> Starting from scratch (cstriton):
   \verbatim
> k1=4 k2=11 n0=12 alg="adaptg2wsat" runs=100 cutoff=100000000; nohup RunVdWk1k2 ${k1} ${k2} ${n0} "${alg}" ${runs} ${cutoff} &
   \endverbatim
   </li>
  </ul>


  \todo vdw_2^pd(4,11) = (343,348)
  <ul>
   <li> Established by minisat-2.2.0. </li>
   <li> Do we have an easy-hard pattern based on parity? </li>
  </ul>

*/
