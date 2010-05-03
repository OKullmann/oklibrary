// Oliver Kullmann, 2.5.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/Transversals/plans/Datak22.hpp
  \brief Investigating the transversal hypergraph of van-der-Waerden hypergraphs for k = 22 (length of arithmetic progressions)


  \todo Elementary statistics for k=22
  <ul>
   <li> Computing the number of minimum transversals:
   \verbatim
oklib_monitor : true;
A22(n):=arithprog_hg(22,n)$
L220_22 : minimum_transversals_mongen(220,A22,[{}])$
   \endverbatim
   </li>
   <li> At C++ level:
   \verbatim
> MinimumTransversals_VanderWaerden-O3-DNDEBUG 300 22 | tee VanderWaerden_Trans_22_300_OUT
   \endverbatim
   </li>
  </ul>


  \todo Only computing the transversal numbers
  <ul>
   <li> Just computing the transversal numbers, using minisat2 and the
   direct translation:
   \verbatim
> VdWTransversalsInc 22 1 0 VDW_22 VDW_22_SAT

L : transform_steps_l(map(third,rest(read_nested_list("VDW_22"))));
 [21,42,64,81,100,119,138,157,176,195,214,233,252,271,290,309,328,347,366
length(L);
 19

> tail -1 VDW_22
22 368 19
   \endverbatim
   </li>
  </ul>

*/
