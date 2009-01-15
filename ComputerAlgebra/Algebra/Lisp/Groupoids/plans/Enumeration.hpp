// Oliver Kullmann, 14.1.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Algebra/Lisp/Groupoids/plans/Enumeration.hpp
  \brief Plans on enumerating groupoids


  \todo Connections
  <ul>
   <li> ComputerAlgebra/Algebra/Lisp/Groupoids/Semigroups/plans/Enumeration.hpp
   </li>
   <li> ComputerAlgebra/Algebra/Lisp/Groupoids/Quasigroups/plans/Enumeration.hpp
   </li>
   <li> Experimentation/Investigations/Groupoids/plans/general.hpp </li>
  </ul>


  \todo Complete the implementation
  <ul>
   <li> Write simple tests. DONE </li>
   <li> Move to dedicated module. DONE </li>
   <li> Complete the implementation according to "XXX". </li>
   <li> Write little helper functions for working with
   all_isotopismisomorphism_classes_actively_grd. </li>
  </ul>


  \todo All isomorphism types of small groupoids
  <ul>
   <li> We need representatives: Perhaps best the lexicographically smallest
   elements (using row-wise order). </li>
   <li> For the basic properties we need counts and representatives.
    <ul>
     <li> We consider all combinations of the following basic properties:
      <ol>
       <li> commutativity </li>
       <li> having a unit </li>
       <li> being idempotent </li>
       <li> having a null </li>
       <li> having non-trivial automorphism group (this is not so common,
       but it can be read off trivially given a complete partitioning
       into isomorphism classes). </li>
      </ol>
     </li>
     <li> In module semigroups then these properties are explored for
     associative semigroups. </li>
     <li> While in module Quasigroups these properties are explored
     for groupoids which are uniquely solvable (then having a null
     is being dropped). </li>
     <li> And in module SemiLattices the basic condition is commutativity,
     idempotence and associativity together. </li>
     <li> Isotopism is considered only for general groupoids and
     for general quasigroups (since from the above properties only unique
     solvability is an invariant under isotopism (but we should give
     examples for that)). </li>
     <li> We need to study the literature, what is known about these
     numbers. </li>
     <li> Currently we have hardcoded the four main properties and their
     combinations, since for them the total number of groupoids is easily
     computable. </li>
     <li> For more properties we need a more general approach, perhaps using
     identifying strings for the properties, and a kind of database function.
     </li>
    </ul>
   </li>
   <li> We need also to build up catalogues. </li>
  </ul>

*/

