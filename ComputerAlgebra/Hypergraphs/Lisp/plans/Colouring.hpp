// Oliver Kullmann, 29.7.2008 (Swansea)
/* Copyright 2008, 2009, 2010, 2011, 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Hypergraphs/Lisp/plans/Colouring.hpp
  \brief Plans regarding (various) colouring problems for hypergraphs


  \todo Relations to other modules
  <ul>
   <li> See ComputerAlgebra/Games/Lisp/plans/general.hpp. </li>
   <li> See ComputerAlgebra/Graphs/Lisp/plans/Colouring.hpp. </li>
   <li> See Combinatorics/Hypergraphs/Colourings/plans/general.hpp. </li>
   <li> See
   Satisfiability/Lisp/LocalSearch/plans/HypergraphColouring.hpp
   for local search algorithms. </li>
   </li>
  </ul>


  \todo Organisation
  <ul>
   <li> Best to create a sub-module Hypergraphs/Lisp/Colouring. </li>
  </ul>


  \todo Translations to SAT problems
  <ul>
   <li> The name-components "std" (as in gcol2sat_stdohg2stdnbfclud) are
   not very appropriate (they cover only a special case, namely that
   standardised hypergraphs yield standardised clause-sets). </li>
   <li> For translation to boolean SAT also the "non-diagonal" versions
   (using two hypergraphs) should be provided. </li>
   <li> Strong colouring (in every hyperedge all colours are different)
   can be handled by first applying section_hg(G,2) and then tcol2sat_hg2fcs;
   should we provide a convenience function combining them? </li>
  </ul>


  \todo Colouring by transversals
  <ul>
   <li> One should work out the precise relation between k-colourings of a
   hypergraph and its independent k-covers. </li>
   <li> For example are there other examples besides those with empty
   hyperedge set where a contraction of two maximal independent subsets
   forbids a colouring? </li>
  </ul>


  \todo Generalised colouring problems
  <ul>
   <li> For non-diagonal Ramsey-problems and van-der-Waerden-type problems,
   we do not need to go directly to non-boolean clause-sets, but we can
   use an intermediate form, with "layers of hyperedges", as discussed in the
   following. </li>
   <li> The problem would be given as a triple [V,C,L], where V is a set
   (the vertices), C is a set (the colours), and each L is a pair [H,c],
   where H is a set of hyperedges on V (i.e., [V,H] is a hypergraph), while
   c is an element of C. </li>
   <li> The problem is to find a map f: V -> C such that for every [H,c]
   and every E in H there exists v in E with f(v) <> c. </li>
   <li> A standard hypergraph colouring problem with colour set C for
   hypergraph [V,E] is translated in [V,C,L], where L consists of the
   pairs [E,c] for c in C. </li>
   <li> Compared with non-boolean clause-sets (clause-sets, where the
   clauses contain literals [v,c]), in this way we express the information
   that the clause-set is organised in layers. </li>
   <li> This is now handled by gcol2sat_ohg2nbfclud (and
   gcol2sat_stdohg2stdnbfclud), but in the somewhat different form of a pair
   [GG,C], where GG is a list of hypergraphs and C is a list of colours. </li>
  </ul>


  \todo Greedy colouring
  <ul>
   <li> Compare
   Combinatorics/Hypergraphs/Colourings/plans/GreedyColouring.hpp. </li>
   <li> Greedy colouring, running through the vertices in some order, and
   giving the vertices the first available colour (not violating the colouring
   condition), can also be done for hypergraphs. </li>
   <li> It should be that for strong colouring (all hyperedges are
   multi-coloured) this strategy yields an optimal colouring, when using a
   suitable vertex ordering. </li>
   <li> However it seems that for (ordinary, i.e., weak) colouring this is
   no longer the case (example?). </li>
   <li> So perhaps we first consider only strong colourings, since this is
   the true generalisation of greedy colouring for graphs here. </li>
   <li> For the implementation we need the adjacency relation between
   vertices, which as graph is realised by the 2-section, but we need
   the square-hypergraph representation of this graph (providing for each
   vertex the set of adjacent vertices). </li>
  </ul>


  \todo Positional games
  <ul>
   <li> See "Generalising positional games to SAT" in
   ComputerAlgebra/Games/Lisp/plans/general.hpp for the SAT-generalisation.
   </li>
   <li> The translation yields quantified satisfiability problems; see
   ComputerAlgebra/Satisfiability/Lisp/Quantification/plans/general.hpp. </li>
   <li> Should this be a submodule of Hypergraphs/Colouring? Or perhaps
   directly of Hypergraphs? Or should be have
   ComputerAlgebra/CombinatorialGames? </li>
   <li> Given a hypergraphs G, a "positional game" (generalising
   "tic-tac-toe") is played by players A, B, which assign to the vertices
   labels A resp. B, and the winner is the first player who, after or before
   his move, can show a monochromatic hyperedge of his "colour". </li>
   <li> So [{},{}] yields is a draw, while [{},{{}}] is a win for A. </li>
   <li> The different goals:
    <ol>
     <li> It is to be decided whether A can force a win, or, equivalently
     (negated), whether B can force a draw. </li>
     <li> Since the goal of B is to establish a proper colouring (whatever
     A does), it seems best to translate the problem, whether B can force
     a draw, into satisfiability problems. </li>
     <li> Still easier to formulate, that B can force *at least* a draw;
     more precisely this is called a "strong draw", that is, over the whole
     length of the game A can be prevented from every creating a monochrome
     hyperedge. </li>
     <li> The "strong draw" (and its negation, the "weak win") ignores who's
     first. </li>
     <li> We should have all four possibilities: "A wins", "B draws", "A weakly
     wins", "B strongly draws". </li>
    </ol>
   </li>
   <li> Let |V(G)| = 2m be even (one can enforce this by adding an uncovered
   vertex). The variables are Va = {A_1,...,A_m,B_1,...,B_m,} each with domain
   V(G), representing the moves of A and B, and the conditions for a strong
   draw are:
    <ol>
     <li> The bijectivity-condition, that a satisfying total assignment
     represents a bijection from Va to V(G).
      <ul>
       <li> More precisely, the whole formula is "RA -> (RB and COL)" (with "R"
       like "Rule"). </li>
       <li> RA is the conjunction of the disequality-literals that every A-move
       is different from all previous moves. </li>
       <li> RB is the conjunction of all the disequality-literals that every
       B-move is different from all previous moves. </li>
       <li> COL is the colouring condition (see below). </li>
      </ul>
     </li>
     <li> For each hyperedge H in E(G), the positively signed CNF-clause
     of length m, with literals "B_i in H" for all i in {1,...,m}. </li>
     <li> One could introduce auxiliary boolean variables c_v for each vertex
     v, which are true if vertex v is taken by B; then COL would be the
     conjunction of implications "B_i=v -> c_v" and "A_i=v -> not c_v" and
     for each hyperedge H the disjunction of c_v for v in H. </li>
     <li> The quantifier prefix is
     [["fa",A_1],["ex",B_1], ..., ["fa",A_m,"ex",B_m]]. </li>
    </ol>
   </li>
   <li> The natural translation of the problem using boolean variables is:
    <ol>
     <li> Each variable A_i resp. B_i is replaced by n = 2m boolean variables
     A_{i,v} resp. B_{i,v}, true iff the move i chooses v in V(G). </li>
     <li> The rule-formulas RA, RB (see above) now also need to contain the
     AMO and ALO conditions. </li>
     <li> For each hyperedge H we have now the clause of length m * |H| with
     literals B_i,v for i in {1,..,m} and v in H. </li>
     <li> Or, using the c_v as above, we again have clauses of length |H|.
     </li>
     <li> ["fa",A_i] is replaced by ["fa",A_{i,v_1}],...,["fa",A_{i,v_n}], and
     ["ex",B_i] is replaced by ["ex",B_{i,v_1}],...,["ex",B_{i,v_n}]. </li>
     </li>
    </ol>
   </li>
   <li> This looks like attractive problems, where the literature on positional
   games yields a rich source of examples, where the outcome is known.
   See "Generators" in
   ComputerAlgebra/Satisfiability/Lisp/Quantification/plans/general.hpp. </li>
   <li> But for the colouring module, the "generator" aspect should be
   sub-ordinated under the "tool" aspect, where we actually use (generalised)
   SAT solving to experimentally explore open questions on positional games.
   </li>
   <li> A nice test-case is tic-tac-toe. </li>
  </ul>


  \todo Achievement and avoidance game
  <ul>
   <li> In [Partitions into sum-free sets, BHR 2006] the following other forms
   of 2-player hypergraph games are considered:
    <ol>
     <li> In "Positional games" above, every plays has his colour, and proper
     colourings are to be constructed. </li>
     <li> Now there are only two players, and every move consists in either
     only choosing a colour (from a fixed set of colours), while the vertices
     are run through in order, or both vertex and colour are chosen, while
     still proper colourings are to be constructed. </li>
     <li> In the "achievement game", the last player who can move wins. </li>
     <li> In the "avoidance game", the last player who can move loses. </li>
     <li> If the hypergraph is infinite, than an order needs to be fixed.
     </li>
     <li> Non-k-colourability of the whole hypergraph guarantees that the games
     end in a fixed number of steps.
     <li> If the hypergraph is finite, then both forms are possible, fixing
     a vertex-order, or allowing free choices. </li>
    </ol>
   </li>
  </ul>

*/

