// Oliver Kullmann, 30.12.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Graphs/plans/general.hpp
  \brief Plans for the general module on graph-components


  \todo Update the namespaces : DONE


  \todo All sub-modules have milestones.


  \todo Clarify the relations
   <ul>
    <li> Concepts/plans/BipartiteGraphs.hpp </li>
    <li> Hypergraph concepts (see Hypergraphs/concepts/plans/general.hpp) </li>
   </ul>


  \todo Update namespace usage.


  \todo Graph generators
  <ul>
   <li> Path and cycle graphs. </li>
   <li> See for first attempts at complete and complete bipartite graphs in
   Combinatorics/Graphs/BoostSupport/Generators.hpp. </li>
   <li> Also complete k-partite graphs. </li>
   <li> Circulant graphs. </li>
   <li> Kneser grapsh (see ComputerAlgebra/Graphs), generalised Kneser graphs,
   and Johnson graphs. </li>
  </ul>


  \todo Graph operations
  <ul>
   <li> Sum </li>
   <li> Various products </li>
   <li> Complement </li>
  </ul>


  \todo Organisation
  <ul>
   <li> New module "TravellingSalesman"; see "Travelling salesman problem"
   in Applications/CombinatorialOptimisation/plans/general.hpp.
    <ol>
     <li> Does this really belong to supermodule "Graphs"? Wouldn't part
     "Optimisation" be better? But that part is only for "real optimisation" ?
     </li>
     <li> Translation of SAT problems (given in Dimacs-format) into the
     various forms of NP-complete TSP-problems (first in Maxima/Lisp). </li>
     <li> And translations between the various forms of TSP-problems. </li>
    </ol>
   </li>
   <li> New module "HamiltonianPaths"; see HamiltonianPaths/plans/general.hpp
   for SAT applications.
    <ol>
     <li> Translation of SAT-problems (given in DIMACS-format) into
     Hamiltonian-path-problems (first in Maxima/Lisp). </li>
    </ol>
   </li>
  </ul>

*/

/*!
  \namespace OKlib::Combinatorics::Graphs
  \brief Supermodule for dedicated graph algorithms

  Alias "Hyp".
*/


namespace OKlib {
  namespace Combinatorics {
    namespace Graphs {
    }
    namespace Gra = Graphs;
  }
}

