// Oliver Kullmann, 27.1.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/Autarkies/plans/general.hpp
  \brief Plans for Maxima-components regarding autarkies


  \todo Create milestones


  \todo Write tests


  \todo Write docus


  \todo Basic translation
  <ul>
   <li> Implement the basic translation, which just translates
   the boolean F into a G with variables over {-1,0,1}. </li>
  </ul>


  \todo Translation to SAT according to Liffiton and Sakallah
  <ul>
   <li> Extend the documentation on the meaning of the variables. </li>
   <li> Implement the back-translation, satisfying assignments
   to autarkies. </li>
   <li> Derive an autarky-search-function (searching for some
   non-trivial autarky, using some SAT-solver as argument). </li>
   <li> Write a generic autarky-search-test (for functions which
   return a non-trivial autarky or "false" if none exists). </li>
   <li> Create a demo. </li>
  </ul>


  \todo Lean kernel via variable-elimination
  <ul>
   <li> Implement the computation of the lean kernel via a SAT-solver,
   which in the unsat-case returns a set of variables used by some
   resolution refutation. </li>
  </ul>


  \todo Matching autarkies
  <ul>
   <li> See MatchingAutarkies/plans/general.hpp. </li>
   <li> Finding some matching autarky. </li>
   <li> Computing the matching-lean kernel. </li>
  </ul>


  \todo Linear autarkies
  <ul>
   <li> Given the clause-variable matrix M, non-trival linear autarkies
   are given as non-trivial solutions of M x >= 0. </li>
   <li> From what Maxima provides, apparently only maximize_lp (in package
   "simplex") is of use. </li>
   <li> One possibility to achieve complete search for linear autarkies
   is to consider one after another the additional constraints x_i >= 1
   and x_i <= -1. </li>
   <li> An alternative is to first consider balanced linear autarkies (by
   linear equations, i.e., M x = 0), and then solving all M x = e_i
   for the possible e_i. </li>
   <li> In both cases the objective function would be constant zero. </li>
   <li> The above methods just search for feasible solutions.
   How can we use the optimisation facilities? Apparently only heuristical
   usage is possible?
    <ol>
     <li> For example maximising the sum of all x_i. </li>
     <li> If this only yields 0, then one can attempt minimising the sum
     of the x_i. </li>
     <li> What can be said in the remaining case? All solutions fulfill
     sum x_i = 0; is this possible if we also know that no balanced
     autarkies (non-trivial solutions of M x = 0) exist? </li>
    </ol>
   </li>
   <li> Finally we need a dedicated approach.
    <ol>
     <li> M has a non-trival linear autarky iff the polyhedron M x >= 0
     is unbounded, and a non-trivial autarky corresponds to an infinite
     ray contained in the polyhedron. Can this be better exploited? </li>
    </ol>
   </li>
  </ul>


  \todo Balanced autarkies


  \todo Pure autarkies


  \todo Autarky search via running through all total assignments
  <ul>
   <li> First implement the autarky extraction as discussed in
   Autarkies/Search/plans/AnalyseTotalAssignment.hpp. </li>
   <li> Then implement a simple search through all total
   assignments. </li>
   <li> We need a module on local search where we implement the basic
   algorithms (of course, this will be slow, but that doesn't matter
   here). </li>
   <li> Then we can experiment with different connections to
   local search (as discussed in
   Autarkies/Search/plans/AnalyseTotalAssignment.hpp. </li>
  </ul>


  \todo Implement the translation of USAT to LEAN according the our
  article with Victor and Mirek


  \todo The autarky monoid
  <ul>
   <li> Implement the full representation, all essential autarkies
   together with the composition of partial assignments. </li>
   <li> Is there some computer-algebra-support for representing
   semigroups (and monoids)? </li>
  </ul>

*/

