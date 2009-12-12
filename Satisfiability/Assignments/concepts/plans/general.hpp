// Oliver Kullmann, 24.7.2005 (Swansea)
/* Copyright 2005 - 2007, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKlib/Satisfiability/Assignments/concepts/plans/general.hpp
  \brief Plans for concepts for total assignments.


  \todo Concept discussion
  <ul>
   <li> The main point here is to consider partial assignments with
   fixed domain (that is, variables for which the value is defined). </li>
   <li> One needs to discuss the relationship to
   Concepts/plans/PartialAssignments.hpp.
    <ol>
     <li> For assignments we do not need to consider all those
     extension- and undo-problems. </li>
     <li> And we should focus on literals with one variable. </li>
     <li> However there is the problem of generalised literals ---
     should this be considered here, or should we consider "assignments" here
     as "total assignments" in the sense of <strong>single models</strong>
     (while partial assignments stand for <strong>sets of models</strong>) ?!
     </li>
    </ol>
   </li>
   <li> Concepts for assignments as specialisations of concepts for maps
   (see my notes as discussed in the MSc thesis of B):
    <ol>
     <li> If f is an assignment, then dom(f) is defined, </li>
     <li> and for v of type dom(f) the expression f(v) can be build
     (v is a variable with values, f(v) is of the value type). </li>
     <li> dom(f) is a set in the general sense (which can have iterators
     and element-decision). As a refinement iterators through all pairs (x, y)
     (so that std::map can easily yield models). </li>
     <li> However, with the directive of being able to use types directly
     supported by C++, "f[variable_index]" would be better, since then
     arrays and vectors could be used. </li>
    </ol>
   </li>
   <li> The main application seems to be local-seach algorithms.
   See LocalSearch/plans/SupportTotalAssignments.hpp. </li>
   <li> But there are many other uses of total assignments: basically every
   SAT algorithms needs to use them, to find out about the state of the
   variables. </li>
  </ul>

*/

