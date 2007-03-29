// Oliver Kullmann, 6.1.2007 (Swansea)

/*!
  \file Concepts/plans/Iterators.hpp
  \brief Plans on concepts for iterators

  \todo First the Boost-iterator-concepts need to be studied,
  and related to our efforts.  (Are they now part of the
  new standard? hopefully there are concept definitions somewhere.)

  \todo Write concepts for
  <ol>
   <li> InputIteratorDefault:
    - refines Concepts::InputIterator and Concepts::ConstructibleDEq
    - <code> i == Iterator() </code> is in the domain of ==
    - <code> Iterator() </code> is the past-the-end iterator
   </li>
  </ol>
*/

