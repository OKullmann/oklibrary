// Oliver Kullmann, 17.2.2006 (Swansea)
/* Copyright 2006 - 2007, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Algorithms/AllSolutions/plans/general.hpp
  \brief Plans for the module on components for finding all
  satisfying assignments.

  Computing representations for the set of all satisfying assignments.


  \todo Update namespaces.


  \todo Update milestones.


  \todo Hypergraph transversals
  <ul>
   <li> See HypergraphTransversals/plans/general.hpp
   for the special problem of finding minimal transversals. </li>
   <li> In AllSolutions/plans/MinimalAssignments.hpp the
   hypergraph-point-of-view is discussed in more details. </li>
   <li> The links to module HypergraphTransversals should be clarified. </li>
   <li> The generic approach would be to only implement the SAT-case,
   and the hypergraph case is then obtained as the special case of
   uniform domain size 1 (for positive monosigned literals). Looks desirable.
   </li>
  </ul>


  \todo Further milestones:
  <ul>
   <li> Shall we first concentrate on the direct "hypergraph-transversal"
   approach? </li>
  </ul>


  \todo A general approach is to find an equivalent DNF by DPLL
  <ul>
   <li> Without further simplification it will actually
   compute a hitting DNF. </li>
   <li> What are measurements for minimisation of the DNF? The tau-value is
   a possibility, but it's 2 for (boolean) hitting clause-sets, and
   greater otherwise, so on its own it just says "use hitting cls".
   The number of partial assignments (the number of clauses in the
   DNF), or some other size measurement of the DNF are possibilities. </li>
   <li> We need a literature survey here. There should be something in
   the computer learning literature (which studies conversion between
   CNF and DNF). </li>
  </ul>


  \todo Another approach is using OBDD's (this should likely go
  into its own module).


  \todo What is the relation to the counting problem?
  <ul>
   <li> Likely counting should go into it's own module; techniques seem to be
   different, and approximation makes sense for counting (not for
   enumeration).) </li>
  </ul>

*/

/*!
  \namespace OKlib::AllSolutions
  \brief Components for extracting all satisfying assignments (in
  some form).
*/

namespace OKlib {
  namespace AllSolutions {
  }
}

