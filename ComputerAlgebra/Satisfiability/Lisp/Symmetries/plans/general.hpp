// Oliver Kullmann, 7.4.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/Symmetries/plans/general.hpp
  \brief Plans for Maxima-components regarding symmetries in satisfiability problems


  \todo Create milestones


  \todo Organisation
  <ul>
   <li> Rename Symmetries.mac to "Isomorphisms.mac". </li>
  </ul>


  \todo Relations to other modules
  <ul>
   <li> See ComputerAlgebra/Satisfiability/Lisp/ClauseSets/plans/Substitutions.hpp
   for substitutions. </li>
   <li> See ComputerAlgebra/Satisfiability/Lisp/Categories/plans/general.hpp
   for morphisms in general. </li>
   <li> See ComputerAlgebra/Graphs/Lisp/Isomorphisms/plans/general.hpp for
   plans on graph isomorphisms. </li>
   <li> See ComputerAlgebra/CombinatorialMatrices/Lisp/plans/Isomorphisms.hpp
   for var-isomorphisms of labelled clause-sets. </li>
   <li> See Isomorphisms/plans/CLSIsomorphisms.hpp for plans on finding and
   utilising isomorphisms between clause-sets. </li>
  </ul>


  \todo Notions for symmetries
  <ul>
   <li> Consider a formal clause-set FF = [V,F]. </li>
   <li> A "var-symmetry" f for FF is a var-substitution with V <= dom(F) such
   that f(F) = F. </li>
   <li> A "sign-symmetry" for FF is a sign-substitution with V <= dom(F)
   such that f(F) = F. </li>
   <li> A "symmetry" for FF is a literal substitution  with  V <= dom(F) such
   that f(F) = F. </li>
  </ul>


  \todo Brute-force algorithms
  <ul>
   <li> Given a formal clause-set FF = [V,F], find all var-symmetries,
   sign-symmetries or symmetries for FF. </li>
   <li> We should also allow a parameter W, a subset of V, such that only
   substitutions are considered affecting W, i.e., the literal substitution
   are identical outside of W. </li>
  </ul>


  \todo Reduction to graph isomorphisms
  <ul>
   <li> What is the standard here for available implementations:
   (undirected) graphs or directed graphs? </li>
   <li> It appears that directed graphs are better here, since they allow
   to express more structure directly. </li>
   <li> Considering undirected graphs, for FF the symmetries of FF should
   "almost always" be the symmetries of the extended clause-literal graph
   G with vertices
    <ol>
     <li> the variables </li>
     <li> the literals </li>
     <li> the clauses </li>
     <li> a vertex symbolising "var(F)", connected with exactly all variables
     </li>
     <li> a vertex symbolising "F", connected with exactly all clauses. </li>
    </ol>
   </li>
   <li> When fixing for the automorphisms of G the vertex "var(F)", then it
   should always be guaranteed that they correspond 1-1 to the symmetries of
   FF. </li>
   <li> DONE (see is_isomorphic_fcs in
   ComputerAlgebra/Satisfiability/Lisp/Symmetries/Symmetries.mac)
   If directed graphs are considered, then one could fix the direction
   "var(F) -> variables -> literals -> clauses -> F" (or vice versa), and get
   in this way rid off any unwanted graph automorphisms. </li>
  </ul>


  \todo is_isomorphic_btr_cs
  <ul>
   <li> We should rename the current "is_isomorphic_cs" etc., so that
   these names reflect the reduction to graph isomorphisms. </li>
   <li> And the current "is_isomorphic_btr_cs" should become
   "is_isomorphic_btr_cs_p", while "is_isomorphic_btr_cs" should return
   instead of true the isomorphism found. </li>
   <li> Actually, then the prefix "is" could be dropped? But perhaps
   here we keep it --- other choices aren't much better. </li>
   <li> We need a version for var-isomorphisms:
    <ol>
     <li> Call it "is_varisomorphic_btr_cs_p" etc. </li>
     <li> DONE
     This problem can also be reduced to the isomorphism-problem of
     the incidence matrices. </li>
    </ol>
   </li>
   <li> is_isomorphic_btr_cs function should become an instance of the
   concept of active clause-sets.
    <ol>
     <li> In this way making it possible to apply all of SAT techniques. </li>
     <li> We must take some care here that the overheads are acceptable, since
     this function is basic for many other functions of the library. </li>
     <li> The techniques from graph theory (see
     ComputerAlgebra/Graphs/Lisp/Isomorphisms/plans/general.hpp) should be
     transferred and generalised. </li>
    </ol>
   </li>
   <li> Computing splitting trees:
    <ol>
     <li> As a first step towards making is_isomorphic_btr_cs an instance of
     a generalised SAT algorithms, create a variant which computes the whole
     splitting tree. </li>
     <li> This then yields a representation of all isomorphisms (with proof
     of completeness!). </li>
    </ol>
   </li>
  </ul>


  \todo Exploring symmetries
  <ul>
   <li> Compute the orbits of literals under the action of the
   automorphism group of a clause-set. </li>
   <li> Stronger one could compute simply all permutations combined with
   sign flips which leave the clause-set invariant, i.e., the full
   automorphism group in the trivial representation. </li>
   <li> Then one needs to look for more intelligent representations
   of the automorphism group. </li>
   <li> Likely, Maxima is only for simple and initial attempts towards
   symmetries, and Gap is the real tool here. </li>
  </ul>


  \todo Homomorphisms
  <ul>
   <li> Create "Homomorphisms.mac" etc. </li>
   <li> Perhaps we have a dedicated module "Homomorphisms"; likely this
   is now a sub-module of ClauseSets/Categories. </li>
   <li> Since homomorphisms are special substitutions, first this notion
   needs to be fixed. </li>
   <li> A basic task is to find the core of a clause-set (according to
   [Szeider, 2003] unique up to isomorphism). </li>
  </ul>

*/

