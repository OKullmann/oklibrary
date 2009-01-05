// Oliver Kullmann, 5.1.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Combinatorics/Lisp/Enumeration/plans/general.hpp
  \brief General plans regarding enumerating combinatorial objects


  \todo Connections
  <ul>
   <li> Compare Combinatorics/Enumeration/plans/general.hpp and the sub-modules
   Combinatorics/Enumeration/plans/GrayCodes.hpp,
   Combinatorics/Enumeration/plans/Permutations.hpp and
   Combinatorics/Enumeration/plans/Subsets.hpp. </li>
  </ul>


  \todo Basic overview
  <ul>
   <li> According to [Stanton, White; Constructive Combinatorics], the
   basic sub-modules would be:
    <ol>
     <li> Permutations; perhaps with submodule Involutions </li>
     <li> Subsets </li>
     <li> Set partitions </li>
     <li> Integer partitions </li>
     <li> Product spaces </li>
     <li> Trees (not as data structures; and in Graphs we do not handle
     the enumerative aspects) </li>
     <li> Tableaux </li>
    </ol>
   </li>
   <li> The appendix of [Stanton, White; Constructive Combinatorics] contains
   enumeration algorithms to start with. </li>
   <li> Another source is [Knuth, Volume 4, Fascicle 3]. </li>
   <li> The basic task seems always to construct some bijection (in both
   directions), more or less efficiently, and study its properties.
    <ol>
     <li> As counting structures we consider intervals of integers. </li>
    </ol>
   </li>
  </ul>


  \todo Enumerating all k-subsets
  <ul>
   <li> Given natural numbers n, k >= 0, the task is to enumerate the set
   binom({1,...,n},k) of all k-subsets of {1,...,n}. </li>
   <li> That is, in the most basic case we construct bijections between
   binom({1,...,n},k) and {1,...,binom(n,k)}. </li>
   <li> We have lexicographical and anti-lexicographical ordering. </li>
   <li> And then there are Gray-codes (including the "standard" one,
   recursively constructed). </li>
  </ul>

*/

