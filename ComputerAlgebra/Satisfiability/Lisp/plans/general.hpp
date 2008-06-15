// Oliver Kullmann, 29.6.2007 (Swansea)
/* Copyright 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/plans/general.hpp
  \brief Plans for Lisp-SAT in general


  \todo Relations to other modules
  <ul>
   <li> What is the relation to
   LogicalReflection/FormalisedProofs/plans/FundamentalADTs.hpp ? </li>
  </ul>


  \todo Update plans:
  <ul>
   <li> Satisfiability/Lisp/plans/Clauses.hpp </li>
   <li> Satisfiability/Lisp/plans/Literals.hpp </li>
   <li> Satisfiability/Lisp/plans/MinimalSatisfyingAssignments.hpp </li>
   <li> Satisfiability/Lisp/plans/SatisfactionProblems.hpp </li>
  </ul>


  \todo Redesign
  <ul>
   <li> See "Clause-lists instead of clause-sets" below. </li>
   <li> See "Better general naming conventions" below. </li>
  </ul>


  \todo Clause-lists instead of clause-sets
  <ul>
   <li> See "General design: Lists *here* are more fundamental than sets"
   in ComputerAlgebra/plans/Maxima.hpp for the general discussion. </li>
  </ul>


  \todo Better general naming conventions
  <ul>
   <li> See "Notions for clauses, clause-sets etc." in
   ComputerAlgebra/Satisfiability/Lisp/ClauseSets/plans/general.hpp
   for special issues regarding clause-sets etc. </li>
   <li> See "Naming" in
   ComputerAlgebra/Satisfiability/Lisp/ClauseSets/plans/Statistics.hpp
   for naming of statistics. </li>
   <li> Systematise the use of suffices like "cs" and "cs_f".
    <ol>
     <li> For variables: "v". </li>
     <li> For literals: "l". </li>
     <li> For clauses: "c". </li>
     <li> For clause-sets: "cs". </li>
     <li> For clause-lists: "cl". </li>
     <li> For formal clause-sets: "fcs". </li>
     <li> For formal clause-lists: "fcl". </li>
    </ol>
   </li>
   <li> Then we have general clause-sets etc.: "gcs", "gcl" ? </li>
   <li> And we have the ordered variations: "ocs", "ofcs", "ogcs".
    <ol>
     <li> An ocs is the same structure as a cl, an ofcs the same structure
     as a fcl, an ogcs the same as gcl, only that repetitions are not allowed
     (at all). </li>
    </ol>
   </li>
   <li> What about literals as sets of pairs [v,e], where e is
   a value (forbidden for CNF, allowed for DNF?
    <ol>
     <li> We could use the prefix "ms" for monosigned? </li>
     <li> So "msl", "msc", "mscs", "mscl", "msfcs", "msgcs"? </li>
     <li> And "msocs", "msofcs", "msogcs". </li>
    </ol>
   </li>
   <li> And then we have signed literals as sets of pairs [v,E],
   where E is a set of values (or a list?)?
    <ol>
     <li> We could use the prefix "s" for signed? </li>
     <li> So "sl", "sc", "scs", "scl", "sfcs", "sgcs"? </li>
     <li> And "oscs", "osfcs", "osgcs". </li>
    </ol>
   </li>
   <li> Then we have the different kind of partial assignments:
    <ol>
     <li> Likely, partial assignments should be syntactically identical
     with clauses. </li>
     <li> Notions: "pa", "mspa", "spa". </li>
     <li> And then, like for literals, clauses, clause-sets etc.
     one has the CNF- and the DNF-*interpretation*. </li>
     <li> For clauses we wanted to stay with sets; perhaps for partial
     assignments we could have them ordered? </li>
     <li> So speaking of "opa", "omspa" and "ospa". </li>
     <li> Perhaps one should actually also allow to have ordered clauses?
     Better not! Then we have too many combinations. </li>
     </li>
    </ol>
   </li>
   <li> It seems that in general we do not mention the output-type in the
   name:
    <ol>
     <li> However, if we do, for example "clause-sets maps to hash-map",
     then we should use "...hm_cs(args)". </li>
     <li> The special syntax "cs2hm(F)" should be reserved for conversions.
     </li>
    </ol>
   </li>
   <li> There is the issue of using underscores ("min_var_deg_cs") or
   not ("varregcsp").
    <ol>
     <li> Perhaps leaving it out only for those predicates. </li>
    </ol>
   </li>
   <li> See discussion of in-place modifications in "Singular DP-reduction" in
   Satisfiability/Lisp/Reductions/plans/DP-Reductions.hpp. </li>
  </ul>


  \todo New module FiniteFunctions
  <ul>
   <li> See also
   Transitional/Satisfiability/FiniteFunctions/plans/general.hpp. </li>
   <li> The main point is the construction of representations of finite
   functions via generalised CNF/DNF, propositional logic, CSPs, alliances
   of active clause-sets etc. </li>
   <li> Collect the links within part ComputerAlgebra:
    <ol>
     <li> We have something in Cryptology (MG). </li>
    </ol>
   </li>
   <li> Study the representations of fundamental functions:
    <ol>
     <li> Cardinality constraints:
      <ul>
       <li> At least k out of n boolean variables are true. </li>
       <li> At most k out of n boolean variables are true. </li>
       <li> Exactly k out of n boolean variables are true. </li>
      </ul>
     </li>
    </ol>
   </li>
  </ul>


  \todo Develops combinations of look-ahead with conflict-driven
  solvers
  <ul>
   <li> See [OK, Present and future of practical SAT solving;
   CSR 8-2008] for first schemes. </li>
   <li> This should go to Backtracking/CombinedSchemes.mac. </li>
   <li> Perhaps first we do not consider autarkies (and we add them
   later). </li>
  </ul>
   

  \todo Symmetries
  <ul>
   <li> Compute the orbits of literals under the action of the
   automorphism group of a clause-set. </li>
   <li> Stronger one could compute simply all permutations combined with
   sign flips which leave the clause-set invariant, i.e., the full
   automorphism group in the trivial representation. </li>
   <li> Then one needs to look for more intelligent representations
   of the automorphism group. </li>
   <li> Likely, Maxima is only for simple and initial attempts towards
   symmetries, and Gap is the real tool here. </li>
  </ul>


  \todo Proof systems
  <ul>
   <li> We need a dedicated module for proof systems. </li>
   <li> See Satisfiability/ProofSystems/plans/general.hpp. </li>
  </ul>

*/

