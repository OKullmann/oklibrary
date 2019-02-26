// Oliver Kullmann, 2.3.2012 (Swansea)
/* Copyright 2012, 2013, 2016 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/PigeonholeFormulas/plans/ExtendedResolution.hpp
  \brief Investigations regarding extended resolution for pigeonhole clause-sets


 \todo Sat solvers on extended PHP clause-sets
  <ul>
   <li> The standardised extended clause-set is weak_php_unsat_ext_stdfcs(N),
   which is output by output_weak_php_unsat_ext_stdname(n). </li>
   <li> Performance of SAT solvers (n is the number of holes):
    <ol>
     <li> First impression with cryptominisat (csltok with 2.0 GHz):
     \verbatim
n=10
18.65s
n=11
38.64s
n=12
249.72s
n=13
886.28s
n=14
13187.05s
# with n=14 worse than PHP itself
     \endverbatim
     </li>
     <li> The other solvers seem to perform worse (and also not better than
     their PHP-performance). Though one needs to see complete data. </li>
    </ol>
   </li>
   <li> So apparently no solver can make use of the extension. This seems
   understandable, given the complicated nature of the resolution proof for
   the extension --- all clauses have to be used, in a special order. </li>
  </ul>


    \todo Hardness
  <ul>
   <li> The hardness of weak_php_fcs(m,m-1) vs weak_php_unsat_ext_fcs(m):
   \verbatim
maxima> for m : 1 while true do
  print([hardness_wpi_cs(weak_php_fcs(m,m-1)[2],{{}}),
         hardness_wpi_cs(weak_php_unsat_ext_fcs(m)[2],{{}})])$
[0,0]
[1,1]
[2,2]
[3,3]
[4,4]
     \endverbatim
   </li>
   <li> weak_php_unsat_ext_fcs(m) has a polynomial size resolution proof.
   However, what is the complexity of the smallest tree resolution proof?
   </li>
  </ul>


  \todo Analysis of blocking structure
  <ul>
   <li> Computing the number of blocked 2-clauses which can be removed:
   \verbatim
experiment(n) := for i : 0 thru n do block([P:weak_php_cs(i+1,i),E:weak_php_unsat_ext_fcs(i)[2],R,R2], R:elim_blocked_cs(E), R2:elim_blockedk_cs(E,2), print(i, length(P), length(E), length(R), length(R2), length(E)-length(R2)));
experiment(7);

0 1 1 1 1 0
1 3 3 3 3 0
2 9 17 9 15 2
3 22 54 22 49 5
4 45 125 45 116 9
5 81 241 81 227 14
6 133 413 133 393 20
7 204 652 204 625 27
   \endverbatim
   </li>
   <li> So it seems the number of removable blocked 2-clauses is n*(n+1)/2-1.
   </li>
   <li> The question is whether after removal of the blocked 2-clauses still
   an exponential resolution lower-bound can be shown?!
   </li>
   <li> One needs to determine which clauses are removed.
    <ol>
     <li> For this the generator in
     ComputerAlgebra/Satisfiability/Lisp/Generators/Pigeonhole.mac needs to be
     given more structure (and better documentation). </li>
     <li> Perhaps the extension should be standardised in the sense that only
     extension via {v,a,b},{-v,-a},{-v,-b} is used? Perhaps then removal of
     blocked 2-clauses becomes more powerful? </li>
    </ol>
   </li>
  </ul>

*/
