// Oliver Kullmann, 4.8.2009 (Swansea)
/* Copyright 2009, 2011, 2012, 2013, 2017 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ExperimentSystem/SolverMonitoring/plans/general.hpp
  \brief General plans regarding monitoring solvers


  \todo Handling changing solver output
  <ul>
   <li> A solver's output can be different depending on the options
   given to it. </li>
   <li> For instance, the OKsolver_2002 will output the additional attributes,
   "splitting directory" and "splitting cases", if one passes in the
   "-S" option, but not otherwise. This is discussed in
   "Handling splitting output" in
   ExperimentSystem/SolverMonitoring/plans/OKsolver.hpp.
   OK: as discussed there, no such case distinctions should be performed. </li>
   <li> The question is how extraction scripts, as described in
   "Extraction tools" below, should handle different outputs from
   the same solver. OK: use NA-values. </li>
   <li> The two most natural options are:
    <ul>
     <li> Extraction scripts take solver arguments: OK: as explained above,
     this should not be done.
      <ul>
       <li> Extraction scripts take the solver options as arguments and
       extract different attributes depending on these arguments. </li>
       <li> The advantage with this method is that the output of
       the extraction script never has any missing data. </li>
       <li> The disadvantage is that the system becomes more
       complicated, and one must remember what options were
       passed to the solver. </li>
      </ul>
     </li>
     <li> Default values for missing data:
      <ul>
       <li> Extraction scripts always output a column for all possible
       data that the solver outputs. When the solver doesn't output certain
       data, the extraction script outputs a default value. </li>
       <li> For the default value, we have two choices: use a default value
       which could be output by the solver (e.g., 0, or whatever is sensible
       for the attribute in question), or use special NA values (from R). </li>
       <li> Using "zero" values has the advantage that a table with these
       values can have statistics calculated, without additional measures
       taken. </li>
       <li> Using NA values has the advantage that it is always clear that a
       value is missing, and it isn't mistaken for "real" data. </li>
       <li> Obviously the NA-value is the *only* possibility, since the data
       is statistical data, and we obviously follow the practice of statistics
       in their own field. And values 0 would just be false in general. </li>
      </ul>
     </li>
    </ul>
   </li>
   <li> In general, it seems best to some form of default values for missing
   data, avoiding the additional complexity of script arguments. </li>
   <li> DONE (if the field is not output by the solver, then obviously it
   gets the NA-value --- never 0; all initialisations must always use NA,
   and in this way automatically we get NA in case the solver didn't give
   data)
   The use of "zero" or NA values should be a matter decided on an
   attribute by attribute basis. </li>
   <li> DONE (there should be no such cases)
   The use of additional options for the extraction scripts should
   be reserved for special cases, where the additional complexity seems
   warranted. </li>
  </ul>


  \todo Testing solver extraction scripts
  <ul>
   <li> We need tests to ensure that the extraction scripts collect the
   statistics correctly. </li>
   <li> Testing solvers behave as we expect:
    <ul>
     <li> Solvers change from version to version, and depending on this update,
     there are three possible scenarios:
      <ul>
       <li> The solver has changed very little, and works and outputs basically
       the same thing, and so the extraction scripts and aspects of the library
       don't need to change. </li>
       <li> The solver has changed its output format slightly, or some minor
       aspect of its inner workings, and so we wish to update extraction
       scripts to reflect this. </li>
       <li> The solver is very different, and should be considered a different
       solver entirely, with new extraction scripts. </li>
      </ul>
     </li>
     <li> We should provide a method of testing that a solver outputs exactly
     the format that we expect:
      <ul>
       <li> What data is output? </li>
       <li> Is this data as we expect, given our knowledge of the current
       behaviour of the solver? </li>
       <li> Is the data still output in the same format? </li>
      </ul>
     </li>
     <li> Such a test allows us to then detect small changes in a solver
     which might break extraction scripts and other tools, but are small
     enough to go unnoticed; perhaps an attribute we rarely look at changes.
     </li>
     <li> This test would also make it easier to write extraction tools for
     new versions of solvers with large differences, as it acts as a form
     of documentation of the features and output format we expect of the
     previous version. </li>
    </ul>
   </li>
  </ul>


  \todo Running experiments
  <ul>
   <li> Compare "run_ubcsat as shell script" in
   ControllingLocalSearch/plans/DataCollection.hpp for a similar system;
   one should create a general naming scheme. </li>
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
     given by such a file, and collecting the statistics. </li>
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


  \todo Extraction tools
  <ul>
   <li> The most fundamental tool for a solver is a script, which takes the
   solver output from standard input, and puts it into a single line (just
   data) on standard output, usable for R. </li>
   <li> First we implement this as an awk script (with documentation of all
   attributes, in the docus):
   <ul>
    <li> ExtractGrasp.awk </li>
    <li> ExtractArgosat.awk </li>
    <li> ExtractCryptominisat.awk: DONE </li>
    <li> ExtractPicosat.awk : DONE </li>
    <li> ExtractMinisat.awk : DONE </li>
    <li> ExtractOKsolver.awk : DONE </li>
    <li> ExtractGlucose.awk : DONE </li>
    <li> ExtractPrecosat236.awk </li>
    <li> ExtractPrecosat570.awk : DONE </li>
    <li> ExtractSatz.awk : DONE </li>
    <li> ExtractMarchpl.awk : DONE </li>
   </ul>
   </li>
   <li> We should provide tests for all of these scripts, as discussed
   in "Testing solver extraction scripts" above. </li>
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
   <li> DONE (actually, we don't need such a script since it is the task of the
   experiment-processing script to collect all data into statistics-files, at
   the time when the data is created)
   And with parameter "dir=Directory" it reads all the files in Directory,
   and prints to standard output the header followed by the data-lines. </li>
   <li> DONE (no need anymore, see above)
   Then an additional column with the filename is added:
    <ol>
     <li> No path information, just the basename. </li>
     <li> Having the possibility for extracting information from the filename
     is also needed. </li>
     <li> This information can be given as a string which specifies a
     sed-script (for transforming the R-output). </li>
     <li> Parameter "extraction=sed-string". </li>
     <li> If "header" is provided to the script, then the user must also
     provide the header for the additional parameter. </li>
     <li> Parameter "extraction_header=string". </li>
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
     <li> ExtractGlucose (see
     ExperimentSystem/SolverMonitoring/plans/Glucose.hpp). </li>
     <li> ExtractSatz </li>
     <li> ExtractMarchpl (not yet implemented) </li>
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
     <li> mcl : integer, maximal clause length. </li>
     <li> rmcl : integer, maximal clause length, as reported by the
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
     <li> nsol : integer, number of solutions. </li>
     <li> mem : double, in MB. </li>
     <li> ptime : double, parse time (in seconds). </li>
     <li> file : string. </li>
     <li> bnds : double, number of backtracking-nodes (exactly two successor).
     </li>
     <li> snds : double, number of single-nodes (exactly one successor). </li>
     <li> lvs : double, number of leaves (no successor). </li>
     <li> r2la : double, number of r_2-look-aheads (for r_2-reductions). </li>
     <li> r3 : double, number of r_2-reductions. </li>
     <li> r3la : double, number of r_3-look-aheads (for r_3-reductions). </li>
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
   cases, appropriate 0-values have to be entered. This is discussed in
   "Handling changing solver output". </li>
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
