// Matthew Gwynne, 26.8.2011 (Swansea)
/* Copyright 2011, 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/Generators/plans/Pigeonhole.hpp
  \brief Plans for Maxima-generators of pigeonhole formulas


  \todo Standard variable ordering for extended-pigeon-hole formulas
  <ul>
   <li> For the extended pigeon-hole formulas with m holes, we have variables
   of the form php_ext(l,i,j) for 1 <= l <= m, 1 <= i <= l+1, 1 <= j <= l. </li>
   <li> The natural ordering is the lexicographical order on
   NN x {1,...,l+1} x {1,...,l}. This has the nice property that
   the ordering doesn't depend on m. </li>
   <li> The standardisation then simply maps php_ext(l,i,j) to
   sum((lp+1) * lp, lp, 1, l-1) + (i-1) * l + j. </li>
   <li> Computing the inverse of
   \verbatim
m = (l^3 - 4*l)/3 + i*l + j
   \endverbatim
   where 1 <= l, 1 <= i <= l+1, 1 <= j <= l.
    <ul>
     <li> We first compute l, by computing the maximum integer l such that
     \verbatim
l^3/3+1*l-4*l/3+1 <= m,
     \endverbatim
     that is, substituting i <- 1 and j <- 1. </li>
     <li> So we solve for l' in (l')^3/3+(l')-4*(l')/3+1 = m
     and take the floor to get such a maximum l. </li>
     <li> The closed-form equation
     \verbatim
(sqrt(243*t^2-486*t+239)/(2*3^(3/2))+(3*t-3)/2)^(1/3)
  +1/(3*(sqrt(243*t^2-486*t+239)/(2*3^(3/2))+(3*t-3)/2)^(1/3)))
     \endverbatim
     is then the real-valued solution from
     \verbatim
solve(l^3/3+l-4*l/3+1 = t, l).
     \endverbatim
     </li>
    </ul>
   </li>
   <li> A problem with the inversion is that:
    <ol>
     <li> solve(l^3/3+l-4*l/3+1 = t, l) computes the correct value for l if
     we substitute a particular value for t. However, it also computes
     imaginary solutions, and it isn't clear from the Maxima documentation
     how to reliably extract only the real solution. </li>
     <li> The closed-form expression
     \verbatim
(sqrt(243*t^2-486*t+239)/(2*3^(3/2))+(3*t-3)/2)^(1/3)
  +1/(3*(sqrt(243*t^2-486*t+239)/(2*3^(3/2))+(3*t-3)/2)^(1/3)))
     \endverbatim
     is the real-valued solution to solve(l^3/3+l-4*l/3+1 = t, l),
     but when t = 71, rounding errors mean we get l = 5 instead
     of l = 6:
     \verbatim
ev(float(floor((sqrt(243*t^2-486*t+239)/(2*3^(3/2))+(3*t-3)/2)^(1/3)
  +1/(3*(sqrt(243*t^2-486*t+239)/(2*3^(3/2))+(3*t-3)/2)^(1/3)))), t: 71);
 5
     \endverbatim
     Float is needed here to force the simplification of the term.
     </li>
     <li> The current solution, in invstandardise_weak_php_unsat_ext_uni, is
     to iteratively search for l from l = 1. This is then linear
     complexity in l. </li>
     <li> The current solution should be reconsidered, and a better method
     found. </li>
    </ol>
   </li>
  </ul>


  \todo Update the php-functions
  <ul>
   <li> All functions need to be based on the list-forms ("fcl" etc.), not
   on the set-form (this is then derived). </li>
   <li> At the same time, we should add tests for all these functions
   (we don't have tests for weak_php_fcs etc). </li>
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

