// Oliver Kullmann, 29.12.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/FiniteFunctions/plans/Thresholds.hpp
  \brief Plans regarding boolean threshold functions


  \todo General threshold functions
  <ul>
   <li> With [Knuth, Vol. 4, Fascicle 0, Section 7.1.1] we consider the
   threshold functions
   f(x_1, ..., x_n) = 1 iff w_1 * x_1 + ... + w_n * x_n >= t
   for integer constants w_i. </li>
   <li> As constraints, these conditions are also known as "pseudo-boolean
   constraints". </li>
   <li> Since we are interested for example in CNF- and DNF-representations,
   we also consider the negations, which (essentially) can be represented
   by allowing "<=" instead of ">=". How to call them? The above as "upper"
   threshold functions, while these as "lower" threshold functions? </li>
   <li> Naming: threshold_bf(W), lthreshold_bf(W). </li>
  </ul>


  \todo Cardinality constraints
  <ul>
   <li> Important special cases of threshold functions are obtained when
   using w_i = 1 for all i. </li>
   <li> So the conditions are x_1 + ... + x_n >= t resp. <= t. </li>
   <li> Naming: cardinality_bf(n,t) resp. lcardinality_bf(n,t). </li>
   <li> For the special case t=1 we use alo_bf(n) resp. amo_bf(n). </li>
   <li> Clause-set realisations are called cardinality_cnf_cs(L,t) resp.
   cardinality_dnf_cs(L,t) etc., where now L is an arbitrary set of literals.
   </li>
   <li> The condition x_1 + ... + x_n = t is expressed by ecardinality_bf(n,t).
   </li>
   <li> For an application see  "Systematisation" in
   ComputerAlgebra/Satisfiability/Lisp/Generators/plans/LatinSquares.hpp
   regarding the CNF-realisations. </li>
  </ul>

*/

