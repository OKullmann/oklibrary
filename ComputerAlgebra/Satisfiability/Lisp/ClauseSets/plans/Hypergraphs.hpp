// Oliver Kullmann, 19.6.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/ClauseSets/plans/Hypergraphs.hpp
  \brief Plans regarding graphs and hypergraph constructions from clause-sets


  \todo Organisation
  <ul>
   <li> Should we distinguish between Graphs.mac and Hypergraphs.mac? </li>
  </ul>


  \todo Lazy combinatorial matrices
  <ul>
   <li> For the different combinatorial matrices of clause-sets, it seems
   best to provide all three different types of implementations (compare
   the discussion "Memoisation for general graphs and multigraphs" in
   ComputerAlgebra/Graphs/Lisp/plans/general.hpp). </li>
   <li> The cm's which always (re)compute their values, as we have them
   now, provide the definitions. </li>
   <li> Perhaps from them via some general framework we obtain the instantiated
   matrices (with values stored in arrays), and the hashed matrics. </li>
  </ul>
   

  \todo Implication graphs
  <ul>
   <li> DONE Given a (boolean) clause-set F, extract the clauses {a,b} of
   length 1 or 2, and create the directed graph with vertices the literals,
   and edges from -a to b and from -b to a. </li>
   <li> Using graph_draw(dg2mdg(implication_dg_fcs(FF))):
    <ol>
     <li> The basic problem is that the vertex labels often cannot be seen
     properly. </li>
     <li> If the graph is rather small,
     \verbatim
draw_graph(G,show_label=true,head_length=0.05,label_alignment=center,vertex_size=5,head_length=0.1);
     \endverbatim
     yields reasonable results. </li>
    </ol>
   </li>
  </ul>

*/

