// Oliver Kullmann, 29.7.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Hypergraphs/Lisp/plans/Colouring.hpp
  \brief Plans regarding (various) colouring problems for hypergraphs


  \todo Organisation
  <ul>
   <li> Best to create a sub-module Hypergraphs/Lisp/Colouring. </li>
  </ul>


  \todo Positional games
  <ul>
   <li> The translation yields quantified satisfiability problems; see
   ComputerAlgebra/Satisfiability/Lisp/Quantification/plans/general.hpp. </li>
   <li> Given a hypergraphs G, a "positional game" (generalising
   "tic-tac-toe") is played by players A, B, which assign to the vertices
   labels A resp. B, and the winner is the first player who, after or before
   his move, can show a monochromatic hyperedge of his "colour". </li>
   <li> So [{},{}] yields is a draw, while [{},{{}}] is a win for A. </li>
   <li> It is to be decided whether A can force a win, or, equivalently
   (negated), whether B can force a draw. </li>
   <li> Since the goal of B is to establish a proper colouring (whatever
   A does), it seems best to translate the problem, whether B can force
   a draw, into satisfiability problems. </li>
   <li> Still easier to formulate, that B can force *at least* a draw. </li>
   <li> Let |V(G)| = 2m be even (one can enforce this by adding an uncovered
   vertex). The variables are Va = {A_1,...,A_m,B_1,...,B_m,} each with domain
   V(G), representing the moves of A and B, and the conditions are:
    <ol>
     <li> The bijectivity-condition, that a satisfying total assignment
     represents a bijection from Va to V(G). </li>
     <li> For each hyperedge H in E(G), the positively signed CNF-clause
     of length m, with literals "B_i in H" for all i in {1,...,m}. </li>
     <li> The quantifier prefix is
     [["fa",A_1],["ex",B_1], ..., ["fa",A_m,"ex",B_m]]. </li>
    </ol>
   </li>
   <li> The natural translation of the problem using boolean variables is:
    <ol>
     <li> Each variable A_i resp. B_i is replaced by n = 2m boolean variables
     A_{i,v} resp. B_{i,v}, true iff the move i chooses v in V(G). </li>
     <li> Besides the AMO and ALO conditions, the bijectivity-condition now
     demands that no A_{i,v},A_{j,v}, no A_{i,v},B_{j,v} and no B_{i,v},B_{j,v}
     are both true for i < j. </li>
     <li> For each hyperedge H we have now the clause of length m * |H| with
     literals B_i,v for i in {1,..,m} and v in H. </li>
     <li> ["fa",A_i] is replaced by ["fa",A_{i,v_1}],...,["fa",A_{i,v_n}], and
     ["ex",B_i] is replaced by ["ex",B_{i,v_1}],...,["ex",B_{i,v_n}]. </li>
     <li> Stop, the quantification is not that straightforward, since for the
     universal variables only exactly one of the A_{i,v_j} should be true?!
     </li>
     <li> One could A letting lose the game if such an illegal move is made.
     But this seems inefficient? </li>
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
  </ul>

*/

