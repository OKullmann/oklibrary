// Oliver Kullmann, 24.7.2005 (Swansea)

/*!
  \file Concepts/plans/Literals.hpp
  \brief Plans for concepts for literals.

  \todo Signed integers should model boolean literals.
  Thus the concept of literals does not have member functions, but
  free standing functions are used. The question is, whether qualified calls
  ore ADT are used? Always writing OKlib::Literals::var(x) seems to uggly
  to me, and for the build-in signed integral types there is only one natural
  way to treat them as literals, and thus we should use unqualified calls.
  
  \todo Requirements atomic conditions

   Basic the concept of an "atomic condition", which just represents
   a set of values for which the condition evaluates to true, and the literal is then
   a variable together with such an atomic condition. The point here is the
   structure (topology) of the value space. More precisely, here we do not
   use infinitary operations, but complementation is always available,
   and thus here we have a boolean algebra. The concept-structure is just that of a
   set-ring, with union, intersection, difference,
   complement, and constructors for singletons and co-singletons.
   It might be useful to have also lazy evaluation here.
   We require that the atomic condition of a literal is never constant.
   A boolean value is an atomic condition for the value_type bool.

   An atomic condition type C is like a container, and should have at least
   OKlib::Literals::traits::value_type<C>::type.
   It seems that in general we do not need the ability to run through all possible
   values, but only to evaluate a condition on a value (returning bool).
   So perhaps operations:
   - OKlib::Literals::traits::value_type<C>::type
   - is_always_true(x) : bool
   - is_always_false(x) : bool
   - eval(x, value) : bool
   - C(value).
   Should we have a special module for atomic conditions (as "sets" or "predicates"), or are they
   part of the literals-module?

  \todo Requirements literals

   A literal has
   - a variable type OKlib::Literals::traits::variables_type<Lit>::type
   - an underlying variable var(x)
   - an atomic condition type OKlib::Literals::traits::condition_type<Lit>::type
   - an underlying condition cond(x).
   Further operations:
   - literals can be complemented : cmpl(x)
   - given a condition, the literal evaluates to boost::tribool : eval(x, cond)
   - restrict(x, cond) restricts the condition in x in-place to the intersection of cond(x) and cond,
     and returns a value convertible to a boost::tribool, which is true or false if the operation would
     have resulted in a constant literal (thus restrict(x, cond) == eval(x, cond);
     for literals without in-place modification the parameter x is actually const, and the
     result cannot be indeterminate.

   A general question is whether literals are assumed to be easily copyable, or should we pass
   them by value? For literals like integers, we should pass them by value, but for literlals
   representing sets we should pass them by reference?! So do we need to employ a traits function,
   which decides whether to pass by value or const reference? Seems to be the case, but would make
   writing functions using literals a bit more complicated.

   For "big literals" one needs to use in-place modification when restricting, which is undone
   when backtracking, while for boolean literals there are no in-place modifications. Can we find
   a common syntax? The solution should be, that such operations only take place inside an active
   clause-set, and the active clause-set should know how to handle its literals.

   Another problem is, that a literal doesn't know the current domain of its variable, and so
   for every evaluation it must consider the question from scratch, while in most case only
   certain values from the domain have been removed?

   Literal forms, which use more variables or are not variable-based at all
   need to use other notions like "GeneralLiteral".

   Out of literals we make partial assignments (DNF-clauses) and (CNF-)clauses.

  \todo Special cases of the above "power literals" (or "signed literals") are
  "monosigned", "boolean", etc.
*/

