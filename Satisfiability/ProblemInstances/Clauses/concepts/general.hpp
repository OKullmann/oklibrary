// Oliver Kullmann, 24.7.2005 (Swansea)
/* Copyright 2005 - 2007, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/ProblemInstances/Clauses/concepts/general.hpp
  \brief Plans for concepts for clauses


  \todo Connections
  <ul>
   <li> See ComputerAlgebra/Satisfiability/Lisp/plans/Clauses.hpp. </li>
   <li> For an earlier attempt see
   OKsolver/Experimental/AllgKlassen200203/Clauses.hpp. </li>
  </ul>


  \todo General ideas
  <ul>
   <li> Do we need to distinguish between and-clauses and or-clauses? </li>
  </ul>


  \todo Watched literals
  <ul>
   <li> Motivated by the submission of Armin Biere to SAT2007, for the
   look-ahead the clauses could manage the two watched literals in them
   (inclusive the two pointers to the respective next clauses). </li>
   <li> The main point here is to find out (as easily as possible) when
   possibly a clause became unit. </li>
   <li> Also interesting here to find out when possibly a clause became binary
   (here we would need three watched literals per clause). </li>
   <li> Taking care of those special literals in a clause would be the
   responsibility of the clause (not some higher-level mechanism). </li>
  </ul>


  \todo Partial assignments
  <ul>
   <li> A fundamental issue here is the relation to the concept of
   partial assignments (see Concepts/plans/PartialAssignments.hpp). </li>
   <li> It should be possible to easily switch between
   the concepts of partial assignments and clauses; perhaps the only
   difference is in the complexity of the operations: while clauses correspond
   to sets (using space proportionally to their size), partial assignments
   correspond to total maps (using a special value "unassigned"; supporting
   constant time access). </li>
   <li> As set-objects partial assignments and clause-sets
   are identical (for standard literals), namely they are sets of pairs of
   variables and (forbidden) values, interpreted as constituting the
   assignment or as literals. </li>
   <li> Regarding the operations considered above, from the concept-side (the
   operations and their complexity) there is quite some difference between
   clauses and partial assignments (for example regarding watched literals).
   So turning a partial assignment into a clause (and vice versa) must be
   possible (lossless), but might be more costly. </li>
   <li> The CNF(DNF)-representation of some condition is just a representation
   of those assignments we realised as falsifying (satisfying). </li>
  </ul>

*/

