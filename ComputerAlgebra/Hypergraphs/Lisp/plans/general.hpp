// Oliver Kullmann, 30.11.2007 (Swansea)
/* Copyright 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Hypergraphs/Lisp/plans/general.hpp
  \brief Plans for Maxima-components for hypergraphs


  \todo Create milestones


  \todo Write tests


  \todo Redesign
  <ul>
   <li> See "Plan the redesign" in ComputerAlgebra/plans/Maxima.hpp for
   the general overview. </li>
   <li> See "Lists instead of sets" and "Naming conventions" below. </li>
  </ul>


  \todo Lists instead of sets
  <ul>
   <li> Additionally to "hypergraphs" and "general hypergraphs" we introduce
   "ordered hypergraphs" and "ordered general hypergraphs":
    <ol>
     <li> Computationally these notions are considered to be more fundamental.
     </li>
     <li> An ordered hypergraphs is a pair [V,E], where V and E are lists
     without repetition, such that [setify(V),setify(E)] is a hypergraph. </li>
     <li> An ordered general hypergraph is a triple [V,E,f], where V, E
     are lists without repetition, such that [setify(V),setify(E)] is a
     general hypergraph. </li>
    </ol>
   </li>
  </ul>


  \todo Naming conventions
  <ul>
   <li> "hg" for hypergraph, "ghg" for general hypergraph, and "ohg", "oghg"
   for the ordered versions. </li>
   <li> Conversions then as "hg2ghg", "ohg2hg" etc. </li>
  </ul>


  \todo Organisation
  <ul>
   <li> Shouldn't Hypergraphs/Lisp/SetSystems.mac have its own module? </li>
   <li> Perhaps in ComputerAlgebra/DataStructures ? </li>
  </ul>


  \todo Write docus
  <ul>
   <li> We should have a list of all available functions. </li>
   <li> Seems that this list needs to be maintained manually? </li>
  </ul>


  \todo Set creation
  <ul>
   <li> It would be nice to have "create_set", as we have "create_list". </li>
   <li> However
   \verbatim
create_set([A]) := setify(apply(create_list,A))$
   \endverbatim
   does not work, since apply evaluates the argument A. </li>
   <li> It should be possible to make this work, but perhaps this is not 
   worth the effort? </li>
   <li> Ask on the Maxima mailing list. </li>
  </ul>


  \todo Hypergraphs/Lisp/Generators.mac
  <ul>
   <li> Create complete r-graphs. </li>
   <li> Create Sudoko-hypergraphs (such that the Sudoko-problems can be
   expressed as strong colouring problems).
    <ol>
     <li> For box dimension p the vertex set should be {1,...,p}^2. </li>
     <li> The 3N hyperedges (N=p^2) just correspond to the rows, columns
     and boxes (the hypergraph is thus N-uniform). </li>
    </ol>
   </li>
   <li> Create Ramsey graphs etc. </li>
  </ul>


  \todo Intersecting hypergraphs
  <ul>
   <li> See "Allowed parameter values" in
   ComputerAlgebra/Satisfiability/Lisp/ConflictCombinatorics/plans/HittingClauseSets.hpp
   for a motivation to study intersecting hypergraphs (no empty hyperedges,
   and every two hyperedges intersect). </li>
  </ul>

*/

