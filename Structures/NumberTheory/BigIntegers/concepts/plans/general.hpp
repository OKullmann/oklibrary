// Oliver Kullmann, 19.10.2005 (Swansea)

/*!
  \file BigIntegers/concepts/plans/general.hpp
  \brief Plans for concepts for big integer classes


  \todo Boost:
  <ul>
   <li> May 2005 there was a simple implementation by Christopher Diggins for
   a class big_uint. </li>
  </ul>


  \todo The basic concept for big integers should (just) include:
   - fully constructible
   - constructor from integral types
   - the assignment operations +=, *=, -=, /=, %=
   - the basic operation +, *, -, /, %
   - linear order
   - swap
   - absolute value
   - div and mod combined
   - partial specialisation of boost::numerical_cast
   - partial specialisation of boost::lexical_cast (from and to strings)
   - input and output streaming.


  \todo First client:
  Most important is what module RandomGenerator needs
  (see RandomGenerator/plans/general.hpp).


  \todo Natural numbers:
  One design issue is how to handle natural numbers (NN_0 and NN).
  Likely we need different concepts (the free ring vs. the free semiring, each
  of dimension 1), where the concepts allow for conversions. (NN is the
  free semigroup with one generator, while NN_0 is also the free monoid with
  one generator.)


  \todo Basic concepts are needed for the basic algebraic structures:
  <ul>
   <li> groupoids (one composition) (Concepts::FullyConstructibleEq minus default construction) </li>
   <li> groupoids with 0 or 1 (Concepts::FullyConstructibleEq) </li>
   <li> ring frames (two compositions) </li>
   <li> ring frames with 0, 1 </li>
   <li> rings (compositions +,-,*) </li>
   <li> skew fields (compositions +,-,*,/) </li>
  </ul>
  For the basic concepts no expression-templates, but only in-place-modification
  via * and + (the latter only for commutative operations). Extensions allow
  also the corresponding free-standing operations.

  Furthermore the following notions are needed:
  <ul>
   <li> compatible orders </li>
   <li> absolute values </li>
   <li> min and max </li>
  </ul>


  \todo What about generalisations/extensions of the integers?
  (Mostly number-theoretical.)

*/

