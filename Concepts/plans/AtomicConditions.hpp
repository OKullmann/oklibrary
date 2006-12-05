// Oliver Kullmann, 5.12.2006 (Swansea)

/*!
  \file Concepts/plans/AtomicConditions.hpp
  \brief Plans for concepts for atomic conditions

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
   A boolean value is an atomic condition for the value_type bool.

   An atomic condition type C is like a container, and should have at least
   OKlib::AtomicConditions::traits::value_type<C>::type.
   It seems that in general we do not need the ability to run through all possible
   values, but only to evaluate a condition on a value (returning bool).
   So perhaps operations:
   - OKlib::AtomicConditions::traits::value_type<C>::type
   - is_always_true(x) : bool
   - is_always_false(x) : bool
   - eval(x, value) : bool.

   Regarding evaluation of literals for two conditions x, y the evaluation of x relativ
   to y is interesting, regarding y as specifying the domain: If y <= x, then return
   true, if y intersect x is empty, then return false, otherwise return indeterminate:
   - eval(x, y) : boost::tribool.

   Operations should (first) be in-place operations. We need complementation and
   addition/elimination of values.

   For construction perhaps
   - C(value, bool=true)
   where the boolean states whether the value represents a singleton or a co-singleton.

   Active clause-set might offer for every possible value access to the literals containing it,
   and then it's up to the active clause-set to remove resp. re-add values,
   but for larger value-domains also literals (i.e., their atomic conditions) offer to
   change and undo the condition themselves.

*/


