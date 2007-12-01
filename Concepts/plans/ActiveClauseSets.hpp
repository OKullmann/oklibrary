// Oliver Kullmann, 3.1.2007 (Swansea)
/* Copyright 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Concepts/plans/ActiveClauseSets.hpp
  \brief Plans for the concept of an active clause-set


  \todo The following needs to be updated.
  <ul>
   <li> See supermodule Satisfiability/ProblemInstances. </li>
   <li> See supermodule ComputerAlgebra/Satisfiability. </li>
   <li> This file shall be partially moved to
   Satisfiability/ProblemInstances/ActiveClauseSets/concepts/plans
   and partially to
   Satisfiability/ProblemInstances/ClauseSets/concepts/plans.
   </li>
  </ul>


  \todo Dictionary:
  (The following is out-dated! "clause" -> "active clause" and
  "clause-set" -> "active clause-set")
  <ol>
   <li> "clause" generalised to "constraint" enhanced to "active clause-set"
   (semantically like a constraint, but can be viewed as a clause-set) </li>
   <li> "clause-set" generalised to "constraint satisfaction problem" enhanced
   to "alliance of active clause-sets". </li>
  </ol>
   Ordinary clause-sets however should now be treated as active clause-sets,
   and not as alliances of active clause-sets, since alliances of active
   clause-sets assume that there constituents are intelligent but not numerous.


  \todo Update with Concepts/plans/ClauseSets.hpp (transferring ideas from
  there to here or the other way around (?!)).


  \todo Partial assignment (see Concepts/plans/PartialAssignments.hpp)
  <ul>
   <li> An active clause-set (ACLS F in the sequel) has a connection to the
   main partial assignment:
   - it can process the new assignments
   - and it can undo them.
   </li>
   <li> The concept of partial assignments needs to support this.
   Actually, speaking of "partial assignments" might be a bit misleading,
   since literals for us are kind of arbitrary constraints ?! </li>
   <li> A problem here is, that we might have different types of literals
   (for example unary and binary literals), and because of static typing then
   we need actually a tuple of partial assignments, collected into the
   "super-assignment" phi. </li>
  </ul>


  \todo Evaluation
  <ul>
   <li> The current partial assignment phi can be evaluated (written phi(F)),
   returning a 4-valued object:
   - 0 if all total assignments extending phi falsify F
   - 1 if all total assignments extending phi satisify F
   - 0/1 if there are total assignments extending phi falsifing and
     satisfying F
   - unknown (can not be returned if all variables of F got a unique value).
   </li>
   <li> So we need to extend Boost::tribool by a "4-valued logic"; we need
   literature on that (see verification of circuits). </li>
  </ul>


  \todo Implied literals
  <ul>
   <li> The literal type of F is the same as for the associated partial
   assignment. </li>
   <li> Processing the new assignment creates a list of implied literals
   (or should this be a partial assignment?). </li>
   <li> If there are for example literals for equality, then this included
   implied equivalences. </li>
  </ul>


  \todo Support for local search
  <ul>
   <li> Given a total assignment for the variables, an alteration is produced
   which satisfies F or comes as close as possible to that.
   (Total assignment means that all variables are fixed; perhaps here we should
   consider only unary literals?) </li>
   <li> For autarky search, here it must also be possible to disable
   variables; this is easiest understand by referring to an underlying
   clause-model. </li>
  </ul>


  \todo Causes of conflict
  <ul>
   <li> For partial assignments, let "phi' <= phi" be true if
   var(phi') <= var(phi) and for all in var(phi') the literals phi'(v)
   weakens phi(v) (the set of allowed values of phi'(v) contains the set
   of allowed values of phi(v)). </li>
   <li> Now if phi(F) = 0, then phi' is computed with phi' <= phi and
   phi'(F) = 0. </li>
  </ul>


  \todo Autarkies:
  What is an appropriate notions of autarkies? Easiest is to refer to some
  underlying clause-model ?!


  \todo For a "strong ACLS" all above operations are poly-time in the number
  of variables, and are always complete (no "unknow" for evaluation,
  all implied literals and equivalences, the changed total assignment is
  satisfying). XXX This should be now "active clauses".


  \todo Should we have statistics for an underlying clause-model?
  XXX This seems to be the point of speaking of "active clause-sets".


  \todo CNF versus DNF
  <ul>
   <li> Regarding QBF for example, we should allow to represent conditions as
   CNFs and as DNFs (that is, as sets of (partial) falsifying assignments
   resp. (partial) satisfying assignments), potentially also allowing
   mixed representations. </li>
   <li> One could also regard partial representations,
   that is, the condition itself recognises the satisfying and the falsying
   total assignments, but it might have only some partial CNF- or DNF-clauses
   "in it" (those found until now). </li>
  </ul>
  
*/
