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


  \todo Systematise the use of suffices like "cs" and "cs_f".


  \todo Input and output
  <ul>
   <li> Functions currently in
   ComputerAlgebra/Satisfiability/Lisp/ClauseSets/BasicOperations.mac
   should move to their own file "InputOutput.mac". </li>
   <li> The usage of "print" likely should be replaced by "printf" from
   package "stringproc". </li>
   <li> We should have options for output:
    <ol>
     <li> Instead of for example "php(3,2)" print out "php_3_2". </li>
     <li> DONE And also print it out in strict Dimacs format, optionally
     with the mapping from natural numbers to original variables given
     in the comments. </li>
    </ol>
   </li>
   <li> We need also reading from Dimacs-files. </li>
  </ul>


  \todo General clause-sets
  <ul>
   <li> A "general clause-set" is a triple [V,F,f], where V is a set of
   variables, F a set of clause-labels, and f assigns to each element of
   F a clause over V. </li>
   <li> As we have a promotion from clause-sets to formal clause-sets, we
   also need a promotion from a formal clause-set to a general clause-set.
   </li>
  </ul>


  \todo standardise_fcs (in
  ComputerAlgebra/Satisfiability/Lisp/ClauseSets/BasicOperations.mac)
  <ul>
   <li> DONE The current implementation (using iterated substitution) is
   incorrect in case the clause-set uses already natural numbers as
   variables. </li>
   <li> So ask on maxima-mailing-list whether a parallel substitution
   is available. </li>
   <li> DONE Otherwise, investigate how hash-maps can be made available,
   store the (whole) substitution via a hash-map, and compute
   the new clause-set via transforming clause for clause. </li>
   <li> Perhaps we could establish general renaming functionality. </li>
  </ul>


  \todo Further statistics (for
  ComputerAlgebra/Satisfiability/Lisp/ClauseSets/BasicOperations.mac))
  <ul>
   <li> A map from literals to literal-degrees. </li>
   <li> A map from variables to variable-degrees. </li>
  </ul>


  \todo Resolution
  <ul>
   <li> Perhaps this topic should go into its own plans-file (and likely
   even into its own module). </li>
   <li> See ProofSystems/Resolution/plans/Search.hpp. </li>
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
     <li> The current implementation of opt_min_dp can be improved if only
     the minimum or the maximum is sought. </li>
     <li> DONE (there is only the technical problem that apparently local hash
     arrays are not recognised by "arrayinfo" and "listarray" ? We should
     ask about this at the maxima-mailing-list)
     To make the current implementation worthwhile, perhaps it should
     show the full distribution of sizes, using a map (size -> count). </li>
     <li> DONE The output of distribution_min_dp should be further processed,
     so that easily all information is available.
      <ol>
       <li> Perhaps we just sort the ocurring sizes together with their
       counts, and then output a list of pairs [size, count], sorted
       by size (ascending). </li>
       <li> It seems actually, that "arrayinfo" already returns a sorted list,
       so that nothing needs to be done. </li>
      </ol>
     </li>
     <li> We need also the greedy heuristics, which chooses the DP-variable
     such that the number of clauses for the next level is minimised. </li>
    </ol>
   </li>
   <li> Resolution proofs
    <ol>
     <li> The natural format for a resolution proof is a non-empty list,
     where each entry is either a clause (an "axiom") or a pair consisting
     of a clause (the "resolvent") and a pair of (smaller) indices (the
     indices of the "parent clauses"). </li>
     <li> We need a correctness-checker. </li>
     <li> We should also investigate the existing file-formats for resolution
     proofs, and we should provide input- and output-facilities. </li>
     <li> This linear format is in 1-1 correspondence to the representation
     via labelled dag's; we need a representation of labelled graphs,
     digraphs and labelled digraphs. </li>
     <li> The above can easily be generalised to non-boolean clause-sets.
     </li>
    </ol>
   </li>
   <li> Bounded resolution
    <ol>
     <li> Implement the different forms of bounded resolution. </li>
     <li> In this way we can determine the width of a clause-set. </li>
    </ol>
   </li>
   <li> Read-once resolution proofs
    <ol>
     <li> Write a checker whether a resolution proof is read-once. </li>
     <li> Implement the translation of "has read-once refutation" into
     SAT. </li>
    </ol>
   </li>
  </ul>

*/

