// Oliver Kullmann, 11.7.2008 (Swansea)
/* Copyright 2008, 2009, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/Categories/plans/general.hpp
  \brief General plans for categories of clause-sets


  \todo Relations to other modules
  <ul>
   <li> See ComputerAlgebra/Categories/Lisp/plans/general.hpp for the
   general framework. </li>
   <li> Symmetries (see
   ComputerAlgebra/Satisfiability/Lisp/Symmetries/plans/general.hpp)
   specifically handles isomorphisms. </li>
  </ul>


  \todo Fundamental notions
  <ul>
   <li> Likely homomorphisms are special cases of substitutions; see
   "Applying substitutions" in
   ComputerAlgebra/Satisfiability/Lisp/Symmetries/plans/general.hpp. </li>
   <li> A morphism f: F -> G would then be a substitution which maps the
   literals from F to the literals from G, and which translates the clauses
   of F into the clauses of G.
    <ol>
     <li> Abbreviations like "morcls" (morphism for clause-sets). </li>
     <li> It seems that the morphism itself just maps literals to literals
     (or variables to variables in case of variable-based categories; see
     below), while the action on the clauses is just induced. </li>
     <li> The two main conditions for clauses C are that f(C) equals a
     clause in the codomain, or, more generally, there exists a clause D
     in the codomain with D <= f(C). </li>
    </ol>
   </li>
   <li> We have variable-based and literal-based categories, which are
   concrete categories of the categories of var-sets resp. lit-sets.
    <ol>
     <li> A var-set is just a set, while a lit-set is a ZZ_2-set. </li>
     <li> A ZZ_2-set is a pair [L,opr], according to
     ComputerAlgebra/Algebra/Lisp/Groupoids/Operations/plans/general.hpp,
     where opr(e,l) for e in {0,1} and l in L yields an element of L. </li>
     <li> This uses ZZ_2 = cyclic_ugrp(2) (see
     ComputerAlgebra/Algebra/Lisp/Groupoids/Groups/CyclicGroups.mac), and so
     actually one can also use opr(e,l), where e is not just 0 or 1, but an
     arbitrary integer (interpreted as 0 or 1 iff its even or odd). </li>
     <li> Could we also just use as set L of integers as a literal set, which
     is interpreted via 1*l = -l ? Likely we should better employ a conversion
     function here. </li>
    </ol>
   </li>
  </ul>


  \todo Categories of boolean functions
  <ul>
   <li> Variable-based: Objects are pairs [V,F], where V is a (finite) set
   of variables, while F: TASS(V) -> {0,1}, where TASS(V) = {0,1}^V. </li>
   <li> Literal-based: Objects are pairs [[L,opr], F], where [L,opr] is a
   literal-system (see above), while F: TASS(L) -> {0,1}, where TASS(L)
   is the set of maps f: L -> {0,1} which preserve the ZZ_2-operation
   (that is, we have ZZ_2-morphisms). </li>
   <li> Morphisms are maps alpha: V -> V' resp. morphisms alpha: L -> L':
    <ol>
     <li> Every total assignment f in the codomain is assigned a total
     assignment alpha(f) in the domain with alpha(f)(x) = f(alpha(x)). </li>
     <li> We have two categories here, the "DNF-form" and the "CNF-form",
     where the CNF-condition is that always F(alpha(f)) >= F'(f) holds,
     while the DNF-condition is F(alpha(f)) <= F'(f). </li>
    </ol>
   </li>
   <li> Let's call the categories BDNF and BCNF. </li>
   <li> Interpretation of clause-sets as CNF's and DNF's:
    <ol>
     <li> From categories of clause-sets we have the functors CNF resp. DNF
     to BCNF resp. BDNF, which assigns to every clause-set F its boolean
     function as CNF resp. DNF. </li>
     <li> So for a clause-set F the two boolean functions DNF(F) and CNF(F)
     are dual to each other, that is, where the dual of a boolean function
     F is given by f -> 1 + F(1+f) (where "+" here is xor). </li>
     <li> On the morphisms the functors just act identical. </li>
    </ol>
   </li>
   <li> Canonical representations of boolean functions:
    <ol>
     <li> From BCNF to categories CLS (in their various forms) we have the
     functors A and P, which assign to the boolean function F the set of
     maximal resp. minimal implicates (w.r.t. subset inclusion; so the
     minimal implicates are the prime implicates); an "implicate" is a clause
     C such that F(f) = 1 implies f(C) = 1 (C as CNF-clause). </li>
     <li> And from BDNF to categories CLS (in their various forms) we have the
     functors A and P, which assign to the boolean function F the set of
     maximal resp. minimal implicants (w.r.t. subset inclusion; so the
     minimal implicants are the prime implicants); an "implicant" is a clause
     C such that f(C) = 1 (C as DNF-clause) implies F(f) = 1. </li>
    </ol>
   </li>
   <li> We have an isomorphism D from BDNF to BCNF as well as from BCNF to
   BDNF given by forming the dual of boolean functions, where D: BDNF -> BCNF
   can be obtained as the composition of first A and then CNF (or first
   P and then CNF). </li>
   <li> Prime clause-sets and dual prime clause-sets:
    <ol>
     <li> Let Pr(F) for a clause-set F be the set of prime clauses. Now Pr
     is a functor from CLS to CLS (in its various forms), namely Pr is
     the composition of first the functor CNF and then the functor P, or,
     yielding the same result, first the functor DNF and then P. </li>
     <li> Let Tr(F) for a clause-set F be the set of dual prime clauses.
     Tr needed to be a contravariant functor from CLS to CLS, as can be seen
     with  {} -> {{}}; however having only maps as morphisms, it seems
     impossible to have any non-trivial contravariant functor? So Tr doesn't
     seem to be a functor. </li>
     <li> The point here is that Tr(F) needed to use the mapping from BNCF
     to CLS given by forming the prime DNF, which doesn't seem to be
     interpretable as functor(?). (Note that using D doesn't help, since
     D just equalises the two versions of P on BCNF and on BDNF.) </li>
    </ol>
   </li>
   <li> The intersection of BCNF and BDNF allows both forms of the P-functor:
    <ol>
     <li> We need to give them now different names: P_CNF and P_DNF. </li>
     <li> Now also the map Tr: CLS -> CLS (dual prime clauses, or
     "transversals") becomes a functor, but one needs to retrict the morphisms
     to, say, isomorphisms, so that the functor CNF (or DNF) becomes a functor
     to BCNF intersect BDNF. </li>
     <li> But this likely only in the form of CLS with subsumption. </li>
    </ol>
   </li>
  </ul>

*/

