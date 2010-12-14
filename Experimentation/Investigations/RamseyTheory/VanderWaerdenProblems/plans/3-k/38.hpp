/ Oliver Kullmann, 14.12.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/3-k/38.hpp
  \brief On investigations into vdw_2(3,38)


  \todo vanderwaerden_2(3,38) > ?
  <ul>
   <li> The very weak current conjecture is vanderwaerden_2(3,38) = ???. </li>
   <li> Starting search with the best palindromic solution:
   \verbatim
> cat AltExp/Exp_PdVanderWaerden_2-3-38_gsat-tabu-???/VanDerWaerden_pd_2-3-38_???.cnf_sol | PdExtend-O3-DNDEBUG ??? > solution

> RunVdW3k 38 ??? rots 100 100000000 solution
   \endverbatim
   </li>
  </ul>


  \todo Palindromic version:
  <ul>
   <li> ? < vanderwaerden_g([3,38],"pd") =(conj) ? =(conj)
   vanderwaerden([3,38]) - ???. </li>
   <li> vanderwaerden_g([3,38],"pdsat") =(conj) ?. </li>
   <li> Certificates:
    <ol>
     <li> n=?:
     \verbatim
     \endverbatim
     </li>
     <li> n=?:
     \verbatim
     \endverbatim
     </li>
    </ol>
   </li>
   <li> "RunPdVdW3k 38 gsat-tabu 100 30000000" yields
   \verbatim
   where all solutions were found within in the first ? runs. </li>
  </ul>

*/
