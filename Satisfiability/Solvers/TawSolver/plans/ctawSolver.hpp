// Oliver Kullmann, 4.3.2021 (Swansea)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Solvers/TawSolver/plans/ctaw.hpp
  \brief Plans regarding c|m|w|p tawSolver


  \todo Support for large (unsigned) integral types
  <ul>
   <li> Boost seems here the best source for us. </li>
   <li> Signed integral types are needed for general literal-weights
        (i.e., with possible negative values). </li>
  </ul>


  \todo ptawSolver ("projection")
  <ul>
   <li> Certain variables are marked as part of the projection, and the number
        of remaining such variables is kept up-to-date on a path; if m are
        left, and we found a satisfying partial assignment, then 2^m solutions
        are to be counted. </li>
  </ul>


  \todo wtawSolver ("weighted")
  <ul>
   <li> For each literal x a weight w(x) in RR is used. </li>
   <li> When a satisfying partial assignment phi with unset variable-set V
        is found, then
          prod_{x \in phi^{-1}(1)} w(x)  *  prod_{v in V} (w(v) + w(-v))
        is added.
   </li>
   <li> If w(v) + w(-v) = 1, then this factor can be discarded. </li>
   <li> If some w(x) < 0, then signed integers are needed. </li>
   <li> If w(x) = 0, then every branch with x->1 can be aborted.
        We can treat this case as a forced assignment x -> 0.
   </li>
   <li> The standard case is w(x) = w(-x) = 1. </li>
   <li> We could combine w with p to "wp" (discarding a variable can not be
        handled -- we needed to have w(v) + w(-v) = 1, but then one of the
        literals contributes wrongly when being part of a partial satisfying
        assignment).
        But one can use w(v) = w(-v) = 0.5, and then the total count obtained
        has to be multiplied with 2^m for m the number of these variables.
        So indeed projection can be simulated. </li>
  </ul>


  \todo Improved distance
  <ul>
   <li> The distance d_0 "sum of weigthed new clauses", which aims at ucp, is
        to be augmented by a distance d_1 which gets the bigger the more
        clauses are satisfied and the longer the remaining clauses are
        ("chances of satisfiability"). </li>
   <li> For example on inputs in 2-CNF the first distance is very weak. </li>
   <li> More generally, d_0 assumes that autarkies can be reduced, which is
        not the case for #SAT. </li>
  </ul>

*/
