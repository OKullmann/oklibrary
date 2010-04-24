// Oliver Kullmann, 23.4.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/Transversals/plans/Datak19.hpp
  \brief Investigating the transversal hypergraph of van-der-Waerden hypergraphs for k = 19 (length of arithmetic progressions)


  \todo Elementary statistics for k=19
  <ul>
   <li> Computing the number of minimum transversals:
   \verbatim
oklib_monitor : true;
A19(n):=arithprog_hg(19,n)$
L190_19 : minimum_transversals_mongen(190,A19,[{}])$
   \endverbatim
   </li>
   <li> At C++ level:
   \verbatim
> MinimumTransversals_VanderWaerden-O3-DNDEBUG 300 19 | tee VanderWaerden_Trans_19_300_OUT
   \endverbatim
   </li>
  </ul>


  \todo Only computing the transversal numbers
  <ul>
   <li> Just computing the transversal numbers, using minisat2 and the
   direct translation:
   \verbatim
> VdWTransversalsInc 19 1 0 VDW_19 VDW_19_SAT

L : transform_steps_l(map(third,rest(read_nested_list("VDW_19"))));
 [18,37,56,75,94,113,132,151,170,189,208,227,246,265,284,303,322,341,342,343,344,345]
length(L);
 22
   \endverbatim
   </li>
  </ul>

*/
