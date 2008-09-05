// Oliver Kullmann, 19.7.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Hypergraphs/Lisp/plans/Generators.hpp
  \brief Plans regarding generators for hypergraphs
  
  
  \todo Statistics
  <ul>
   <li> For all generated hypergraphs we need accompanying
   statistic-functions. </li>
   <li> As for clause-set statistics. </li>
  </ul>


  \todo Create complete r-graphs
  <ul>
   <li> Implement complete_ohg(n,r) by employing a standardised order on
   the r-subsets of {1,...,n} (see "Module Enumeration" in
   ComputerAlgebra/plans/general.hpp). </li>
  </ul>


  \todo Sudoku
  <ul>
   <li> Create Sudoku-hypergraphs (such that the Sudoko-problems can be
   expressed as strong colouring problems). </li>
   <li> For box dimension p the vertex set should be {1,...,p}^2. </li>
   <li> The 3N hyperedges (N=p^2) just correspond to the rows, columns
   and boxes (the hypergraph is thus N-uniform). </li>
  </ul>


  \todo Ramsey hypergraphs
  <ul>
   <li> One needs to revise the extreme cases; compare "Ramsey problems"
   in ComputerAlgebra/Satisfiability/Lisp/Generators/plans/RamseyProblems.hpp.
   </li>
   <li> Providing a standardised vertex set:
    <ol>
     <li> To handle large hypergraphs, we need to provide a version with
     standardised vertex names, using a standard enumeration of r-subsets
     (compare "Create complete r-graphs" above). </li>
     <li> This standardised hypergraph has then a canonical order-preserving
     isomorphism to ramsey_ohg(q,r,n). </li>
     <li> The same numbering should also be used in the C++ generator
     (see Ramsey.cpp). </li>
     <li> So that we can easily create additional clauses with Maxima,
     added then to the C++-generated files. </li>
    </ol>
   </li>
   <li> Accompanying statistics are needed.
    <ol>
     <li> See okltest_ramsey_hg for the formula for the number of
     hyperedges. </li>
     <li> Likely only basic statistics should be computed, while for example
     the size of the automorphism group etc. is handled in module.
     RamseyTheory/Lisp/Ramsey. </li>
    </ol>
   </li>
   <li> We can define Ramsey graphs for arbitrary hypergraphs G
   (ramsey_hg uses the complete r-graph).
    <ol>
     <li> The vertices are the hyperedges of G. </li>
     <li> Hyperedges are sets of hyperedges of G which are the hyperedge-set
     of a complete q-graph. </li>
     <li> If G is a graph, then we are considering the hypergraph of
     "cliques", restricted to cliques of size q, and where the clique
     is viewed as a collection of edges. </li>
     <li> We need to check whether this is the common point of view. </li>
    </ol>
   </li>
  </ul>

*/

