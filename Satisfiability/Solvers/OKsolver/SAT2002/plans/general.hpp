// Oliver Kullmann, 17.8.2007 (Swansea)
/* Copyright 2007, 2008 Oliver Kullmann
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
   at each node.
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
   <li> Why are there these comments that string.h is included because of C++? Seems we should
   remove them. </li>
   <li> For the xml-output we should then state that C is the programming language. </li>
   <li> Where we touch the code, we change the style to the standard
   (otherwise we leave it). </li>
   <li> All includes need to be changed to the library-style. Or? Perhaps, due to the
   exceptional "historical" character, we don't do this here? Emphasising, that there
   are no reusable components here?! </li>
   <li> Since the function specifier "inline" is available with C99, we should
   remove the little apparatus with "KEININLINE" and "__inline__" (and replace
   "__inline__" everywhere with "inline"). </li>
   <li> Deeper changes like const-introduction only later. </li>
   <li> DONE (stick to C) Perhaps move everything to C++ (but no real changes to any data structures, etc.,
   only using C++ header files etc.). Or?? </li>
   <li> DONE (GesamtOKs.cpp removed) Why does GesamtOKs.cpp want to be a C++ program --- maybe we just stick to C?
   Would be more honest! On the other hand, we are more knowledgeable with C++.)
   The purpose of GesamtOKs.cpp likely was to enable the use of C++ inlining --- but
   also C99 has the inline keyword. </li>
   <li>  DONE (stick to C) The program uses typical C-methods to simulate abstract data types (a functional
   interface is build, hiding all pointer access, which happens in the implementation files).
   So it appears that we better stick to C. </li>
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
     <li> OKsolver_2002.c : standard C program (assumes linking with the other compilation units) </li>
     <li> OKsolver_2002_lnk.c : Same as OKsolver_2002.c, but includes all other compilation units, and thus
     needs no linking. </li>
     <li> DONE (removed) GesamtOKs.cpp : Produced by UebersetzungOKs.plx out of all the compilation units
     (and is treated as C++). (However OK.h and Parameter.h are not inserted, but included.) </li>
    </ol>
   <li> There is no need anymore for the created file GesamtOKs.cpp, however we need some
   tests on the running times of GesamtOKs, OKsolver_2002 and OKsolver_2002_lnk, and this also
   compared with the existing OKsolver-binaries. Optimally, we could do this simply with our build system
   (as planned), but perhaps for the time being we just do some ad-hoc testing.
   </li>
   <li> The difference between OKsolver_2002.c and OKsolver_2002_lnk.c is that the latter enables more inlining ---
   can't this be achieved otherwise (through inline-specifications)? </li>
   <li> The build system links the C programs as C++ programs; shouldn't make a big
   difference for now, but should be rectified with the new system. </li>
   <li> The build system doesn't know about the dependency of OKsolver_2002.c on the other .c-programs
   (to which it links) --- this needs to be addressed!
    <ol>
     <li> For each program P, one additional dependency file has to be created which contains
     the rule with target P, where the prerequisites are all the files P links together. </li>
     <li> See the plans under Buildsystem. </li>
    </ol>
   </li>
   <li> Compilation of lokalesLernen.c on its own should only happen with LOKALLERNEN defined
   (without it a compile-time error should ensue). </li>
  </ul>


  \todo Complete the help facilities of the OKsolver
  <ul>
   <li> There are quite a few options available, and these should be documented. </li>
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
   <li> Differentiate between solver runs which are expected to succeed and runs which
   are expected to fail (so that in the latter case the output to stderr can be suppressed). </li>
   <li> All possible syntactic errors diagnosed by the OKsolver need be checked. </li>
   <li> Tautological clauses and repeated literals in clauses need to be checked. </li>
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
   <li> Possibly this is not the case; then we need a precise specification what
   actually is counted, so that then the output can be declared as correct. </li>
  </ul>


  \todo Incorrect output of monitoring-data to files
  <ul>
   <li> When using "-M" together with "-F", then we get spurious output lines
   (console and file), like
   \verbatim
0 :  (null) 0
   \endverbatim
   (the last entry seems always to be 0 or 1). </li>
   <li> Directly at the beginning, but also later. </li>
   <li> Another problem here is that if the output-file already exists,
   then apparently nothing happens. </li>
  </ul>


  \todo Improve statistics
  <ul>
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
   <li> Timing:
    <ol>
     <li> On Unix/Linux machine process-timing data is availabe for (much)
     longer periods (of course with lower resolution). </li>
     <li> We already have the macro-option SYSTIME, which when activated (i.e.,
     defined), uses sys/times.h and different definitions. </li>
     <li> We need to check whether this is already what we want (the time used
     by the process (not wall-clock time) with a resolution of a second). </li>
     <li> Since we have already other non-standard-C parts (from the Unix
     libraries), this should be alright. </li>
     <li> However we should check what BOOST has to offer. </li>
     <li> We should then make clear the two options: short times (<= 25 m)
     with high resolution, or long times with low resolution. </li>
     <li> It shouldn't be a big deal to actually make it a run-time option
     (just providing both definitions, and a run-time switch regulates which
     to use) --- this would be much more convenient. </li>
    </ol>
   </li>
  </ul>


  \todo Declare variable as close to their first usage as possible
  <ul>
   <li> Since C99 also allows to appear declaration everywhere, one
   should move many variable declarations to their first usage point. </li>
   <li> And, as with C++, one can now declare loop-variables inside the loop. </li>
  </ul>


  \todo Use const-qualification
  <ul>
   <li> Where possible, const-qualification should be applied. </li>
  </ul>


  \todo Use restrict-qualification
  <ul>
   <li> We should investigate at which places this pointer-qualification can be used. </li>
  </ul>


  \todo Elimination of compile-time options
  <ul>
   <li> LOKALLERNEN
    <ol>
     <li> Extract ideas. </li>
     <li> Eliminate option and code. </li>
    </ol>
   </li>
   <li> FASTAUTARKIEN
    <ol>
     <li> Extract ideas. </li>
     <li> Eliminate option and code. </li>
    </ol>
   </li>
  </ul>


  \todo Enable finding all solutions
  <ul>
   <li> Perhaps we should abandon this extension:
    <ol>
     <li> It is more complicated than anticipated. </li>
     <li> And it is not sensibly, because different strategies,
     reductions, heuristics are to be used for finding all solutions. </li>
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
     <li> Reduktion1 can find a satisfying assignment; these are forced assignments
     (via failed literals). The solutions found are added, and backtracking is
     performed (in the same way as if a contradiction would have been found).
     (One has to see whether Reduktion1 did everything properly --- it could
     be that since everything stopped anywhere no care has been taken to maintain
     proper invariants. But it looks alright.) </li>
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
   <li> Apply the time-measurement-system (assuming it is available by now). </li>
  </ul>

*/


