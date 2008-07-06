// Oliver Kullmann, 6.7.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Algebra/Lisp/Groupoids/plans/Closures.hpp
  \brief Plans on computing hulls for groupoids


  \todo Write tests


  \todo Closures
  <ul>
   <li> A fundamental operation is the closure of a subset under the
   operations. </li>
   <li> As a result we obtain the closure plus shortest terms in the
   generators representing the elements of the closure, plus all relations
   found such that the generated structure is determined by these relations.
   </li>
   <li> Easiest for semigroups, where the terms are just words over the
   generators (for which it should be possible to specify names). </li>
   <li> Here, starting with a set of elements, which yields the set of
   words of length 1, we form all words of length 2, determine equalities to
   the already existing elements and to the new elements, which yield new
   relations, given that all existing relations have been taken into account;
   iterating this process expands only the words of full length. </li>
   <li> For groups we have to take inversion into account; and relations, 
   which for semigroups are equations, that is, 2-element sets of words, can
   now be simplified to just single words (equal to the neutral element). 
   </li>
  </ul>

*/

