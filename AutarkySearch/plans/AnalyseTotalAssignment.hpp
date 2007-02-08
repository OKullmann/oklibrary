// Oliver Kullmann, 31.8.2005 (Swansea)

/*!
  \file AutarkySearch/plans/AnalyseTotalAssignment.hpp
  \brief Decision, whether a given partial assignment contains an autarky, and
  applications for autarky search (via enumeration of all total assignments, or
  local search through total assignments).

  \todo Improved installation of UBCSAT:
  <ol>
   <li> Improve the current build, so that there is exactly one directory containing
   everything offered by Ubcsat, that is, the appropriate src-directory is moved to
   Ubcsat/1-0-0, and possibly the make-variables are updated. </li>
   <li> The source-files from ubcsat should be converted to unix-files (using
   dos2unix). </li>
   <li> No "modified files" anymore, but NEW FILES, with proper change
   documentation, in the OKlibrary; by appropriate settings of variable
   source-libraries the directory with the updated ubcsat-files is dominant
   over the original ubcsat-source-directory. </li>
   <li> Some documentation is needed:
    <ul>
     <li> how to use the binary </li>
     <li> how to use the library files. </li>
    </ul>
   </li>
  </ol>
     
  \todo No C code anymore:
  Create AnalyseTotalAssignment.cpp as (initially) a copy of AnalyseTotalAssignment.c,
  and then change the code to proper C++, introducing an additional header file for
  declarations. Then remove AnalyseTotalAssignment.c from the repository.
  (This should be done even if some parts of AnalyseTotalAssignment.c will be removed later
  ---  we need the declarations anyway, and library development must happen in small
  controlled steps.)

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

