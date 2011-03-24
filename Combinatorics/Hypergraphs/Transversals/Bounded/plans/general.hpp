// Oliver Kullmann, 22.9.2007 (Swansea)
/* Copyright 2007, 2009, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Combinatorics/Hypergraphs/Transversals/Bounded/plans/general.hpp
  \brief Plans for the module on computing hypergraph transversals
  of size at most k


  \todo Connections to other modules
  <ul>
   <li> See ComputerAlgebra/Hypergraphs/Lisp/Transversals/Bounded/general.hpp
   for general plans at computer-algebra level. </li>
   <li> Satisfiability/Transformers/Generators/BoundOnes translates the
   problem into a pseudo-boolean problem (using bound "<= k"; alternatively
   also "= k" is possible). </li>
   <li> Via Satisfiability/Transformers/Generators/LinInequal.cpp one
   can add the clauses expressing the bound "<= k" (or "= k"), obtaining
   an ordinary SAT problem. </li>
   <li> See Investigations/Transversals/Bounded/plans/general.hpp for
   investigations. </li>
  </ul>


  \todo Trivial algorithm for input G, k
  <ul>
   <li> The task is to find a transversal T with |T| <= k. </li>
   <li> Outline of the algorithm:
    <ol>
     <li> Another input is a vertex set T of size <= k, to be
     extended to a transversal of size at most k. </li>
     <li> If all hyperedges are covered, return T. </li>
     <li> Otherwise, if |T| is of size = k, then return "impossible". </li>
     <li> Otherwise choose some hyperedge H which is not hit by T, and for all
     v in H try recursively T + {v}. </li>
    </ol>
   </li>
   <li> If a minimal transversal T is needed (%w.r.t. set-inclusion),
   then by some heuristics unnecessary vertices in T are removed
   (iteratively). </li>
   <li> For the choice of H the simplest heuristics is to choose a
   smallest H.
    <ol>
     <li> Simplest to order the hyperedges once at the beginning. </li>
     <li> Stronger if the hypergraph datastructure gives access to the
     current smallest hyperedge; but this is not enough, we need the
     smallest *uncovered* hyperedge (one whose size has not changed
     since the beginning). </li>
    </ol>
   </li>
   <li> For the order of vertices, the simplest heuristics is to order
   them in descending degree. </li>
   <li> Special services needed from the hypergraph:
    <ol>
     <li> Necessary is a largest hyperedge. </li>
     <li> For heuristical purposes a smallest hyperedge (strongest:
     amongst the untouched hyperedges). </li>
     <li> For heuristical purposes a vertex of largest degree. </li>
    </ol>
   </li>
  </ul>


  \todo An "eager, binary" variation of the (above) trivial algorithm
  <ul>
   <li> The algorithms transversals_be, transversals_bes in
   ComputerAlgebra/Hypergraphs/Lisp/Transversals/Bounded/MaintainingBound.mac
   implement the above algorithm, but with the following two differences:
    <ol>
     <li> Above we use "clause branching", while these algorithms use
     binary branching; this should be better. </li>
     <li> Above the handling of the hypergraph is "lazy" (not performing
     the assignment), while these algorithms are "eager" (actually performing
     the assignment). </li>
    </ol>
   </li>
   <li> DONE (implemented transversals_bvs, but without any further
   embellishments)
   We should directly implement the algorithms transversals_be,
   transversals_bes in C++:
    <ol>
     <li> As a first prototype for the concept of hypergraphs as (just)
     containers of hyperedges; here perhaps using std::vector for the
     list, and std::set for the hyperedges. </li>
     <li> A simple generic strategy provides the possibility to use
     different heuristics for the choice of the branching vertex. </li>
     <li> For vertices we just have integers in mind (but this is not
     hardcoded). </li>
    </ol>
   </li>
   <li> This would extend the already existing "non-abstract" hypergraph
   concepts; programming at this level quite similar to the Maxima/Lisp level.
   </li>
   <li> Compare also the point below about enumerating all transversals. </li>
  </ul>


  \todo Enumerating all transversals T with |T| <= k
  <ul>
   <li> The above trivial algorithm also enumerates all transversals
   T with |T| <= k, when just continuing the process until the end.
    <ol>
     <li> However, to really obtain all such transversals, one needs to
     use the set of formal vertices, not just those left over. </li>
     <li> One should construct a little example. </li>
     <li> Of course, we are not interesting in computing all transversals
     (from the minimal transversals we trivially could obtain them by
     padding), but the point is that considering only the existing vertices
     already achieves some simple form of "tree pruning". </li>
     <li> See the specification of transversals_be in
     ComputerAlgebra/Hypergraphs/Lisp/Transversals/Bounded/MaintainingBound.mac.
     </li>
    </ol>
   </li>
   <li> However now we really only want to find minimal transversals (%w.r.t.
   inclusion).
    <ol>
     <li> This can be achieved by computing all and then applying
     subsumption elimination, but we need also more efficient solutions. </li>
     <li> It seems natural, as above for a transversal T found just to minimise
     it (trivially), and then to prune parts of the tree which cannot
     contribute a minimal transversal anymore. This process is similar to
     tree pruning ("intelligent backtracking") for SAT. </li>
     <li> Of course, if k is at most the transversal number, then no further
     steps are needed; this situation arises naturally when either searching
     for a minimum transversal from below, or when computing transversal
     numbers for monotonically increasing sequences of hypergraphs (at one
     new vertex at each step); see
     Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/Transversals/plans/general.hpp.
     </li>
     <li> Perhaps actually such pruning cannot be done efficiently? It should
     be the case that when branching on a still occurring vertex, then the
     subtree contains at least one minimal transversal (if not already some
     superfluous vertex has been included)? </li>
    </ol>
   </li>
  </ul>


  \todo Application BoundedTransversals_bv
  <ul>
   <li> Perhaps we should provide some variations.
    <ol>
     <li> Instead of using sets sorted by size one could just use sets
     (this should be slower). </li>
     <li> Instead of using std::set, one could use std::vector, and sort
     it when needed. </li>
    </ol>
   </li>
   <li> The various set-operation used in Transversals::Bounded_transversals_bv
   should be provided independently, so that the code becomes clearer. </li>
   <li> And G_with_a could be a vector. </li>
  </ul>


  \todo As generalised-SAT algorithms
  <ul>
   <li> The above algorithms can be viewed as very simple backtracking
   generalised-SAT solvers, where the condition on the number of 1-settings
   in the partial assignment is integrated into the backtracking mechanism.
   </li>
  </ul>


  \todo Overview on more sophisticated algorithms

*/

/*!
  \namespace OKlib::Combinatorics::Hypergraphs::Transversals::Bounded
  \brief Components for handling hypergraph transversals of bounded size

  Alias "Bnd".
*/

namespace OKlib {
  namespace Combinatorics {
    namespace Hypergraphs {
      namespace Transversals {
        namespace Bounded {
        }
        namespace Bnd = Bounded;
      }
    }
  }
}

