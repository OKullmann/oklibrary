// Matthew Gwynne, 22.8.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ExperimentSystem/SolverMonitoring/docus/general.hpp
  \brief Docus on monitoring solvers


  <h1> %Tools for extracting and monitoring </h1>

  %Tools for extracting and monitoring the running of solvers on experiments.


  <h2> Overview </h2>

  <ul>
   <li> We provide tools for running solvers and extracting R-compatible data
   tables from solver output. </li>
   <li> This allows one to run experiments over many instances,
   extracting the solver data from each solver run into a statistics
   file. </li>
   <li> This statistics file can then load this data directly into R for
   analysis using <code>read.table</code> </li>
   <li> The extraction tools:
    <ul>
     <li> ExtractMinisat </li>
    </ul>
   </li>
   <li> See "Column naming conventions" for a list of naming conventions
   for the data columns. </li>
   <li> See SolverMonitoring/headers/solver_name for the list of columns
   output for solver_name. </li>
  </ul>


  <h2> Column naming conventions </h2>

  <ul>
   <li> The extraction tools use the following abbreviations
   for column names, relating to the extract solver data:
    <ul>
     <li> n : integer, number of variables. </li>
     <li> c : integer, number of clauses. </li>
     <li> l : integer, number of literal occurrences. </li>
     <li> Such general measures always refer to the original input (not
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
   <li> The above attributes always occur first in the list of columns,
   in the that order, although depending on the solver, not all columns
   are output. </li>
   <li> The following abbreviations are used for more solver-specific
   data columns:
    <ul>
     <li> n2cr : number of 2-clauses after reduction. </li>
     <li> n2cs : number of new 2-clauses. </li>
     <li> m2cs : maximal number of added 2-clauses. </li>
     <li> dmcl : difference in maximal clause-length after preprocessing. </li>
     <li> dn : difference in number of variables after preprocessing. </li>
     <li> dc : difference in number of clauses after preprocessing. </li>
     <li> dc : difference in number of literal after preprocessing. </li>
     <li> snds : number of single nodes. </li>
     <li> qnds : number of quasi-single nodes. </li>
     <li> mnds : number of missed single modes. </li>
     <li> tel : number of table-enlargements. </li>
     <li> oats : number of 1-autarkies. </li>
    </ul>
   </li>
   <li> For these solver-specific attributes, they occur in the table
   in the order they are output by the solver. </li>
  </ul>

*/
