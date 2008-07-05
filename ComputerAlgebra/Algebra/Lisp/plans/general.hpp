// Oliver Kullmann, 21.3.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Algebra/Lisp/plans/general.hpp
  \brief Plans in general for algebra functionality via Maxima/Lisp


  \todo Modules
  <ol>
   <li> One module for sets with one binary operation: "Magmas" or
   "Groupoids". With submodules
    <ol>
     <li> Actions (of a set on a set or a groupoid) </li>
     <li> Operations (of a groupoid on a set or on a groupoid; the more
     "specialised" considerations are in module Algebra/Acts) </li>
     <li> Groups (with submodules PermutationGroups and Presentations) </li>
     <li> Quasigroups </li>
     <li> Semigroups </li>
     <li> Semilattices </li>
    </ol>
   </li>
   <li> One module for sets with two binary operations; how to call it,
   "Ringoids" or "DoubleMagmas"?? There seems to be no notion for it.
   With submodules
    <ol>
     <li> Fields (moving FiniteFields.mac there) </li>
     <li> Lattices </li>
     <li> Rings </li>
     <li> Semirings </li>
    </ol>
   </li>
   <li> One submodule for two sets A, B together with f: A x B -> B, i.e.,
   Actions (including left and right actions).
    <ol>
     <li> The alternative representation is as a map A -> B^B. </li>
     <li> An "action in general" is a map from a set to the set of
     endomorphisms of some structure. </li>
     <li> The question is whether actions e.g., on groupoids (via
     homomorphisms) etc. are to be found in module Groupoids or in module
     Actions? </li>
     <li> Then we have "operations" of a groupoid or a ringoid on some
     structure whose endomorphism-monoid can also be equipped with an
     "addition" in the second case, so that the operation then is
     homomorphism between groupoids resp. ringoids. </li>
     <li> This "addition" of morphisms is (typically? always?) the elementwise
     addition of functions. </li>
     <li> Most prominent the operation of a monoid on a set, the operation
     of a semiring on a semiring (semimodules), the operation of a ring
     on a ring (modules), and the operation of a field on a field (vector
     spaces). </li>
     <li> Special cases of the operation of a monoid on a set are all the
     variations on dynamical systems (see below). </li>
     <li> The operation of a monoid on a set generalised the notion of a
     finite automata. </li>
     <li> Normally the operation of a monoid on a set is just called an "act";
     perhaps this does not clash with our terminology, since we speak of an
     "action" ?! </li>
     <li> So we could have, as the most prominent cases, (independent) modules
     "Acts" (for the operation of a semigroup/monoid on a set) and "Moduloids"
     (for semimodules, modules, and vector spaces), while the rest is found
     in the other modules. </li>
    </ol>
   </li>
   <li> Since groups are a big field on themselves, perhaps we emancipate
   it, making it a module on its own?
    <ol>
     <li> Same for Fields (in Ringoids). </li>
     <li> And also Lattices (in Ringoids; of course, BooleanAlgebras are
     also a topic here?!?) </li>
    </ol>
   </li>
   <li> Another organisational problem is what to do with the Lisp and
   Aldor submodules? Of course, we are already in the Lisp-part, and
   so can just ignore this question (for the moment). </li>
   <li> Then we have UniversalAlgebra. But there are also relational
   structures, and more general, first-order structures?!?
    <ol>
     <li> Perhaps we have a module ComputerAlgebra/Structures, which contains
     sub-modules AlgebraicStructures, RelationalStructures, FirstOrder. </li>
     <li> So perhaps we don't put universal algebra into Algebra. </li>
    </ol>
   </li>
  </ul>
     

  \todo Groupoids, groups etc.
  <ul>
   <li> We need a submodule for groupoids and specialisations. </li>
   <li> Calling it "Groupoids"? Or "Magmas"? </li>
   <li> Concepts and abbreviations:
    <ol>
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
   <li> A fundamental operation is the closure of a subset under the
   operations.
    <ol>
     <li> As a result we obtain the closure plus shortest terms in the
     generators representing the elements of the closure, plus all relations
     found such that the generated structure is determined by these relations.
     </li>
     <li> Easiest for semigroups, where the terms are just words over the
     generators (for which it should be possible to specify names). </li>
     <li> Here, starting with a set of elements, which yields the set of
     words of length 1, we form all words of length 2, determine equalities to
     the already existing elements and to the new elements, which yield new
     relations, given that all existing relations have been taken into account;
     iterating this process expands only the words of full length. </li>
     <li> For groups we have to take inversion into account; and relations, 
     which for semigroups are equations, that is, 2-element sets of words, can
     now be simplified to just single words (equal to the neutral element). 
     </li>
    </ol>
   </li>
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
     <li> (i) A function which is a bijection; a problem is how to provide
     an order, so that we can put these elements into a set? </li>
     <li> (ii) A set-map. </li>
     <li> (iii) A list of elements. </li>
     <li> (iv) A set of cycles (each a non-empty list which altogether
     partition the base set). </li>
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

