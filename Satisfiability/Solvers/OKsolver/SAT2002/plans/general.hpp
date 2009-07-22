// Oliver Kullmann, 17.8.2007 (Swansea)
/* Copyright 2007, 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Solvers/OKsolver/SAT2002/plans/general.hpp
  \brief Plans on the maintenance of the code for the old OKsolver


  \todo OUTPUTTREEDATAXML
  <ul>
   <li> We need to compile versions with this macro defined. </li>
   <li> We need to test these versions. </li>
   <li> Document these versions.
    <ol>
     <li> Specify the DTD. </li>
    </ol>
   </li>
   <li> Apparently yet only the number of r_2-reductions is output
   at each %node.
    <ol>
     <li> We need to strengthen this by outputting most data. </li>
     <li> Definitely also the branching literal. </li>
    </ol>
   </li>
   <li> We need also Maxima-readable output:
    <ol>
     <li> The output should be the expression computed by the
     analogous Maxima program. </li>
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
   <li> We need to incorporate this functionality with our
   build system. </li>
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
  </ul>


  \todo Output of monitoring-data to files
  <ul>
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


  \todo Improve statistics
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


  \todo Simple parallelisation
  </ul>
   <li> As a simple means to achieve parallelisation, for a given depth
   the clause-sets at that level should be output (to files, with
   suitable names indicating the decisions leading to that clause-set). </li>
   <li> Then a simple script would allocate these problem-instances to
   the available machines, monitor their execution, record the results,
   and schedule new jobs if possible (and needed). </li>
   <li> To handle a large number of jobs, it must also be possible to
   just output a certain clause-set from the long list; for example
   when using a depth D=13, then by specifying a number from 1 to 8192
   one should get the corresponding clause-set; this actually then
   didn't need to be output, but could just be processed. </li>
   <li> However it is less intrusive to just use the above output facilities
   to output the clause-set obtained and then to stop (for solving the
   instance then the OKsolver (or any other solver) is called again). </li>
   <li> A script would then just manage the distribution of the jobs
   to the available machines. </li>
   <li> The index (in the above case 1 <= i <= 8192) would be represented
   in binary, and the bits would specify the decisions; if a decision would
   be achieved on the way, then nothing needed to be done. </li>
   <li> Simplest is just to choose the branches according to the path
   given by i; if a decision is obtained earlier then there is no need
   to go further. </li>
   <li> So the simplest mode is just to follow a path given by a binary word,
   and either output SAT, UNSAT or the (reduced) clause-set finally reached.
    <ol>
     <li> Say the parameter would be "-P=word" (--path=word), where "word"
     is a string of 1's and 2's (first branch or second branch). </li>
     <li> Branchings with just one successor are ignored here. </li>
     <li> If an output flag is set, then the final clause-set is output
     to a file (if no decision was reached). </li>
     <li> The program is exit then (using the standard exit, thus printing
     the statistics line). </li>
    </ol>
   </li>
   <li> In order to see what really needs to be done, for a given depth
   one actually wants also the list of indices of nodes which still need to be
   processed; if a satisfying assignment was found then the whole process
   can stop (likely we don't do counting in this framework). </li>
   <li> So this task is to compute a complete splitting tree according to
   ExperimentSystem/plans/DistributedSolving.hpp.
    <ol>
     <li> Here one could detect that branches don't need to be processed due
     to resolution tree pruning or autarky reduction. </li>
     <li> This could be achieved by forced backtracking when the given depth D
     is reached, assuming for the intelligent backtracking that all variables
     (respectively available) are used. Here then there are no "missed single
     nodes", but tree resolution pruning is always to be performed. </li>
    </ol>
   </li>
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
     <li> The levelled height would be interesting. </li>
     <li> Quasi-single nodes should perhaps be ignored here. </li>
     <li> On the other hand, they are also taken into account for the
     (ordinary) height, and so perhaps we should here do the same. </li>
    </ol>
   </li>
   <li> Counting unit-clause propagations:
    <ol>
     <li> Shall we do so? A question is what to do with the reduction. </li>
    </ol>
   </li>
   <li> More information on variables and literals:
    <ol>
     <li> At least the initial variable and literal degrees (maximal, and
     average) are of interest. </li>
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
  </ul>


  \todo More influence on heuristics
  <ul>
   <li> We should have an additional option which reverts the order of
   branches (this is to study the "missed single nodes"). </li>
   <li> We should also re-activate the command line for randomisation of
   the branching heuristics (so that it makes sense to re-run the solver).
   </li>
  </ul>


  \todo Start planning on evaluating and optimising heuristics
  <ul>
   <li> Based on OK's SAT-handbook article. </li>
   <li> First done at the Maxima-level. </li>
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


  \todo Apply time-measurements
  <ul>
   <li> Apply the time-measurement-system (assuming it is available by now).
   </li>
  </ul>


  \todo Optimising the code
  <ul>
   <li> Valgrind offers the tools Cachegrind, Callgrind, Massif. </li>
  </ul>

*/


