// Oliver Kullmann, 6.1.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Combinatorics/Lisp/Enumeration/plans/Subsets.hpp
  \brief Plans regarding enumerating subsets (all, or only specific ones)


  \todo Enumerating all k-subsets
  <ul>
   <li> Given natural numbers n, k >= 0, the task is to enumerate the set
   binom({1,...,n},k) of all k-subsets of {1,...,n}. </li>
   <li> That is, in the most basic case we construct bijections between
   binom({1,...,n},k) and {1,...,binom(n,k)}. </li>
   <li> We have lexicographical and colexicographical ordering.
    <ol>
     <li> The lexicographical order is realised by
     \verbatim
listify(powerset(setn(n),k))
     \endverbatim
     (however this is obviously very inefficient, since it needs to
     enumerate always *all* subsets, and this at once). </li>
    </ol>
   </li>
   <li> And then there are Gray-codes (including the "standard" one,
   recursively constructed). </li>
  </ul>

*/

