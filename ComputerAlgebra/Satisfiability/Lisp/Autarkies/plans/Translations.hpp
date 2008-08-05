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

