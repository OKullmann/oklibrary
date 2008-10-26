// Oliver Kullmann, 18.9.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Algebra/Lisp/Groupoids/Groups/PermutationGroups/plans/general.hpp
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
     <li> The size is n! / |K(x)|, where K(x) is the conjugacy class of x.
     (We need a function for computing this size, and also computing
     the conjugacy class needs to be provided as listing all permutations
     with the same cycle type.) </li>
     <li> Assume the complete cycle decomposition of x is
     (c_1, ..., c_k). </li>
     <li> Then a generating set consists of the c_1, ..., c_k together
     with generating sets for the block permutation groups which
     permute cycles of the same length (using the given order, so
     that only whole blocks are commuted). </li>
     <li> In this way some form of "product decomposition" is achieved? </li>
    </ol>
   </li>
  </ul>

*/

