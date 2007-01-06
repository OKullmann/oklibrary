// Oliver Kullmann, 5.12.2006 (Swansea)

/*!
  \file Concepts/plans/AtomicConditions.hpp
  \brief Plans for concepts for atomic conditions

  \todo Arity:
  It seems necessary that atomic conditions can have several arguments (where
  the arity is a compile-time constant). Then an atomic conditions has an
  associated std::tr1::tuple-type (which provides the value-types and the
  arity). Perhaps arity 0 should be avoided? And for arity 1 we should
  have specialisations, so that the current syntax works (if this is
  possible?).

  An example for a binary atomic condition is equality (and inequality).

  By using tuples we prevent active clause-sets from becoming literals.
  (But one can use a literal with one "super-variable", which stands for
  the many variables?!) Perhaps this is necessary for "SAT", where literals
  should be kind of "small units".

  \todo Refinement for iteration:

  A concept ConstantSequence is needed for the value-type, allowing to enumerate all values. The compiler
  should be able to perform loop-unrolling. Three possibilities:
   <ol>
    <li> An attempt to avoid past-the-end values:
      - value_type::first
      - value_type::last
      - value_type::next(v)
      - value_type::prior(v)

    (Instead of "value_type::" we must use traits-classes here.) Then value_type should also be linearly ordered.
    However how then to handle loops? So this seems not practical.
    </li>
    <li> With an additional iterator it would be easier. But using iterators it seems unlikely that the compiler
    can unroll loops, or? Conceptually easiest would be to model ConstantSequence as a ConstantRange (of input
    iterators); via traits functions one could use integers as iterators for booleans (and then actually
    loop-unrolling should be possible).
    </li>
    <li> The other possibility would be to use indices (or "positions"). Loops would work like
    for (value_type::size_type i = 0; i < value_type::size; ++i) {
      const value_type v(i);
    }
    Likely value_types are often integral, and then this is quite natural.
    </li>
   </ol>
   The ConstantRange seems easiest (and most appropriate for C++). Then just the range concepts
   can be used; on the negative side we do not have compile-time constants.

  \todo Precise formulation of ConstantSequence:

  X is a model of ConstantSequence if
   - AtomicConditions::traits::range_type<X>::type is a model of RandomAccessRange (see Boost range concepts),
     where the iterator type models InputIterator.
   - AtomicConditions::range<X>() yields a range of type range_type<X>::type.

  The value type of an atomic conditions models ConstantSequence.
   - Is is worth here to use a more fine-grained concept hierarchy regarding the traversal-category?
   - One could require only a ForwardRange ?! This should be sufficient.
   
  \todo Once implemented, use Concepts::FullyConstructibleLo (two times) to express the concepts.

  \todo Requirements atomic conditions

   It might be useful to have also lazy evaluation here. Operations should (first) be in-place operations.

   Active clause-set might offer, for every possible value, access to the literals containing it,
   and then it's up to the active clause-set to remove resp. re-add values;
   but for larger value-domains also literals (i.e., their atomic conditions) offer to
   change and undo the condition themselves.

   \todo Empty and full:
   Do we need operations empty(ac) and full(ac) for atomic conditions ac? Can be expressed via ac.size(), as well
   as singleton(ac) and co_singleton(ac). But perhaps it's more expressive using these functions, and so we might
   use these 4 functions already at the base level.
   Question here: When using function call syntax f(x), then it seems the return value is never treated as a
   (compile-time) constant. But for the boolean atomic condition, it would be good to have the four above
   functions returning constants (false for empty and full, true for singleton and co_singleton)? Perhaps it suffices
   that the compiler will do optimisation (when defining these functions as inline). (One could use macros, but
   this seems ugly, and then one couldn't use namespaces.)

   \todo Refinement for size:
   A refined version of AtomicConditions has
   - ac.size()
   as well as
   - value_type::size (see below --- perhaps better this is not part of the concept AtomicConditions, but the
   concept AtomicConditions requires the value type to be a model of ConstantSequence).

    
   \todo Changing atomic conditions:
   Removal and addition of single values seems to be basic (but not for bool).

   \todo Evaluation:
   Regarding evaluation of literals, for two conditions x, y the evaluation of x relativ
   to y is interesting, regarding y as specifying the domain: If y <= x, then return
   true, if y intersect x is empty, then return false, otherwise return indeterminate:
   - eval(x, y) : boost::tribool.

   \todo Write semantic tests.

*/



