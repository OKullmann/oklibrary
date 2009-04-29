// Oliver Kullmann, 28.4.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/LocalSearch/plans/HypergraphColouring.hpp
  \brief Plans for local search algorithms specifically for hypergraph colouring


  \todo Relations to other modules
  <ul>
   <li> For applications see
   Experimentation/Investigations/RamseyTheory/GreenTaoProblems/plans/general.hpp.
   </li>
   <li> See Combinatorics/Hypergraphs/Colourings/plans/general.hpp for the
   C++ level. </li>
   <li> See ComputerAlgebra/Hypergraphs/Lisp/plans/Colouring.hpp for general
   plans on hypergraph colouring. </li>
  </ul>


  \todo First prototypes
  <ul>
   <li> According to the motivation by computing Green-Tao numbers (see
   Investigations/RamseyTheory/GreenTaoProblems/plans/general.hpp, especially
   "Faster local search"), we first only consider rnovelty+ and adaptnovelty+.
   </li>
   <li> The difference to running a SAT solver is in the more efficient data
   representation and in, when using the standard translation, not letting
   the amo- and alo-clauses interfere. </li>
   <li> In order to be able to compare results with the boolean CNF encoding,
   we should also implement rnovelty+ and adaptnovelty+ for boolean and
   for non-boolean clause-sets. </li>
  </ul>

*/

