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


  \todo Basic functionality
  <ul>
   <li> Check whether a partial assignment is an autarky:
    <ol>
     <li> See
     ComputerAlgebra/Satisfiability/Lisp/ClauseSets/BasicOperations.mac. </li>
     <li> Use analyse_pa. </li>
    </ol>
   </li>
  </ul>


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


  \todo Lean kernel via oracle for leanness-decision
  <ul>
   <li> See [Kullmann 2003 (DAM)], Lemma 8.6. </li>
   <li> In [Kullmann, CSR 12-2007], Lemma 3.1 this is generalised to
   non-boolean clause-sets. </li>
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


  \todo Implement the translation of USAT to LEAN according to our
  article with Victor and Mirek


  \todo The autarky monoid
  <ul>
   <li> Implement the full representation, all essential autarkies
   together with the composition of partial assignments. </li>
   <li> Is there some computer-algebra-support for representing
   semigroups (and monoids)? </li>
  </ul>

*/

