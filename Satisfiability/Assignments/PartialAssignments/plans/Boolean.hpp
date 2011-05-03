// Oliver Kullmann, 2.5.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKlib/Satisfiability/Assignments/PartialAssignments/plans/Boolean.hpp
  \brief Plans for for boolean partial assignments


  \todo Class BPass0
  <ul>
   <li> BPass0::fill_map should be made superfluous:
    <ol>
     <li> By using a transform-iterator, directly filling the map with
     the pairs (var, cond). </li>
    </ol>
   </li>
   <li> We need tests. </li>
   <li> Update the general concepts, and create a suitable concept for
   BPass0. </li>
  </ul>


  \todo Application ApplyPASS
  <ul>
   <li> Takes exactly one argument, a file with the partial assignment phi.
   </li>
   <li> See "Input and output of partial assignments" in
   Interfaces/InputOutput/plans/general.hpp. </li>
   <li> From standard input the clause-list F in Dimacs format is read, and
   to standard output phi * F is output, again in Dimacs format. </li>
   <li> Parameter handling:
    <ol>
     <li> Since the application uses only one pass and does not store the
     partial assignment, the p-line is copied verbatim from F. </li>
     <li> However then we have a false number of clauses? </li>
     <li> Seems unavoidable, and needed to be repaired afterwards. </li>
     <li> See for example the application "Script for applying partial
     assignments" in Solvers/OKsolver/SAT2002/plans/SimpleParallelisation.hpp.
     </li>
    </ol>
   </li>
   <li> Just before the p-line, as a comment the partial assignment is given.
   </li>
  </ul>

*/
