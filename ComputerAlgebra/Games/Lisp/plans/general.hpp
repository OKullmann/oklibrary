// Oliver Kullmann, 23.5.2010 (Swansea)
/* Copyright 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Games/Lisp/plans/general.hpp
  \brief General plans regarding games


  \todo Connections
  <ul>
   <li> See Applications/Games/plans/general.hpp. </li>
  </ul>


  \todo Create milestones


  \todo Generalising positional games to SAT
  <ul>
   <li> See "Positional games" in
   ComputerAlgebra/Hypergraphs/Lisp/plans/Colouring.hpp for the hypergraph
   versions. </li>
   <li> The right generalisation of the n-player versions seems to be as
   follows:
    <ol>
     <li> Given n clause-sets F_1,...,F_n. </li>
     <li> At the beginning the partial assignment phi to be constructed is
     empty. </li>
     <li> A move of player i is to choose a yet unassigned variable v and
     a value e for it, and to extend phi by <v -> e> </li>
     <li> The winner is i who has first an empty clause in phi * F_i. </li>
     <li> More precisely i wins iff it is his turn to move, and either there is
     already the empty clause in phi * F_i, or after his move the empty clause
     is in phi * F_i. </li>
     <li> It seems important that the players are not associated with truth
     values. </li>
     <li> So instead of boolean clause-sets F_1,..., F_n we can also have
     generalised clause-sets with non-boolean values --- this just yields more
     choices for the players. </li>
     <li> For a hypergraph G and n players the positional game on G, where
     every player has his own colour, is represented by the generalised
     clause-set representing the generalised colouring problem for n
     copies of G, where player i has the clauses using his colour. Here
     essentially there is no choice of truth-value, since only the truth
     value i can falsify a literal in a clause of F_i. </li>
    </ol>
   </li>
  </ul>

*/
