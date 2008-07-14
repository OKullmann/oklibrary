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


  \todo Bug in Maxima: corr_cartesian_product
  <ul>
   <li> DONE
   Report the bug corrected by corr_cartesian_product to the Maxima
   mailing list and to the bug-reporting system. </li>
   <li> Once corrected, remove all applications of corr_cartesian_product.
   </li>
   <li> See "Document important programming techniques" in
   ComputerAlgebra/plans/Maxima.hpp. </li>
  </ul>


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
     are lists without repetition, such that [setify(V),setify(E),f] is a
     general hypergraph. </li>
    </ol>
   </li>
   <li> Multi-versions:
    <ol>
     <li> We also should have "multi-hypergraphs", triples [V,E,c], s.t.
     [V,E] is a hypergraph, and c: E -> NN. </li>
     <li> And "ordered multi-hypergraphs, [V,E,c] s.t. [setify(V),setify(E),c]
     is a multi-hypergraph, and [V,E] is an ordered hypergraph. </li>
  </ul>


  \todo Naming conventions
  <ul>
   <li> "hg" for hypergraph, "ghg" for general hypergraph, and "ohg", "oghg"
   for the ordered versions. </li>
   <li> "mhg" for multi-hypergraph, and "omhg" for ordered multi-hypergraph.
   </li>
   <li> Conversions then as "hg2ghg", "ohg2hg" etc. </li>
   <li> Connections to graphs (see
   ComputerAlgebra/Graphs/Lisp/plans/general.hpp):
    <ol>
     <li> g, gl <= hg </li>
     <li> mug, mugl <= mhg </li>
     <li> gg <= ghg </li>
     <li> og, ogl <= ohg </li>
     <li> omug, omugl <= omhg </li>
     <li> ogg <= oghg </li>
    </ol>
   </li>
  </ul>


  \todo Organisation
  <ul>
   <li> Shouldn't Hypergraphs/Lisp/SetSystems.mac have its own module
    <ol>
     <li> Perhaps in ComputerAlgebra/DataStructures ? </li>
     <li> Better in ComputerAlgebra/Sets! </li>
    </ol.
   </li>
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
   does not work, since "apply" evaluates the argument A. </li>
   <li> It should be possible to make this work, but perhaps this is not 
   worth the effort? </li>
   <li> Ask on the Maxima mailing list. </li>
   <li> This should belong to ComputerAlgebra/Sets. </li>
  </ul>


  \todo Special properties of hypergraphs
  <ul>
   <li> Perhaps we have a module "Properties.mac", where basic properties
   of hypergraphs are tested. </li>
   <li> Whether a general hypergraph has repeated hyperedges. </li>
   <li> Whether a hypergraph has subsumed hyperedges. </li>
   <li> Was a hypergraph is "downward hereditary" or "upward hereditary". </li>
   <li> What about all forms of stability under set-operations? Likely these
   special hypergraphs should go to ComputerAlgebra/Sets. </li>
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

