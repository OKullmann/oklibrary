// Oliver Kullmann, 31.8.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Algorithms/LocalSearch/plans/SupportTotalAssignments.hpp
  \brief Supporting data structures for total assignments to be used
  for efficient enumeration and local search (for satisfying assignments
  as well as autarkies). Operations to support variable flips.


  \todo Concepts:
  <ul>
   <li> See Concepts/plans/Assignments.hpp. </li>
   <li> The first "client" seems to be module AutarkySearch. </li>
  </ul>


  \todo Design and implement
  <ul>
  <li> Should utilise the clause-literal graph. </li>
  <li> Default statistics: Number of falsified and satisfied literals
  per clause, </li>
  <li> and the total number of satisfied and critical clauses. </li>
  <li> Policies should allow for other statistics (for example when
  enumerating all assignments, all kinds of maximisations/minimisations
  could be performed; </li>
  <li> or when using the data structure for local search for
  autarkies, then the number of satisfied and critical clauses are needed
  for all levels). </li>
 </ul>

*/

