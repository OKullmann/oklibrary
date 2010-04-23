// Oliver Kullmann, 23.4.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/Transversals/plans/Datak17.hpp
  \brief Investigating the transversal hypergraph of van-der-Waerden hypergraphs for k = 17 (length of arithmetic progressions)


  \todo Elementary statistics for k=17
  <ul>
   <li> Computing the number of minimum transversals:
   \verbatim
oklib_monitor : true;
A17(n):=arithprog_hg(17,n)$
L170_17 : minimum_transversals_mongen(170,A17,[{}])$
   \endverbatim
   </li>
   <li> At C++ level:
   \verbatim
> MinimumTransversals_VanderWaerden-O3-DNDEBUG 300 17 | tee VanderWaerden_Trans_17_300_OUT
   \endverbatim
   </li>
  </ul>


  \todo Only computing the transversal numbers
  <ul>
   <li> Just computing the transversal numbers, using minisat2 and the
   direct translation:
   \verbatim
> VdWTransversalsInc 17 1 0 VDW_17 VDW_17_SAT

transform_steps_l(map(third,rest(read_nested_list("VDW_17"))));

length(L);
 
   \endverbatim
   </li>
  </ul>

*/
