// Oliver Kullmann, 23.8.2007 (Swansea)
/* Copyright 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/plans/general.hpp
  \brief Plans for the experimentation support in general


  \todo Update namespaces.


  \todo General plans on evaluating solvers, algorithms, heuristics
  <ul>
   <li> Given a set S of solvers and a set G of groups of benchmarks, the task
   is to rank the solvers. </li>
   <li> The solvers can be completely different, or they could only differ
   in some heuristical aspect. </li>
   <li> One possibility is to run a SAT competition; see
   Experimentation/Competition/plans/general.hpp.
    <ol>
     <li> This can be done on the exact benchmarks as in an original
     competition. </li>
     <li> Or on any other benchmark set. </li>
     <li> To eliminate the dependency on magic numbers involved in the
     evaluation, a sensitivity analysis needs to be performed. </li>
    </ol>
   </li>
   <li> Another possibility is to run a tournament, where everybody competes
   against everybody else.
    <ol>
     <li> In [Hooker, Vinay, 1995, Branching Rules for Satisfiability]
     competition on groups is used, and determination of a winner happens
     by the Wilcoxon signed rank test. (Instead of fixing the
     "95% significance level", perhaps one should report the dependency of the
     outcome (A wins, B wins, or a draw) on the significance level.) </li>
     <li> See [Ouyang, 1999, Implementations of the DPLL algorithm] for
     further statistical tests to compare two solvers. </li>
     <li> Easier to let the solver compete on single instances. </li>
     <li> Given that the single games are determined, the question then
     is how to derive a global ranking. </li>
    </ol>
   </li>
   <li> Analysis of dependencies
    <ol>
     <li> In [Hooker, Vinay, 1995] the possibility of using ANOVA is dismissed.
     </li>
     <li> Instead a linear regression model is proposed. (But apparently not
     really used?) </li>
    </ol>
   </li>
   <li> OK has some initial sketches on these problems. </li>
  </ul>
  
*/

