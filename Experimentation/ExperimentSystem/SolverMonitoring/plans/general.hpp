// Oliver Kullmann, 4.8.2009 (Swansea)
/* Copyright 2009, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ExperimentSystem/SolverMonitoring/plans/general.hpp
  \brief General plans regarding monitoring solvers


  \todo Improve docus
  <ul>
   <li> The docus at ExperimentSystem/SolverMonitoring/docus/general.hpp
   must be improved. </li>
   <li> Examples of how to use the experiment scripts and their outputs are
   needed. </li>
   <li> Full information on all solver data is needed. </li>
   <li> Information on the methodology and best practice when using these
   tools is needed, or at least links to other docus which provide this
   information. </li>
  </ul>


  \todo Running experiments
  <ul>
   <li> Currently we have RunMinisat. </li>
   <li> Running experiments from a directory by "ExpRunMinisat":
    <ul>
     <li> Often we have a directory full of instances which we wish
     to run a solver on in a specific order. </li>
     <li> The order we wish to run the solver in is usually dependent on a
     parameter, present in the filename of the instance. </li>
     <li> However, a better solution, which by itself documents which instances
     were considered in each order, and which is also simpler, just uses a file
     with one instance per line. </li>
     <li> We should provide a mechanism for running the solver on instances
     given in by such a file, and collecting the statistics. </li>
     <li> We have several possibilities:
      <ul>
       <li> Manually running the solvers on each instance, and providing
       a script which collects Statistics from a number of Experiment
       directories into a single Statistics file. </li>
       <li> Adding a "--dir" option to RunMinisat etc. which processes
       a directory of instances, and creates a directory with all of
       the RunMinisat directories inside it, plus an overall Statistics
       file. </li>
       <li> Best is to provide a dedicated script, ExpRunMinisat, which
       creates, as usual, an experiment directory, copies and documents its
       arguments, which are typically configuration-files, and then runs the
       computations. </li>
       <li> Important that this script can be (just) restarted. </li>
       <li> One needs to see whether the script itself collects the statistics
       from the various experiment-directories, or whether that is done by
       an external tool. </li>
      </ul>
     </li>
     <li> DONE (this is a bad hack)
     In each case we need standard tools to extract parameters
     from filenames; see also "Extraction tools". </li>
    </ul>
   </li>
   <li> Statistics:
    <ul>
     <li> These scripts should also (at least) collect full data on the true
     number of variables, clauses and literals present in the instance, adding
     this data to the Statistics file as the n, c and l columns. </li>
     <li> Computation as with the OKsolver_2002, that is, tautological clauses
     and repeated literals are removed, and then c is the number of remaining
     clauses, l the number of remaining literal occurrences, and n the number
     of remaining variables. </li>
     <li> We likely also want to collect more statistics on
     these instances, for example, min/max/avg clause-length, deficiency and so
     on. </li>
    </ul>
   </li>
   <li> We need scripts like that for all solvers. </li>
  </ul>


  \todo Better summary statistics
  <ul>
   <li> It it important that functions like read_processsplit_minisat (in
   CubeAndConquer.R) automatically output good summary statistics (since
   typically only these numbers will be used). </li>
   <li> Currently we use "summary". </li>
   <li> We should also use "sd", possibly also "mean" (already provided in
   summary, but now with a different formatting, better suited for sd). </li>
   <li> Library "pastecs" offers "stat.desc", which has more on the side of
   variance. </li>
   <li> Library "Hmisc" offers "describe", which has more quantiles, and a
   bit of other data. </li>
   <li> We need to provide our own summary-functions, tailored for the special
   situations. </li>
   <li> For a start, see the approach in CubeAndConquer.R:
    <ol>
     <li> The general functions there, once approved, need to move to more
     general places. </li>
    </ol>
   </li>
  </ul>


  \todo Standardising names and order of solver data columns
  <ul>
   <li> We need a global order on the columns of solver data
   extracted using the tools in "Extraction tools". </li>
   <li> Standardised column names:
    <ol>
     <li> rn : integer, number of variables. </li>
     <li> rc : integer, number of clauses. </li>
     <li> rl : integer, number of literal occurrences. </li>
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
     <li> The above attributes are always as reported by the solver. </li>
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
   <li> Additional statistics computed by experiment running scripts:
    <ul>
     <li> n : integer, number of variables. </li>
     <li> c : integer, number of clauses. </li>
     <li> l : integer, number of literal occurrences. </li>
     <li> Such measures (n, c and l) always refer to the measure
     after removal of tautological clauses and repeated literals. </li>
     <li> These statistics are discussed in "Running experiments". </li>
    </ul>
   </li>
   <li> Columns listed in this todo should be kept up to date with those
   documented under "Column naming conventions" in
   SolverMonitoring/docus/general.hpp. </li>
   <li> The following columns need standardised names and a standardised
   order:
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
     <li> All solver data must be added to this list a full global order
     on the solver data determined. </li>
    </ul>
   </li>
  </ul>


  \todo Understanding solver output
  <ul>
   <li> Most solvers output a lot of data; what this output means
   is often not documented. </li>
   <li> We want to document all data we extract, see "Extraction tools"
   below and "Column naming conventions" in
   SolverMonitoring/docus/general.hpp. </li>
   <li> For solver output which is unclear, we should contact the authors
   and confirm the meaning. </li>
   <li> The following data items in solver output need to be explained:
    <ul>
     <li> satz215 output:
     \verbatim
**** The instance is unsatisfiable. *****
NB_MONO= 75, NB_UNIT= 476, NB_BRANCHE= 79, NB_BACK= 40
Program terminated in 0.000 seconds.
satz215 PHP_weak_6_5.cnf 0.000 79 40 1916 80 0 30 81 0 0 0

> cat ExternalSources/builds/SAT/Satz/satz215/satz215.2.c
<snip>
  printf("satz215 %s %5.3f %ld %ld %ld %ld %d %d %d %d %ld %ld\n",
          saved_input_file, ((double)(endtime-begintime)/CLOCKS_PER_SEC),
          NB_BRANCHE, NB_BACK,  NB_SEARCH, NB_FIXED,
          satisfiable(), NB_VAR, INIT_NB_CLAUSE, NB_CLAUSE-INIT_NB_CLAUSE,
          NB_SECOND_SEARCH, NB_SECOND_FIXED);
     \endverbatim
     What is:
      <ul>
       <li> NB_MONO (single nodes??)? </li>
       <li> NB_BRANCHE (nodes + unit props??)? </li>
       <li> NB_BACK (nodes??)? </li>
       <li> NB_SEARCH? </li>
       <li> NB_FIXED? </li>
       <li> NB_VAR? </li>
       <li> NB_SECOND_SEARCH? </li>
       <li> NB_SECOND_FIXED? </li>
      </ul>
     </li>
     <li> Other solver output needs to be considered and added to this todo.
     </li>
    </ul>
   </li>
  </ul>


  \todo Extraction tools
  <ul>
   <li> The most fundamental tool for a solver is a script, which takes the
   solver output from standard input, and puts it into a single line (just
   data) on standard output, usable for R. </li>
   <li> First we implement this as an awk script (with documentation of all
   attributes, in the docus):
   <ul>
    <li> ExtractGrasp.awk </li>
    <li> ExtractPicosat.awk </li>
    <li> ExtractArgosat.awk </li>
    <li> ExtractOKsolver.awk </li>
    <li> ExtractGlucose.awk </li>
    <li> ExtractPrecosat236.awk </li>
    <li> ExtractPrecosat570.awk </li>
    <li> ExtractSatz </li>
    <li> ExtractMinisat.awk : DONE </li>
    <li> ExtractMarchpl : DONE </li>
   </ul>
   </li>
   <li> DONE (see ExtractMinisat for the role model)
   This is without parameters; with parameter "header" (as string) it
   just prints the header (which is also at the top of the R-output).
    <ol>
     <li> How to handle this data-dependency? </li>
     <li> Perhaps, at least at the beginning, we just state in comments that
     the header must be the same as used in the awk-script, but don't provide
     some mechanism for avoiding the duplication. </li>
     <li> The string uses a single space for separation, and no leading or
     trailing space(-symbols). </li>
     <li> The header-string is provided in a data-file, e.g., "Minisat.header".
     Or one could use a directory "headers", containing e.g. "minisat" etc.
     </li>
     <li> In case something else than "header" is provided to the script, and
     which can not be interpreted, an error occurs. </li>
     <li> Best we have possible parameter-values "header-only"
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
     <li> rn : integer, number of variables as reported by the solver. </li>
     <li> c : integer, number of clauses. </li>
     <li> rc : integer, number of clauses, as reported by the solver. </li>
     <li> l : integer, number of literal occurrences. </li>
     <li> rl : integer, number of literal occurrences, as reported by the
     solver. </li>
     <li> Such general measures (n, c and l) always refer to the original
     input (not after preprocessing). </li>
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
  <ul>
   <li> This is about analysing the "verbose" output. </li>
   <li> For OKsolver_2002 we have some functions in OKsolver.R. </li>
  </ul>

*/
