// Oliver Kullmann, 27.3.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/Resolution/plans/Basics.hpp
  \brief Plans for Maxima-components regarding the basic functions for the resolution calculus


  \todo resolvable versus resolvable_p
  <ul>
   <li> We should reconsider the naming scheme used in
   Satisfiability/Lisp/Resolution/Basics.mac. </li>
   <li> The only issue here is that in "resolvable_p" and
   "two_subsumption_resolvent_p" the "_p" is misleading:
    <ol>
     <li> It stands for "partial". </li>
     <li> This is rather irrelevant --- we often have that. </li>
     <li> Furthermore in "resolvable_p" the adjective is false. </li>
     <li> However "resolution_literal" is already used. </li>
     <li> So we emphasise that we have a list as result. </li>
     <li> I.e., "resolvable_p" -> "resolution_literal_l". </li>
     <li> And "two_subsumption_resolvent_p" -> "two_subsumption_resolvent_l".
     </li>
    </ol>
   </li>
   <li> DONE (false understanding of the issue, and of the Maxima-level)
   Currently
    <ul>
     <li> resolvable(C,D) is a predicate which returns whether C and D are
     resolvable, and </li>
     <li> resolvable_p(C,D) returns either the empty list if C and D are not
     resolvable or a list containing the resolvent if they are. </li>
    </ul>
   </li>
   <li> DONE (false understanding)
   This also occurs with two_subsumption_resolvable (predicate) and
   two_subsumption_resolvent_p (returns list). </li>
   <li> Typically, in the Maxima system, the "p" suffix denotes a predicate.
   </li>
   <li> DONE ("p" stands not for "predicate", but for "partial")
   resolvable_p should be a predicate, returning a boolean for
   consistency. </li>
   <li> DONE (false understanding)
   Making resolvable_p a predicate ensures we obey the principle of
   "least surprise" for the user of the library. </li>
   <li> DONE (false understanding)
   Possible solutions:
    <ul>
     <li> Switch the definitions of resolvable and resolvable_p.
      <ul>
       <li> This has the disadvantage that it is still not clear from the
       name what resolvable (previously resolvable_p) returns. </li>
      </ul>
     </li>
     <li> DONE (we never use such hacks)
     Add a notion of "safe" functions which compute the result of
     a computation and return the empty list otherwise.
      <ul>
       <li> We could use "safe" or the abbreviation "_sfe". </li>
       <li> resolvable would be renamed to resolvable_p. </li>
       <li> resolvable_p would be renamed to resolvable_safe. </li>
       <li> This would then be applied to the rest of the library. </li>
      </ul>
     </li>
    </ul>
   </li>
  </ul>


  \todo Renewal
  <ul>
   <li> Iterated resolution and DP should go into its own files. </li>
   <li> Write tests. </li>
   <li> The partial forms (like resolvable_p) should be supplied throughout.
   </li>
   <li> Establish consistent naming-practice. </li>
  </ul>


  \todo DP
  <ul>
   <li> The current implementation of opt_min_dp can be improved if only
   the minimum or the maximum is sought. </li>
   <li> DONE (there is only the technical problem that apparently local hash
   arrays are not recognised by "arrayinfo" and "listarray" ? We should
   ask about this at the maxima-mailing-list)
   To make the current implementation worthwhile, perhaps it should
   show the full distribution of sizes, using a map (size -> count). </li>
   <li> DONE The output of distribution_min_dp should be further processed,
   so that easily all information is available.
   <ol>
    <li> Perhaps we just sort the ocurring sizes together with their
    counts, and then output a list of pairs [size, count], sorted
    by size (ascending). </li>
    <li> It seems actually, that "arrayinfo" already returns a sorted list,
    so that nothing needs to be done. </li>
   </ol>
   </li>
   <li> We need also the greedy heuristics, which chooses the DP-variable
   such that the number of clauses for the next level is minimised. </li>
  </ul>


  \todo resolution_closure_cs
  <ul>
   <li> The implementation is very similar to min_resolution_closure_cs:
   Should we construct a common generalisation? </li>
   <li> DONE Same regarding monitoring as min_resolution_closure_cs. </li>
  </ul>


  \todo min_resolution_closure_cs : DONE
  <ul>
   <li> DONE As in "Maxima"-"Monitoring" in ComputerAlgebra/plans/general.hpp,
   perhaps the monitoring-output should be standardised, containing the name
   of the function? </li>
   <li> A second output is needed, with the list of c(F_i)
   for the successive stages. DONE </li>
   <li> In monitoring mode these numbers are output once a round is
   completed (compare "Maxima"-"Monitoring" in
   ComputerAlgebra/plans/general.hpp). DONE </li>
  </ul>

*/

