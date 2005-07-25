// Oliver Kullmann, 24.7.2005 (Swansea)

/*!
  \file GrayCodes.hpp
  \brief Tools to enumerate combinations, that is all elements of
  A_1 x ... x A_n for finite lineary ordered sets A_i, in the order given by Gray codes.
  \todo Write an iterator (at least a forward iterator) that, after initialising
  the A_i in appropriate forms (as a sequence of ranges, or simpler by a
  sequence of natural numbers (the sizes of the A_i)), iterates through
  the code for example by means of pairs (i, j) of iterators, where i points
  to the sequence A_i, while j points to the element of A_i (or simple
  by pairs (i, j) of indices).
  \todo An efficiency problem occurs, when we for example want to enumerate
  all total assignments for a problem instance, and then doubling of the
  code representation occurs: For the application algorithm we likely need
  our own data structures for total assignments, while the iterator also needs
  an internal representation of the current code word. Best, if in such situations
  the iterator could use here the total assignment (instead of its own representation).
*/

#ifndef GRAYCODES_096tGcv

#define GRAYCODES_096tGcv

namespace OKlib {

  namespace Combinatorics {

  }

}

#endif
