// Oliver Kullmann, 17.12.2005 (Swansea)
/* Copyright 2005 - 2007, 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Reductions/KLevelForcedAssignments/plans/general.hpp
  \brief Plans for the module KLevelForcedAssignments

  The special case K=2 (and extensions) is handled in module
  FailedLiteralReduction (see FailedLiteralReduction/plans/general.hpp),
  while "real" resolution reductions are handled in module Resolution
  (see Resolution/plans/Reductions.hpp). This module considers
  algorithms and data structures for the general case (arbitrary K)
  of generalised unit-clause-propagation.


  \todo DONE Update namespaces.


  \todo Connections
  <ul>
   <li> See ComputerAlgebra/Satisfiability/Lisp/Reductions/GeneralisedUCP.mac
   for Maxima-specifications. </li>
  </ul>


  \todo Create milestones.


  \todo First implementation, based on watched literals
  <ul>
   <li> The role of UCP:
    <ol>
     <li> One could consider UCP for the input as preprocessing,
     and so only consider computing r_k(F) for k >= 2, where all
     clauses of F have at least two literals. </li>
     <li> If however the recursive r_k-procedure performs UCP directly
     on its input, which seems sensible, then we do not need a special
     UCP-preprocessing. </li>
     <li> The point is that all what r_k is doing is UCP plus assuming
     certain literals. And UCP with watched literals works with whatever
     assignments are in the buffer (to be processed). </li>
     <li> These can be the initial unit-clauses found, the assumptions,
     or the forced assignments. </li>
    </ol>
   </li>
   <li> Binary clauses are taken out of F: for every literal x there are
   vectors U(x) of literals, and for {x,y} we have U(x) containing y, and
   U(y) containing x. </li>
   <li> Clauses with length >= 3 support watched literals; see
   Satisfiability/ProblemInstances/Clauses/concepts/WatchedLiterals.hpp.
    <ol>
     <li> The invariant is that watched literals are (currently) not set
     to false w.r.t. those assignments in the partial assignment which
     have been processed. </li>
     <li> The only exception is when a clause has been falsified. </li>
     <li> So processing a falsified literal means going to all the clauses
     on its watch-list and find other literals to watch in those clauses. </li>
     <li> So normally for a falsified literal the list of its watched clauses
     is just emptied, and these clauses are distributed to other literals.
     </li>
     <li> The only exception is when a unit-clause was found, or when the
     empty clause was found: in both cases that clause stays on the list.
     </li>
     <li> The reason is that if not the empty clause has been produced, then
     that clause has been satisfied, and satisfied clauses can be ignored,
     while in the case of the empty clause backtracking is performed. </li>
     <li> So the invariant should be more precise that either the watched
     literal is not set to false, or else another literal in the clause
     is set to true, and that literal is either "older" than the watched
     literal, or it is forced at the same level as the watched literal. </li>
     <li> So that when backtracking, it can never happen that the watched
     literal is still falsified while all satisfied literal in the clause
     vanished. </li>
     </li>
    </ol>
   </li>
   <li> We also need a component containing the current partial assignment,
   its presentation as total assignment, and the sequence of unassigned
   variables:
    <ol>
     <li> This component should be a model of an assignment; see
     OKlib/Satisfiability/Assignments/concepts/plans/general.hpp. </li>
     <li> One can add "forced literals" and "free literals" (via two
     member functions); call it perhaps push_forced(x) and push_free(x). </li>
     <li> These functions return a boolean, which is false iff this addition
     contradicts an assignment already active (while if it is just the same,
     then the operation has no effect). </li>
     <li> For a literal x we can request its state (via the total assignment).
     Syntax perhaps "[x]". </li>
     <li> And one can reset the state to just before the last decision
     literal; perhaps reset_last_free(). </li>
     <li> That means all forced literals from the back of the partial
     assignment as stack are taken away, until possibly one hits a decision
     literal - also this is taken away, and then the process stops. </li>
     <li> This "reset" is for making a test-assignment undone (the recursive
     process will leave exactly the assignments forced by this test-assignment
     in the assignment-component; one might also use them for local learning).
     </li>
     <li> Finally, the literals added are entered into some buffer, and
     one can pop literals from the buffer (for processing them). Perhaps
     using "pop()". Or better asking via top() for the literal to be removed,
     removing it via pop(), while by empty() one checks whether there is
     still some literal to be processed. </li>
     <li> Resetting the state assumes that all literals have been processed,
     and thereafter the buffer is empty. </li>
     <li> Assignments are considered as DNF-clauses, and thus literals themself
     are assumed to be satisfied. </li>
     <li> Perhaps best to realise the list of unassigned variables via a
     vector, which for each variable-index contains pointers to the next
     and the previous active variable (as in the OKsolver_2002). </li>
     <li> When a variable is assigned, then its predecessor and successor
     are re-linked to skip that variable. </li>
     <li> If the variable is un-assigned (by reset), then its links are
     re-activated. </li>
     <li> Additionally to f[v] for a variable v, perhaps we should supply also
     f(x) for a literal x; this would make usage much more convenient. </li>
     <li> One could also use f[x] (overloaded); however this would not work
     for integral type, and so f(x) needs to be used. </li>
     <li> See
     OKlib/Satisfiability/Assignments/TotalAssignments/AssignmentsWithBuffer.hpp
     for models. </li>
    </ol>
   </li>
   <li> Ignoring the case of finding satisfying assignments:
    <ol>
     <li> This (first) implementation shall just completely ignore the
     case that a satisfying assignment might have been found. </li>
     <li> This safes a lot of tests for satisfying assignments. </li>
     <li> This is also as r_k has been defined in [ECCC 1999, report 041].
     </li>
     <li> So the Maxima-specification is generalised_ucp_cs (see
     ComputerAlgebra/Satisfiability/Lisp/Reductions/GeneralisedUCP.mac). </li>
     <li> Additional consideration of satisfying assignments, or, better,
     autarkies found will be considered later. </li>
    </ol>
   </li>
   <li> Overall algorithmic flow:
    <ol>
     <li> The recursive procedure r(k,F,T) takes parameters k >= 0, the
     "clause-set" F (as reference), and the "total assignment" T (as
     reference). </li>
     <li> F contains the binary clauses as vectors (see above), plus the
     clauses of length at least three with watched literals. </li>
     <li> T contains the unit-clauses found, plus any other assignment. </li>
     <li> Precondition is that watched literals which are false w.r.t. T
     are in the list of assignments to be processed. </li>
     <li> Return value is a boolean, true iff the clause-set was found
     unsatisfiable. </li>
     <li> Other output is T with all additional forced assignments found, and
     F in a state consistent with T. </li>
     <li> If k=0, then F is just checked whether it contains the empty clause.
     </li>
     <li> If k=1, then just UCP is performed. </li>
     <li> If k >= 2, then first UCP is performed (on T), and then a loop
     through all (elementary) assignments is performed, seeking for a
     contradiction at level k-1. </li>
     <li> So a literal x is "assumed", added via T.push_free(x), and r(k-1,F,T)
     is called. </li>
     <li> If the return-value is true (a contradiction was found), then
     T.reset() is performed, and then T.push_forced(-x), and UCP is performed.
     </li>
     <li> Otherwise just T.reset() is performed. </li>
     <li> The loop over all other assignments is finished in any case, and
     restarted iff some reduction was found. </li>
    </ol>
   </li>
   <li> Performing UCP:
    <ol>
     <li> One assignment x (i.e., x -> 1) is popped from T. </li>
     <li> First one runs through all binary clauses activated by this
     assignment, and enters the assignment into T (as forced, of course). </li>
     <li> Then one runs through all watched clauses for -x, and requests a
     new watched literal y. </li>
     <li> The clause is then added to the watch-list of y. </li>
     <li> And this clause is removed from the watch-list of -x. </li>
     <li> If during these processes a contradiction was found, then the whole
     process stops. </li>
     <li> And the whole procedure returns with "contradiction found", since
     UCP is performed upon entry. </li>
    </ol>
   </li>
   <li> Watched literals:
    <ol>
     <li> The "user" of watched literals doesn't bother about whether it is
     the "first" or "second" watched literal. </li>
     <li> It is the clause which needs to store this; best it just stores the
     two watched literals as literals (together with other information if
     needed), and then when called to update a watched literal, the literal
     is given as parameter (to be checked against the two watched ones). </li>
    </ol>
   </li>
  </ul>

*/

/*!
  \namespace OKlib::Satisfiability::Reductions::KLevelForcedAssignments
  \brief Components for generalised unit-clause elimination

  Alias "Klfa".
*/

namespace OKlib {
  namespace Satisfiability {
    namespace Reductions {
      namespace KLevelForcedAssignments
      }
      namespace Klfa = KLevelForcedAssignments;
    }
  }
}

