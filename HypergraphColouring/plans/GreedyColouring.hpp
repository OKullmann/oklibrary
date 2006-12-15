// Oliver Kullmann, 24.11.2006 (Swansea)

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

