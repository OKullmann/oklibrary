// Oliver Kullmann, 5.1.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Combinatorics/Lisp/Enumeration/plans/general.hpp
  \brief General plans regarding enumerating combinatorial objects


  \todo Main concepts
  <ul>
   <li> Given a parameter p (which in general is a tuple) and a set M(p)
   of "combinatorial objects", the task is to "enumerate" M(p). </li>
   <li> "Enumeration" means several things:
    <ol>
     <li> Computing (all of) M(p) as a (repetition-free) list L(p).
      <ol>
       <li> This could be called the task of "completely listing" M(p). </li>
       <li> The general naming should follow e.g. "lex_subsets_l",
       "colex_subsets_l", "lex_permutations_l" etc. </li>
      </ol>
     </li>
     <li> Computing for an element x in M(p) its index in L(p), and given
     an index i in {1, ..., |M(p)|}, computing the element L(p)[i],
      <ol>
       <li> This is the task of "ranking" and "unranking". </li>
       <li> Possibly the index range is not a standard interval {1, ..., n},
       but some other interval of integers, but in any case the total number
       n of elements needs to be computed. However, using other start indices
       than 1 perhaps just introduces unnecessary complications, and we better
       standardise the index range. </li>
       <li> Naming e.g. "rank_lex_subsets", "rank_colex_subsets",
       "rank_lex_permutations" etc. </li>
      </ol>
     </li>
     <li> Computing the first element of L(p), and given an object x, computing
     its successor in L(p) or deciding that x is the last element of L(p).
      <ol>
       <li> This could be called "state-free iteration" through M(p). </li>
       <li> If x is the last element, then "done" is returned (otherwise
       the next element). </li>
       <li> Naming "first_lex_subsets", "first_colex_subsets",
       "first_lex_permutations" etc., and "next_lex_subsets",
       "next_colex_subsets" and "next_lex_permutations" etc. </li>
      </ol>
     </li>
     <li> Finally we have the task of "(general) iteration" through M(p).
      <ol>
       <li> This is given by a function-object I(p) which can be initialised,
       queried whether the past-the-end has been reached, and if this is not
       the case, can return the current object and can be advanced to the
       next element (possibly "past-the-end"). </li>
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
       level we should avoid hiding information, and so perhaps the general
       concept of an "iterator" is that of a triple [true/false,i,state],
       where the first boolean entry specifies whether we are past-the-end;
       if not, then the natural number i is the current rank, and "state"
       finally contains all the information to make it easy to easily compute
       M(p)[i] and to go to the successor i+1. </li>
      </ol>
     </li>
    </ol>
   </li>
   <li> DONE (see above)
   For these basic functions we need to establish naming conventions.
   </li>
   <li> In module ComputerAlgebra/Combinatorics we only consider "classical"
   combinatorial objects. An attempt to define this could use the following
   properties:
    <ol>
     <li> The size of M(p) must be relatively easy to compute. </li>
     <li> Enumeration is not a "hard" task. </li>
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
     <li> Trees (not as data structures; and in %Graphs we do not handle
     the enumerative aspects) </li>
     <li> Tableaux </li>
    </ol>
   </li>
   <li> The appendix of [Stanton, White; Constructive %Combinatorics] contains
   enumeration algorithms to start with. </li>
   <li> Another source is [Knuth, Volume 4, Fascicle 3]. </li>
  </ul>

*/

