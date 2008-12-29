// Oliver Kullmann, 1.1.2007 (Swansea)
/* Copyright 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file AlgebraicStructures/plans/Groupoids.hpp
  \brief Plans related to the search for (certain) groupoids


  \todo Encodings
  <ul>
   <li> For a natural number n, the groupoid structures on {1,...,n}
   are captured by the n^2 variables c_{i,j}, i,j in {1,...,n} with
   value-domains {1,...,n} (the value of c_{i,j} is the composition of
   i and j). The two most basic constraints are:
    <ul>
     <li> Commutativity: c_{i,j} = c_{j,i} (for i <> j).
     Are there interesting active clause-sets? Seems to be completely
     straight-forward, and only a question of appropriate data structures.
     </li>
     <li> Associativity: Much less straight-forward, and active-clause-sets
     could be very interesting.
    </ul>
   </li>
  </ul>


  \todo Quasigroups
  <ul>
   <li> Quasigroups are just latin squares (see module LatinSquares), however
   the questions related to quasigroups likely are quite different from
   those occurring in the field of latin squares (?). </li>
   <li> See
   ComputerAlgebra/Algebra/Lisp/Groupoids/Quasigroups/plans/general.hpp. </li>
  </ul>


  \todo Literature surview
  <ul>
   <li> We need overview on the literature. </li>
   <li> Searching for interesting problems. </li>
   <li> Searching for interesting algorithms. </li>
  </ul>


  \todo Discrete groupoids
  <ul>
   <li> I (OK) call a groupoid discrete if x * y in {x,y} is always the case
   (are there notions for that in the literature?). </li>
   Discrete groupoids are idempotent, and can be represented by n^2 - n
   many boolean variables (for each pair (i,j) with (i,j) the variables
   states whether the composition is i or j). </li>
   <li> Are there interesting problems?
    <ul>
     <li> For a start, one could just search for non-commutative discrete
     semigroups (while the commutative discrete semigroups correspond
     1-1 to the linear orders on the ground set --- composition is min
     (or max)). </li>
     <li> Stronger would be to count all discrete semigroups (on {1,...,n}).
     For that we need the active clause-set for associativity from above
     to work with these special variables. </li>
    </ul>
   </li>
  </ul>

*/

