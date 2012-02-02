// Oliver Kullmann, 2.5.2011 (Swansea)
/* Copyright 2011, 2012 Oliver Kullmann
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
   <li> We need unit-tests. </li>
   <li> Update the general concepts, and create a suitable concept for
   BPass0. </li>
  </ul>


  \todo Computing the clashing variables in PassClashes
  <ul>
   <li> Currently PassClash exits with code 0 if the two input partial
   assignments, phi1 and phi2, clash, and 1 otherwise. </li>
   <li> It would be useful to also output the variables which the two
   partial assignments clash on. </li>
   <li> Rather than outputting just the variables, it seems best to
   output the clashing part of the first partial assignment. This
   yields the most information about how they clash, including
   which assignment had the variable in which sign. </li>
   <li> The output is then the partial assignment which assigns to all
   the variables that phi1 and phi2 clash in, and for which phi1 is
   an extension. </li>
   <li> To output this partial assignment, we then need a standard
   concept for the output of partial assignments, as discussed in
   "Partial assignment output adaptor" below. </li>
  </ul>


  \todo Partial assignment output adaptor
  <ul>
   <li> We need some form of output adaptor for partial assignments.
   </li>
   <li> The output adaptors for clause-sets, such as CLSAdaptorDIMACSOutput
   in OKlib/Satisfiability/Interfaces/InputOutput/ClauseSetAdaptors.hpp,
   should be used as a model. </li>
   <li> An example of the need for this is given in
   "Computing the clashing variables in PassClashes" above. </li>
  </ul>


  \todo Adaptor ApplyPassAdaptor
  <ul>
   <li> We need unit-tests. </li>
  </ul>


  \todo DONE
  Application ApplyPass
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


  \todo Write application tests for ApplyPass

*/
