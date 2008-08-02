// Oliver Kullmann, 1.8.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Matroids/Lisp/Greedoids/plans/general.hpp
  \brief General plans regarding greedoids


  \todo Fundamental notions
  <ul>
   <li> A "greedoid" can be considered, like a matroid, as a special type
   of hypergraph (the hereditarity-condition is replaced by the weaker
   condition of "accessibility", that is, for every non-empty hyperedge there
   exists an element which can be removed). </li>
   <li> These are the unordered versions. And then there is the notion of an
   "ordered greedoid", which is a pair [V,W], where V is a finite set while
   W is a set of repetition-free lists over V. </li>
   <li> The notion of "greedoid language" is used for these "ordered"
   versions". </li>
   <li> Perhaps by default the greedoid languages have implicitly given set
   of words, since these are just the permutations of the underlying greedoid.
   </li>
   <li> Let's use the abbreviations "gred" and "gredl". </li>
  </ul>

*/

