// Oliver Kullmann, 11.5.2013 (Swansea)
/* Copyright 2013 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/BooleanFunctions/plans/Hardness/LinearEquations.hpp
  \brief Investigations regarding hardness of representations of systems of linear equations


  \todo gen_2xor_fcl
  <ul>
   <li> Creation:
   \verbatim
output_gen_2xor_stdname(n);
   \endverbatim
   </li>
   <li> The hardness of output_gen_2xor_stdname(n) is precisely n. </li>
   <li> OKsolver_2002-O3-DNDEBUG behaves completely predictable:
    <ol>
     <li> max_tree_depth = n-2 </li>
     <li> number_of_nodes = 2^(n-1) - 1 </li>
     <li> number_of_quasi_single_nodes = 2^(n-3) </li>
     <li> number_of_2-reductions = 2^(n-2) </li>
    </ol>
   </li>
   <li> satz215 behaves completely predictable:
    <ol>
     <li> NB_BACK = 2^(n-3) </li>
     <li> NB_BRANCHE = 2^(n-2) - 1 </li>
    </ol>
    satz215 seems slower than the OKsolver.
   </li>
   <li> march_pl solves these instances via equivalence-reasoning. </li>
   <li> minisat also solves all instances via (quick) preprocessing. Would be
   interesting to find it what happens here. </li>
   <li> Also minisat-no-pre solves large instances very quickly, and all
   other conflict-driven solvers (with or without preprocessing). </li>
   <li> The w-hardness of these instances seems to be 2:
   \verbatim
for k : 0 thru 5 do print(k, min_kresolution_closure_cs(fcl2cs(gen_2xor_fcl(k)),2));
0 [{{}},[1,1]]
1 [{{}},[2,1,1]]
2 [{{}},[4,4,1,1]]
3 [{{}},[12,30,36,10,1,1]]
4 [{{}},[20,58,240,142,16,1,1]]
5 [{{}},[28,90,508,1882,592,34,1,1]]
   \endverbatim
   </li>
  </ul>


*/



