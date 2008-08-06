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
   <li> So every clause of F yields a clause of G; this is yet not strong
   enough, since we must forbid that a clause C in F contains falsified
   literals but no satisfied literals. For this we need |C| many clauses
   of length |C|  which express that if a literal becomes false then some
   other literal becomes true. So we actually need monosigned literals. </li>
   <li> Having those "autarky clauses", we do not need the direct translations
   of the original clauses. </li>
   <li> The requirement that the autarky is non-trivial is expressed by
   the disjunction "a(v) <> 0" over all variables v. </li>
   <li> The boolean translation of this non-boolean CNF just uses variables
   a(v,e) for variables v and signs e in {-1,0,+1}. </li>
   <li> Non-triviality becomes the clause {a(v,0) : v in var(F)}. </li>
   <li> The ALO-clauses {a(v,-1), a(v,0), a(v,+1)} for all v in var(F)
   are needed, and since we use monosigned literals, also the AMO clauses
   {-a(v,e), -a(v,e')} are needed. </li>
  </ul>


  \todo Translation to SAT according to Liffiton and Sakallah
  <ul>
   <li> We need the usual accompanying statistics-functions. </li>
   <li> See the Guangzhou-2008-talk for more example applications. </li>
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
   <li> The solver of Liffiton/Sakallah uses AtMost-constraints to restrict
   the number of clauses not affected by the autarky:
    <ol>
     <li> This avoids the problem with the long clause enforcing a non-trivial
     autarky. </li>
     <li> We also need soon a framework where such "constraints" can be used;
     of course, that's at the heart of "generalised SAT". </li>
     <li> This is also useful for the above basic translation. </li>
     <li> Liffiton/Sakallah start searching for as many satisfied clauses
     as possible --- for hard instances the opposite direction should be
     favourable (given that smaller autarkies exist!). </li>
    </ol>
   </li>
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
  </ul>

*/

