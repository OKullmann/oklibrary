// Oliver Kullmann, 23.5.2008 (Swansea)
/* Copyright 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Graphs/Lisp/BicliquePartitions/plans/general.hpp
  \brief Plans regarding biclique partitions of (general) graphs


  \todo Translations to clause-sets
  <ul>
   <li> See "Conflict graph" in
   ComputerAlgebra/Satisfiability/Lisp/ConflictCombinatorics/plans/general.hpp
   </li>
   <li> Organisation:
    <ol>
     <li> Does this belong to ComputerAlgebra/Graphs, or to
     ComputerAlgebra/Satisfiability? </li>
     <li> Formation of conflict graphs etc. seem naturally to belong to
     Satisfiability/ConflictCombinatorics, since these graphs are first of
     all tools to analyse clause-sets. </li>
     <li> What about translations of biclique partitions (and of graphs
     for given methods of biclique partitions) into clause-sets?
      <ul>
       <li> Placing them in Satisfiability/ConflictCombinatorics seems again
       most natural; perhaps in a sub-module "Realisations" ? </li>
      </ul>
     </li>
    </ol>
   </li>
  </ul>


  \todo The conjecture of [Galesi, Kullmann]
  <ul>
   <li> Using the (deprecated) Mupad-functions, the conjecture is
   that if ist_exakt_transversal(M) is true, then
   ist_speziell_exakt_transversal(M) is true. </li>
   <li> This should be equivalent to the statement, that every minimally
   unsatisfiable clause-set F, whose conflict multigraph is a complete
   bipartite graph, has a read-once resolution refutation. </li>
   <li> First approach for searching for a counter-example:
    <ol>
     <li> Enumerate or sample biclique partitions of complete bipartite
     graphs. </li>
     <li> Get the corresponding clause-set. </li>
     <li> Perform subsumption-elimination. </li>
     <li> Remove satisfiable clause-sets (via SAT or via the decision procedure
     for exact transversal hypergraphs). </li>
     <li> Check the clause-set for the special condition. </li>
    </ol>
   </li>
   <li> We need to implement a test for the special condition:
    <ol>
     <li> This should be a simple recursive procedure. </li>
     <li> ist_speziell_exakt_transversal(M) is a starting point (but
     it doesn't recursively check the condition). </li>
    </ol>
   </li>
  </ul>


  \todo NP-completeness of determining bcp
  <ul>
   <li> Find literature on the NP-completeness of the determination of the
   biclique partition number bcp(G). </li>
   <li> Implement it. </li>
  </ul>

*/

