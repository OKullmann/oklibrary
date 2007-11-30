// Oliver Kullmann, 30.11.2007 (Swansea)
/* Copyright 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/Backtracking/plans/general.hpp
  \brief Plans for Maxima-components regarding backtracking algorithms


  \todo Create milestones


  \todo Write tests


  \todo Write docus


  \todo optimal_splitting_tree
  <ul>
   <li> Perhaps storing the trees also for the worst case. </li>
   <li> When searching only for the best case, then branches which
   cannot improve the current minimal size can be cut off. </li>
   <li> Instead of minimising the number of overall nodes, that is,
   minimising the number of leaves (since every inner node has exactly
   two successors), one can also minimise the number of true-leafes (the
   number of clauses in the DNF-representation) or the number of false-leaves
   (the number of clauses in the CNF-representation). </li>
   <li> Introduce an additional parameter d (from 0 to inf), and for depth < d
   then the orbits of the automorphism group acting on the set of 2n literals
   is computed, and instead of evaluated all 2n trees only one element of
   each orbit is chosen (evaluated appropriately).
    <ol>
     <li> See "Symmetries" in
     ComputerAlgebra/Satisfiability/Lisp/plans/general.hpp. </li>
    </ol>
   </li>
   <li> Instead of just getting the minimal size, a variations return
   the complete distribution (for every occurring tree size the number of trees
   for that size). </li>
  </ul>


  \todo Display splitting trees
  <ul>
   <li> Latex output of splitting trees </li>
  </ul>

*/

