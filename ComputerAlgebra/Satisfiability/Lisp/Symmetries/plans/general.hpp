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
   <li> See ComputerAlgebra/Graphs/Lisp/Isomorphisms/plans/general.hpp for
   plans on graph isomorphisms. </li>
   <li> See Isomorphisms/plans/CLSIsomorphisms.hpp for plans on finding and
   utilising isomorphisms between clause-sets. </li>
  </ul>


  \todo Applying substitutions
  <ul>
   <li> See "standardise_fcs" in
   ComputerAlgebra/Satisfiability/Lisp/ClauseSets/plans/general.hpp. </li>
   <li> A general framework of "substitutions" and applying them is needed.
   </li>
   <li> See "Renaming" in
   ComputerAlgebra/Satisfiability/Lisp/ClauseSets/BasicOperations.mac. </li>
   <li> The notion of a "literal substitution":
    <ol>
     <li> These are functions with literals as values; the question is
     whether variables or literals are the basic arguments.
      <ul>
       <li> Mathematically, (all) literals (relevant here) seems appropriate.
       </li>
       <li> But programmatically variables seems more appropriate, since
       so redundancy in the presentation is avoided. </li>
      </ul>
     </li>
     <li> On a set V of variables: each variable v in V -> literal f(v). </li>
     <li> We need then a standard wrapper which extends it to the literals:
     if the input is a negative literal, return -f(-v). </li>
     <li> Applying a literal substitution f to a clause C is possible iff C
     is contained in the domain of f, and then yields f(C) = {f(x) : x in C}.
     </li>
     <li> Applying a literal substitution f to a clause-set F is possible iff
     f is applicable to all C in F, and the result then is f(F) = {f(C) :
     C in F}. </li>
     <li> A "var-substitution" is a literal substitution which doesn't flip
     signs, i.e., for a variable v in the domain of f also f(v) is a variable.
     </li>
     <li> A "sign-substitution" is a literal substitution which doesn't change
     underlying variables, i.e., for x in the domain of f we have var(f(x)) =
     var(x). </li>
    </ol>
   </li>
   <li> A problem is that substitution are not generally applicable
   to arbitrary literals.
    <ol>
     <li> One could extend them, using that "false" or "0" is not a literal.
     </li>
     <li> We could use (additionally) "elsub" etc. for these extended
     versions? Or should we only use the extended versions? </li>
     <li> But these extended versions should know about their domains. </li>
     <li> Perhaps these extended substitutions are pairs [V,f], where V is
     the variable-domain. </li>
    </ol>
   </li>
   <li> Abbreviations:
    <ol>
     <li> "lsub" for "literal-substitution". </li>
     <li> "vsub" for "variable-substitution". </li>
     <li> "ssub" for "sign-substitution". </li>
     <li> These only operate on variables. Extended to literals, clauses,
     clause-sets, we could use the prefixes used for them, perhaps
     with an underscore. </li>
     <li> So for example "fcs_lsub". </li>
    </ol>
   </li>
   <li> Given a basic literal-substitution, it needs to be extended to
   literals:
    <ol>
     <li> Since it's just a case distinction, it seems just a trivial
     lambda-wrapper is appropriate. </li>
     <li>
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


  \todo Homomorphisms
  <ul>
   <li> Create "Homomorphisms.mac" etc. </li>
   <li> Since homomorphisms are special substitutions, first this notion
   needs to be fixed. </li>
  </ul>

*/

