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
  <ul>
   <li> DONE
   One module for sets with one binary operation: "Magmas" or
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
   <li> DONE (possibly later, but not yet)
   Since groups are a big field on themselves, perhaps we emancipate
   it, making it a module on its own?
    <ol>
     <li> Same for Fields (in Ringoids). </li>
     <li> And also Lattices (in Ringoids; of course, BooleanAlgebras are
     also a topic here?!?) </li>
    </ol>
   </li>
   <li> DONE
   Another organisational problem is what to do with the Lisp and
   Aldor submodules? Of course, we are already in the Lisp-part, and
   so can just ignore this question (for the moment). </li>
   <li> Then we have UniversalAlgebra. But there are also relational
   structures, and more general, first-order structures?!?
    <ol>
     <li> Perhaps we have a module ComputerAlgebra/Structures, which contains
     sub-modules %AlgebraicStructures, RelationalStructures, FirstOrder. </li>
     <li> So perhaps we don't put universal algebra into %Algebra. </li>
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

