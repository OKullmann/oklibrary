// Oliver Kullmann, 26.7.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Algorithms/AllSolutions/plans/Enumeration.hpp
  \brief Solving SAT via enumeration of all total assignments


  \todo Trivial SAT algorithm
  <ul>
   <li> Implementing the trivial SAT algorithm, using any enumeration; likely
   some kind of visitor concept is useful here. </li>
   <li> The trivial algorithm should at least compute the number of
   satisfying assignments (if not stopped after finding the first one) and
   the maximal number of simultaneously satisfiable clauses. </li>
   <li> Optionally he can also decide minimal unsatisfiability of F by the
   following process: Each time an assignment satisfying c(F)-1 clauses is
   found, the falsified clause is marked. F is minimally unsatisfiable iff
   F is unsatisfiable and at the end all clauses have been marked. </li>
  </ul>


  \todo Improving the trivial algorithm
  <ul>
   <li> Using Gray codes and a "flip operation" of the formula (which can be
   implemented efficiently using literal-clause graphs and storing information
   about the assignments for each clause; this data structure is also
   fundamental for local-search algorithms (and in general any algorithm using
   total assignments)). </li>
   <li> See Combinatorics/Enumeration/plans/GrayCodes.hpp. </li>
   <li> See "Create module for solving small SAT problems" in
   Satisfiability/Algorithms/plans/general.hpp. </li>
  </ul>


  \todo Variations
  <ul>
   <li> The trivial enumeration algorithm can be used for many things. </li>
   <li> The search for autarkies goes through all partial assignments. </li>
   <li> See Hypergraphs/Colourings/plans/general.hpp. </li>
   <li> We could also have a common scheme for enumerating all combinations
   and enumerating all permutations (etc.). Compare
   Combinatorics/Enumeration/plans/general.hpp. </li>
  </ul>

*/

