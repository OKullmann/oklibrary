// Oliver Kullmann, 6.1.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Combinatorics/Lisp/Enumeration/plans/Subsets.hpp
  \brief Plans regarding enumerating subsets (all, or only specific ones)


  \todo Enumerating all k-subsets lexicographically
  <ul>
   <li> Given natural numbers n, k >= 0, the task is to enumerate the set
   binom({1,...,n},k) of all k-subsets of {1,...,n}. (See "Main concepts"
   in ComputerAlgebra/Combinatorics/Lisp/Enumeration/plans/general.hpp.) </li>
   <li> We have lexicographical and colexicographical ordering.
    <ol>
     <li> More precisely, one considers lists of integers, ordered
     lexicographically, and for the lexicographical ordering of subsets
     the subset is transformed into a list in ascending order, while
     for the colexicographical order descending order is used. </li>
     <li> The lexicographical order is realised by
     \verbatim
listify(powerset(setn(n),k))
     \endverbatim
     (however this is obviously very inefficient, since it needs to
     enumerate always *all* subsets, and this at once). </li>
     <li> Another, fundamental, problem here is that apparently neither
     is the order of elements of a set specified, nor can it be
     controlled --- this issue must be raised at the Maxima mailing
     list! (It renders the set-concept useless.) </li>
    </ol>
   </li>
  </ul>


  \todo Enumerating all k-subsets in Gray-code manner
  <ul>
   <li> The point here is that only one element is changed at a time (when
   proceeding to the successor). </li>
   <li> There should be a standard, recursive way of achieving such an
   order. </li>
  </ul>

*/

