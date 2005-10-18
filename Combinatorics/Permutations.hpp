// Oliver Kullmann, 20.8.2005 (Swansea)

/*!
  \file Permutations.hpp
  \brief Tools for enumeration of permutations.
  \todo First concepts for permutations are needed. The two natural possibilities are:
   - A permutation as a sequence of objects.
   - A permutation as a map.
  \todo A special case (of importance) is the handling of S_n. Having this, using indices
  we can solve the general problem of enumeration by using indices, at least when
  all objects are different.
  \todo One should study the permutation generator from the C++ standard library
  (it is unclear whether this thing handles equivalent objects or not; I assume it does).
  Likely this generator must be relatively inefficient, since it must always compute
  the supporting structure from scratch; thus it's a kind of "non-intrusive" generator,
  while a more efficient generator would be "intrusive".
  \todo Implement lexicographic enumeration.
  \todo Implement enumeration with only one neighbour-swap to get to the next permutation.
  This is the equivalent to Gray-codes (see GrayCodes.hpp, and similar considerations apply).
*/

#ifndef PERMUTATIONS_077yuJ

#define PERMUTATIONS_077yuJ

namespace OKlib {

  namespace Combinatorics {

  }

}

#endif
