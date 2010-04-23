// Oliver Kullmann, 23.4.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/Transversals/plans/Datak18.hpp
  \brief Investigating the transversal hypergraph of van-der-Waerden hypergraphs for k = 18 (length of arithmetic progressions)


  \todo Elementary statistics for k=18
  <ul>
   <li> Computing the number of minimum transversals:
   \verbatim
oklib_monitor : true;
A18(n):=arithprog_hg(18,n)$
L180_18 : minimum_transversals_mongen(180,A18,[{}])$
   \endverbatim
   </li>
   <li> At C++ level:
   \verbatim
> MinimumTransversals_VanderWaerden-O3-DNDEBUG 300 18 | tee VanderWaerden_Trans_18_300_OUT
   \endverbatim
   </li>
  </ul>


  \todo Only computing the transversal numbers
  <ul>
   <li> Just computing the transversal numbers, using minisat2 and the
   direct translation:
   \verbatim
> VdWTransversalsInc 18 1 0 VDW_18 VDW_18_SAT

transform_steps_l(map(third,rest(read_nested_list("VDW_18"))));

length(L);
 
   \endverbatim
   </li>
  </ul>

*/
