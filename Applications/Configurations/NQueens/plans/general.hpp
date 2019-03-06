// Oliver Kullmann, 8.7.2007 (Swansea)
/* Copyright 2007, 2011, 2018 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Configurations/NQueens/plans/general.hpp
  \brief Plans on the n queens problem


  \todo Update namespace.


  \todo Create milestones.


  \todo Literature review
  <ul>
   <li> What is the complexity of finding one solution and
   of counting all solutions (in n) ?
    <ol>
     <li> A first introduction one finds in
     http://en.wikipedia.org/wiki/Eight_queens_puzzle. </li>
     <li> There is also an algorithm given for quickly finding one
     solution, so that's not a problem (but see below). </li>
     <li> The true challenge is counting all solutions. </li>
     <li> https://oeis.org/A000170 gives an overview on the computations.
     With a lot of computing power n = 27 was solved 2016
     https://github.com/preusser/q27 . </li>
     <li> https://oeis.org//A002562
     is about counting symmetric solutions only once. Here also (due to
     formula) n = 27 is known. </li>
    </ol>
    If we could count for n=28, this would yield some nice
    publicity.
   </li>
   <li> One can also consider the enumeration of all
   solutions, for example in lexicographical order,
   with polynomial delay (or finding the first and
   the next solution). </li>
   <li> For what n can solvers find one solutions, and for what n can
   one find all solutions (or count them) ? </li>
   <li> In [P.A.Geelen. Dual viewpoint heuristics for binary constraint
   satisfaction problems. 10th European Conference on Artificial Intelligence,
   31-35, 1992] apparently the n-Queens problem is considered with a
   heuristics similar (in "spirit") to the tau-heuristics. </li>
   <li> Compare "Heuristics" in
   ComputerAlgebra/Satisfiability/Lisp/Backtracking/plans/general.hpp. </li>
  </ul>


  \todo Organisation
  <ul>
   <li> See ComputerAlgebra/Satisfiability/Lisp/ConstraintProblems/Generators.mac
   for a constraint propagator. </li>
   <li> And see "n Queens" in
   ComputerAlgebra/Satisfiability/Lisp/ConstraintProblems/plans/general.hpp
   for further plans (in the context of the Lisp/Maxima system). </li>
   <li> See "Relations" in Hypergraphs/Matchings/plans/general.hpp for
   the reduction to the exact-cover problem. </li>
  </ul>


  \todo Different perspectives:
  <ul>
   <li> A natural formulation considers the hypergraph with
   n^2 vertices and with hyperedges corresponding to the rows,
   columns and diagonals, and searches for a strong independent
   set (see Hypergraphs/IndependentSets/plans/general.hpp) of
   size n. </li>
   <li> Another formulation is as a crisp CSP-problem (see
   Transformers/CrispCSP/plans/general.hpp and
   ComputerAlgebra/Satisfiability/Lisp/ConstraintProblems/Generators.mac).
    <ol>
     <li> The following assumes the columns-representation (see below). </li>
     <li> The conditions on the moves of the queens can be formulated
     directly by one constraint. </li>
     <li> More powerfull is to use two constraints, one is the bijectivity
     constraint, and the other captures the 2 * (2 n - 1) diagonales:
     The power comes from the bijectivity constraint. </li>
     <li> The bijectivity constraint allows for intelligent algorithms;
     it comprises the vertical and horizontal moves. Could one also
     have such matching techniques for the diagonal moves? (Or for
     all moves together??) </li>
     <li> The quest is to use more active *clauses* (like the bijectivity
     condition). </li>
    </ol>
   </li>
   <li> And, of course, various (generalised) clause-sets-
   formulations. </li>
   <li> And, of course, active clause-sets. </li>
  </ul>


  \todo Different formulations
  <ul>
   <li> The most "natural" perspective is the queens-perspective,
   that is, n variables for the n queens (where the value set
   is the set of squares). The set of total assignments has
   size n^(n^2) here. </li>
   <li> The queens-perspective has the disadvantage of added symmetries,
   which are avoided by the column perspective (n variables for the
    columns, each with values {1,...,n}), or, equivalently,
    the row perspective. We get the smaller state-space n^n.
    It seems that this perspective is inherently better than
    the "extreme" queens-perspective from above (but perhaps this
    "extreme" perspective has other representations in it(!)). </li>
  <li> One could also consider the field-perspective, that
   is, n^2 boolean variables (with a queen or not). This yields the
   state-space size 2^(n^2). </li>
    <li> And one can combine these perspectives. </li>
   <li> See [Barbara M. Smith, Modelling, 2006] for an entry into the
   literature; [Nadel 1990; Representation Selection for Constraint Satisfaction:
   A Case Study Using n-Queens] seems to be the major study. </li>
  </ul>


  \todo Active clause-sets for the moves-condition
  <ul>
   <li> In the following we are using the columns-perspective (see above).
   </li>
   <li> The domain association perhaps best is realised here as a square matrix
   of order n with boolean entries. </li>
   <li> Propagation happens as follows:
    <ol>
     <li> There is the deque of unit-clauses to be processed. Once we reach
     a column (again) which is in this deque we remove it (this should always
     be the leftmost entry of the deque). </li>
     <li> We run through all columns from left to right, at each step
     always performing all eliminations for all elements of the deque
     (respectively removing it from the deque if we reach it (again)). </li>
     <li> When processing a column and we find it become a unit, it is
     (immediately) added to the end of the deque (and thus is processed
     directly with the new column). </li>
     <li> We stop processing once a round didn't create a new unit-clause. </li>
     <li> Since we need only to run through all current unit-clauses and to
     add to the right, remove from the left, perhaps a list is better
     than a deque. </li>
     <li> We also stop once a column becomes empty. We have counters for
     the domain sizes (the occupied elements in a column). </li>
    </ol>
   </li>
   <li> An interesting question is, whether the look-ahead for this condition
   (that is, the condition strengthened by (complete) look-ahead) could
   be implement more efficient, using the properties of this special
   case? In general, when implementing the look-ahead as an operator
   (creating a new active clause-set from a given one), it would need
   from the given active clause-set the set of implied units (and the
   given active clause-set should be operatable in "look-ahead mode"). </li>
   <li> As noted above, for for each n the problem is solvable and and also
   to find one solution is easy, however the real question is about the
   complexity of the *partially instantiated* board!
    <ol>
     <li> The most intuitive situation is, that some queens are already
     placed on the board, and the problem is to decide whether this
     position can be extended to one using n queens. </li>
     <li> More generally, the question is about the abbove different
     problem formulation, and whether the n-queens active-clause-set
     can actually be turned into an active clause. </li>
    </ol>
   </li>
  </ul>


  \todo The challenge of counting
  <ul>
   <li> The big problem for counting all solutions is the large
   number of solutions, which are all total, and thus a SAT solver,
   which must create a node for each such (total) solution, has to
   create a large number of nodes even if he can completely avoid
   to explore unsatisfiable nodes. </li>
   <li> One possibility would be to invent some new form of ("intelligent")
   literals which would allow for partial solutions. </li>
   <li> Another possibility is to use symmetry; though it seems that
   counting solutions modulo symmetry makes the problem actually harder.
   Of course, the question here is what kinds of symmetries to consider. </li>
   <li> The program apparently currently the fastest (jmsnqueens.zip;
   see Buildsystem/ExternalSources/SpecialBuilds/plans/SAT.hpp),
   is actually a very simple backtracker! It's all about brute force. </li>
   <li> The current best processing speed seems to be around 1.7 * 10^6
   solutions per second for n=17, and 10^6 solutions per second for
   n=23 (Pentium4 Xeon 2.8 GHz;
   http://www.arch.cs.titech.ac.jp/~kise/doc/paper/uec-is-2004-06.pdf,
   [Solving the 24-queens Problem using MPI on a PC Cluster].
   Likely the numbers need to be divided by 2 due to the simple
   symmetry-trick. </li>
   <li> So actually, despite its simplicity, the program doesn't seem
   to spend much time on unsatisfiable branches? One should obtain
   a node count. </li>
   <li> The general question is, how easy it is to avoid unsatisfiable
   branches (see above). Perhaps it is quite easy? </li>
   <li> Parallelisation:
    <ol>
     <li> This should work very well with a look-ahead architecture. </li>
     <li> Basically we have two levels: Say we want to split into 64
     problems. Then we look at depth d = 6:
      <ul>
       <li> Up to depth d we use strong inference, strong heuristics, etc.
       </li>
       <li> Once the problem is split, then we use weaker methods. </li>
      </ul>
     </li>
     <li> One would assume that not much load balancing is needed, but that
     automatically the 2^d processes are very similar. </li>
     <li> The distributed processes should have many variables already
     eliminated, and they should be able to exploit this (so "eager
     preprocessing" here is needed). </li>
    </ol>
   </li>
   <li> The DNF structure is very "simple": only full clauses. But perhaps
   the prime implicates have some interesting structure ?!? (Using non-boolean
   clause-sets.) </li>
  </ul>

*/
