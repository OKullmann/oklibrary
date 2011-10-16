// Matthew Gwynne, 26.8.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/Generators/plans/Pigeonhole.hpp
  \brief Plans for Maxima-generators of pigeonhole formulas


  \todo Update the php-functions
  <ul>
   <li> All functions need to be based on the list-forms ("fcl" etc.), not
   on the set-form (this is then derived). </li>
  </ul>


  \todo Extended Resolution clauses for the Pigeon Hole Principle
  <ul>
   <li> See [A short proof of the pigeon hole principle using
   Extended Resolution; Stephen Cook]. </li>
   <li> See weak_php_fcs in
   ComputerAlgebra/Satisfiability/Lisp/Generators/Generators.mac. </li>
   <li> Consider positive integers m and n, and Pigeon Hole Principle (PHP)
   for m pigeons and n holes. </li>
   <li> That is, consider the decision problem: is there an injective map
   from {1,...,m} to {1,...,n}? </li>
   <li> The clause-set for the weak/strong SAT translation of PHP is given by
   weak/strong_php_fcs(m,n). </li>
   <li> The clause-set weak_php_fcs(m+1,m) has only exponential size
   resolution proofs. Investigations started with [The intractability of
   resolution; Armin Haken]. </li>
   <li> In [A short proof of the pigeon hole principle using Extended
   Resolution; Stephen Cook], Cook shows that there is a polynomial size
   Extended Resolution proof of weak_php_fcs(m+1,m). </li>
   <li> That is, Cook defines a set of new variables V_S and a set of clauses
   F_S which when added to weak_php_fcs(m,n), has a polynomial size resolution
   proof. </li>
   <li> The fact that there is no injective map from {1,...,m+1} into
   {1,...,m} can be proven by induction:
    <ul>
     <li> Base case: there is no map from {1} into {}, as there are
     no maps into {}. </li>
     <li> Induction hypothesis: for all n' < n, there is no map from
     {1,...,n'+1} into {1,...,n'}. </li>
     <li> Induction Step:
      <ol>
       <li> assume for the sake of contradiction we have an injective map from
       {1,...,n+1} to {1,...,n}; </li>
       <li> we can then produce an injective map phi_n from {1,...,n} to
       {1,...,n-1} from phi; </li>
       <li> For a map phi : {1,...,n+1} -> {1,...,n}, the map
       phi_n : {1,...,n} -> {1,...,n-1} is defined as:
       \verbatim
phi_n(i) = phi(i) if phi(i) # n
           phi(n+1) if phi(i) = n
       \endverbatim
       </li>
       <li> by the induction hypothesis, this is a contradiction. </li>
      </ol>
     </li>
    </ul>
   </li>
   <li> If one could find such a phi then expanding the induction steps of
   the proof, one would get a series of maps: phi, phi_m, phi_(m-1),
   phi_(m-2), ..., phi_2, phi_1, each defined in terms of the last. </li>
   <li> Cook encodes the induction proof as an Extended Resolution proof
   by introducing new variables php_ext(n,i,j) for n, j in {1,...,m} and i in
   {1,...,m+1}. </li>
   <li> php_ext(n,i,j) encodes that in the map phi_n : {1,...,n} ->
   {1,...,n-1}, i maps to j. </li>
   <li> %Clauses are then added to define phi_n for each n in {1,...,m-1},
   based on phi_(n+1). </li>
   <li> For each n in {1,...,m-1}, F_n is the union over i in {1,...,n-1} and
   j in {1,...,n-2} of the set of prime implicates of the relation
   php_ext(n,i,j) <=> (php_ext(n+1, i,j) or (php_ext(n+1,i,n) and
   php_ext(n+1,n+1,j))). </li
   <li> In the case where n = m - 1, we have php_ext(n+1,i,j) = php(i,j)
   for all i and j. </li>
   <li> F_S is then the union of F_n for all n in {1,...,m-1}. </li>
   <li> The clause-set union(weak_php_fcs(m+1,m),F_S) then has a polynomial
   size resolution proof. </li>
   <li> Construction of the resolution proof:
    <ol>
     <li> First a representation of resolution proofs is needed. This is
     discussed in "Resolution proofs" in
     ComputerAlgebra/Satisfiability/Lisp/Resolution/plans/general.hpp. </li>
     <li> Labelled trees are a reasonable starting point, and are already
     there. </li>
     <li> Those trees which represent dag-like structures must have identical
     subtrees where clauses are "reused". </li>
     <li> The notions (and tools) for such presentations of resolution proofs
     of course go to module Resolution. </li>
     <li> A first tool is a correctness-checker. </li>
     <li> Once this has been established, the short resolution refutations of
     PHP have to be implemented. </li>
     <li> This is the only way that we can check correctness of the ER clauses!
     </li>
    </ol>
   </li>
   <li> See weak_php_unsat_ext_fcs in
   ComputerAlgebra/Satisfiability/Lisp/Generators/Generators.mac
   for the generator of the weak pigeon-hole principle with the
   extended resolution clauses. </li>
   <li> See also "Hardness of boolean function representations" in
   Experimentation/Investigations/BooleanFunctions/plans/general.hpp. </li>
  </ul>


  \todo DONE Add tests for weak_php_ext_fcs
  <ul>
   <li> DONE Tests are needed for:
    <ul>
     <li> DONE weak_php_unsat_ext_fcs; </li>
     <li> DONE php_induction_cl; </li>
     <li> DONE php_induction_step_cl. </li>
    </ul>
   </li>
  </ul>

*/

