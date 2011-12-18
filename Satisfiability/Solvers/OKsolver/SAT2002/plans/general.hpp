// Oliver Kullmann, 17.8.2007 (Swansea)
/* Copyright 2007, 2008, 2009, 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Solvers/OKsolver/SAT2002/plans/general.hpp
  \brief Plans on the maintenance of the code for the old OKsolver


  \todo More information on the current path
  <ul>
   <li> Currently we know the total number npa of assignments on the path
   ("Tiefe") and the number d of decisions ("Rekursionstiefe"). </li>
   <li> It should not be too complicated to also provide the following
   information for the current partial assignment (leading to the current
   node):
    <ol>
     <li> the number npl of pure literals </li>
     <li> the number naut of variable set by (non-trivial) autarkies </li>
     <li> the number nr1 of variables set by r_1-reduction </li>
     <li> the number nr2 of variables set by r_2-reduction. </li>
    </ol>
    We have then npa = d + npl + naut + nr1 + nr2. </li>
    <li> This information should be output to a data-file with the splitting
    option:
     <ol>
      <li> How to call the file? </li>
      <li> One could add a new option, which contains the filename and which
      triggers the output of the information. </li>
      <li> But better we should always output the information. </li>
      <li> How not another file in the Instances-directory. </li>
      <li> So perhaps a file "Data" in the current directory. </li>
     </ol>
    </li>
  </ul>


  \todo Splitting-output in iCNF format
  <ul>
   <li> See http://users.ics.tkk.fi/swiering/icnf/ . </li>
   <li> For a directory created by SplittingViaOKsolver, the script
   ExtractiCNF computes the desired .CNF-file. However here we want to
   compute it directly (which is much faster). </li>
   <li> This yields the script "iSplittingViaOKsolver". </li>
   <li> DONE (we collect the usual partial assignments)
   Simplest is just to put the partial assignments into one single
   file, each line starting with "a " (and concluding with " 0"). </li>
   <li> Then a small tool is needed which takes a CNF file F and a file
   containing the partial assignments, replacing the "v"s by "a"s, and
   concatenates the two files, replacing the "p cnf"-line in F with (just)
   "p inccnf". </li>
  </ul>


  \bug False counting of initial unit-clause eliminations
  <ul>
   <li> Counting of initial unit-clause propagations is not correct. </li>
   <li> For example using the examples provided by "Smusat_Horn-O3-DNDEBUG k"
   (see "Improve efficiency of UnitClausePropagation.cpp" in
   Satisfiability/Reductions/UnitClausePropagation/plans/general.hpp),
   the count seems always to be 2, while it should be k-1. </li>
   <li> Compare also "Correct computation of basic statistics" below. </li>
   <li> On the other hand, the count for "Musatd2 n 1", namely n-1, seems
   correct. </li>
  </ul>


  \todo Differences
  <ul>
   <li> With VanDerWaerden_2-3-13_159.cnf, VanDerWaerden_2-3-13_160.cnf in
   Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/3-k/general.hpp
   we get different results, compared to older records. </li>
   <li> We need to investigate this. </li>
   <li> Likely we have a different order of clauses now, and perhaps also
   a different order of literals in the clauses. </li>
   <li> Are the running-times now, where variables etc. need 64 bits (as
   pointers), really so much worse, or is it due to "overload" (w.r.t.
   caching) on csltok? </li>
   <li> Also the node-count is different : is this due to double-computations,
   or is there a systematic reason for the difference of 2 nodes? </li>
   <li> Interesting that the differences in the number of 2-reductions is
   considerable. </li>
  </ul>


  \todo Improve the Dimacs-output
  <ul>
   <li> DONE (used "reddiff" instead)
   Wording of information about reductions:
    <ol>
     <li> It should be "reduction_in_maximal_clause_length" instead of
     "reduced_maximal_clause_length". </li>
     <li> "reduction_in_number_of_variables" instead of
     "reduced_number_of_variables". </li>
     <li> "reduction_in_number_of_clauses" instead of
     "reduced_number_of_clauses". </li>
     <li> "reduction_in_number_of_literal_occurrences" instead of
     "reduced_number_of_literal_occurrences". </li>
    </ol>
   </li>
   <li> DONE (no option --- this would be unnecessary clutter)
   The default of output should be to have each measurement on its
   own line (with "c "):
    <ol>
     <li> With an option enabling the old form (all on one line). </li>
     <li> Likely we should also left-align all output numbers. </li>
    </ol>
   </li>
   <li> At this time also all other output-strings should be inspected, in
   all supported languages. </li>
   <li> Likely it would be useful to output the time and date. </li>
   <li> It would also be good if the output of data which doesn't change
   anymore (number of variables etc.) would only happen once, at the
   beginning; perhaps included also in the final output.
    <ol>
     <li> Compare "Output to files" below. </li>
    </ol>
   </li>
  </ul>


  \todo Introduce error-codes
  <ul>
   <li> For each error-situation there should be a dedicated return-code
   (different from 0,10,20). </li>
   <li> Check all returns in main, and all exit's elsewhere. </li>
  </ul>


  \todo Output to files
  <ul>
   <li> Monitoring output to file:
    <ol>
     <li> When using "-M" together with "-F", then the output of branching
     literals is activated, interspersed with the monitoring output (to
     stdout as well as to the file). </li>
     <li> However this works currently only when output of a satisfying
     assignment is activated, since only then the symbol table is created.
     </li>
     <li> This needs to be cleaned up; and this output should have its
     own switch. </li>
     <li> Another problem here is that if the output-file already exists,
     then apparently nothing happens. </li>
    </ol>
   </li>
   <li> Output to .res-file:
    <ol>
     <li> Here it would be good also to have the output created by the
     option "--version" (since this file collects various results). </li>
     <li> On the other hand, this information in principle is contained in
     the full name of this result-file? </li>
     <li> While for screen-output we don't want to see again and again the
     same numbers which don't change (like the number of variables etc.),
     likely for the file output however all this should be included. </li>
    </ol>
   </li>
  </ul>


  \todo OUTPUTTREEDATAXML
  <ul>
   <li> DONE
   We need to compile versions with this macro defined. </li>
   <li> DONE (seems better to stick with the compile-time option)
   Wouldn't it be better, if this wouldn't be a compile-time option,
   but simply a run-time option?
    <ol>
     <li> But there is some overhead, including the statistical computation.
     </li>
     <li> And of course there is the overhead for the output. </li>
    </ol>
   </li>
   <li> What to do with the generated .xml-files (for the tests)?
    <ol>
     <li>
     <li> DONE
     Likely best that the test-system removes them. </li>
     <li> DONE
     Perhaps just all .xml-files. </li>
     <li> DONE (for now just the test-directory)
     But perhaps this should happen in a new sub-directory of the
     test-directory. </li>
     <li> Likely best if the directory for running the OKsolver (and where then
     the output-files are placed) is located in system_directors/tests. </li>
     <li> Then either we continue to use relative path-names, since the
     test-output-templates must be independent from the repository,
     or (more complicated) these templates are preprocessed (writing into
     them the path-names). </li>
    </ol>
   </li>
   <li> We need to test these versions.
   </li>
   <li> DONE Output format:
    <ol>
     <li> DONE
     Yet the tree-data is output just on one line --- likely we should
     output every node on its own line. </li>
     <li> DONE (doesn't seem a problem)
     Then the files get a bit bigger. </li>
    </ol>
   </li>
   <li> DONE Document these versions.
    <ol>
     <li> Specify the DTD. </li>
     <li> Specify the data. </li>
    </ol>
   </li>
   <li> Yet only the number of r_2-reductions is output
   at each %node.
    <ol>
     <li> We need to strengthen this by outputting most data. </li>
     <li> Definitely the branching literal is needed. </li>
     <li> If we have 2 successors, then we need the distance values. </li>
     <li> For each node we should have the basic statistics: n and the
     list of clause-length, together with the counts. </li>
     <li> However, we might use options: For big trees we get quite big
     files, and then perhaps one prefers just minimal information. </li>
     <li> See also "Annotated tree output" in
     ComputerAlgebra/Satisfiability/Lisp/Backtracking/plans/OKsolver2002.hpp
     for the Maxima-level. </li>
    </ol>
   </li>
   <li> DONE (script Xml2Maxima does the translation)
   We need also Maxima-readable output:
    <ol>
     <li> The output should be the expression computed by the
     analogous Maxima program. </li>
     <li> The version OKsolver_2002_ast (see
     ComputerAlgebra/Satisfiability/Lisp/Backtracking/OKsolver2002.mac)
     already provide the splitting trees. </li>
     <li> The labels are already lists, so we can just add further
     node-information; however do we want to keep the partial assignments?
     </li>
     <li> So the general scheme for solvers producing amended splitting
     trees needs to be genealised, allowing to put various statistics
     into the nodes (additionally to or instead of the partial assignments).
     </li>
     <li> We really should avoid here another compile-time option.
      <ol>
       <li> So we need a run-time option. </li>
       <li> Or we just produce both versions (in different files). </li>
       <li> Or, alternatively, the OKsolver_2002 (C-version) only outputs the
       xml-information, and an external programs converts this into the
       Maxima format; this perhaps is best. </li>
       <li> We need then a C++ xml-library; would be great if Boost meanwhile
       had something? (Or the new standard?? But don't believe so.) </li>
       <li> We have some simple functionality in OKlib/General/XMLHandling.hpp.
       </li>
       <li> For transferring the random-formulas-experiment-xml-files to
       the database, we used the xerces library (see
       Experimentation/RandomGenerator/ExperimentUebertragung.cpp). </li>
      </ol>
     </li>
    </ol>
   </li>
  </ul>


  \todo Documentation problems
  <ul>
   <li> Problems with macros:
    <ol>
     <li> A lot of code (and doxygen-documentation) is conditionalised
     --- and thus doesn't show up in the doxygen-documentation! </li>
     <li> However we want the documentation of the whole code; how to
     achieve this? </li>
     <li> On the other hand, it it informative what is (and what is not)
     defined by default. </li>
    </ol>
   </li>
   <li> The files which are just symbolic links either should be excluded
   from the documentation, or, perhaps we can define the macros for them
   in the right way so that doxygen shows the real code applicable here. </li>
  </ul>


  \todo Language standards
  <ul>
   <li> The old OKsolver is a C99 program. </li>
   <li> Use standard include-guards. </li>
   <li> Why are there these comments that string.h is included because of C++?
   Seems we should remove them. </li>
   <li> For the xml-output we should then state that C is the programming
   language. </li>
   <li> Where we touch the code, we change the style to the standard
   (otherwise we leave it). </li>
   <li> All includes need to be changed to the library-style. Or? Perhaps, due
   to the exceptional "historical" character, we don't do this here?
   Emphasising, that there are no reusable components here?! </li>
   <li> Since the function specifier "inline" is available with C99, we should
   remove the little apparatus with "KEININLINE" and "__inline__" (and replace
   "__inline__" everywhere with "inline"). </li>
   <li> Deeper changes like const-introduction only later. </li>
   <li> DONE (stick to C) Perhaps move everything to C++ (but no real changes
   to any data structures, etc., only using C++ header files etc.). Or?? </li>
   <li> DONE (GesamtOKs.cpp removed) Why does GesamtOKs.cpp want to be a C++
   program --- maybe we just stick to C? Would be more honest! On the other
   hand, we are more knowledgeable with C++.) The purpose of GesamtOKs.cpp
   likely was to enable the use of C++ inlining --- but also C99 has the
   inline keyword. </li>
   <li>  DONE (stick to C) The program uses typical C-methods to simulate
   abstract data types (a functional interface is build, hiding all pointer
   access, which happens in the implementation files). So it appears that we
   better stick to C. </li>
  </ul>


  \todo Buildsystem (replacing Buildtools/UebersetzungOKs.plx)
  <ul>
   <li> That Perl-script had the following purpose:
    <ol>
     <li> Reflect the settings of macro, compilation-options and
     the choice of heuristics etc. in the name of the
     executable (with suffix ".sa"). </li>
     <li> Compile always a standard selection of combinations of
     macro-settings (for the different variants of the OKsolver). </li>
     <li> Create directories with a running number, containing
     the created executables, for ease of archiving. </li>
     <li> Take all compilation units, and insert them into one big
     stand-alone file "GesamtOKs.cpp". </li>
    </ol>
   </li>
   <li> We need to incorporate this functionality into our build system. </li>
   <li> Since we do not do any more experiments on variations of
   the old OKsolver, we do not need to create the sequence
   of directories anymore here. </li>
   <li> There are three programs:
    <ol>
     <li> OKsolver_2002.c : standard C program (assumes linking with the other
     compilation units) </li>
     <li> OKsolver_2002_lnk.c : Same as OKsolver_2002.c, but includes all other
     compilation units, and thus needs no linking. </li>
     <li> DONE (removed)
     GesamtOKs.cpp : Produced by UebersetzungOKs.plx out of all the compilation
     units (and is treated as C++). (However OK.h and Parameter.h are not
     inserted, but included.) </li>
    </ol>
   </li>
   <li> There is no need anymore for the created file GesamtOKs.cpp, however
   we need some tests on the running times of GesamtOKs, OKsolver_2002 and
   OKsolver_2002_lnk, and this also compared with the existing
   OKsolver-binaries. Optimally, we could do this simply with our build system
   (as planned), but perhaps for the time being we just do some ad-hoc testing.
   </li>
   <li> The difference between OKsolver_2002.c and OKsolver_2002_lnk.c is that
   the latter enables more inlining ---
   can't this be achieved otherwise (through inline-specifications)? </li>
   <li> The build system links the C programs as C++ programs; shouldn't make
   a big difference for now, but should be rectified with the new system. </li>
   <li> The build system doesn't know about the dependency of OKsolver_2002.c
   on the other .c-programs (to which it links) --- this needs to be addressed!
    <ol>
     <li> For each program P, one additional dependency file has to be created
     which contains the rule with target P, where the prerequisites are all
     the files P links together. </li>
     <li> See the plans under Buildsystem. </li>
    </ol>
   </li>
   <li> Compilation of lokalesLernen.c on its own should only happen with
   LOKALLERNEN defined (without it a compile-time error should ensue). </li>
   <li> Optimisation options:
    <ol>
     <li> As one can see under "OKsolver" in
     Experimentation/Investigations/plans/RamseyProblems.hpp, the executable
     on the 64-bit machines seems to run nearly 3 times slower than the
     executable on the 32-bit machine. </li>
     <li> We need to do some experimentation with the different compiler
     options. </li>
    </ol>
   </li>
  </ul>


  \todo Complete the help facilities of the OKsolver
  <ul>
   <li> There are quite a few options available, and these should be
   documented. </li>
  </ul>


  \todo Write docus-pages
  <ul>
   <li> Should explain the history and usage. </li>
   <li> Also the extension possibilities. </li>
  </ul>


  \todo Add doxygen-documentation
  <ul>
   <li> We want to leave the code mainly as it is, but we can add doxygen
   documentation. </li>
   <li> List of files (basic documentation):
    <ol>
     <li> OKsolver/SAT2002/VarLitKlm.h : DONE </li>
    </ol>
  </ul>


  \todo Eliminate old history in code.


  \todo Create systematic application tests
  <ul>
   <li> Differentiate between solver runs which are expected to succeed and
   runs which are expected to fail (so that in the latter case the output to
   stderr can be suppressed). </li>
   <li> All possible syntactic errors diagnosed by the OKsolver need be
   checked. </li>
   <li> Tautological clauses and repeated literals in clauses need to be
   checked. </li>
   <li> Output of satisfying assignments needs to be checked. </li>
   <li> We need also to check the other output formats. </li>
   <li> And monitoring output, to stdout and to file, needs to be checked.
   </li>
   <li> Finally, a general system for testing SAT solvers, with a general
   problem database, must be created. </li>
  </ul>


  \todo Add asserts throughout


  \todo Investigate unit-testing
  <ul>
   <li> Optimally, we write unit tests for all main components. </li>
   <li> Possibly, the code doesn't allow easy unit testing? </li>
  </ul>


  \todo Correct computation of basic statistics
  <ul>
   <li> Counting for example test_cases/TwoUnit.cnf is not correct. </li>
   <li> See whether this can be easily rectified. </li>
   <li> Possibly this is not the case; then we need a precise specification
   what actually is counted, so that then the output can be declared as
   correct. </li>
   <li> Table enlargements:
    <ol>
     <li> With the instance GreenTao_N_6-2-2-2-2-3-4_136.cnf (see
     Experimentation/Investigations/RamseyTheory/GreenTaoProblems/plans/GreenTao_te_m-3-4.hpp)
     we have only relatively few nodes, but get (actually the first time that
     I see it at all) several table enlargements? Is this correct? </li>
    </ol>
   </li>
  </ul>


  \todo Apply Valgrind
  <ul>
   <li> Consider all the possibilities Valgrind offers (regarding correctness
   checking; for optimisation see below). </li>
   <li> This might be an opportunity to add a bit to our own documentation /
   system. </li>
   <li> Also it should be run until completion on non-trivial examples. </li>
  </ul>


  \todo Apply code analysis tools (like Splint)
  <ul>
   <li> See "Installation of C/C++ tools" in
   Buildsystem/ExternalSources/SpecialBuilds/plans/general.hpp. </li>
  </ul>


  \todo Improve statistics code
  <ul>
   <li> Types of statistics variables:
    <ol>
     <li> Typedefs are needed for the two classes of statistics variables:
     \verbatim
extern long unsigned int Knoten, SingleKnoten, VerSingleKnoten,
  QuasiSingleKnoten, PureL, Autarkien, V1KlRed, FastAutarkien, InitEinerRed,
  neue2Klauseln, maxneue2K;
extern unsigned int Suchbaumtiefe, Ueberschreitung2, init2Klauseln;
     \endverbatim
     </li>
     <li> Perhaps via new macros then these typedefs are set. </li>
     <li> Default is that "long unsigned int" should become guaranteed 64 bits,
     while "unsigned int" should become guaranteed 32 bits. </li>
    </ol>
   </li>
   <li> Monitoring:
    <ol>
     <li> Likely also on 32-bit machines using 64-bit types for the statistics,
     enabling a maximal depth of up to 60, should be possible with negligible
     overhead (see above). </li>
    </ol>
   </li>
  </ul>


  \todo Declare variables as close to their first usage as possible
  <ul>
   <li> Since C99 also allows to use declaration everywhere, one
   should move many variable declarations to their first usage point. </li>
   <li> And, as with C++, one can now declare loop-variables inside the loop.
   </li>
  </ul>


  \todo Use const-qualification
  <ul>
   <li> Where possible, const-qualification should be applied. </li>
  </ul>


  \todo Use restrict-qualification
  <ul>
   <li> We should investigate at which places this pointer-qualification can
   be used. </li>
  </ul>


  \todo Elimination of compile-time options
  <ul>
   <li> LOKALLERNEN
    <ol>
     <li> Extract ideas. </li>
     <li> Eliminate option and code. </li>
     <li> With the generic version of OKsolver-2002, we can handle this
     much better. </li>
    </ol>
   </li>
   <li> FASTAUTARKIEN
    <ol>
     <li> Extract ideas. </li>
     <li> Eliminate option and code. </li>
     <li> Again, with the generic version of OKsolver-2002, we can handle this
     much better. </li>
    </ol>
   </li>
  </ul>


  \todo Output of clause-sets
  <ul>
   <li> We need the ability to output the following intermediate clause-sets:
    <ol>
     <li> The original clause-sets, but "cleaned-up". </li>
     <li> The original clause-set after preprocessing. </li>
     <li> The current residual clause-set. </li>
    </ol>
   </li>
  </ul>


  \todo Inefficiency on 64-bit platforms
  <ul>
   <li> It seems that the OKsolver runs by a factor of four(!) slower on
   64-bit platforms (compared to 32-bit platforms). </li>
   <li> See
   Experimentation/Investigations/RamseyTheory/GreenTaoProblems/plans/GreenTao_te_m-4-4.hpp
   for an example. </li>
   <li> First one needs precise measurements:
    <ol>
     <li> A collection of testcases is needed, including various cases
     from Ramsey-theory, as above,and random clause-sets. </li>
     <li> Are more cases needed? </li>
     <li> Then measurements need to be taken on various machines. </li>
    </ol>
   </li>
   <li> One would think that the main problem is that variables and literals
   are now 64-bit words. </li>
   <li> Perhaps the problem could be solved by fixing the main types to
   32-bit types. </li>
   <li> Some types, for examples for statistics, should be 64-bits: Perhaps
   we should simply fix for all types their precision. </li>
  </ul>


  \todo Extend statistics
  <ul>
   <li> Information on where in the tree the solver gets stuck:
    <ol>
     <li> A useful number should be the (current) minimal depth of a %node
     which has been solved (i.e., its satisfiability-status has been
     determined). </li>
     <li> We should actually have two such numbers, one for the left main
     branch (which becomes 1 when the "first half" of the search space has been
     completed), and one for the right main branch (which finally becomes
     zero when the whole problem was solved). </li>
     <li> While the first branch is still under processing, that second number
     is not considered. </li>
     <li> First we should implement this with the Maxima OKsolver-2002. </li>
    </ol>
   </li>
   <li> More informations on autarkies:
    <ol>
     <li> Useful is the average size of an autarky. </li>
     <li> Together with minimum and maximum size; also standard
     deviation can't hurt. </li>
     <li> Perhaps useful is also the relative size of the autarky (w.r.t. the
     number of variables), that is, the number of variables in the autarky
     divided by the total number of variables (again, max, min, average, etc.).
     </li>
    </ol>
   </li>
   <li> More information on the tree:
    <ol>
     <li> DONE
     The proportion of the tree solved (in terms of total assignments
     covered). </li>
     <li> The levelled height would be interesting. </li>
     <li> Quasi-single nodes should perhaps be ignored here. </li>
     <li> On the other hand, they are also taken into account for the
     (ordinary) height, and so perhaps we should here do the same. </li>
     <li> Elementary statistics on the distribution of leaf-depths should
     be very relevant: counting the leaves, and the sum and the sum of
     squares of their depths (so that mean and standard deviation can
     be computed). </li>
     <li> Type "double" should be alright for the sums. </li>
     <li> It shouldn't be too hard to compute the size of the resolution
     refutation found (including pruning). </li>
    </ol>
   </li>
   <li> Statistics regarding reductions:
    <ol>
     <li> The distribution of the number of 2-reduction-rounds per nodes (sum
     and sum of squares for mean and standard deviation) should be interesting.
     </li>
     <li> The number of unit-propagations after branching could also be of
     interest. </li>
     <li> And the number of unit-propagations after the assignment from a
     2-reduction. </li>
    </ol>
   </li>
   <li> More information on variables and literals:
    <ol>
     <li> At least the initial variable and literal degrees (maximal, and
     average) are of interest. </li>
    </ol>
   </li>
   <li> More information on single-nodes:
    <ol>
     <li> Data on the tree-depth (average, deviation) should be of interest.
     </li>
     <li> Also data on the length of the path-assignment. </li>
     <li> And data on the clause learned, if possible. Then likely we should
     remove from the resolution-variable-set at each resolution step the
     resolution variable (this is not done yet, since we don't use that
     clause). </li>
    </ol>
   </li>
   <li> Monitoring:
    <ol>
     <li> The prediction e.g. at the beginning is distorted, since the build-up
     of the tree up to this monitoring node is taken into account. </li>
     <li> So that e.g. the very first monitoring node has to take into account
     all the d nodes on the path leading to it. And after half of the tree
     is processed, that starting node of the second half has d-1 nodes
     additionally to take into account (where d is the monitoring depth). </li>
     <li> Likely it would be better if one had available for each monitoring
     node the size of its subtree and the time needed for it. </li>
     <li> Then perhaps a second average would be needed, namely for nodes
     with depth strictly smaller than the monitoring depth. This would
     estimate the effort needed for such nodes, and the total estimate
     would add this to the effort for monitoring nodes. </li>
     <li> Also the size of the resolution tree could be interesting (per
     monitoring node). </li>
     <li> Regarding the prediction formula:
      <ol>
       <li> Are there standards for analysing the various time-series' ? </li>
      </ol>
     </li>
     <li> See Heuristics/StatisticalAnalysis/plans/TimeSeriesAnalysis.hpp
     for an alternative approach for prediction, not just counting the nodes
     at the monitoring level. Perhaps this isn't too hard to do for the
     OKsolver_2002. </li>
     <li> How could one do periodisation? For example with
     GreenTao_3-2-3-5_558.cnf (see
     Experimentation/Investigations/RamseyTheory/GreenTaoProblems/plans/GreenTao_te_m-3-5.hpp)
     very late a big explosion happens, and all predictions, based on taking
     all nodes into account, become meaningless. </li>
    </ol>
   </li>
   <li> MAXSAT information
    <ol>
     <li> When arriving at a leaf, the number of satisfied clauses should be
     easily available. </li>
     <li> So we should record the maximum number of satisfied clauses so
     far. </li>
     <li> Since the branch is not further explored, in general this does
     not yield good information on the MAXSAT problem, however it should
     yield some information "how far we got". </li>
     <li> Also relevant in this direction is the maximum number of variables
     assigned on the paths to the leaves. </li>
     <li> Remark: One could study the variation of the MAXSAT problem,
     perhaps called "MIN-MAXSAT", where we still look at the maximum number
     of satisfiable clauses, but only admit either only partial assignments
     which do not falsify any clause, or which can falsify a clause, but
     there must exist a variable such that removal of that assignment removes
     all falsified clauses (the partial assignments considered by the OKsolver
     should be of this type). </li>
    </ol>
   </li>
   <li> With all this additional data, the output formatting needs to be
   improved:
    <ol>
     <li> For the "DIMACS" format we should have the option of tabular
     format, spreading the output over several lines, using a sensible
     grouping and tabular formatting (so that we still have several entries
     on one line, but this in a readable way, so that several outputs
     can be quickly compared). </li>
     <li> One could ask for a "change log", that is, for the
     changing data we only report the difference to the last output (using
     "+" and "-" to mark the direction of the change). </li>
    </ol>
   </li>
   <li> More on-the-fly information
    <ol>
     <li> It should be possible to send a signal to a running OKsolver, so that
     the current partial assignment is output, marking the decision variables
     with their levels. </li>
    </ol>
   </li>
  </ul>


  \todo Output intermediate results
  <ul>
   <li> Also if the instances couldn't be solved, often something was achieved,
   and this should be extracted. </li>
   <li> Output of unit-clauses inferred:
    <ol>
     <li> As an option, the output of forced assignments once they were found,
     should be enabled. </li>
     <li> Perhaps best in the form on unit-clauses in Dimacs-format, so that
     these clauses can be appended to the original instance. </li>
     <li> To start with, we have the effect of r_1-reduction on the input.
     </li>
     <li> Then we have the effect of r_2-reduction. </li>
     <li> The next forced assignment found is when at level 0 we go to examine
     the second branch: then the assignment for that second branch is enforced.
     </li>
     <li> Again all following r_1-reduction are forced too. </li>
     <li> And again all following r_2-reductions are forced. </li>
     <li> The same is repeated when (then!) at level 1 we go to examine the
     second branch. </li>
     <li> And so on, always increasing the reached level by +1. </li>
    </ol>
   </li>
  </ul>


  \todo More influence on heuristics
  <ul>
   <li> We should have an additional option which reverts the order of
   branches (this is to study the "missed single nodes"). </li>
   <li> We should also re-activate the command line for randomisation of
   the branching heuristics (so that it makes sense to re-run the solver).
   </li>
  </ul>


  \todo Improving the implementation
  <ul>
   <li> At least the initial unit-clause elimination (in
   OKsolver/SAT2002/Einlesen.c) is very inefficient; see
   Satisfiability/Reductions/UnitClausePropagation/plans/general.hpp).
    <ol>
     <li> First one needs to reconstruct the underlying algorithm. </li>
    </ol>
   </li>
   <li> We should also investigate the unit-clause elimination(s) used then
   during the normal processing (one form for the reduction, one for the
   look-ahead, and one for branching). </li>
   <li> Handling of autarkies:
    <ol>
     <li> On instances where there are many autarkies (see for example
     Experimentation/Investigations/RamseyTheory/GreenTaoProblems/plans/GreenTao_te_m-3-5.hpp),
     it would be more efficient not to restart after an autarky was found, but
     first to complete the list of variables (checking for autarkies, but not
     computing statistics), and only then to restart. </li>
     <li> Likely this strategy is less efficient on most instances (with
     autarkies --- without autarkies it doesn't matter), and so perhaps could
     be a command-line option (or compile-time --- or automatically detected
     by the solver (once one node had more than one autarky, the behaviour
     switches from "immediately restart" to "first complete")). </li>
    </ol>
   </li>
  </ul>


  \todo Start planning on evaluating and optimising heuristics
  <ul>
   <li> Based on OK's SAT-handbook article. </li>
   <li> First done at the Maxima-level. </li>
  </ul>


  \todo Apply time-measurements
  <ul>
   <li> Apply the time-measurement-system (assuming it is available by now).
   </li>
  </ul>


  \todo Optimising the code
  <ul>
   <li> Valgrind offers the tools Cachegrind, Callgrind, Massif. </li>
  </ul>


  \todo Why so slow on easy instances?
  <ul>
   <li> Using SplittingViaOKsolver together with minisat-2.2.0, it seems best
   performance is obtained when we have sub-problems which all take at most
   a few seconds. </li>
   <li> This is much faster than minisat-2.2.0 alone or OKsolver-2002 alone.
   </li>
   <li> Why is OKsolver-2002 so slow on these easy instances?
    <ol>
     <li> Could be the different strategies. </li>
     <li> Or the datastructures of the OKsolver might not be suitable for small
     problems? </li>
     <li> Perhaps for small (sub-)problems one should switch to a lazy
     datastructure? </li>
    </ol>
   </li>
  </ul>


  \todo Extension by cardinality constraints
  <ul>
   <li> Allowing cardinality constraints as a special form of active clauses
   is perhaps rather easy to do? </li>
   <li> See "Cardinality constraints as active clauses" in
   ComputerAlgebra/Satisfiability/Lisp/PseudoBoolean/plans/CardinalityConstraints.hpp.
   </li>
   <li> Forced assignments are easy to find by just arithmetical comparison.
   </li>
   <li> For the heuristics the constraint is just the set of its prime
   implicates (numerically). </li>
   <li> Regarding the variables used (tree-pruning): If it comes to a
   forced assignment, then just all variables assigned to true resp. to
   false are involved. </li>
   <li> And a partial assignment is considered to be an autarky for a
   cardinality constraint iff it is an autarky for the corresponding
   set of prime implicates. </li>
  </ul>


  \todo Improved Delta(n)-distance
  <ul>
   <li> Given the number a(v) of variables which during the r_2-reduction
   set v to true, and the number b(v) setting it to false, we can use
   (a(v)+1, b(v)+1) as a branching tuple. </li>
   <li> We should have n(vp_1) <= a(v)+1 <= n(vp_2), where vp_1/2 extends
   v -> 0 by r_1/2-reduction. </li>
   <li> A problem is that some variables are skipped during r_2-reduction
   (if they were triggered before, while meanwhile no reduction took place).
   </li>
   <li> The whole area is closely related to the problem of local learning,
   and which binary clauses to learn. </li>
   <li> So perhaps this should wait wait for the generic form of the
   OKsolver-2002. </li>
  </ul>


  \todo Enable finding all solutions
  <ul>
   <li> Perhaps we should abandon this extension:
    <ol>
     <li> It is more complicated than anticipated. </li>
     <li> And it is not sensibly, because different strategies,
     reductions, heuristics are to be used for finding all solutions. </li>
     <li> But perhaps with the generic form of the OKsolver-2002 it is more
     sensible. </li>
    </ol>
   </li>
   <li> See
   ComputerAlgebra/Satisfiability/Lisp/Backtracking/plans/OKsolver2002.hpp.
   </li>
   <li> Reduktion2 applies pure literal elimination --- this is no longer
   possible.
    <ol>
     <li> The OKsolver assumes that the formula is reduced w.r.t.
     pure literals; this must then be changed. </li>
     <li> We must apply Reduction1 to these variables (only the
     falsifying value needs to be tested). </li>
    </ol>
   </li>
   <li> The (real) autarky reduction (these are found by the filter)
   can no longer be applied.
    <ol>
     <li> Except for the heuristics this can just be ignored. </li>
     <li> A problem is that the distance needs to be non-zero, and
     this is not the case for an autarky. </li>
     <li> It seems most natural to try to avoid branching after such variables
     (thus one could simply use the product value 0). </li>
     <li> If only autarky variables are left, then Delta(n) is used for the
     heuristics. </li>
     <li> Of course, it could be that a branching with an autarky branch is
     attractive, not because of new clauses now, but because of a big reduction
     in problem size. However likely we should ignore this. </li>
    </ol>
   </li>
   <li> All places need to be identified where a solution
   can be found:
    <ol>
     <li> Reduktion1 can find a satisfying assignment; these are forced
     assignments (via failed literals). The solutions found are added, and
     backtracking is performed (in the same way as if a contradiction would
     have been found). (One has to see whether Reduktion1 did everything
     properly --- it could be that since everything stopped anywhere no care
     has been taken to maintain proper invariants. But it looks alright.) </li>
     <li> Reduktion2 can find a satisfying; that is a satisfying assignment via
     a pure autarky. Since Reduction2 won't be applied anymore, this is not
     a problem. </li>
     <li> The filter can find a satisfying assignment (this is where "erfuellt"
     is used); this is a satisfying autarky. Here we have to add the satisfying
     assignments found in this branch, and then the assignment for the other
     branch has to performed, and the reduction cycle has to be re-started.
      <ol>
        <li> Currently in case the filter found a satisfying assignment,
       the assignment is actually applied (the filter uses a lazy
       data structure to perform assignments, without actually changing
       the formula). Why is this needed?? The comment says it is needed
       for the output?
       <ol>
        <li> The reason is simply convenience: The satisfying assignment
        is split in the current working assignment and the look-ahead
        assignment, and by applying the look-ahead assignment we have
        everything "in one place". </li>
        <li> This hack should be eliminated, and the function for outputting
        the satisfying assignment should accept the partial assignments
        as input. </li>
       </ol>
      </li>
     </ol>
    </li>
   </li>
   <li> The output "UNSAT" at the root must be prevented, if
   number_satisfying_assignments != 0. </li>
   <li> With the new option "ALLSAT" in principle we have another
   factor of 2 for the test cases. There is no way out, must be done. </li>
   <li> Interaction with option BELEGUNG
    <ol>
     <li> Hopefully only function AusgabeBelegung needs to be called
     additionally. </li>
     <li> For that to work, the file pointer needed in case of output
     to a file needs to be computed in advance, and a global variable
     stores the output-stream pointer (either the file or stdout; null
     if no output of satisfying assignments). </li>
     <li> In case of file-output we just output all satisfying
     assignments into the file. </li>
     <li> A problem arises with option OUTPUTTREEDATAXML --- what
     to do here (and what is this option about?)?
     </li>
    </ol>
   </li>
   <li> Interaction with option BAUMRES
    <ol>
     <li> As soon as one satisfying assignment was found, then
     above that point tree pruning is turned off. </li>
     <li> Perhaps easiest by using a null pointer. </li>
    </ol>
   </li>
   <li> A simple improvement is tree pruning (as discussed in
   AllSolutions/plans/MinimalAssignments.hpp):
    <ol>
     <li> Once a solution is found, the solution is minimised
     by removing unnecessary assignments (minimisation w.r.t.
     set-inclusion, just find one minimal subset). </li>
     <li> And then the tree is pruned if possible. </li>
     <li> Of course, this will make the old OKsolver even more
     complicated, and thus perhaps should perhaps be postponed
     until the re-implementation (see
     Solvers/OKsolver/plans/OKsolver_1_0.hpp). </li>
    </ol>
   </li>
  </ul>

*/


