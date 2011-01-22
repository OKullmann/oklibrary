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

*/
