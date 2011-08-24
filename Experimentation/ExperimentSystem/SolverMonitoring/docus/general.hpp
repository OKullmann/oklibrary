// Matthew Gwynne, 24.8.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ExperimentSystem/SolverMonitoring/docus/general.hpp
  \brief Docus on monitoring solvers


  <h1> %Tools for extracting and monitoring </h1>

  %Tools for extracting data from and monitoring the running of solvers on
  experiments.


  <h2> Overview </h2>

  <ul>
   <li> We provide tools for running solvers and extracting R-compatible data
   tables from solver output. </li>
   <li> This allows one to run experiments over many instances,
   extracting the solver data from each solver-run into a statistics
   file. </li>
   <li> The extraction tools:
    <ul>
     <li> ExtractMinisat. </li>
    </ul>
   </li>
   <li> A list of the data columns extracted from each solver is
   provided under "Solver data" below. </li>
   <li> A list of naming conventions for the data columns extracted is
   available under "Column naming conventions" below. </li>
  </ul>


  <h2> Solver data </h2>

  <ul>
   <li> The following solvers output the following data columns. </li>
   <li> The explanations for the abbreviations, as well as the datatypes
   for the columns listed here are provided in "Column naming conventions". </li>
   <li> minisat-2.2.0:
    <ul>
     <li> List of data columns extracted:
     \verbatim
> cat Experimentation/ExperimentSystem/SolverMonitoring/headers/minisat; echo
rn rc t sat cfs dec rts r1 mem ptime stime cfl
     \endverbatim
     </li>
    </ul>
   </li>
  </ul>


  <h2> Column naming conventions </h2>

  <ul>
   <li> The extraction tools use the following abbreviations
   for column names:
    <ul>
     <li> n : integer, initial number of variables. </li>
     <li> rn : integer, number of variables as reported by the solver. </li>
     <li> c : integer, number of clauses after the removal of tautological
     clauses. </li>
     <li> rc : integer, number of clauses, as reported by the solver. </li>
     <li> l : integer, number of literal occurrences after the removal
     of tautological clauses and repeated literals. </li>
     <li> rl : integer, number of literal occurrences, as reported by the
     solver. </li>
     <li> Such general measures (n, c and l) always refer to the original
     input after removal of tautological clauses and repeated literals (not
     after preprocessing). </li>
     <li> t : double, solution time (in seconds). </li>
     <li> sat : in {0,1,2} for UNSAT, SAT, UNKNOWN. </li>
     <li> nds : double, number of nodes for look-ahead solvers. </li>
     <li> cfs : double, number of conflicts for conflict-driven solvers. </li>
     <li> dec : double, number of decisions for conflict-driven solvers. </li>
     <li> rts : double, number of restarts. </li>
     <li> r1 : double, number of unit-clause propagations. </li>
     <li> r2 : double, number of failed-literal reductions. </li>
     <li> pls : double, number of pure literals. </li>
     <li> ats : double, number of autarkies (not pure literals). </li>
     <li> h : integer, height of search-tree for look-ahead solvers. </li>
     <li> mem : double, in MB. </li>
     <li> ptime : double, parse time (in seconds). </li>
     <li> file : string. </li>
    </ul>
   </li>
   <li> All attributes always occur in extracted output in the order given
   above. </li>
   <li> Not all columns are output by all solvers; which columns occur in
   output depends on which solver's output is being extracted. </li>
   <li> A list of which columns are output by which solvers is provided
   under "Solver data". </li>
  </ul>

*/
