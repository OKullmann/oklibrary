// Oliver Kullmann, 24.7.2005 (Swansea)

/*!
  \file Concepts/plans/Literals.hpp
  \brief Plans for concepts for literals.

  \todo Arity:
  Now actually it seems best to have literals with arity n. This is accomplished by
  using the generalised AtomicConditions-concept (see Concepts/plans/AtomicConditions.hpp),
  and by generalising traits::var_type so that is actually returns a tuple type (of
  the same arity as the value-tuple-type in traits::cond_type). For arity 1 there
  should be also short-cuts, so that the current syntax works (can this be accomplished?
  there is no conversion from an unary tuple to its value-type?)

  \todo Requirements literals

  It seems best that only refined versions of Literals offer operation
  static_cast<cond_type&>(OKlib::Literals::cond(l));
  while for general literals we have only
  set_cond(x, cond).

  A general question is whether literals are assumed to be easily copyable, or should we pass
  them by reference? For literals like integers, we should pass them by value, but for literals
  representing sets we should pass them by reference?! So do we need to employ a traits function,
  which decides whether to pass by value or const reference? Seems to be the case, but would make
  writing functions using literals a bit more complicated. First we pass them by const reference,
  and then later we'll see.

  Out of literals we make partial assignments (DNF-clauses) and (CNF-)clauses.

  \todo Special cases of the above "power literals" (or "signed literals") are
  "monosigned", "boolean", etc.
*/

