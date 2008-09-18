// Oliver Kullmann, 18.9.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Algebra/Lisp/Groupoids/PermutationGroups/plans/general.hpp
  \brief General plans for permutation groups


  \todo Orbits and stabilisers
  <ul>
   <li> The first task is to implement the calculation of orbits and
   stabilisers according to section 4.1 in [Handbook of computational group
   theory]. </li>
   <li> And then, of course, we need to connect to Gap. </li>
  </ul>


  \todo Centralisers
  <ul>
   <li> With exercise 2 in section 4.6.5 in [Handbook of computational group
   theory] for a subgroup H of S_n (H likely just given by a set of
   generators) the centraliser C(H) is computable in polynomial time (that
   is, a set of generating elements). </li>
   <li> This includes the case of the centraliser of a single element x.
    <ol>
     <li> Assume the cycle decomposition of x is (c_1, ..., c_k). </li>
     <li> Then every permutation with disjoint support (i.e., whole cycle
     decomposition is completely disjoint from the one for x) is in the
     centraliser of x. </li>
     <li> And every of the 2^k compositions of the cycles c_i yields an
     element in the centraliser, where furthermore each c_i can be
     replaced by some power. </li>
     <li> However this is not everything (MG to provide an example). </li>
    </ol>
   </li>

*/

