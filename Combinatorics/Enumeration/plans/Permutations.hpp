// Oliver Kullmann, 20.8.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Combinatorics/Enumeration/plans/Permutations.hpp
  \brief %Tools for enumeration of permutations.


  \todo First concepts for permutations are needed. The two natural possibilities are:
   - I A permutation as a sequence of objects.
   - II A permutation as a map.
   For the first option we have the advantage of using the standard concept of a range, for the second
   option we have the advantage of using customisable types. Let's consider the first option.

   Concept IndexPermutation, Option I:
     A random access range (in the Boost::range sense) of size n >= 0 over n different
     elements from {0, ..., n-1}.
   If p is an object of a model of IndexPermutation, then for 0 <= i < boost::size(p) the value p(i)
   as accessed as *(p + i), which is slightly inconvenient; but by using ranges we have the advantage
   that all methods for ranges apply to IndexPermutations.

   This is likely the most basic and perhaps also the most useful concept here. Arbitrary permutations
   of different concept can be reduced to IndexPermutations by using indices.

   The elements of S_n are now treated as objects for some model of IndexPermutation for a fixed n.

   The three basic operations with index-permutations are construction of the identity, composition and
   inversion. For the identity we write

   IndexPermutation(n) (with *(IndexPermutations + i) = i)

   for composition of index-permutations f and g of the same size we write

   compose(g, f) (with *(compose(g, f) + i) = *(g + *(f + i)))

   and for inversion of index-permutation p we write

   inverse(p) (with compose(p, inverse(p)) == IndexPermutation(n) and
     compose(inverse(p), p) == IndexPermutation(n)).

   As in Boost::uBLAS, here expression templates should be used, enabling lazy evaluation
   as well as creation of temporary (for eager evaluation). An open issue whether we should
   use unqualified operation names (as above, using ADL) or qualified operation names.
   Now, since ranges could be used for everything, it seems better to use OKlib::Matrices::compose
   and OKlib::Matrices::inverse. Now it seems that in-place modification of ranges is difficult,
   and thus perhaps we should consider the second option.

   Concept IndexPermutation, Option II:
   Refines concept FullyConstructibleLo; default construction depends on whether IndexPermutation
   is refined to have a fixed size n (in which we case get the identity with n elements) or not (in which case we get
   the empty permutation). The refinement is called IndexPermutationFixed, and supports the additional
     operation IndexPermutationFixed::size.
     The relation < is lexicographical order.
     For an object p of type IP we have the operations
     size_type<IP>::type
     size(p) of type size_type
     p(i) for 0 <= i < size(p) (so IndexPermutation should be a refinement of a function-concept)
     IP(r) for a range r (interpreted as a cycle; if too long for fixed size, then cut off)
     range(p) (yields a range of size size(p))
     p.invert() (in-place modification)
     p.left_multiply(q), p.right_multiply(q) (in-place modifications)
     p.resize(m) (in-place modification; extending with the identity or truncating if necessary
       (can yield undefined behaviour))
   A refined concept IndexPermutationExp includes also the expressions
     inverse(p)
     g * f.

   Again expression templates should be used (see above), and now using ADL seems the right choice.

   This seems better to me. Default implementations of IndexPermutation:
    - as a vector resp.~array
    - as two vectors (including the inverse) resp.~two arrays.

   Special constructions:
    - transpositions.


  \todo Now we know what a permutation is, we need a standard way of enumerating permutations.
  The best possibility seems just to use ranges of iterators, where the value type is a model
  of IndexPermutation or IndexPermutationFixed. The concept of a PermutationGenerator
  then is given by the expressions

  ::OKlib::Combinatorics::traits::range_type<PermutationGenerator>::type
  PermutationGenerator(n) creates a range of iterators of permutation_type, running through
  all elements of S_n in some order.

  Refinements are LexicographicalPermutationGenerator and GrayPermutationGenerator.


  \todo One should study the permutation generator from the C++ standard library
  (it is unclear whether this thing handles equivalent objects or not; I assume it does).
  Likely this generator must be relatively inefficient, since it must always compute
  the supporting structure from scratch; thus it's a kind of "non-intrusive" generator,
  while a more efficient generator would be "intrusive".


  \todo Implement lexicographic enumeration.


  \todo Implement enumeration with only one neighbour-swap to get to the next permutation.
  This is the equivalent to Gray-codes (see GrayCodes.hpp), and similar considerations apply).

*/

