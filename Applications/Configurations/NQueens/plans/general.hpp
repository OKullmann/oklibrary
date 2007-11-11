// Oliver Kullmann, 8.7.2007 (Swansea)
/* Copyright 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Configurations/NQueens/plans/general.hpp
  \brief Plans on the n queens problem


  \todo Update namespace.


  \todo Create milestones.


  \todo Literature review
  <ul>
   <li> What is the complexity of finding one solution and
   of counting all solutions (in n) ? </li>
   <li> One can also consider the enumeration of all
   solutions, for example in lexicographical order,
   with polynomial delay (or finding the first and
   the next solution). </li>
  </ul>


  \todo Different perspectives:
  <ul>
   <li> A natural formulation considers the hypergraph with
   n^2 vertices and with hyperedges corresponding to the rows,
   columns and diagonals, and searches for a strong independent
   set (see Hypergraphs/IndependentSets/plans/general.hpp) of
   size n. </li>
   <li> Another formulation is as a crisp CSP-problem (see
   Transformers/CrispCSP/plans/general.hpp). </li>
   <li> And, of course, various (generalised) clause-sets-
   formulations. </li>
   <li> and, of course, active clause-sets. </li>
  </ul>


  \todo Different formulations
  <ul>
   <li> The most natural perspective is the queens-perspective,
   that is, n variables for the n queens. </li>
   <li> One could also consider the field-perspective, that
   is, boolean n^2 variables (with a queen or not). </li>
   <li> And one can combine these perspectives. </li>
  </ul>

*/

