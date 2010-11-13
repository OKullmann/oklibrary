// Oliver Kullmann, 5.1.2009 (Swansea)
/* Copyright 2009, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Combinatorics/Lisp/Enumeration/plans/general.hpp
  \brief General plans regarding enumerating basic combinatorial objects (in
  the algorithmic sense, as opposed to just counting)

  For pure counting see
  ComputerAlgebra/Combinatorics/Lisp/Counting/plans/general.hpp.


  \todo Iteration
  <ul>
   <li> Given a parameter p (which in general is a tuple) and a set M(p)
   of "combinatorial objects", the task is to "enumerate" M(p). </li>
   <li> Concepts for counting and ranking/unranking one finds in
   ComputerAlgebra/Combinatorics/Lisp/Enumeration/Basics.mac. </li>
   <li> Computing the first element of L(p), and given an object x, computing
   its successor in L(p) or deciding that x is the last element of L(p).
    <ol>
     <li> This could be called "state-free iteration" through M(p). </li>
     <li> If x is the last element, then "done" is returned (otherwise
     the next element). </li>
     <li> Naming "first_lex_ksubsets", "first_colex_ksubsets",
     "first_lex_permutations" etc., and "next_lex_ksubsets",
     "next_colex_ksubsets" and "next_lex_permutations" etc. </li>
    </ol>
   </li>
   <li> Finally we have the task of "(general) iteration" through M(p).
    <ol>
     <li> The following needs updating according to the general concepts
     in ComputerAlgebra/AbstractDataTypes/Lisp/plans/general.hpp. </li>
     <li> And see "Iteration through lexicographical order" in
     ComputerAlgebra/Combinatorics/Lisp/Enumeration/plans/Subsets.hpp for
     a specific example. </li>
     <li> Iteration is given by a function-object I(p) which can be
     initialised, queried whether the past-the-end has been reached, and if
     this is not the case, can return the current object and can be advanced
     to the next element (possibly "past-the-end"). </li>
     <li> This is like forward-iterators in C++, only that we are running
     through a fixed collection (not an arbitrary one). Likely we don't
     need equality-comparison, but we should provide a method of ranking
     the current element (faster than by calling the ranking-function for
     the current element). </li>
     <li> The point of such an iterator would be that it is more efficient
     than the state-free iteration. </li>
     <li> Perhaps such an iterator could be just some object which contains
     all the necessary state-information (like the current index i, the
     current element x, and the underlying information on x such that
     the successor x' can be quickly determined. </li>
     <li> The iterator-methods would then just inspect this object, or
     compute a new object from an old one. </li>
     <li> However, according to the "concrete" character of the Maxima/Lisp
     level we should avoid hiding information. </li>
     <li> So perhaps the general concept of an "iterator" is that of a triple
     I(p) = [true/false,i,state], where the first (boolean) entry specifies
     whether we are past-the-end; if not, then the natural number i is the
     current rank, and "state" finally contains all the information to make
     it easy to compute M(p)[i] and to go to the successor i+1. </li>
     <li> Or perhaps the first (indicator) entry is "done/true", and
     the while-condition then is "while it[1] # done" ? </li>
     <li> Or perhaps we have a pair [i,state], where i is either "done"
     or the rank? This would also fit better with the state-free approach.
     </li>
    </ol>
   </li>
  </ul>


  \todo Connections
  <ul>
   <li> Compare Combinatorics/Enumeration/plans/general.hpp and the sub-modules
   Combinatorics/Enumeration/plans/GrayCodes.hpp and
   Combinatorics/Enumeration/plans/Permutations.hpp. </li>
  </ul>


  \todo Basic overview
  <ul>
   <li> According to [Stanton, White; Constructive %Combinatorics], the
   basic sub-modules would be:
    <ol>
     <li> Permutations; perhaps with submodule Involutions </li>
     <li> Subsets </li>
     <li> Set partitions </li>
     <li> Integer partitions </li>
     <li> Product spaces </li>
     <li> Trees (not as data structures; in
     ComputerAlgebra/Graphs/Lisp/Trees/plans/Generators.hpp we do not
     handle the proper enumerative aspects) </li>
     <li> Tableaux </li>
    </ol>
   </li>
   <li> The appendix of [Stanton, White; Constructive %Combinatorics] contains
   enumeration algorithms to start with. </li>
   <li> Another source is [Knuth, Volume 4, Fascicle 3]. </li>
   <li> Apparently not handled by the above sources are for given n,k,s
   the multisubsets of {1,...,n} with k elements, where every element has
   multiplicity at most s.
    <ol>
     <li> Generalising the recursive formulas for binomial coefficients we
     have
     \verbatim
count_multisubsets_1[n,k,s] := if n=0 then (if k=0 then 1 else 0) else 
  sum(count_multisubsets_1[n-1,k-i,s],i,0,min(k,s));
     \endverbatim
     </li>
     <li> This recurses in n; alternatively we can recurse in s:
     \verbatim
count_multisubsets_1[n,k,s] := if s=0 then (if k=0 then 1 else 0) else 
  sum(binomial(n,i)*count_multisubsets_2[n-i,k-i*s,s-1],i,0,min(n,floor(k/s)));
     \endverbatim
     </li>
     <li> The latter seems slower? </li>
    </ol>
   </li>
  </ul>

*/

