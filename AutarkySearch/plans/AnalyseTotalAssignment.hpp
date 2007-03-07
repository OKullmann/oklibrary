// Oliver Kullmann, 31.8.2005 (Swansea)

/*!
  \file AutarkySearch/plans/AnalyseTotalAssignment.hpp
  \brief Decision, whether a given partial assignment contains an autarky, and
  applications for autarky search (via enumeration of all total assignments, or
  local search through total assignments).
  
  \bug Todos below are stale (needed update by ML).
  OK: Is this done now ? (then it needs to be marked with "DONE")
  OK: IT GETS MORE AND MORE STALE

  \todo Improved installation of UBCSAT:
  <ol>
   <li> Some documentation is needed:
    <ul>
     <li> how to use the binary </li>
     <li> how to use the library files </li>
     <li> what are those library files> </li>
    </ul>
   </li>
   <li> No "modified files" anymore, but NEW FILES, with proper change
   documentation, in the OKlibrary; by appropriate settings of variable
   source-libraries the directory with the updated ubcsat-files is dominant
   over the original ubcsat-source-directory.
   This is IMPORTANT --- the hack in ubcsat.mak must vanish, and everything
   we do MUST be under in the source control for Transitional. DONE
   </li>

   <li> Improve the current build, so that there is exactly one directory containing
   everything offered by Ubcsat, that is, the appropriate src-directory is moved to
   Ubcsat/1-0-0, and possibly the make-variables are updated.
   ML : I propose that the Ubcsat/l-0-0/lib folder now contains the .o files
   from the /src .c files that have used the changed .h files in AutarkySearch/.
   One problem of doing this is getting gcc to use the .h files in AutarkySearch/ instead of the ones in /src. Is there a known way of doing this?
   OK: Either create a temporary directory, or use one of the gcc options -IDIR,-nostdinc,-isystem,-iquote
   (as usual, one should use e-mail lists --- the gcc-help list for example (they react normally quite quickly)). DONE (the lib-files
   are only relevant to us, thus use the modified source-files).
   </li>

   <li> The source-files from ubcsat should be converted to unix-files (using
   dos2unix): DONE
   </li>
   <li> Modified .h  files are added to Transitional/AutarkySearch/ which will contain
   definitions of fixed width types from stdint.h instead. DONE
   </li>
   <li> 
   </li>
  </ol>

  \todo Compilation in AutarkySearch:
  <ul>
   <li> Compilation in AutarkySearch is done correctly:   
   ML: Currently, compilation in AutarkySearch during "make all" involves including AutarkySearch/ubcsat.h
   which then includes Ubcsat/1-0-0/src/ubcsat-types.h instead of AutarkySearch/ubcsat-types.h
   (due to definitions.mak). One idea I can think of to overcome this is to move all .h files in
   src/ to AutarkySearch/. This yields correct compilation that works on cs-oksvr.
   ML: Idea of having ubcsat.h in AutarkySearch/ as well as modified ubcsat-types.h allows correct compilation of
   ubcsat src/ files and AutarkySearch files.

   OK: We should have only modified ubcsat-files in the OKlibrary, and also
   this in a sub-directory (AutarkySearch/ubcsat). But before making any
   changes, these should be announced in the plans. I don't understand your
   above argumentation; please state precisely the requirements on the build
   system, so that it can be seen either how to get it with the existing
   system or how to extend it. Regarding the compiler-question: Did you
   contact gcc-help (the question of how to force a source directory)?

   Final point: In code (re)written by us, we should incude the files we
   really mean, that is, using for example #include AutarkySearch/ubcsat/ubcsat.h.

   ML: when ubcsat-types.h is added to AutarkySearch/ubcsat (only modified file)
   and definitions.mak includes this folder as source folder, we get errors in
   Initialise function. Adding all ubcsat's .h files to AutarkySearch/ubcsat
   however yields a different error later on, namely that of multi definition
   of pActiveAlgorithm. I cannot work out a way around either errors in 
   Initialise function or errors of multiple definitions.

   </li>
   <li>
   It seems just putting Transitional/AutarkySearch/ubcsat in front of the ubcsat-library
   in definitions.mak (source_libraries-definition) should solve the problem?
   So definitions.mak will now have another directory under source_libraries variable namely
   Transitional/AutarkySearch/ubcsat.
   </li>
  </ul>
     
  \todo No C code anymore:
  Create AnalyseTotalAssignment.cpp as (initially) a copy of AnalyseTotalAssignment.c,
  and then change the code to proper C++, introducing an additional header file for
  declarations. Then remove AnalyseTotalAssignment.c from the repository.
  (This should be done even if some parts of AnalyseTotalAssignment.c will be removed later
  ---  we need the declarations anyway, and library development must happen in small
  controlled steps.)

  OK: What about this???

  \todo Connect with LocalSearch/plans/SupportTotalAssignments.hpp.

  \todo Design and implement class ComputeLargestAutarky (goes to
  AutarkySearch/AnalyseTotalAssignment.hpp):
  Connecting to UBCSAT, computes largest autarky given a current total assignment and clause-set.
  <ol>
   <li> First a concept is needed. </li>
   <li> Then a prototype is created --- with tests! </li>
  </ol>

  \todo Design and implement class AutarkySearchUbcsat (goes to
  AutarkySearch/AnalyseTotalAssignment.hpp):
  <ul>
    <li> Use the class ComputeLargestAutarky: Run any UBCSAT-algorithm for finding a satisfying
    assignment, and check the total assignments considered for a non-trivial autarky. If none
    is found until termination of UBCSAT-algorithm, then the result is the empty autarky,
    while otherwise the result is that largest sub-autarky found. </li>
    <li> Write to a file the reduced formula (after application of the (possibly empty)
    autarky). </li>
  </ul>
  This must working with *any* local-search-algorithm from Ubcsat (without alteration).

  \todo AnalyseTotalAssignment.cpp:
  This application program is just a thin wrapper around class AutarkySearchUbcsat,
  handling how the process is to be iterated (of course, again the main thing is
  a class which handles the iteration; the program AnalyseTotalAssignment.cpp just
  manages input and output, and uses this class).

  \todo Including source-files from external libraries:
  It seems reasonable to use e.g. include <ubcsat/reports.h>.

  \todo Complete autarky search:
  Via the appropriate components from the combinatorics module for enumeration of total
  assignments, we obtain complete algorithms for autarky search.

  \todo New file structure:
  Likely we need more files (for the analysis of total assignments in general, for the
  methods exploiting Ubcsat, for the complete search).

  \todo Changing UBCSAT:
  Change the heuristics in UBCSAT, where now the goal is not to find a satisfying (total) assignment,
  but a total assignment containing a non-trivial autarky.

  As the value of a total assignments the minimal number
  of critical clauses over all (non-empty) levels could be used. (If a low number
  of variables is involved, then a low number of critical clauses is to be expected;
  so perhaps better to use the ratio (number of variables in level) / (number of
  critical clauses for the level) --- the biggger this number the better.)
  A general approach is as follows: The total assignment phi yields a sequence
  phi_0 = phi, phi_1, ..., phi_m = empty assignment, m > 0,
  of partial assignments (in the unsuccessful case; following the stages of
  unassigning variables). This gives us a list (phi_0, ..., phi_{m-1})
  of candidates for autarkies. Now we need an evaluation of phi_i
  regarding its autarky-goodness, say eval(phi_i) >= 0, where eval(phi_i) = 0
  iff phi_i is an autarky. So we get a (non-empty) list
  (eval(phi_0), ..., eval(phi_{m-1}))
  of non-negative real numbers, and one can use measures of this list like
  the min for obtaining an evaluation of phi (the smaller the better).
  So basically the problem of evaluating phi is delegated to the evaluation of
  partial assignments; see EvaluatePartialAssignments.

  \todo Integrating/assimilating UBCSAT
  After we got an autarky searcher running, and also played around with the heuristics (mainly based
  on UBCSAT components), we need to think about more appropriate data structures, this
  all integrated with the OKlibrary, but perhaps using the UBCSAT framework and components.
  Finally we need to think about to do everything within OKlibrary (using the generic
  components, and thus enjoying greater generality and re-usability).

*/

