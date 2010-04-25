// Oliver Kullmann, 25.4.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/Transversals/plans/Datak20.hpp
  \brief Investigating the transversal hypergraph of van-der-Waerden hypergraphs for k = 20 (length of arithmetic progressions)


  \todo Elementary statistics for k=20
  <ul>
   <li> Computing the number of minimum transversals:
   \verbatim
oklib_monitor : true;
A20(n):=arithprog_hg(20,n)$
L200_20 : minimum_transversals_mongen(200,A20,[{}])$
   \endverbatim
   </li>
   <li> At C++ level:
   \verbatim
> MinimumTransversals_VanderWaerden-O3-DNDEBUG 300 20 | tee VanderWaerden_Trans_20_300_OUT
   \endverbatim
   </li>
  </ul>


  \todo Only computing the transversal numbers
  <ul>
   <li> Just computing the transversal numbers, using minisat2 and the
   direct translation:
   \verbatim
> VdWTransversalsInc 20 1 0 VDW_20 VDW_20_SAT

L : transform_steps_l(map(third,rest(read_nested_list("VDW_20"))));

 length(L); 

> tail -1 VDW_20

   \endverbatim
   </li>
  </ul>

*/
