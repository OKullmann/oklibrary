// Oliver Kullmann, 20.12.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/ProblemInstances/Clauses/concepts/WatchedLiterals.hpp
  \brief Plans for concepts for clauses with "watched literals"


  \todo Basic concepts
  <ul>
   <li> Motivated by the submission of Armin Biere to SAT2007, for the
   look-ahead the clauses could manage the two watched literals in them
   (inclusive the two pointers to the respective next clauses). </li>
   <li> The main point here is to find out (as easily as possible) when
   possibly a clause became unit. </li>
   <li> Also interesting here to find out when possibly a clause became binary
   (here we would need three watched literals per clause). </li>
   <li> Taking care of those special literals in a clause would be the
   responsibility of the clause (not some higher-level mechanism). </li>
   <li> For a first usage, see "First implementation, based on watched
   literals" in Reductions/KLevelForcedAssignments/plans/general.hpp. </li>
   <li> And for its use in an UCP-application see
   Satisfiability/Reductions/UnitClausePropagation/UnitClausePropagationW.cpp.
   </li>
   <li> "Head-tail clauses" perhaps don't need a different concept? See
   "Head-tail clauses" in ProblemInstances/Clauses/plans/general.hpp for
   plans for a first implementation. </li>
   <li> The basic concept:
    <ol>
     <li> Construction by a sequence of literals (as usual); at least
     two literals. </li>
     <li> Offering the functionality of an immutable sequence of literals.
     </li>
     <li> By first() and second() we get the two watched literals. </li>
     <li> Actually, this might not be needed. </li>
     <li> By remove_first/second(TotalAssignment f) a new first/second
     watched literal is requested; returned is a pair (sat-status, literal),
     where the literal is null iff it has not been changed. </li>
     <li> This is definitely the case when the clause was found falsified.
     </li>
     <li> In case the clause was found satisfied also the watched literal
     can stay unchanged. </li>
     <li> An alternative is to just use remove(Lit x, TotalAssignment f),
     where it is the reponsibility of the clause to handle x as "first"
     or "second" watched literal. This makes usage easier. </li>
     <li> And actually the return-type should just be a literal, with
     the following three cases:
      <ol>
       <li> it is 0 iff the clause is falsified; </li>
       <li> it is x (the input literal) iff the clause is satisfied; </li>
       <li> otherwise it is the new literal. </li>
      </ol>
     </li>
    </ol>
   </li>
   <li> See Clauses::WatchedLiterals_mono in
   OKlib/Satisfiability/ProblemInstances/Clauses/WatchedLiterals.hpp for a
   first prototype. </li>
  </ul>

*/
