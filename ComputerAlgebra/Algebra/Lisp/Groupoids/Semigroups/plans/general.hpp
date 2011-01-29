// Oliver Kullmann, 22.7.2008 (Swansea)
/* Copyright 2008. 2009, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Algebra/Lisp/Groupoids/Semigroups/plans/general.hpp
  \brief Plans in general regarding semigroups
  
  
  \todo Transformations
  <ul>
   <li> The abbreviation for transformations is "trf", in the two forms
   "trf_l" (via lists), and "trf_sm" (more general, via set-maps). </li>
   <li> Provide trf_sm_compo and trf_sm_mon. </li>
   <li> Compare "Acronyms" in
   Algebra/Lisp/Groupoids/Groups/plans/SymmetricGroups.hpp. </li>
   <li> It seems we should switch "trf_l" -> "trfl", and likewise "trf_sm"
   -> "trfsm". </li>
   <li> There is also "trff", that is, transformations as functions. </li>
  </ul>


  \todo The natural operation of N
  <ul>
   <li> For every semigroup (S,*) we have the natural operation of N
   by "exponentiation", (n,x) -> x^n for x in S and n in N. </li>
   <li> This operation is an operation of the prering (N,+,*,0,1) on (S,*)
   (i.e., a "premodule"; see
   ComputerAlgebra/Algebra/Lisp/Ringframes/plans/BasicNotions.hpp), that
   is, a homomorphism of (N,+,*,0,1) into the transformation-prering of S
   (with map-composition as multiplication, and pointwise-composition
   (in S) as addition). </li>
   <li> If S is a monoid, than N_0 can operate, and if S is a group, then
   Z can operate.
    <ol>
     <li> Algorithmically, these are just case distinctions: if n = 0, then
     return the neutral element, if n < 0, then compute the inverse x' and
     return (x')^(-n). </li>
     <li> According to our general philosophy "no polymorphism at the
     Maxima/Lisp level", we should thus have three functions, providing
     these three different operations. </li>
     <li> Perhaps all this should go to Algebra/Lisp/Ringframes/Operations ?
     </li>
     <li> And perhaps the group-case to Algebra/Lisp/Ringframes/Modules ? </li>
     <li> Or perhaps we keep it all in this module, with links to the other
     modules ? </li>
     <li> Compare "Module Operations" in
     ComputerAlgebra/Algebra/Lisp/Ringframes/plans/general.hpp. </li>
    </ol>
   </li>
   <li> In [Henri Cohen, A Course in Computational Algebraic Number Theory,
   Springer, 1993] one finds four basic algorithms.
    <ol>
     <li> "Right-left binary"
      <ul>
       <li> See Algorithms::Power_natural in General/Algorithms.hpp for a
       C++ implementation. </li>
      </ul>
     </li>
     <li> "Left-right binary" </li>
     <li> "Left-right binary, using bits" </li>
     <li> "Left-right base 2^k"; with a "flexible" improvement in the
     Addendum (page 547). </li>
     <li> Furthermore "addition chains" are mentioned. </li>
     <li> And we need also to provide the simple iterated multiplication. </li>
    </ol>
   </li>
   <li> An additional "service" possibly provided by a semigroup in this
   context is a (fast) squaring algorithm.
    <ol>
     <li> This could be an additional parameter for all the above (fast)
     methods. </li>
     <li> While instances are provided which just use the semigroup
     multiplication. </li>
    </ol>
   </li>
   <li> Semigroups could also provide the complete operation of N:
    <ol>
     <li> In permutation groups where the elements are given by their
     cycle presentation, powers can be computed directly very efficiently;
     see "Computing powers, based on the cycle representation" in
     ComputerAlgebra/Algebra/Lisp/Groupoids/Groups/plans/SymmetricGroups.hpp.
     </li>
     <li> So if an algorithm uses the operation of N (or N_0, or Z) on a
     semigroup (monoid, group), then the operation should be provided as
     a parameter. </li>
    </ol>
   </li>
  </ul>


  \todo Semilattices
  <ul>
   <li> Should semilattices yield of submodule of this module, or should it
   be separate? </li>
  </ul>

*/

