// Oliver Kullmann, 13.7.2008 (Swansea)
/* Copyright 2008, 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Matroids/Lisp/plans/general.hpp
  \brief Plans regarding matroids in Maxima/Lisp


  \todo Fundamental notions
  <ul>
   <li> A "matroid" is a special type of hypergraph. </li>
   <li> A problem are the those many variations, i.e., whether to specify
   a matroid by the "independent subsets", by "circuits" etc. </li>
   <li> Let "mtr" be the abbreviation for "matroid"; then perhaps we
   have "mtrx" for the variations:
    <ol>
     <li> "ins" for independent subsets (a hypergraph [V,M] such that M is
     non-empty, hereditary (stable under subset-formation), and fulfils
     the augmentation property, i.e., for A, B in M with |A| < |B| there exists
     x in B - A with A + {x} in M; for implicitly given matroids, which
     could be infinite, we need further distinctions); </li>
     <li> "ds" for dependent subsets; </li>
     <li> "cr" for circuits; </li>
     <li> "bs" for bases; </li>
     <li> "rk" for a rank-function; here we don't have a hypergraph but
     a pair [V,f], where V is a set and f yields for every subset of V
     a natural number (with zero). </li>
     <li> "clos" for closure; perhaps we have a module in
     ComputerAlgebra/Sets, where we handle closure systems, i.e.,
     pairs [X,f], X a set and f a map, which transforms subsets into subsets,
     in general. </li>
    </ol>
   </li>
   <li> The hypergraph-based notions should be given more "notational
   prominence" (than the others), perhaps always using 2-letters ? </li>
   <li> Important to be complete and precise. </li>
   <li> Important that all dual notions are easily at hand, perhaps the letter
   "d" is reserved to indicate "dual notions". </li>
   <li> We also need to be compatible with oriented matroids; see
   ComputerAlgebra/Matroids/Lisp/OrientedMatroids/plans/general.hpp. </li>
   <li> The direct definition is inefficient:
    <ol>
     <li> The possibility of implicit representations of the base set is of
     importance especially for infinite matroids. </li>
     <li> And perhaps the existential quantifiers in the axioms can
     additionally be realised, e.g. for "_is" a function f(A,B) which
     yields x in B - A. </li>
     <li> In this way, through the different axiomatisations, many different
     functions arise, given directly or derived. One needs a system for that.
     </li>
     <li> Perhaps we use for example "isr", that is, "independent set
     realiser". One could also use "issf" ("sf" for "Skolem function"). </li>
     <li> Perhaps important also the variation where e.g. f(A,B) is the set
     of *all* x in B - A s.t. A + x is independent. </li>
     <li> Importantly, also for the various set systems (independent sets,
     circuits, etc.) we can have explicit and implicit representations. One
     could use "imp" for "implicit", and "exp" for "explicit". </li>
     <li> Perhaps we make implicit representation of the set of
     independent subsets the default, while for the bases the explicit
     representation is the default. </li>
    </ol>
   </li>
  </ul>


  \todo Examples
  <ul>
   <li> Vector matroid M[A]:
    <ol>
     <li> Given a combinatorial matrix A over a skew field, subsets of the
     column-set, or, alternatively, of the row-set, are independent iff they
     are linearly independent. </li>
     <li> With the Maxima function "rank" one can handle the field of reals or
     the field of rationals, while for other fields apparently no functions are
     provided, and so we need to provide generic functions ourselves. </li>
     <li> See "Missing functionality" in
     ComputerAlgebra/Algebra/Lisp/plans/FiniteFields.hpp. </li>
    </ol>
   </li>
   <li> Cycle matroid M(G):
    <ol>
     <li> Given a general graph G, the sets of edges of cycles yield the set of
     circuits of a matroid on E(G). </li>
     <li> Here the check is easy: Compute the induced subgraph and check
     whether it is a cycle graph; this is done by cycle_gg_p and
     edge_induced_subgraph_gg. </li>
    </ol>
   </li>
  </ul>


  \todo Greedy algorithm
  <ul>
   <li> Given a hereditary hypergraph [V,E] with non-empty E, and a weight
   function w: V -> RR, the greedy algorithm determines a maximal element B
   of E as follows:
    <ol>
     <li> B := {}; </li>
     <li> if there is no x in V - B with B + {x} in E, return B; </li>
     <li> otherwise choose such x with w(x) maximal, set B := B + {x},
     and repeat from the previous step. </li>
    </ol>
   </li>
   <li> The hereditarity condition exactly ensures that every B could be
   chosen. </li>
   <li> [V,E] is a matroid if and only if the greedy algorithm yields for
   every w a maximal element of E which is also of maximal weight (amongst
   all maximal elements of E). </li>
   <li> The two main variation seem to dependent on how to find x:
    <ol>
     <li> Either one has to run through all of V - B, </li>
     <li> or a special function computes them. </li>
    </ol>
   </li>
   <li> So here for more efficient implementation a function f(I) would be
   needed, which for a given independent set I computes the set of all
   x in V - I s.t. I + {x} is still independent.
    <ol>
     <li> For example for Kruskal's algorithm, f needed internal state,
     and would start with all edges possible, and with every edge added
     to B would remove all edges which now lead to cycles. </li>
     <li> How is Kruskal's algorithm doing that? </li>
    </ol>
   </li>
   <li> The greedy algorithm can find all bases of maximal weight; if x
   is always chosen randomly, do we then sample all bases fairly? </li>
  </ul>

*/

