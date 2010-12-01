// Oliver Kullmann, 25.9.2008 (Swansea)
/* Copyright 2008, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Graphs/Lisp/plans/Colouring.hpp
  \brief Plans for graph colouring


  \todo Connections to other modules
  <ul>
   <li> See ComputerAlgebra/Hypergraphs/Lisp/plans/Colouring.hpp. </li>
   <li> In principle one can always generalise everything to hypergraphs,
   but in this module we consider the cases where graphs behave "special".
   </li>
   <li> Yet, in OKlib/Combinatorics we have put also graph colouring
   to the hypergraph module there; perhaps we should change this. </li>
  </ul>


  \todo Greedy colouring
  <ul>
   <li> See "Greedy colouring" in
   ComputerAlgebra/Hypergraphs/Lisp/plans/Colouring.hpp for the hypergraph
   context. </li>
   <li> See Hypergraphs/Colourings/plans/GreedyColouring.hpp for plans at
   C++ level, and see Combinatorics/Hypergraphs/Colourings/GreedyColouring.cpp
   for a C++ implementation. </li>
   <li> DONE (yes, special algorithms are of interest)
   Should we also provide special functionality for graphs? </li>
   <li> DONE (implemented greedy_colouring_og)
   Specifying input and output:
    <ol>
     <li> Input a graph and a vertex ordering, output the derived colouring,
     as a map. </li>
     <li> More precisely, input is an ordered graph. </li>
     <li> The output is a pair [n,M], where n is the number of colours used,
     while M is the colouring itself as a map. </li>
     <li> Actually, it seems better to produce just the list of colours,
     using the given order of the vertices. </li>
     <li> Colours are natural numbers from 1 to n. </li>
     <li> To summarise: Input is an og G, output is a pair [n,C], where C
     is a list of numbers from 1 to n. </li>
     <li> Calling it greedy_colouring_og(G). </li>
    </ol>
   </li>
  </ul>

*/

