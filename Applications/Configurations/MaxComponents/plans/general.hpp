// Oliver Kullmann, 11.11.2007 (Swansea)
/* Copyright 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Configurations/MaxComponents/plans/general.hpp
  \brief Plans on the "Enigma1277" ("Faron's Puzzle") and generalisations


  \todo Update namespace.


  \todo Create milestones.


  \todo Empirical study
  <ul>
   <li> Under Experimentation/ExperimentSystem we need a program which allows
   to run a collection of solvers of a collection on benchmarks, storing
   the results in some appropriate directory structure. </li>
   <li> Solvers to investigate:
    <ol>
     <li> OKsolver2002 in all variations </li>
     <li> The march solvers </li>
     <li> Ubcsat solvers </li>
     <li> Minisat </li>
    </ol>
   </li>
   <li> Parameters to investigate:
    <ol>
     <li> Number of solutions, number of essentially different solutions
     (at least for the maximal score) </li>
     <li> Tree-resolution hardness </li>
     <li> Structure of autarky monoid </li>
    </ol>
   </li>
  </ul>


  \todo Alternative formulations
  <ul>
   <li> Quantified Boolean Formulas ? </li>
   <li> Obviously we can use non-boolean variables; also more powerful
   types of clauses? </li>
   <li> First-order logic ? </li>
   <li> Is there an alternative propositional translation? Using different
   auxiliary variables? </li>
   <li> Active clauses and active clause-sets? </li>
  </ul>


  \todo Special algorithms
  <ul>
   <li> The brute-force search through all 2^(d^2) colourings should be
   implemented:
    <ol>
     <li> Input a graph. </li>
     <li> Run through all 0-1-assignments to the vertices, eliminate all
     edges having different colours, and determine the number of connected
     components. </li>
     <li> Report for each score how often it has been achieved. </li>
    </ol>
   </li>
  </ul>

*/

