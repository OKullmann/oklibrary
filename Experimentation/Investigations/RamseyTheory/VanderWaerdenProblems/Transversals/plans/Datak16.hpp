// Oliver Kullmann, 22.4.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/Transversals/plans/Datak16.hpp
  \brief Investigating the transversal hypergraph of van-der-Waerden hypergraphs for k = 16 (length of arithmetic progressions)


  \todo Elementary statistics for k=16
  <ul>
   <li> Computing the number of minimum transversals:
   \verbatim
oklib_monitor : true;
A16(n):=arithprog_hg(16,n)$
L160_16 : minimum_transversals_mongen(160,A16,[{}])$
   \endverbatim
   </li>
   <li> At C++ level:
   \verbatim
> MinimumTransversals_VanderWaerden-O3-DNDEBUG 300 16 | tee VanderWaerden_Trans_16_300_OUT
   \endverbatim
   </li>
  </ul>


  \todo Only computing the transversal numbers
  <ul>
   <li> Just computing the transversal numbers, using minisat2 and the
   direct translation:
   \verbatim
> VdWTransversalsInc 16 1 0 VDW_16 VDW_16_SAT

transform_steps_l(map(third,rest(read_nested_list("VDW_16"))));
 [15,30,46,57,70,83,96,109,122,135,148,161,174,187,196,198,199]
length(L);
 17
   \endverbatim
   Values 17 is attained 2 times. </li>
  </ul>

*/
