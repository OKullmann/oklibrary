// Oliver Kullmann, 31.8.2005 (Swansea)

/*!
  \file AutarkySearch/plans/AnalyseTotalAssignment.hpp
  \brief Decision, whether a given partial assignment contains an autarky, and
  applications for autarky search (via enumeration of all total assignments, or
  local search through total assignments).

  \todo The basic algorithm is: Given a partial assignment phi, unassign all
  variables in critical clauses, and repeat this process until no critical clauses
  are left --- the maximal autarky contained in phi is obtained. Design and
  implement, using the data structures from LocalSearch/plans/SupportTotalAssignments.hpp.

  \todo Combination with enumeration of all total assignments yields
  autarky search in time O(2^n).

  \todo Local installation of UBCSAT.
   - Installing UBCSAT in ExternalSources : DONE
   - Creating a simple application, which just consists of a standard UBCSAT solver,
     and compile it completely within the OKsystem (using our build system; likely
     some changes are needed).

  \todo Using UBCSAT
  Analysing total assignments (and autarky reduction,
  if successful) is added to UBCSAT, without further changes.

  For that, include and use all triggers and their data structures needed from UBCSAT
  library in AutarkySearch.
   - These include: 
     ReadCNF, LitOccurence, CandidateList, DefaultProcedures and Flip+FalseClauseList.

  QUESTION (OK): Does this mean that we have already a first complete implementation?
  If yes, then the current milestone 0.0.6 should become 0.1.

  ANSWER (ML): These triggers were originally used by calling the trigger functions of the
  UBCSAT library. Therefore they rely on the UBCSAT main program running for it to work
  To get the AnalyseTotalAssignment to work on its own without UBCSAT main, the code needs to be
  modified slightly which I am currently doing. Therefore I will add more plans for this if you
  feel this is a good idea.
  

  \todo Changing UBCSAT
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

