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

   Operations should (first) be in-place operations. We need complementation and
   addition/elimination of values.

   Active clause-set might offer for every possible value access to the literals containing it,
   and then it's up to the active clause-set to remove resp. re-add values,
   but for larger value-domains also literals (i.e., their atomic conditions) offer to
   change and undo the condition themselves.

   \todo Empty and full:
   Do we need operations empty(ac) and full(ac) for atomic conditions ac? Can be expressed via ac.size(), as well
   as singleton(ac) and co_singleton(ac). But perhaps it's more expressive using these functions, and so we might
   use them already at the base level.
   Question here: When using function call syntax f(x), then it seems the return value is never treated as a
   (compile-time) constant. But for the boolean atomic condition, it would be good to have the four above
   functions returning constants (false for empty and full, true for singleton and co_singleton)? Perhaps it suffices
   that the compiler will do optimisation (when defining these functions as inline). (One could use macros, but
   this seems ugly, and then one couldn't use namespaces.)

   \todo Refinement for size:
   A refined version of AtomicConditions has
   - ac.size()
   as well as
   - value_type::size (see below --- perhaps this is not part of the concept AtomicConditions, but the concept
   AtomicConditions requires the value type to be a model of ConstantSet).

   \todo Refinement for iteration:
   Furthermore the value type can be enumerated, which should be formulated in such a way that the compiler
   might perform loop-unrolling. Natural would be:
   - value_type::first
   - value_type::last
   - value_type::next(v)
   - value_type::prior(v)
   (Instead of "value_type::" we must use traits-classes here.) Then value_type should also be linearly ordered.
   And it needs a past-the-end marker --- but this is not given for booleans ? With an additional iterator it would
   be easier. But using iterators it seems unlikely that the compiler can unroll loops, or?

   Perhaps this is not part of the concept of an atomic condition, but we have a concept "ConstantSet", with
   value_type::size and the above functions. Conceptually easiest would be to model ConstantSet as a
   constant range. The other possibility would be to use indices (or "positions").

   \todo Changing atomic conditions:
   Removal and addition of single values seems to be basic (but not for bool).

   \todo Evaluation:
   Regarding evaluation of literals, for two conditions x, y the evaluation of x relativ
   to y is interesting, regarding y as specifying the domain: If y <= x, then return
   true, if y intersect x is empty, then return false, otherwise return indeterminate:
   - eval(x, y) : boost::tribool.

*/



