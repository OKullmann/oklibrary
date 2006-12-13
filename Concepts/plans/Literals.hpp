// Oliver Kullmann, 24.7.2005 (Swansea)

/*!
  \file Concepts/plans/Literals.hpp
  \brief Plans for concepts for literals.

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

  Literal forms, which use more variables or are not variable-based at all
  need to use other notions like "GeneralLiteral".

  Out of literals we make partial assignments (DNF-clauses) and (CNF-)clauses.

  \todo Special cases of the above "power literals" (or "signed literals") are
  "monosigned", "boolean", etc.
*/

