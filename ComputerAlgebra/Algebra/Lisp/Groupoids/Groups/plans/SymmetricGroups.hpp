// Oliver Kullmann, 6.7.2008 (Swansea)
/* Copyright 2008, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Algebra/Lisp/Groupoids/Groups/plans/SymmetricGroups.hpp
  \brief Plans on providing the basic symmetric groups


  \todo Implementations
  <ul>
   <li> As special cases of groups we provide the symmetric groups sym_grp(X)
   and sym_grp(n). </li>
   <li> In SymmetricGroups.mac; the submodule PermutationGroups contains then
   all these special algorithms. </li>
   <li> For a permutation there are four basic models:
    <ol>
     <li> (i) A function which is a bijection; a problem is how to provide
     an order, so that we can put these elements into a set? </li>
     <li> (ii) A set-map. </li>
     <li> (iii) A list of elements. </li>
     <li> (iv) A set of cycles (each a non-empty list, smallest element first,
     which altogether partition the base set). </li>
    </ol>
   </li>
   <li> Likely we should provide all four models, with conversions between
   them.
    <ol>
     <li> For (ii), (iii), (iv) equality between elements is clear, but for (i)
     we need an equality test. </li>
     <li> Composition for (ii), (iii), (iv) must happen eagerly, but for (i)
     it can happen "totally lazily" and "lazily". </li>
     <li> The problem with (iv) is that to enforce uniqueness, we need an order
     on the elements, so that the cycle always starts with the first element
     in the order.
      <ul>
       <li> So perhaps it is only provided with ordered base-set, or for
       standardised elements. </li>
       <li> Perhaps, since the cycle presentation is combinatorial in nature,
       we provide it only for standardised elements (i.e., in {1,...,n}). </li>
       <li> Furthermore, perhaps for a cycle presentation of a permutation,
       only the non-trivial cycles are given, while trivial (one-element)
       cycles are discarded. </li>
       <li> Or we speak of "reduced cycles". </li>
       <li> Or, as in [Rotman], of a "complete factorisation". </li>
      </ul>
     </li>
    </ol>
   </li>
  </ul>


  \todo Acronyms
  <ul>
   <li> Compare "Transformations" in
   Algebra/Lisp/Groupoids/Semigroups/plans/general.hpp. </li>
   <li> For transformations as lists, yet we use "trf_l" as well as "trfl".
   </li>
   <li> For transformations as functions yet we use "trff". </li>
   <li> Perhaps the forms without underscore are preferable, since we get
   more underscores in the rest of the names. </li>
   <li> DONE
   For permutations (special transformations) we use "per"; perhaps it
   should be "pmt"? </li>
  </ul>


  \todo Computing powers, based on the cycle representation
  <ul>
   <li> An additional service for permutation groups with cycle representation
   is that integer powers are implemented directly. </li>
   <li> Compare "The natural operation of N" in
   ComputerAlgebra/Algebra/Lisp/Groupoids/Semigroups/plans/general.hpp. </li>
   <li> The basic algorithm is the exponentiation of a cycle:
    <ol>
     <li> XXX </li>
    </ol>
   </li>
  </ul>

*/

