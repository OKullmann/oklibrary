// Oliver Kullmann, 21.3.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/Autarkies/plans/LinearAutarkies.hpp
  \brief Plans for Maxima-components regarding linear autarkies


  \todo Finding linear autarkies
  <ul>
   <li> DONE (predicate laut_paocs_p)
   Given the clause-variable matrix M, non-trival linear autarkies
   are given as non-trivial solutions of M x >= 0. </li>
   <li> DONE (helper functions are provided by us)
   From what Maxima provides, apparently only maximize_lp (in package
   "simplex") is of use. </li>
   <li> DONE (find_linearautarky_ocs)
   One possibility to achieve complete search for linear autarkies
   is to consider one after another the additional constraints x_i >= 1
   and x_i <= -1. </li>
   <li> An alternative is to first consider balanced linear autarkies (by
   linear equations, i.e., M x = 0), and then solving all M x = e_i
   for the possible e_i. </li>
   <li> In both cases the objective function would be constant zero. </li>
   <li> The above methods just search for feasible solutions.
   How can we use the optimisation facilities? Apparently only heuristical
   usage is possible?
    <ol>
     <li> For example maximising the sum of all x_i. </li>
     <li> If this only yields 0, then one can attempt minimising the sum
     of the x_i. </li>
     <li> What can be said in the remaining case? All solutions fulfill
     sum x_i = 0; is this possible if we also know that no balanced
     autarkies (non-trivial solutions of M x = 0) exist? </li>
    </ol>
   </li>
   <li> Finally we need a dedicated approach.
    <ol>
     <li> M has a non-trival linear autarky iff the polyhedron M x >= 0
     is unbounded, and a non-trivial linear autarky corresponds to an infinite
     ray contained in the polyhedron. Can this be better exploited? </li>
    </ol>
   </li>
  </ul>

*/

