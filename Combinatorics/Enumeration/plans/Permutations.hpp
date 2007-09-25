// Oliver Kullmann, 20.8.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Combinatorics/Enumeration/plans/Permutations.hpp
  \brief %Tools for enumeration of permutations.


  \todo First concepts for permutations are needed.
  <ul>
   <li> The two natural possibilities are:
    <ul>
     <li> I A permutation as a sequence of objects. </li>
     <li> II A permutation as a map. </li>
    </ul>
    For the first option we have the advantage of using the standard concept of a range,
    for the second option we have the advantage of using customisable types.
   </li>
   <li> Concept IndexPermutation, Option I:
    <ol>
     <li> A random access range (in the Boost::range sense) of size n >= 0 over n
     different elements from {0, ..., n-1}. </li>
     <li> If p is an object of a model of IndexPermutation, then for
     0 <= i < boost::size(p) the value p(i) is accessed as *(p + i),
     which is slightly inconvenient; but by using ranges we have the advantage
     that all methods for ranges apply to IndexPermutations. </li>
     <li> This is likely the most basic and perhaps also the most useful concept here. </li>
     <li> Arbitrary permutations of objects can be reduced to IndexPermutations
     by using indices. </li>
     <li> The elements of S_n are now treated as objects for some model of
     IndexPermutation for a fixed n. </li>
     <li> The three basic operations with index-permutations are construction of the
     identity, composition and inversion. </li>
     <li> For the identity we write
     \code
IndexPermutation(n) 
assert(*(IndexPermutations + i) == i)
     \endcode
     </li>
     <li> For composition of index-permutations f and g of the same size we write
     \code
compose(g, f) 
assert(*(compose(g, f) + i) == *(g + *(f + i)))
     \endcode
     </li>
     <li> And for inversion of index-permutation p we write
     \code
inverse(p)
assert(compose(p, inverse(p)) == IndexPermutation(n))
assert(compose(inverse(p), p) == IndexPermutation(n))
     \endcode
     </li>
     <li> As in Boost::uBLAS, here expression templates should be used,
     enabling lazy evaluation as well as creation of temporary (for eager evaluation). </li>
     <li> An open issue whether we should use unqualified operation names (as above,
     using ADL) or qualified operation names. </li>
     <li> Now, since ranges could be used for everything, it seems better to use
     OKlib::Matrices::compose and OKlib::Matrices::inverse. </li>
     <li> Now it seems that in-place modification of ranges is difficult,
     and thus perhaps we should consider the second option. </li>
    </ol>
   </li>
   <li> Concept IndexPermutation, Option II:
    <ol>
     <li> Refines concept FullyConstructibleLo; default construction depends on
     whether IndexPermutation is refined to have a fixed size n (in which we case get
     the identity with n elements) or not (in which case we get the empty permutation).
     </li>
     <li> The refinement is called IndexPermutationFixed, and supports the additional
     operation IndexPermutationFixed::size. </li>
     <li> The relation < is lexicographical order. </li>
     <li> For an object p of type IP we have the operations
      <ul>
       <li> size_type<IP>::type </li>
       <li> size(p) of type size_type </li>
       <li> p(i) for 0 <= i < size(p) (so IndexPermutation should be a refinement of a
       function-concept) </li>
       <li> IP(r) for a range r (interpreted as a cycle; if too long for fixed size,
       then cut off) </li>
       <li> range(p) (yields a range of size size(p)) </li>
       <li> p.invert() (in-place modification) </li>
       <li> p.left_multiply(q), p.right_multiply(q) (in-place modifications) </li>
       <li> p.resize(m) (in-place modification; extending with the identity or
       truncating if necessary (can yield undefined behaviour)) </li>
      </ul>
     </li>
     <li> A refined concept IndexPermutationExp includes also the expressions
      <ol>
       <li> inverse(p) </li>
       <li> g * f. </li>
      </ol>
     </li>
     <li> Again expression templates should be used (see above), and now using ADL
     seems the right choice. </li>
     <li> This seems better to me. </li>
     <li> Default implementations of IndexPermutation:
      <ol>
       <li> as a vector resp.~array </li>
       <li> as two vectors (including the inverse) resp.~two arrays. </li>
      </ol>
     </li>
     <li> Special constructions:
      <ol>
       <li> transpositions. </li>
      </ol>
     </li>
    </ol>
   </li>
  </ul>


  \todo Now we know what a permutation is, we need a standard way of enumerating permutations.
  <ul>
   <li> The best possibility seems just to use ranges of iterators, where the value
   type is a model of IndexPermutation or IndexPermutationFixed. </li>
   <li> The concept of a PermutationGenerator then is given by the expressions
   \code
::OKlib::Combinatorics::traits::range_type<PermutationGenerator>::type
   \endcode
  PermutationGenerator(n) creates a range of iterators of permutation_type,
  running through all elements of S_n in some order. </li>
  <li> Refinements are LexicographicalPermutationGenerator and GrayPermutationGenerator. </li>


  \todo One should study the permutation generator from the C++ standard library
  (it is unclear whether this thing handles equivalent objects or not; I assume it does).
  Likely this generator must be relatively inefficient, since it must always compute
  the supporting structure from scratch; thus it's a kind of "non-intrusive" generator,
  while a more efficient generator would be "intrusive".


  \todo Implement lexicographic enumeration
  <ul>
   <li> Simple implementations of LexicographicalPermutationGenerator
   based on the permutation generator from the standard library,
   and yielding fixed and arbitrary permutations, with and without
   fast inversion. </li>
  </ul>


  \todo Implement enumeration with only one neighbour-swap to get to the next permutation.
  This is the equivalent to Gray-codes (see GrayCodes.hpp), and similar considerations apply).

*/

