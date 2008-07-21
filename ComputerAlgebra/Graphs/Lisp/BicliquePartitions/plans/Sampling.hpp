// Oliver Kullmann, 21.6.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Graphs/Lisp/BicliquePartitions/plans/Sampling.hpp
  \brief Plans regarding sampling of biclique partitions in (general) graphs


  \todo Sampling maximal vertex-bicliques
  <ul>
   <li> First task, given an ordered graph, find a maximal ordered biclique.
    <ol>
     <li> Parameter 0 <= p <= 1. </li>
     <li> Run through the vertices in the given order, try to add it to
     one of the sides of the biclique, and if both are possible, then
     use p for a random choice. </li>
     <li> One possibility for the check is that the new vertex is always
     checked against all vertices already in the biclique. </li>
     <li> The other possibility is that for both parts of the biclique
     we maintain the set of vertices addable to it, and then for a new
     vertex we only need to check membership. </li>
    </ol>
   </li>
  </ul>


  \todo Uniformly sampling biclique partitions
  <ul>
   <li> Given the uniform probability distribution on the set of all biclique
   partitions of a given general graph, can we efficiently sample it?
   <li> This appears to be difficult. </li>
   <li> However as a "benchmark" this is relevant. </li>
  </ul>


  \todo Random processes
  <ul>
   <li> A simple approach is to construct random processes P for constructing
   random biclique partitions.
   <li> A simple model P1(p) is, given a probability p, find a random maximal
   biclique MB, keep edges with probability p, obtain a biclique B,
   remove B, and repeat the process. </li>
   <li> The fundamental problem with this edge-oriented approach is that
   edges cannot be eliminated in isolation, but eliminating one edge means
   eliminating its two endpoints (with all incident edges). </li>
   <li> So likely the vertex-oriented approach is better. </li>
   <li> Given a biclique MB = K_{a,b}, one can construct random subsets of
   the two sides (each subset equally likely, or each size equally likely,
   or considering some fixed size etc.), and obtain so B. </li>
   <li> This yields a process P2, allowing many parameterisations. MB here
   is constructed by choosing a random order of the vertices and then
   including every vertex if possible (in this order). </li>
   <li> Given a parameter p, we can construct a simpler process P3(p) as
   follows:
    <ol>
     <li> Set B := {} and choose a random ordering v_1, ..., v_n of the
     vertices. </li>
     <li> Run trough v_1, ..., v_n, and include v_i into B with
     probability p if {v_i} union B yields a biclique. </li>
     <li> Output B. </li>
    </ol>
   </li>
   <li> For the cases we can enumerate all possible biclique partitions, we
   should run experiments with the different random process in order to
   estimate which probability distributions they induce:
    <ol>
     <li> That is, we repeat the process many times and determine the frequence
     that a biclique partition is computed. </li>
     <li> Once we have the data, we can also consider isomorphism types of
     biclique partitions are their frequencies of being chosen. </li>
    </ol>
   </li>
   <li> Sampling of general graphs is definitive of high interest, since
   it yields an alternative random formula generator (for SAT problems)!
    <ol>
     <li> However we should also consider special graph types for G. </li>
     <li> One example is G = K_{n,m} (complete bipartite graphs); see
     below "The conjecture of [Galesi, Kullmann]". </li>
     <li> Another example is G = K_n (complete graphs), which yields
     1-regular hitting clause-sets. </li>
     <li> We should also consider various forms of random graphs G. </li>
     <li> For the latter we should study the distribution of SAT/UNSAT,
     and whether threshold phenomena occur. </li>
     <li> Of course, in all cases the (empirical) hardness of the created
     SAT problems for SAT solvers is of interest. </li>
    </ol>
   </li>
  </ul>

*/

