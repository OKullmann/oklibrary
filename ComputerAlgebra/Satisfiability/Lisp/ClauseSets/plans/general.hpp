// Oliver Kullmann, 30.11.2007 (Swansea)
/* Copyright 2007, 2008 Oliver Kullmann
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


  \todo Notions for clauses, clause-sets etc.
  <ul>
   <li> See "Better general naming conventions" in
   ComputerAlgebra/Satisfiability/Lisp/plans/general.hpp
   for the general naming-discussion. </li>
   <li> See ComputerAlgebra/Satisfiability/Lisp/plans/Literals.hpp
   for general discussions on "literals". </li>
   <li> See ComputerAlgebra/Satisfiability/Lisp/plans/Clauses.hpp for
   general discussions on "clauses". </li>
   <li> See ComputerAlgebra/Satisfiability/Lisp/plans/PartialAssignments.hpp
   for general discussions on "partial assignments". </li>
   <li> All variations on clause-sets should still be "syntactical objects",
   so the discussion in
   ComputerAlgebra/Satisfiability/Lisp/plans/SatisfactionProblems.hpp
   is not relevant yet. </li>
   <li> At the base level we have
    <ol>
     <li> "clause-sets" as now; </li>
     <li> "ordered clause-sets", repetition-free lists of clauses; </li>
     <li> "clause-lists" (lists of clauses). </li>
    </ol>
   </li>
   <li> "Formal" versions (just adding variables):
    <ol>
     <li> "formal clause-sets" as now; </li>
     <li> "formal ordered clause-sets"; </li>
     <li> "formal clause-lists". </li>
    </ol>
    Additionally, the formal clause-sets also exist as "formal
    multi-clause-sets" and "formal labelled clause-sets".
   </li>
   <li> Should we also allow "multi-clause-sets" and "labelled clause-sets"
   (without the variables)?
    <ol>
     <li> Otherwise the "formal" in this context is superfluous. </li>
     <li> On the other hand, the multi- and labelled versions are introduced
     in the graph-theoretical context, and there we always have the set
     of vertices given (except of set-systems). </li>
    </ol>
   </li>
   <li> A "formal multi-clause-set" is a triple [V,F,c] s.t. [V,F] is a
   formal clause-set and c: F -> NN; accordingly "formal ordered multi-
   -clause-sets". </li>
   <li> Labelled clause-sets (not "general clause-sets" to avoid confusion)
    <ol>
     <li> A "formal labelled clause-set" is a triple [V,F,f], where V is a
     set of variables, F a set of clause-labels, and f assigns to each
     element of F a clause over V. </li>
     <li> Accordingly, "formal ordered labelled clause-sets are triples
     [V,F,f], where now V,F are repetition-free lists. </li>
    </ol>
   </li>
   <li> Relations to hypergraphs (see
   ComputerAlgebra/Hypergraphs/Lisp/plans/general.hpp):
    <ol>
     <li> fcs <-> hg, focs <-> ohg </li>
     <li> fmcs <-> mhg, fomcs <-> omhg </li>
     <li> flcs <-> ghg, folcs <-> oghg. </li>
    </ol>
    Additionally we have
    <ol>
     <li> "cs" corresponds to "set-systems" </li>
     <li> "ocs" corresponds to "ordered set-systems" </li>
     <li> "cl" corresponds to "lists of sets". </li>
    </ol>
    A somewhat subtle point: clause-sets here are the only type of
    objects which are deeply copied (as sets!), while the rest is, since we
    have lists, copied in the shallow way.
   </li>
   <li> Formal clause-lists (fcl) have no correspondence at the hypergraph
   side:
    <ol>
     <li> If we have a list V of vertices and a list E of hyperedges, then
     we use the ghg [V,E,identity]. </li>
     <li> Seems alright. </li>
     <li> So perhaps also for clause-sets we should abandon "fcl" (but keep
     "cl"), since we have already "flcs". </li>
    </ol>
   </li>
   <li> Then we have the "monosigned" versions, where literals are pairs
   [v,e], with e a value. </li>
   <li> And there a "signed" versions, where then e is a set of values. </li>
   <li> A problem with non-boolean variables is, where to put the information
   about the domain of the variables:
    <ol>
     <li> See the discussions in
     ComputerAlgebra/Satisfiability/Lisp/plans/SatisfactionProblems.hpp.
     </li>
     <li> Perhaps it's not part of clause-sets etc., but only part of
     "problems" given for example to SAT-solvers. </li>
    </ol>
   </li>
   <li> Conversions:
    <ol>
     <li> "cs2cl": from clause-sets to clause-lists </li>
     <li> Likely "cs_to_fcs" should become "cs2fcs". </li>
    </ol>
   </li>
  </ul>


  \todo Organisation
  <ul>
   <li> We should have "Substitutions.mac":
    <ol>
     <li> See "Applying substitutions" in
     ComputerAlgebra/Satisfiability/Lisp/Symmetries/plans/general.hpp. </li>
     <li> Everything currently under "Substitution and renaming" goes there.
     </li>
    </ol>
   </li>
   <li> We should create "Statistics.mac".
    <ol>
     <li> DONE : move. </li>
     <li> A problem is now how to handle inclusion. </li>
     <li> For the moment, "Statistics.mac" and "BasicOperations.mac" are
     inclusions-wise equivalent. </li>
    </ol>
   </li>
   <li> See "Input and output" below. </li>
   <li> We should create "PartialAssignments.mac". </li>
  </ul>


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


  \todo standardise_fcs (in
  ComputerAlgebra/Satisfiability/Lisp/ClauseSets/BasicOperations.mac)
  <ul>
   <li> See "Applying substitutions" in
   ComputerAlgebra/Satisfiability/Lisp/Symmetries/plans/general.hpp.
   </li>
   <li> See "Organisation" above. </li>
   <li> Perhaps, once a framework for literal-substitutions is in place,
   then the renaming-functions are updated using these more general
   substitution-maps instead of hash-maps. </li>
   <li> Ask on maxima-mailing-list whether a parallel substitution
   is available (this should speed up renaming). </li>
   <li> DONE The current implementation (using iterated substitution) is
   incorrect in case the clause-set uses already natural numbers as
   variables. </li>
   <li> DONE Otherwise, investigate how hash-maps can be made available,
   store the (whole) substitution via a hash-map, and compute
   the new clause-set via transforming clause for clause. </li>
   <li> DONE (we can now rename w.r.t. a given list of variables)
   Perhaps we could establish general renaming functionality. </li>
  </ul>

*/

