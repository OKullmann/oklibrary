// Oliver Kullmann, 29.7.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/Autarkies/plans/Translations.hpp
  \brief Plans on autarky search via translation to other problems


  \todo Basic translation
  <ul>
   <li> Implement the basic translation, which just translates
   the boolean F into a G with variables over {-1,0,1}. </li>
   <li> That is, for every old boolean variable v we obtain a new
   variable a(v) with domain {-1,0,+1}, every positive literal v
   becomes "a(v) <> -1", and every negative literal -v becomes
   "a(v) <> 1". </li>
   <li> The requirement that the autarky is non-negative is expressed by
   the disjunction "a(v) <> 0" for all variables v. </li>
   <li> The boolean translation of this non-boolean CNF just uses variables
   a(v,e) for variables v and signs e in {-1,0,+1}, and translates literals
   v into a(v,-1), -v into a(v,+1). </li>
   <li> Non-triviality becomes the clause {a(v,0) : v in var(F)}. </li>
   <li> The ALO-clauses {a(v,-1), a(v,0), a(v,+1)} for all v in var(F)
   are need, but (as usual), the AMO clauses {-a(v,e), -a(v,e')} are optional
   (though typically they help). </li>
  </ul>


  \todo Translation to SAT according to Liffiton and Sakallah
  <ul>
   <li> See the Guangzhou-2008-talk for more example applications. </li>
   <li> DONE Extend the documentation on the meaning of the variables. </li>
   <li> DONE Implement the back-translation, satisfying assignments
   to autarkies. </li>
   <li> DONE
   Derive an autarky-search-function (searching for some
   non-trivial autarky, using some SAT-solver as argument). </li>
   <li> DONE
   Write a generic autarky-search-test (as a testfunctions, testing
   functions which return a non-trivial autarky or "false" if none exists).
   </li>
   <li> Extend the tests. </li>
   <li> Via creating a splitting tree for the translated instance,
   and translating the satisfying assignments, compute all autarkies.
    <ol>
     <li> Due to the symmetries we have many satisfying assignments;
     can we improve on that? </li>
    </ol>
   </li>
   <li> lean_usat_ls:
    <ol>
     <li> Generalise the translation, so that one can specifically ask for
     certain variables to be used by the autarky. </li>
     <li> One can also ask for a set of clauses (or a single clause) to
     be satisfied. </li>
    </ol>
   </li>
   <li> Create a demo. </li>
   <li> We need also a module MaxSAT, and using a MAXSAT-solver we then can
   compute a maximal autarky. </li>
  </ul>

*/

