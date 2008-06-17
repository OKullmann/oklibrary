// Oliver Kullmann, 23.5.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Graphs/Lisp/plans/BicliquePartitions.hpp
  \brief Plans regarding biclique partitions of (general) graphs


  \todo Basic concepts
  <ul>
   <li> A "biclique partition" of a general graph is a partition of the
   edge set such that each part constitutes a complete bipartite graph.
    <ol>
     <li> We take "partition" here in the formal sense, that is, a set of
     subsets, none empty, union the whole, pairwise disjoint. </li>
     <li> Later we also need "polarised" versions where the parts of the
     biclique are also specified. </li>
     <li> Here we need, given an edge-subset of a general graph, to
     construct the "induced" general sub-graph, whose vertex set is
     just the vertices used by those edges. </li>
     <li> Then we need a check for simplicity of a general graph. </li>
     <li> And then we need a bipartiteness test for general graphs
     (using the Maxima function). </li>
    </ol>
   </li>
   <li> Write predicate "bicliquepartp" to check for this property. </li>
   <li> Likely this more algebraic aspects of graph theory is handled
   better by combinatorial matrices; see
   ComputerAlgebra/CombinatorialMatrices/Lisp/plans/general.hpp. </li>
   <li> Also for translating biclique partition into clause-sets we
   do most work at the level of combinatorial matrices. </li>
  </ul>


  \todo Enumerating biclique partitions
  <ul>
   <li> Given a general graph G, enumerate all biclique partitions. </li>
   <li> This is the same as finding all clause-sets without pure literals
   with a given conflict multigraph (namely G). </li>
   <li> The simplest approach:
    <ol>
     <li> Via set_partitions(edge_set) create all possible partitions
     of the edge-set, and then we compute the subset given by bicliquepartp.
     </li>
    </ol>
   </li>
   <li> Alternatively, we translate the biclique partition problem into
   a SAT problem (see below) and enumerate all solutions.
    <ol>
     <li> However, this is then a harder problem, since we demand certain
     properties of the biclique partition. </li>
    </ol>
   </li>
   <li> Can we write an efficient generator, which creates one biclique
   partition at a time, with polynomial delay, running through all biclique
   partitions without repetition?
    <ol>
     <li> For example, running through the lexicographical ordering?! </li>
     <li> Most natural seems to consider a backtracking approach. </li>
    </ol>
   </li>
   <li> Given all biclique partitions, we can then find all isomorphism
   types:
    <ol>
     <li> Two biclique partitions are isomorphic if the associated clause-sets
     are isomorphic. </li>
     <li> The problem of finding all isomorphism types is then the same as
     determining all isomorphism types of clause-sets without pure literals
     and with conflict multigraph G. </li>
    </ol>
   </li>
   <li> Again the question is whether there is an efficient generator for
   all isomorphism types?
    <ol>
     <li> Definitely we do not need to generate first all biclique partitions.
     </li>
     <li> Instead we use a backtracking approach, where we investigate new
     branches only if they are not isomorphic to branches already considered.
     </li>
    </ol>
   </li>
  </ul>


  \todo Sampling biclique partitions
  <ul>
   <li> Given the uniform probability distribution on the set of all biclique
   partitions of a given general graph, can we efficiently sample it?
    <ol>
     <li> This appears to be difficult. </li>
     <li> So best we start with simpler approaches. </li>
    </ol>
   </li>
   <li> A simpler approach is to construct random processes P for constructing
   random biclique partitions.
    <ol>
     <li> A simple model P1(p) is, given a probability p, find a random maximal
     biclique MB, keep edges with probability p, obtain a biclique B,
     remove B, and repeat the process. </li>
     <li> For 0 < p < 1 this is complete. Likely, for every p the induced
     probability is not the uniform one, since emphasise is put on larger
     bicliques. </li>
     <li> For p = 1/2, B is a random subset of MB (i.e., all subsets of MB
     are equally likely.
      <ul>
       <li> The empty subset has to be rejected. </li>
       <li> A different process (resulting in P2(p) is to choose a random
       number k from 1 to |MB|, and then to choose a random subset of size
       k. </li>
      </ul>
     </li>
     <li> Finding a "random maximal biclique" perhaps can be achieved by
     the process of selecting a random edge, then again, amongst those edges
     which form a biclique with the given ones, choose a random edge,
     and repeat this process. </li>
     <li> Process P1(p) then should be equivalent to the following,
     simpler one:
      <ol>
       <li> Set B := {} and choose a random ordering e_1, ..., e_m of the
       edges. </li>
       <li> Run trough e_1, ..., e_m, and including e_i into B with
       probability p if {e_i} union B is a biclique. </li>
       <li> Output B. </li>
      </ol>
     </li>
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
   <li> Sampling of of general graphs is definitive of high interest, since
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


  \todo Finding specific biclique partitions via SAT
  <ul>
   <li> See TransformationsBiclique.hpp and
   Transitional/OKsolver/plans/general.hpp. </li>
   <li> We must also give an overview on the space of possible
   generalised SAT-Translations.
    <ol>
     <li> The most basic parameter is an upper bound on the number of
     biclique. </li>
     <li> More precisely, we could have list of biclique sizes. </li>
    </ol>
   </li>
  </ul>


  \todo Tools for graphs
  <ul>
   <li> From the Maxima graphs-library:
    <ol>
     <li> complete_bipartite_graph (n, m) </li>
     <li> bipartition (gr) </li>
     <li> is_bipartite (gr) </li>
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
  </ul>

*/

