// Oliver Kullmann, 13.8.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/PseudoBoolean/plans/general.hpp
  \brief Plans regarding pseudo-boolean constraints


  \todo The notion of a "pseudo-boolean constraint"
  <ul>
   <li> Given literals x_1, ..., x_k and integers a_1, ..., a_k, b,
   a pseudo-boolean constraint expresses sum_{i=1}^k a_i * x_i >= b,
   where a satisfied literal means integer 1, a falsified literal means
   integer 0, and instead of ">=" we can also have "<=" or "=". </li>
   <li> Compare "Representing cardinality constraints" in
   ComputerAlgebra/Satisfiability/Lisp/PseudoBoolean/plans/CardinalityConstraints.hpp
   for the special case where all a_i = 1. </li>
   <li> Where for cardinality constraint we just had a set L of literals, now
   we have a list L of literals and a list A of the same length of integers
   (containing the coefficients). </li>
   <li> It just remains the question on the precise composition of the list
   which represents a pseudo-boolean constraint. </li>
   <li> One could use lists [a,A,L,b], where a, b are integers or inf or minf,
   meaning a <= A*L <= b. </li>
  </ul>

*/

