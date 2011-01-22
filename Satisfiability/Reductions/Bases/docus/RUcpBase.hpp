// Matthew Gwynne, 21.1.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Reductions/Bases/docus/RUcpBase.hpp
  \brief Docus file for r_1-base computations at the C++ level


  <h1> Computing r_1 bases (in C++) </h1>

  <h2> Basic definitions </h2>

  Consider a reduction r and a clause-set F.
  <ul>
   <li> An r-base of F is a sub-clause-set F' of F where all the clauses of F
   follow from those in F' by r. </li>
   <li> That is, for all C in F we have that r(phi_C * F') contains the empty
   clause. </li>
   <li> Here phi_C is the partial assignment associated with the clause C, and
   "*" is the operator for the application of partial assignments. </li>
  </ul>

  A simple algorithm to compute such an r-base (not necessarily a minimum
  one) is to go through all clauses C of F and check whether after
  removing C, all clauses removed so far still follow by r.
  
  This algorithm is presented for r_1, i.e., "unit-clause-propagation", in
  Satisfiability/Reductions/Bases/RUcpBase.cpp.


  <h2> Computing r_1 bases </h2>

  To compute an r_1-base for a clause-set given in <code>F.cnf</code>, one runs
  \verbatim
> RUcpBase-O3-DNDEBUG < F.cnf > Base.cnf
  \endverbatim
  which outputs the resulting base to <code>Base.cnf</code>.
  
  
  <h3> Correctness of RUcpBase </h3>

  The unit-clause-propagation object used in <code>RUcpBase</code> will return
  true (i.e., it has found the empty clause) for all assignments given to
  it after any assignment conflicts with a unit-clause in the input.

  This makes it seem like there is the possibility for false output, as
  one might have a clause in the input which causes this behaviour, and
  then some other clause, shouldn't be removable but is as the Ucp object
  returns the wrong value.

  However, this is not the case as:
  <ul> 
   <li> When checking if it can add a new clause to the removed clause list, 
   it adds it to the beginning and checks that clause first to see if it 
   follows. </li>
   <li> Therefore the only possibility is that the current clause being
   checked (call it C) contradicts some unit-clause U with literal L in the
   input, and removing C from the clause-set means another clause D,
   already removed (i.e., later in the removed list), doesn't follow any more.
   </li>
   <li> However, if D no longer follows, then C must have been involved
   in some unit-clause propagation when applying phi_D. </li>
   <li> For C to create a contradiction with U, it must contain L,
   and therefore, we have two options:
   <ol>
    <li> phi_D applied to the clause-list including C involves C propagating 
    L, in which case we get this anyway from U. </li>
    <li> Applying phi_D involves propagating on another literal in C, which
    means that phi_D must set -L, which will invalidate U, and therefore
    again we get falsehood. </li>
   </ol>
   </li>
   <li> In other words, in any such case, we can actually remove the clause 
   anyway, as the unit-clause that contradicts with removed clause yields
   the same propagations (i.e., we can always removed such subsumed clauses).
   </li>
  </ul>
  

*/
