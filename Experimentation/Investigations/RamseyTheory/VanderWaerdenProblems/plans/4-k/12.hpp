// Oliver Kullmann, 6.3.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/4-k/12.hpp
  \brief On investigations into vdw_2(4,12)


  \todo Best ubcsat-solver
  <ul>
   <li> We need to find the best algorithm from the ubcsat-1-2-0 suite. </li>
  </ul>


  \todo vanderwaerden_2(4,12) > 393
  <ul>
   <li> Start with the palindromic solution for n=393. </li>
  </ul>


  \todo vdw_2^pd(4,12) >= (387,394)
  <ul>
   <li> minisat-2.2.0:
    <ol>
     <li> n=388: aborted:
     \verbatim
restarts              : 2490360
conflicts             : 2538607498     (2596 /sec)
decisions             : 2932947182     (0.00 % random) (3000 /sec)
propagations          : 62093142862    (63504 /sec)
conflict literals     : 75126436549    (30.65 % deleted)
Memory used           : 276.00 MB
     \endverbatim
     (cpu-time unreliable). </li>
     <li> This is a hard, but rather small clause-set:
     \verbatim
> cat VanDerWaerden_pd_2-4-12_388.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
194 15544 87698 0 87698 1 1
 length count
2 65
4 12259
6 18
12 3202
     \endverbatim
     </li>
    </ol>
   </li>
   <li> Do we have an easy-hard pattern based on parity? </li>
   <li> Certificates:
    <ol>
     <li> n=386:
     \verbatim
12,17,19,22,28,32,41,43,48,50,
51,53,56,61,70,72,77,79,86,91,
92,101,102,104,105,107,108,109,121,129,
132,138,142,143,147,148,150,152,158,161,
169,174,176,181,182,183,185,186,188,189
     \endverbatim
     </li>
     <li> n=393:
     \verbatim
5,7,12,16,17,26,28,34,35,42,
52,57,65,71,74,79,83,85,89,90,
91,94,100,104,106,111,114,115,117,121,
123,125,126,132,139,148,149,158,162,171,
172,174,176,180,182,183,186,187,188,191,
193,197
     \endverbatim
    </ol>
   </li>
   <li> We need to find the best algorithm from the ubcsat-1-2-0 suite. </li>
   <li> For now we consider adaptg2wsat as best (it was determined as best
   for vdw_2^pd(5,8) in VanderWaerdenProblems/plans/5-k/general.hpp. </li>
   <li>  "RunPdVdWk1k2 4 12 adaptg2wsat 100 1000000" yields
   \verbatim
Break point 1: 387
Break point 2: 394
   \endverbatim
   finding all solutions at the latest in the first run with random seed.
   </li>
  </ul>

*/
