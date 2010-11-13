// Oliver Kullmann, 26.5.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/FuzzySets/Lisp/plans/general.hpp
  \brief Plans in general for fuzzy sets in Maxima/Lisp


  \todo Basic notions
  <ul>
   <li> As abbreviation for a fuzzy set we could use "fys". </li>
   <li> A basic problem is that "fuzzy sets" are related to some base set;
   and this base set in general is infinite (like the natural or the real
   numbers). </li>
   <li> Perhaps this base set is just left unspecified in general, and must
   be stated for each concrete application. </li>
   <li> Then a fys A is a map A: B -> [0,1] (where the right-hand side denotes
   the interval from 0 to 1), where B is the base set. </li>
   <li> So as a maxima object a fys would just be a function f(x), where
   all f(b) for b in B can be evaluated and yield a real number from 0 to 1.
   </li>
   <li> We should allow as "number" here precise terms (for integers and
   rational numbers) as well as floating-point numbers (ordinary, or "big
   floats"). </li>
   <li> Should we use for example "fysr" for fuzzy-subsets of RR, "fysz" for
   fuzzy subsets of ZZ, "fysn", "fysn0" for fuzzy subsets of NN resp. NN0, and
   "fysq" for fuzzy subsets of QQ? </li>
   <li> We need then the standard fuzzy set-operations, "fysunion",
   "fysintersection" and "fyscomplement", for the standard operation (max,
   min, difference with 1) as well as for arbitrary operations. </li>
   <li> Or should we use "union_fys" etc. ? However, since it is not union,
   but only something similar, we should use "fysunion(A,B)" etc. </li>
  </ul>


  \todo More general notions
  <ul>
   <li> What about more general types of "fuzzyfication"? </li>
   <li> In a different context, one should also allow arbitrary "truth values",
   for example elements of a boolean algebra or a hyting lattice, that is,
   "subsets" of B of the form A: B -> P, where P is some quasi-ordered set.
   </li>
   <li> Perhaps calling such "sets" "P-sets" ? </li>
   <li> The basic P-set operations would be determined by operations from
   P^2 to P resp. P to P. </li>
   <li> What are general axioms here? </li>
  </ul>

*/
