// Oliver Kullmann, 3.1.2007 (Swansea)

/*!
  \file Concepts/plans/ActiveClauseSets.hpp
  \brief Plans for the concept of an active clause-set

  \todo Dictionary:
  <ol>
   <li> "clause" generalised to "constraint" enhanced to "active clause-set" (semantically
        like a constraint, but can be viewed as a clause-set) </li>
   <li> "clause-set" generalised to "constraint satisfaction problem" enhanced to
        "alliance of active clause-sets". </li>
  </ol>
   Ordinary clause-sets however should now be treated as active clause-sets, and not
   as alliances of active clause-sets, since alliances of active clause-sets assume
   that there constituents are intelligent but not numerous.

  \todo Update with Concepts/plans/ClauseSets.hpp (transferring ideas from
  there to here or the other way around (?!)).

  \todo Partial assignment (see Concepts/plans/PartialAssignments.hpp):
  An active clause-set (ACLS F in the sequel) has a connection to the
  main partial assignment:
   - it can process the new assignments
   - and it can undo them.

  The concept of partial assignments needs to support this. Actually, speaking
  of "partial assignments" might be a bit misleading, since literals for us
  are kind of arbitrary constraints ?!

  A problem here is, that we might have different types of literals (for example
  unary and binary literals), and because of static typing then we need
  actually a tuple of partial assignments, collected into the "super-assignment" phi.

  \todo Evaluation:
  The current partial assignment phi can be evaluated (written phi(F)),
  returning a 4-valued object:
   - 0 if all total assignments extending phi falsify F
   - 1 if all total assignments extending phi satisify F
   - 0/1 if there are total assignments extending phi falsifing and
     satisfying F
   - unknown (can not be returned if all variables of F got a unique value).

  So we need to extend Boost::tribool by a "4-valued logic"; we need literature
  on that (see verification of circuits).

  \todo Implied literals:
  The literal type of F is the same as for the associated partial assignment.
  Processing the new assignment creates a list of implied literals
  (or should this be a partial assignment?).

  If there are for example literals for equality, then this included
  implied equivalences.

  \todo Support for local search:
  Given a total assignment for the variables, an alteration is produced which
  satisfies F or comes as close as possible to that.
  (Total assignment means that all variables are fixed; perhaps here we should
  consider only unary literals?)

  For autarky search, here it must also be possible to disable variables; this is
  easiest understand by referring to an underlying clause-model.

  \todo Causes of conflict:
  For partial assignments, let "phi' <= phi" be true if var(phi') <= var(phi)
  and for all in var(phi') the literals phi'(v) weakens phi(v) (the set of
  allowed values of phi'(v) contains the set of allowed values of phi(v)).

  Now if phi(F) = 0, then phi' is computed with phi' <= phi and phi'(F) = 0.

  \todo Autarkies:
  What is an appropriate notions of autarkies? Easiest is to refer to some
  underlying clause-model ?!

  \todo For a "strong ACLS" all above operations are poly-time in the number
  of variables, and are always complete (no "unknow" for evaluation, all implied
  literals and equivalences, the changed total assignment is satisfying).

  \todo Should we have statistics for an underlying clause-model?

  \todo CNF versus DNF:
  Regarding QBF for example, we should allow to represent conditions as CNFs and as DNFs
  (that is, as sets of (partial) falsifying assignments resp. (partial) satisfying assignments),
  potentially also allowing mixed representations. One could also regard partial representations,
  that is, the condition itself recognises the satisfying and the falsying total assignments, but
  it might have only some partial CNF- or DNF-clauses "in it" (those found until now).
  
*/
