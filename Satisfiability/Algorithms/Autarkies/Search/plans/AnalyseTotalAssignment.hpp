// Oliver Kullmann, 31.8.2005 (Swansea)
/* Copyright 2005 - 2007, 2008, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Autarkies/Search/plans/AnalyseTotalAssignment.hpp
  \brief Decision, whether a given partial assignment contains an autarky, and
  applications for autarky search (via enumeration of all total assignments, or
  local search through total assignments).

  
  \todo Complete correct compilation:
   <ul>
    <li> Submit an error report to Gcc: Option "-I-" *cannot* be replaced by
    "-iquote", and there is also no alternative! </li>
    <li> See "Installation of UBCSAT completed" in
    Satisfiability/Algorithms/LocalSearch/plans/general.hpp. </li>
   </ul>  


  \todo Connections with other modules and parts
  <ul>
   <li> See ComputerAlgebra/Satisfiability/Lisp/Autarkies/plans/general.hpp.
   </li>
   <li> Development in this module (C++) is postponed until all basic
   implementations are done at the Maxima/Lisp level. </li>
   <li> Connect with LocalSearch/plans/SupportTotalAssignments.hpp.
  </ul>


  \todo Design and implement class ComputeLargestAutarky (goes to
  Autarkies/Search/AnalyseTotalAssignment.hpp)
  <ul>
   <li> Connecting to UBCSAT, computes largest autarky given a current total
   assignment and clause-set. </li>
   <li> First a concept is needed. </li>
   <li> Then a prototype is created --- with tests! </li>
  </ul>


  \todo Design and implement class AutarkySearchUbcsat (goes to
  Autarkies/Search/AnalyseTotalAssignment.hpp)
  <ul>
   <li> Use the class ComputeLargestAutarky: Run any UBCSAT-algorithm for
   finding a satisfying assignment, and check the total assignments considered
   for a non-trivial autarky. </li>
   <li> If none is found until termination of UBCSAT-algorithm,
   then the result is the empty autarky, while otherwise the result is that
   largest sub-autarky found. </li>
   <li> Write to a file the reduced formula (after application of the (possibly
   empty) autarky). </li>
   <li> This must working with *any* local-search-algorithm from Ubcsat
   (without alteration). </li>
  </ul>


  \todo AnalyseTotalAssignment.cpp
  <ul>
   <li> This application program is just a thin wrapper around class
   AutarkySearchUbcsat, handling how the process is to be iterated (of course,
   again the main thing is a class which handles the iteration; the program
   AnalyseTotalAssignment.cpp just manages input and output, and uses this
   class). </li>
  </ul>


  \todo Including source-files from external libraries:
  It seems reasonable to use e.g. include <ubcsat/reports.h>.


  \todo Complete autarky search
  <ul>
   <li> Via the appropriate components from the combinatorics module for
   enumeration of total assignments, we obtain complete algorithms for autarky
   search. </li>
  </ul>


  \todo New file structure
  <ul>
   <li> Likely we need more files (for the analysis of total assignments in
   general, for the methods exploiting Ubcsat, for the complete search). </li>
  </ul>


  \todo Changing UBCSAT
  <ul>
   <li> Change the heuristics in UBCSAT, where now the goal is not to find a
   satisfying (total) assignment, but a total assignment containing a
   non-trivial autarky. </li>
   <li> As the value of a total assignments the minimal number
   of critical clauses over all (non-empty) levels could be used. </li>
   <li> If a low number of variables is involved, then a low number of
   critical clauses is to be expected; so perhaps better to use the ratio
   (number of variables in level) / (number of critical clauses for the level)
   --- the biggger this number the better. </li>
   <li> A general approach is as follows:
    <ol>
     <li> The total assignment phi yields a sequence
     phi_0 = phi, phi_1, ..., phi_m = empty assignment, m > 0,
     of partial assignments (in the unsuccessful case; following the stages of
     unassigning variables). </li>
     <li> This gives us a list (phi_0, ..., phi_{m-1}) of candidates for
     autarkies. </li>
     <li> Now we need an evaluation of phi_i regarding its autarky-goodness,
     say eval(phi_i) >= 0, where eval(phi_i) = 0 iff phi_i is an autarky. </li>
     <li> So we get a (non-empty) list (eval(phi_0), ..., eval(phi_{m-1}))
     of non-negative real numbers, and one can use measures of this list like
     the min for obtaining an evaluation of phi (the smaller the better). </li>
     <li> So basically the problem of evaluating phi is delegated to the
     evaluation of partial assignments; see EvaluatePartialAssignments. </li>
    </ol>
   </li>
  </ul>


  \todo Integrating/assimilating UBCSAT
  <ul>
   <li> After we got an autarky searcher running, and also played around with
   the heuristics (mainly based on UBCSAT components), we need to think about
   more appropriate data structures, this all integrated with the OKlibrary,
   but perhaps using the UBCSAT framework and components. </li>
   <li> Finally we need to think about to do everything within OKlibrary (using
   the generic components, and thus enjoying greater generality and
   re-usability). </li>
  </ul>

*/

