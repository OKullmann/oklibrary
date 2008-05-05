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


  \todo Applying substitutions
  <ul>
   <li> See "standardise_fcs" in
   ComputerAlgebra/Satisfiability/Lisp/ClauseSets/plans/general.hpp. </li>
   <li> A general framework of "substitutions" and applying them is needed.
   </li>
   <li> See "Renaming" in
   ComputerAlgebra/Satisfiability/Lisp/ClauseSets/BasicOperations.mac. </li>
   <li> A "literal substitution" on a set L of literals is a map f (that is,
   we can write "f(x)") defined on a set of literals including at least L, which
   is compatible with negation, i.e., f(x) is defined iff f(-x) is defined for
   a literal x, and f(-x) = -x, such that f(L) <= L. </li>
   <li> Applying a literal substitution f to a literal x is possible iff x is
   in the domain of f, and the result then is f(x). </li>
   <li> Applying a literal substitution f to a clause C is possible iff C
   is contained in the domain of f, and then yields f(C) = {f(x) : x in C}.
   </li>
   <li> Applying a literal substitution f to a clause-set F is possible iff
   f is applicable to all C in F, and the result then is f(F) = {f(C) : C in F}.
   </li>
   <li> A "var-substitution" is a literal substitution which doesn't flip signs,
   i.e., for a variable v in the domain of f also f(v) is a variable. </li>
   <li> A "sign-substitution" is a literal substitution which doesn't change
   underlying variables, i.e., for x in the domain of f we have var(f(x)) =
   var(x). </li>
  </ul>


  \todo Notions for symmetries
  <ul>
   <li> Consider a formal clause-set FF = [V,F]. </li>
   <li> A "var-symmetry" f for FF is a var-substitution with V <= dom(F) such
   that f(F) = F. </li>
   <li> A "sign-symmetry" for for FF is a sign-substitution with V <= dom(F)
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
   <li> If directed graphs are considered, then one could fix the direction
   "var(F) -> variables -> literals -> clauses -> F" (or vice versa), and get
   in this way rid off any unwanted graph automorphisms. </li>
  </ul>

*/

