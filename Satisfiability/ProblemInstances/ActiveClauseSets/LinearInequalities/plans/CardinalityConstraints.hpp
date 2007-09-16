// Oliver Kullmann, 10.6.2007 (Swansea)
/* Copyright 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file LinearInequalities/plans/CardinalityConstraints.hpp
  \brief Plans for the special linear inequality where all
  coefficients are 0 or 1


  \todo Upper bounds
  <ul>
   <li> A very basic (and important) form is the constraint
   <center>
   "v_1 + ... + v_k <= K" (call it UPPER({v_1, ..., v_k}, K)
   </center>
   for a set {v_1, ..., v_k} of boolean variables and a constant K (meaning that
   at most K of the variables v_i are true; one would assume that
   not allowing a multi-set {v_1, ..., v_k} should enable more
   efficient algorithms; if {v_1, ..., v_k} is a multi-set with multiplicities
   from ZZ, then we have the general case of linear inequalities). OF course,
   instead of considering only variables, one should allow literals.
   </li>
   <li> For non-boolean variables v_i the constraint
   <center>
   "|{v_1, ..., v_k}| <= K" (call it UPPERV({v_1, ..., v_k}, K)
   </center>
   seems to be the "right form" (with "V" for "values"; meaning that the
   set of (different) values of variables v_i has at most K members).
   Using the standard translation to boolean variables, UPPERV is
   expressed by UPPER over all new variables v_e over all possible values,
   where v_e is equivalent to the disjunction of all possible literals v_i=e.
   </li>
   <li> However, UPPERV cannot express UPPER ? A real generalisation would
   be UPPERO(V, e, K) ("O" for "occurrences") for some value e, meaning that
   at most K of the variables in V take value e (this can be generalised
   using a set E of values).
   </li>
   <li> See Generators/plans/AMO.hpp for the special case UPPER(L,1). </li>
  </ul>


  \todo Lower bounds
  <ul>
   <li> Accordingly we have LOWER(V, K) (sum of V is at least K). </li>
   <li> And for non-boolean variables we have LOWERV(V,K) (number of
   (different) values is at least K) and LOWERO(V,e,K) (number of variables
   taking value e is at least K). </li>
   <li> See Generators/plans/AMO.hpp for the special case LOWER(L,1). </li>
   <li> See InjectivityConstraints/plans/InjectivityConstraints.hpp for the
   special case LOWERV(V, |V|). </li>
  </ul>

*/

