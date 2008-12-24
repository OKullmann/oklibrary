// Oliver Kullmann, 15.3.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Lisp/Reductions/plans/GeneralisedUCP.hpp
  \brief Plans for Maxima-components regarding r_k-reductions


  \todo More efficient implementations
  <ul>
   <li> Instead of generalised_ucp(F,1) the more efficient ucp_cs_0
   (or ucp_cs_1 etc.) should be used where possible. </li>
   <li> Except of the most basic implementations. </li>
   <li> Perhaps we should allow alternative implementations of r_1 as
   additional parameters; or we should use a special variable (which
   can be dynamically altered). </li>
  </ul>


  \todo Strengthen the tests
  <ul>
   <li> We need more systematic coverage (with small clause-sets). </li>
   <li> We should also employ some form of data-tables so that the tests
   mainly run through these tables. </li>
   <li> And we should strengthen re-use of sub-tests between the various (rather
   similar) test-functions. </li>
  </ul>


  \todo Use oracles
  <ul>
   <li> Implement the generalisations of r_k using oracles S for satisfiability
   and U for unsatisfiability (see [Investigating a general hierarchy of
   polynomially decidable classes of {CNF}'s based on short tree-like resolution
   proofs; Kullmann, 1999, ECCC], and [Upper and Lower Bounds on the Complexity
   of Generalised Resolution and Generalised Constraint Satisfaction Problems;
   Kullmann, 2004, AMAI]. </li>
   <li> Perhaps two variations: Early detection of oracle-applicability, and
   late detection (at the leaves). </li>
   <li> For additional oracle-examples see
   ComputerAlgebra/Satisfiability/Lisp/Backdoors/plans/general.hpp. </li>
   <li> Likely the combination with autarky-search needs more thought. </li>
   <li> 2-CNF for U and S can at most increase k by 2, but is easy to achieve.
   </li>
   <li> Renamable Horn is only interesting for S. </li>
   <li> An extension is given by linearly satisfiable clause-sets. </li>
   <li> Another class contained in the class of linearly satisfiable clause-sets
   is given by matching satisfiable clause-sets. </li>
   <li> All the above classes are stable under application of forced partial
   assignments, and likely we should only consider such classes. For d >= 1,
   the class of satisfiable clause-sets of maximal deficiency d (as well as
   the class of unsatisfiable clause-sets of maximal deficiency d) seems not
   to be stable (examples ???), and thus perhaps should be excluded from
   consideration. </li>
  </ul>

*/

