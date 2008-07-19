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


  \todo Sudoku
  <ul>
   <li> Create Sudoku-hypergraphs (such that the Sudoko-problems can be
   expressed as strong colouring problems). </li>
   <li> For box dimension p the vertex set should be {1,...,p}^2. </li>
   <li> The 3N hyperedges (N=p^2) just correspond to the rows, columns
   and boxes (the hypergraph is thus N-uniform). </li>
  </ul>


  \todo Ramsey graphs
  <ul>
   <li> Accompanying statistics are needed.
    <ol>
     <li> See okltest_ramsey_hg for the formula for the number of
     hyperedges. </li>
    </ol>
   </li>
   <li> We should perhaps have a module ComputerAlgebra/RamseyTheory. </li>
   <li> There we would have NR(L,r) for a list L of natural numbers:
    <ol>
     <li> Containing somehow everything known about Ramsey-numbers (i.e.,
     NR([3,3],2)=6 and NR([4,4],2)=18). </li>
     <li> Best is to return a pair [lower bound, upper bound]. </li>
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
    </ol>
   </li>
  </ul>

*/

