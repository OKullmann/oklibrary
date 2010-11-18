// Oliver Kullmann, 29.6.2007 (Swansea)
/* Copyright 2007, 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/plans/SatisfactionProblems.hpp
  \brief Plans for satisfaction problems (in general)
  
  
  \todo %Variables
  <ul>
   <li> The set of all variables in some context is Var. </li>
   <li> However, it seems to be to use this concept only "implicitely". </li>
  </ul>

  
  \todo Value set
  <ul>
   <li> The set of all possible values for the variable in Var (in some
   context) is Val. </li>
   <li> Again, likely this is best only used implicitely. </li>
  </ul>


  \todo Total assignments
  <ul>
   <li> One total assignment: f: Var -> Val, just treated as a function which
   can be evaluated on all elements of Var (while otherwise behaviour is
   undefined) and yields and element of Val. </li>
   <li> The set of all total assignments is TASS (again, likely only to be
   used implicitely). </li>
  </ul>
   

  \todo Domain association and allowed total assignments
  <ul>
   <li> A domain association is a special kind of condition, given by a set
   of signed literals, which either specify the set of allowed or forbidden
   values for some variables. </li>
   <li> So a "domain association" is nothing than a partial assignment, and
   perhaps we  should drop this notion at all --- there is a set of variables
   and its associated value set (the set of possible values for any variable),
   and if one wishes to introduce restricted variable domains, then a partial
   multiassigment is to be considered ?! </li>
   <li> So a "problem" typically is given as a pair (P, phi), where P is a
   problem instance and phi is a partial multiassignment. One can speak of
   phi as the "domain association". </li>
  </ul>
  

  \todo The notion of "condition"
  <ul>
   <li> See
   ComputerAlgebra/Satisfiability/Lisp/ConstraintProblems/plans/Conditions.hpp
   for the realisation of these concepts. </li>
   <li> A condition is a map from TASS to {false,true}. </li>
   <li> A solution is a total assignment evaluating to true; perhaps better,
   a "satisfying assignment", since as interesting (in general) are the
   "falsifying assignments". </li>
   <li> So for a most general condition, all what can be done (at first) is
   to run through all total assignment; this is the "oracle model". </li>
   <li> The fundamental task (for the theory of generalised satisfiability(!))
   for a condition P (like "problem"):
   <center> Represent P^{-1}(false) and P^{-1}(true). </center>
   "Representation" of sets of total assignments can mean different things:
    <ol>
     <li> Measure the set, either with its natural probability in the product
     probability space TASS, or just classifying it as empty or non-empty.
     </li>
     <li> Give a power-clause-set representation, i.e., a signed
     CNF-representation of P^{-1}(false) resp. a signed DNF-representation of
     P^{-1}(true), where we have several refinements:
      <ol>
       <li> A prime power-clause-set, that is, a prime signed CNF resp. a
       prime signed DNF: no clause can be replaced by a smaller one (where
       for CNF-clauses C,D it is C smaller than D iff C semantically implies
       D). </li>
       <li> A hitting power-clause-set, that is, a hitting signed CNF resp. a
       hitting signed DNF: each pair of clauses clash, i.e., the sets of
       represented total assignments are disjoint. </li>
       <li> A special case of a hitting clause-set is a full clause-set, where
       each pair of clauses contains (exactly) the same variables. A full DNF
       representation is just how "constraint satisfaction problems" are
       commonly understood. </li>
      </ol>
      The power-clause-set can be given either explicitly listed, or "online",
      one clause after another.
     </li>
     <li> Give a BDD representation; this represents directly P^{-1}(false) and
     P^{-1}(true) together. </li>
    </ol>
   In each case, also partial information is important:
    <ol>
     <li> Regarding measurement, we can have
      <ol>
       <li> upper bounds </li>
       <li> lower bounds </li>
       <li> approximations </li>
      </ol>
     </li>
     <li> For power-clause-set representations, we can have sub-clause-sets.
     </li>
     <li> BDD representations are harder to make partial, since it represents
     satisfying and falsifying assignments at the same time. </li>
    </ol>
   </li>
   <li> Given a partial multi-assignment phi, the most fundamental operation is
   the application phi*P, which is the condition which is false for total
   assignments f not covered by phi, and P(f) otherwise.
    <ol>
     <li> So phi*P in general is just the pair (P,phi) explained above as
     "problem". The fact, that a variable v has been eliminated, since phi
     assigned a specific value to v (in general phi might only restrict v to
     a subset of its domain), can be recorded explicitly; forgetting this
     explicit recording, we arrive at the usual notion of the application of
     a partial assignment to a generalised clause-set. </li>
     <li> For clause-sets we can forget the variables which already have been
     assigned, since we can define a composition of partial assignments where
     the first assignments always renders a later assignment void; however in
     the present of multi-assignments this is not possible. </li>
     <li> This notion of phi*P is satisfiability centered, we also need phi*'P,
     which renders every total assignment not covered by phi true. </li>
    </ol>
   </li>
   <li> Now the above fundamental task is to be considered for phi * P (and
   phi *' P). </li>
   <li> Compare with
   ComputerAlgebra/Satisfiability/Lisp/plans/PartialAssignments.hpp. </li>
   <li> The "point of view of satisfiability" is the emphasise on partial
   assignments and clause-sets relative to a notion of "literal":
    <ol>
     <li> A "literal" is a special condition such that "x implies y" can always
     be decided "very quickly" for literals x, y, and such that all fundamental
     tasks can be performed "very quickly". </li>
     <li> A "clause" is a finite set of literals, either interpreted as
     CNF-clause ("or") or as DNF-clause ("and"). There are three normalising
     conditions on clauses:
      <ol>
       <li> No literal implies another literal. </li>
       <li> Stronger: Two literals cannot be combined into an equivalent
       literal. </li>
       <li> The clause does not represent a constant condition. </li>
      </ol>
     It must hold for normalised clauses, that a CNF-clause C implies a
     CNF-clause D iff for all x in C there is a literal y in D such that x
     implies y. This restricts the notion of literals. </li>
     <li> "Parallel" to clauses one has partial assignments, which semantically
     correspond to DNF-clauses for satisfiability testing, and to CNF-clauses
     for falsifiability testing, but which have a different meaning: Partial
     assignments steer the backtracking process. </li>
     <li> Clause-sets are set of clauses (with finitely many variables). </li>
     <li> The fundamental tasks are solved by returning clause-sets, where
     typically only the cumulative k-section (all clauses up to length k) is of
     interest: k = 0 is just the sat-problem, k = 1 yields all implied
     unit-clauses (or some of them), and so on. </li>
     <li> One problem is, how to call these generalised "literals", "clauses",
     "clause-sets":
      <ul>
       <li> "abstract" literals, ... ? </li>
       <li> "active" literals, ... ? </li>
      </ul>
      As examples for such "abstract literals" we have already
       <ol>
        <li> boolean literals </li>
        <li> (generalised) literals </li>
        <li> signed (or power-) literals. </li>
       </ol>
       See ComputerAlgebra/Satisfiability/Lisp/plans/Literals.hpp.
     </li>
     <li> For an "effective condition" we have one associated literal type, and
     "most" basic tasks can be solved "efficiently" (generalised clauses).
      <ol>
       <li> These would be the "active clauses". </li>
       <li> While the fundamental tasks can be performed in constant time
       for active literals, now we can do then in polynomial time. </li>
       <li> Having a OBDD representation should mean that we have
       an active clause. </li>
       <li> And perhaps also representations as hitting clause-sets
       imply "active clause". </li>
      </ol>
      This kind of discussion needs clean-up (should be moved to
      Satisfiability/ProblemInstances/concepts/plans).
     </li>
     <li> A "condition set" is a set of effective conditions, either a
     conjunction or a disjunction (generalising clause-sets). </li>
     <li> Finally we have "alliances of condition sets", which are combinations
     of condition sets, allowing for different literal types to be used. </li>
    </ol>
  </ul>


  \todo Constructing conditions
  <ul>
   <li> We have the propositional connectives. </li>
   <li> And the quantifiers "for all" and "exists". </li>
  </ul>


  \todo Functions for conditions
  <ul>
   <li> var(P) yields a set of variables, such that P does not depend on
   variables not in var(P). </li>
  </ul>


  \todo Backdoors
  <ul>
   <li> A set of variables V is a "backdoor" for condition P regarding one of
   the above fundamental tasks, if for every assignment of values to the
   variables in V the task can be "efficiently" solved. The knowledge about
   some such backdoors is important structural information, either given with
   the problem instance itself (a priori), or computed later (a posteriori).
   Given some set of variables, an interesting task is to find a smallest
   backdoor (from the ones we know) which includes this set.
    <ol>
     <li> Consider as an example the condition given by AES
     (see Cryptanalysis/plans/Rijndael.hpp): This is a condition on n+n+k
     boolean variables, where n is block length and k the key length. Given
     n+?+k or ?+n+k variables, the remaining variables are uniquely determined
     and can be efficiently computed by the encryption resp. decryption
     algorithm. </li>
     <li> So we have two backdoors for computing a full DNF representation.
     </li>
    </ol>
   </li>
  </ul>

*/

