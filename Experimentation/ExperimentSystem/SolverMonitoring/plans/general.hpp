// Oliver Kullmann, 4.8.2009 (Swansea)
/* Copyright 2009, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ExperimentSystem/SolverMonitoring/plans/general.hpp
  \brief General plans regarding monitoring solvers


  \todo Create milestones


  \todo Extraction tools
  <ul>
   <li> The most fundamental tool for a solver is a script, which takes the
   solver output from standard input, and puts it into a single line (just
   data) on standard output, usable for R. </li>
   <li> First we implement this as an awk script. See
   <ul>
    <li> ExtractMinisat.awk. </li>
    <li> ExtractOKsolver.awk. </li>
    <li> ExtractGlucose.awk. </li>
    <li> ExtractPrecosat236.awk. </li>
    <li> ExtractPrecosat570.awk. </li>
    <li> ExtractSatz. </li>
   </ul>
   </li>
   <li> This is without parameters; with parameter "header" (as string) it
   just prints the header (which is also at the top of the R-output).
    <ol>
     <li> How to handle this data-dependency? </li>
     <li> Perhaps, at least at the beginning, we just state in comments that
     the header must be the same as used in the ask-script, but don't provide
     some mechanism for avoiding the duplication. </li>
     <li> The string uses a single space for separation, and not leading or
     trailing space(-symbolds). </li>
     <li> The header-string is provided in a data-file, e.g., "Minisat.header".
     Or one could use a directory "headers", containing e.g. "minisat" etc.
     </li>
     <li> In case something else than "header" is provided to the script, and
     which can not be interpreted, an error occurs. </li>
     <li> Perhaps we should have possible parameter-values "header-only"
     and "data-only" (while without anything both header and data are
     printed). </li>
    </ol>
   </li>
   <li> And with parameter "dir=Directory" it reads all the files in Directory,
   and prints to standard output the header followed by the data-lines. </li>
   <li> Then an additional column with the filename is added:
    <ol>
     <li> No path information, just the basename. </li>
     <li> Having the possibility for extracting information from the filename
     is also needed. </li>
     <li> This information can be given as a string which specifies an
     sed-script (for transforming a script). </li>
     <li> Parameter "extraction=sed-string". </li>
    </ol>
   </li>
   <li> Then we do not need wrappers. This is better since it avoids possible
   complications. </li>
   <li> Remark: with the Ubcsat-wrapper it's different, since there we only
   need to supply default parameter (which can also be overwritten if needed).
   </li>
   <li> For each solver-class we have such a script:
    <ol>
     <li> ExtractMinisat (see
     ExperimentSystem/SolverMonitoring/plans/Minisat2.hpp). </li>
     <li> ExtractPicosat (see
     ExperimentSystem/SolverMonitoring/plans/Picosat.hpp). </li>
     <li> ExtractOKsolver (see
     ExperimentSystem/SolverMonitoring/plans/OKsolver.hpp). </li>
     <li> ExtractSatz </li>
     <li> ExtractMarchpl </li>
     <li> To be completed </li>
    </ol>
   </li>
   <li> Standardised column names:
    <ol>
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
     <li> There can be more attributes; the above ones always occur in that
     order. </li>
     <li> DONE (no need to make incomparable data comparable)
     For handling parameters that aren't produced by certain solvers,
     for example nds by minisat-2.2.0, there are two options:
     <ol>
      <li> Output "NA" for that column. </li>
      <li> Don't output an nds column. This has the disadvantage that
      outputs from different solvers are harder to compare. </li>
     </ol>
     For now the awk scripts (see above) don't output the column.
     </li>
    </ol>
   </li>
   <li> Some solvers do not always output their full statistics. In such
   cases, appropriate 0-values have to be entered. </li>
   <li> DONE (solver-outputs in general are not comparable, only similar
   families of solvers should be treated similarly)
   Not all solver outputs are comparable.
   <ul>
    <li> DONE (different solvers have different interpretations, that's it;
    if we want additional input statistics, then it can be easily provided
    for each investigation)
    Variable numbers:
     <ul>
      <li> Consider the Dimacs %file:
      \verbatim
shell> cat test.cnf
p cnf 100 1
100 0
      \endverbatim
      </li>
      <li> OKsolver_2002 returns the true number of variables:
      \verbatim
shell> OKsolver_2002-O3-DNDEBUG test.cnf
c initial_number_of_variables           1
      \endverbatim
      </li>
      <li> minisat-2.2.0 returns the upper bound on the variable index:
      \verbatim
shell> minisat-2.2.0 test.cnf
|  Number of variables:           100                                         |
      \endverbatim
      </li>
      <li> DONE (no need)
      We should output both of these outputs (true number and
      maximum variable index). </li>
     </ul>
    </li>
   </ul>
   </li>
   <li> With such tools in place, the current R-functions should be transformed
   to functions for reading and evaluating files created with the tools. </li>
   <li> We need also a simple system to connect a solver with its extraction
   tool:
    <ol>
     <li> So that in an application, where the solver is run, one can also
     easily do the extraction. </li>
     <li> Perhaps just a helper-shell-script "ExtractInfo", which takes the
     solver-name as input, and returns the name of the extraction-script. </li>
    </ol>
   </li>
  </ul>


  \todo Add monitoring for all other solvers


  \todo DONE Inappropriate column names
  <ul>
   <li> DONE Column names must be short, never like
   "decisions_per_sec" or "bin_clauses_after" etc.:
    <ol>
     <li> DONE (shortened names and ensured consistent use of "_c", "_n",
     and "_l")
     Most of the names regarding the OKsolver_2002 are badly chosen. </li>
     <li> DONE (this was from the OKsolver output)
     For example "max_tree_depth": Why "max"? And to what could a depth
     refer than to a tree? </li>
     <li> DONE Instead of "clauses" we use "c". Etc. </li>
     <li> DONE (renamed "max_added_2cl" to "max_added_2c")
     However "l" in "cl" refers to "clause-length"! </li>
    </ol>
   </li>
   <li> DONE
   Once the new names are in places (for all such functions), then also
   specification must be provided (not just some example). </li>
   <li> DONE (removed all uses of the words variables, clauses and literals)
   We also use standardised notations where possible:
    <ol>
     <li> "n" for the number of variables </li>
     <li> "c" for the number of clauses </li>
    </ol>
   </li>
   <li> DONE (added type-casts for all fields)
   And there is also a type mismatch: we get warnings
   "1: inaccurate integer conversion in coercion". </li>
  </ul>


  \bug DONE (types of columns specified)
  False treatment of "decisions"
  <ul>
   <li> Column "decisions" is treated as factor! This is obviously false.
   </li>
   <li> The cause of the problem is that the dataframe was never specified,
   but just some "data put into it". </li>
  </ul>

*/

