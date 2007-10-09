// Oliver Kullmann, 24.11.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file HypergraphColouring/plans/GreedyColouring.hpp
  \brief Plans regarding the greedy graph colouring algorithm

  \todo Outsourcing:
  In GreedyColouring.hpp the components
  <ul>
    <li> Out_degree_order </li>
    <li> Edge </li>
    <li> EdgeVector </li>
    <li> read </li>
    <li> output_vertex_degrees </li>
  </ul> 
  should be moved to a Graphs-module (of course, with added doxygen-
  documentation, and with more (generic) power for input and output).
  
  \todo Full_greedy_colouring:
   - The design needs an overhaul.
   - Use Boost::permutation_iterator (and it should be made possible to iterate through
     the vertex-permutations in some user-defined order).
   - Test it.

  \todo GreedyColouring.cpp:
   - Use Messages.
   - Use ProgramOptions.
   - Doxygen documentation.
   - Test it.

  \todo Literature:
  Search for literature on heuristics for vertex orderings; from
  CS-232 we have the traversing greedy colouring; it seems sensible to
  combine this with a largest degree first-stratey.

  \todo How do these algorithms translate into SAT algorithms?

*/

