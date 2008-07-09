// Oliver Kullmann, 9.7.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Algebra/Lisp/Groupoids/plans/BasicNotions.hpp
  \brief Plans on groupoids regarding the most basic notions


  \todo Notions and notations
  <ul>
   <li> A "groupoid" ("grd") is a pair [V,f] such that V is a set and
   f: V^2 -> V; thus a groupoid is formally exactly a square combinatorial
   matrix over V with index set V. </li>
   <li> A "semigroup" ("sgr") is a groupoid with associative f. </li>
   <li> A "unital groupoid" ("ugrd") is a triple [V,f,e] such that [V,f]
   is a groupoid and e is the identity element. </li>
   <li> A "monoid" ("mon") is a unital groupoid [V,f,e] such that [V,f] is
   a semigroup. </li>
   <li> For groups we provide four different signatures:
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
   <li> A fundamental problem is that for example lambda-expressions cannot
   be put (reliably) into a set (equality would be decided here w.r.t.
   identicallity of terms):
    <ol>
     <li> So besides "set-groupoids" we need other forms of groupoids. </li>
     <li> The base-set could be specified by a pair [n,l], where n is a
     natural number from 0 to inf, specifying the size of the initial segment
     of NN to be considered, while l is a labelling functions, which assigns
     a label to each now standardised element. </li>
     <li> In this way also certain infinite groups could be handled; but the
     rational numbers for example only clumsily. </li>
     <li> On the other hand we could use "implicit" representations by pairs
     [p,l], where p is some predicate (the inverse image of true specifies the
     set), while l is a labelling function defined on the elements of the
     set). </li>
     <li> Perhaps we introduce a module "Sets" in
     ComputerAlgebra/DataStructures for handling all these different set 
     representations. </li>
     <li> On the other hand, the labelling function can be used with arbitrary
     sets, and this seems preferable. </li>
     <li> And the labelling function can be hidden in the other functions
     which need them (the composition etc.), and perhaps this suffices, at
     least for the moment. </li>
     <li> So we just stick to the naive concepts? </li>
     <li> But a problem occurs with automorphism groups, since our morphisms
     are just maps?! </li>
     <li> One solution would be to make the group element then just the
     corresponding lists-as-permutations, or some other presentation
     (the index in some standard enumeration, a cycle presentation etc). </li>
     <li> The only point in attaching the labelling function to the group
     presentation would be that then we know how to find it; perhaps we have
     the convention, that labelling functions are collected into one list,
     which is attached as the last element of the groupoid-as-list etc. </li>
    </ol>
   </li>
  </ul>


  \todo Differernt forms of tests
  <ul>
   <li> Especially for groups there are different forms of the tests for
   defining properties, which exploit that the basic "axioms" are not
   independent. </li>
   <li> We need to provide these different versions. </li>
   <li> How to name them? </li>
   <li> And which version shall be the default? </li>
  </ul>

*/

