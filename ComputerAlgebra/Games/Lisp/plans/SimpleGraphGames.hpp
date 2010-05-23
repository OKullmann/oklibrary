// Oliver Kullmann, 23.5.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Games/Lisp/plans/SimpleGraphGames.hpp
  \brief Plans regarding so-called "simple graph games"


  \todo Connections
  <ul>
   <li> See "Parity games" in
   Buildsystem/ExternalSources/SpecialBuilds/plans/Games.hpp. </li>
   <li> The vertex set V with a strategy s yields an irreversible discrete
   dynamical system [V,s]; see "Dynamical systems" in
   ComputerAlgebra/plans/general.hpp. </li>
  </ul>


  \todo The general setting
  <ul>
   <li> Here "simple graph games", abbreviated "sigrga", are defined. </li>
   <li> Input is a directed graph with loops G = [V,E], where the vertices are
   natural numbers, together with a map P(v), which assigns to every vertex v
   in V the number 0 or 1 (for "player 0" resp. "player 1"). </li>
   <li> It is assumed furthermore that the outdegree of every vertex is at
   least 1. </li>
   <li> So for input I = [G,P] validity of the input is determined by the
   following predicate:
   \verbatim
sigrga_p(I) := if not listp(I) or not length(I) = 2 then false else
 block([G : I[1], V, P : I[2]],
  if not dgl_p(G) then false else (
    V : G[1],
    every_s(lambda([v], integerp(v) and is(v >= 1) and elementp(P(v), {0,1})),
            V) and
    min_vertex_outdegree_dgl(G) >= 1
  )
)$
   \endverbatim
   </li>
   <li> A "game" is a sequence g = (v_n)_{n in NN} of vertices describing a
   path in G; the "repeated vertices" are those vertices which occur
   infinitely often in g, and the "winner" of g is the player i in {0,1} such
   that for the minimum repeated vertex v (using that vertices are natural
   numbers) it holds P(v) = i. </li>
   <li> A game g is "played" in such a way that for a given start vertex v_1
   the vertices v_2, ..., v_i, ... are chosen by player P(v_{i-1}). </li>
   <li> A "memoryless strategy" is a map s which assigns to every vertex v one
   of its outneigbhours. </li>
   <li> Every vertex v yields now a game g_s(v) := v, s(v), s(s(v)),
   ..., and as such is qualified as winning for player 0 or player 1. </li>
   <li> So w.r.t. s we have a map W_s(v), which assigns to every vertex v the
   player in {0,1} winning the game g_s(v). </li>
   <li> W_s is computable by considering G restricted to the choices of s,
   where now every vertex has outdegree 1, and which is thus a functional
   graph (representing a transformation of V), where each (weakly) connected
   component has the structure of one (directed) cycle with additional paths
   leading to it. The owner of the minimal vertex on such a cycle is the winner
   of all the nodes of the connected component. </li>
   <li> And given W_s, we can derive a strategy s_W (with W_s = W_{s_W}) by
   choosing for each node v with W(v) = P(v) a node also won by P(v), while
   otherwise we make an arbitrary choice. </li>
   <li> There exists now an optimal strategy s_opt, characterised by the
   condition that for every vertex v as start, any alteration of s_opt w.r.t.
   the moves by player 1-W_{s_opt}(v) (the "looser") do not change the
   outcome of the game (player W_{s_opt}(v) always wins). </li>
   <li> All these optimal strategies coincide in their determination of
   which player wins any vertex, and so we have a unique winner-determination
   W_opt(v) in {0,1} for v in V. </li>
   <li> So for given input [[V,E], P] the output is either an optimal strategy
   s_opt (a map from V to V), or the winner determination W_opt (a map from
   V to {0,1}). </li>
  </ul>


  \todo Implementing the recursive algorithm according to [AB, Moller]
  <ul>
   <li> According to [Beckmann, Moller]. </li>
   <li> Input is a sigrga [G,P], output the winner determination. </li>
  </ul>

*/
