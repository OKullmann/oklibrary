// Oliver Kullmann, 30.11.2007 (Swansea)
/* Copyright 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/ClauseSets/plans/general.hpp
  \brief Plans for Maxima-components regarding clause-sets


  \todo Create milestones


  \todo Write tests


  \todo Write docus


  \todo Input and output
  <ul>
   <li> Functions currently in
   ComputerAlgebra/Satisfiability/Lisp/ClauseSets/BasicOperations.mac
   should move to their own file "InputOutput.mac". </li>
   <li> The usage of "print" likely should be replaced by "printf" from
   package "stringproc". </li>
   <li> We need also reading from Dimacs-files. </li>
  </ul>


  \todo Random generator
  <ul>
   <li> Using the maxima-aes-implementation, implement the random generator
   as in Experimentation/RandomGenerator/plans/general.hpp. </li>
  </ul>


  \todo Resolution
  <ul>
   <li> min_resolution_closure_cs :
    <ol>
     <li> As in "Maxima"-"Monitoring" in ComputerAlgebra/plans/general.hpp,
     perhaps the monitoring-output should be standardised, containing the name
     of the function? </li>
     <li> A second output is needed, with the list of c(F_i)
     for the successive stages. DONE </li>
     <li> In monitoring mode these numbers are output once a round is
     completed (compare "Maxima"-"Monitoring" in
     ComputerAlgebra/plans/general.hpp). DONE </li>
    </ol>
   </li>
   <li> resolution_closure_cs
    <ol>
     <li> Same regarding monitoring as min_resolution_closure_cs. </li>
     <li> The implementation is very similar to min_resolution_closure_cs:
     Should we construct a common generalisation? </li>
    </ol>
   </li>
   <li> DP
    <ol>
     <li> We need also the greedy heuristics, which chooses the DP-variable
     such that the number of clauses for the next level is minimised. </li>
    </ol>
   </li>
  </ul>


*/

