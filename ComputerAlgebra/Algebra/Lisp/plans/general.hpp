// Oliver Kullmann, 21.3.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Algebra/Lisp/plans/general.hpp
  \brief Plans in general for algebra functionality via Maxima/Lisp


  \todo Write tests for all functions provided


  \todo Maxima bugs
  <ul>
   <li> Register all gf-bugs we are aware of with the Maxima bug-tracker,
   and also send a summarising e-mail to the Maxima mailing list.
    <ol>
     <li> Consider also the kind-of-bugs as in, e.g., okltest_gf_stand. </li>
     <li> In any way, the gf-documentation should discuss the representation
     issue. </li>
    </ol>
   </li>
  </ul>


  \todo Complete docus
  <ul>
   <li> All functions need to be covered, using prime fields GF(p)
   and more complex fields GF(p^n). </li>
  </ul>


  \todo Demos


  \todo Groupoids, groups etc.
  <ul>
   <li> We need a submodule for groupoids and specialisations. </li>
   <li> Calling it "Groupoids"? </li>
   <li> Concepts and abbreviations:
    <ol>
     <li> A "groupoid" ("grd") is a pair [V,f] such that V is a set and
     f: V^2 -> V; thus a groupoid is formally exactly a square combinatorial
     matrix over V with index set V. </li>
     <li> A "semigroup" ("sgr") is a groupoid with associative f. </li>
     <li> A "unital groupoid" ("ugrd") is a triple [V,f,e] such that [V,f]
     is a groupoid and e is the identity element. </li>
     <li> A "monoid" ("mon") is a unitial groupoid [V,f,e] such that [V,f] is
     a semigroup. </li>
     <li> For groups we provide for different signatures:
      <ul>
       <li> A "group" ("grp") is a semigroup [V,f] such that there exists an
       identity element and every element has an inverse. </li>
       <li> A "group with identity" ("grp0") is a monoid [V,f,e] such that
       every element has an inverse. </li>
       <li> A "group with inverse" ("grp1") is a triple [V,f,i] such that [V,f]
       is a semigroup which has an identity element, and where i: V -> V provides
       for every element an inverse. </li>
       <li> A "group with identity and inverse" ("grp01") is a quadruple [V,f,e,i]
       such that [V,f,e] is a group with identity and [V,f,i] is a group with
       inverse. </li>
      </ul>
     </li>
     <li> Quasigroups:
      <ul>
       <li> A "quasigroup" ("qgrp") is a groupoid [V,f] such that every linear
       equation a*x = b has a unique solution x. </li>
       <li> A "quasigroup with solution" ("qgrp2") is a triple [V,f,s] such that
       [V,f] is a quasigroup and s: V^2 -> V provides the solution, i.e., for all
       a,b in V we have a*s(a,b) = b. </li>
       <li> There is the issue of a*x=b and x*a=b ? </li>
      </ul>
     </li>
    </ol>
   </li>
   <li> Promotions are grp2grp0, grp2grp1, grp2grp01, and then we have the
   down-casts ugrd2grd, grp02grp, grp12grp, etc..
    <ol>
     <li> Two consecutive digits don't look good?
      <ol>
       <li> We could speak of "grpn" (group with neutral element) and
       "grpi" (group with inverse), and "grpni". </li>
       <li> Then we should also use "qgrps". </li>
      </ol>
     </li>
     <li> It seems that for example mon2sem is not needed since we have
     ugrd2grd, and there are no types attached to the objects. </li>
    </ol>
   </li>
   <li> All the above likely should also exist in ordered versions, with
   prefix "o". </li>
   <li> A fundamental operation is the closure of a subset under the
   operations. </li>
   <li> And we need homomorphism notions and tests. </li>
   <li> The most fundamental construction for new groupoids is the product
   construction. </li>
   <li> And given a groupoid, we can form its "complex groupoid", given by
   all subsets and the complex product (by all combinations).
    <ol>
     <li> Here it would be good if the base sets could be given lazily. </li>
     <li> How to achieve that?? </li>
     <li> Perhaps just the law of composition is given, and the base set is
     only implicitly given. </li>
    </ol>
   </li>
   <li> Then we need all the known generators for semigroups etc. </li>
   <li> Perhaps we have modules "Groupoids", "Semigroups", "Quasigroups" and
   "Groups", for the main theories. </li>
   <li> "PermutationGroups" perhaps also on its own? </li>
  </ul>


  \todo Permutation groups
  <ul>
   <li> As special cases of groups we provide the symmetric groups sym_grp(X)
   and sym_grp(n). </li>
   <li> This likely needs its own sub-module. </li>
   <li> For a permutation there are four basic models:
    <ol>
     <li> (i) A function which is a bijection. </li>
     <li> (ii) A set-map. </li>
     <li> (iii) A list of elements. </li>
     <li> (iv) A set of cycles (each a non-empty list which altogether partition
     the base set). </li>
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
       we procide it only for standardised elements (i.e., in {1,...,n}). </li>
       <li> Furthermore, perhaps for a cycle presentation of a permutation, only
       the non-trivial cycles are given, while trivial (one-element) cycles are
       discarded. </li>
       <li> Or we speak of "reduced cycles". </li>
       <li> Or, as in [Rotman], of a "complete factorisation". </li>
      </ul>
     </li>
    </ol>
   </li>
  </ul>


  \todo Semirings, rings, fields
  <ul>
   <li> We need modules "Semirings", "Rings", "Fields". </li>
   <li> Likely also "Lattices" etc. </li>
  </ul>


  \todo Modular arithmetic
  <ul>
   <li> The rings ZZ_n should be provided. </li>
  </ul>
  

  \todo Actions and operations
  <ul>
   <li> Likely we should also have a module for the action of a set on a set.
   </li>
   <li> With the most prominent cases the action of a monoid on a set, and the
   operation of a group on a set. </li>
  </ul>

*/

