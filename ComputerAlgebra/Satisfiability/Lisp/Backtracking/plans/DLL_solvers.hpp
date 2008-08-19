// Oliver Kullmann, 7.8.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/Backtracking/plans/DLL_solvers.hpp
  \brief Plans for DLL-solvers


  \todo Organisation
  <ul>
   <li> Split DLL_solvers.mac into several submodules. </li>
  </ul>


  \todo Heuristics, distances etc.
  <ul>
   <li> Likely these things should go into a dedicated module. </li>
   <li> We should also implement the well-known branching rules from
   the literature as heuristics (but in the documentation we should
   somehow emphasise their "historical status").
    <ol>
     <li> Jeroslow-Wang ([Jeroslow, Wang, 1990, Solving propositional
     satisfiability problems]) </li>
     <li> 2-sided Jeroslow-Wang ([Hooker, Vinay, 1995, Branching rules for
     satisfiability]) </li>
     <li> dsj ([Van Gelder, Tsuji, 1996, Satisfiability Testing with More
     Reasoning and Less Guessing]) </li>
     <li> B ([Ouyang, 1999, Implementations of the DPLL algorithm]) </li>
     <li> MAR ([van Maaren, Warners, 2000, Solving satisfiability problems
     using elliptic approximations --- Effective branching rules]);
     based on using var_int_scom_fcs
     (ComputerAlgebra/Satisfiability/Lisp/ClauseSets/Hypergraphs.mac).
     Additionally here we need appropriate weights for clause-lengths. </li>
    </ol>
   </li>
  </ul>


  \todo Concepts for heuristics
  <ul>
   <li> Instead of passing arguments, one could use "inheritance",
   i.e., dynamic binding, for heuristic-functions? </li>
  </ul>

*/

