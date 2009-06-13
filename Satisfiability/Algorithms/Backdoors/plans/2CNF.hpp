// Oliver Kullmann, 21.9.2007 (Durham)
/* Copyright 2007, 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Algorithms/Backdoors/plans/2CNF.hpp
  \brief Plans on the module for backdoor algorithms targeting 2CNF


  \todo Design for the algorithm "sb-2cnf" from [Nishimura, Ragde, Szeider;
  2004, Detecting Backdoor Sets with Respect to Horn and Binary
  Clauses] (developers SS, MS, OK)
  <ul>
   <li> See ComputerAlgebra/Satisfiability/Lisp/Backdoors/plans/general.hpp.
   </li>
   <li> Input-output:
    <ul>
     <li> Needed is the clause-set F, together with a heap
     of pairs of (clause-pointer, current clause-size), making
     available a clause of maximal current size. </li>
     <li> The algorithm is a simple recursive function, taking
     the clause-set via const reference, and the heap a reference
     parameter. </li>
     <li> Further parameters are the maximal depth k
     and the current variable-set V. </li>
     <li> Better, we have an enclosing class with the following
     data members:
      <ol>
       <li> The clause-set F (const; input). </li>
       <li> The maximal size k (const, input). </li>
       <li> V (being expanded and shrunk; input and output). </li>
       <li> The heap (private). </li>
      </ol>
     </li>
     <li> Return value of the member function "search"
     is a boolean, indicating whether V can be extended to a backdoor set
     of size at most k; upon success V is set. </li>
    </ul>
   </li>
   <li> The algorithm:
    <ul>
     <li> The input-precondition of the algorithm (an invariant of the class)
     is that the heap contains the current clause-sizes of V * F. </li>
     <li> If there is no clause or the current maximal clause-size is
     at most k, then "success" is returned. If otherwise
     the size of V is k, then "no success" is returned. </li>
     <li> Otherwise the top clause is obtained from the heap, three
     variables are choosen with maximal occurrence numbers (in V*F;
     if this information is not available, then in F; otherwise
     just any three), and the algorithm branches recursively
     in three branches, removing one of the three variables in
     each branch. </li>
     <li> When branching and when backtracking, the heap is updated
     by running through all occurrences of the variables (as in the
     original F --- this doesn't change), and either decrementing or
     incrementing the current clause-sizes. And also the chosen variable
     is added to V or removed. </li>
    </ul>
   </li>
   <li> It would make sense to offer a concept of a clause-set with
   clause-lengths available and with the operation of crossing out
   variables and undoing this operation. Though the operation of retrieving
   a clause of maximal size is rather unusual. Perhaps such considerations
   are better postponed, until some experience has been gained. </li>
   <li> The algorithm works with any type of one-variable-literals (it
   considers only the variable structure), and should be able to run on such
   generalised clause-sets (a poly-time backdoor is only given in general
   for boolean variables, but that doesn't matter for the algorithm (the
   problem just being whether a variable set V with |V| <= k exists, such that
   V * F contains only clauses of size at most 2 --- this just operates on the
   underlying variable hypergraph)). </li>
   <li> So perhaps it would make sense to ask just for a hypergraph as input?
   </li>
   <li> And it wouldn't make a difference to the algorithm if instead of trying
   to make a graph out of the given hypergraph, more generally one tries to
   create an s-graph (the graph case is s=2). </li>
   <li> This complete algorithm likely works out only for small k. Are there
   interesting incomplete versions (which could handle larger k)?
    <ol>
     <li> One can try a greedy approach (which always returns some V,
     disregarding k). </li>
     <li> This could be randomised, and tried a couple of times. </li>
     <li> The above complete algorithm could be used with random restarts (this
     would make more sense if stronger heuristics would be used, and
     randomisation is needed). </li>
    </ol>
   </li>
   <li> One can translate the problem into a SAT problem (input a hypergraph,
   every vertex becomes a boolean variable ("crossed out or not")).
    <ol>
     <li> For every hyperedge one can express that at most s vertices are not
     crossed out. </li>
     <li> Or for every hypedge of length k on can express that k-2 vertices
     are crossed out. </li>
    </ol>
   </li>
   <li> However, the easiest reduction (which also shows the fpt-property) is
   to the hypergraph transversal problem:
    <ol>
     <li> For input hypergraph G compute the (s+1)-section G' (containing all
     (s+1)-subsets of hyperedges). The solutions to the original problem on G
     (all vertex sets V such that V * G has degree at most s) are exactly the
     transversals of G'. </li>
     <li> Since the problem of finding k-transversals in hypergraphs of
     bounded degree is fpt, we get an fpt-algorithm as required; and the
     trivial algorithm as in
     Hypergraphs/Transversals/Bounded/plans/general.hpp then yields exactly
     the above algorithm. </li>
     <li> So best is not to implement the algorithm above, but just
     instantiate the generic algorithms from
     Hypergraphs/Transversals/Bounded/plans/general.hpp. </li>
     <li> The derived hypergraph G' can be computed directly, or an "active
     hypergraph" for construction of sections is offered (which does not store
     the hyperedges, but just simulates them). </li>
    </ol>
   </li>
  </ul>

*/

