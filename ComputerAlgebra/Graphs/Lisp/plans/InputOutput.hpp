// Oliver Kullmann, 30.6.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Graphs/Lisp/plans/InputOutput.hpp
  \brief Plans regarding input and output of graphs in Maxima/Lisp


  \todo Documentation
  <ul>
   <li> Extend our documentation. </li>
   <li> Link to Combinatorics/Graphs regarding the dot-format. </li>
  </ul>


  \todo Dot-format
  <ul>
   <li> Update the function-names:
    <ol>
     <li> "print_graph_dot" -> "print_dot_g" </li>
    </ol>
   </li>
   <li> Handle ordered graph-versions:
    <ol>
     <li> Perhaps for all versions, the functions handling the
     unordered versions can also handle the ordered versions. </li>
    </ol>
   </li>
   <li> Does the Maxima graphs-package handle this format? </li>
   <li> Should we give the graphs in the dot-output a dedicated name?
    <ol>
     <li> At least we should have an optional parameter. </li>
    </ol>
   </li>
   <li> Generalise the graph output, so that arbitrary graphs
   can be output.
    <ol>
     <li> Make function "string_vertex" a parameter. </li>
     <li> A question is, what kind of vertex-names are directly accepted
     by the dot-format, and which need to be additional labels? </li>
    </ol>
   </li>
   <li> Output multi-graphs. </li>
   <li> Output general graphs. </li>
   <li> Output directed graphs. </li>
   <li> Output multi-digraphs. </li>
   <li> Output general digraphs. </li>
  </ul>


  \todo Reading graphs in dot-format
  <ul>
   <li> First of all, we need a better understanding of the dot-format. </li>
   <li> The "official" specification should be available in the OKlibrary.
   </li>
  </ul>


  \todo Maxima graphs-package
  <ul>
   <li> What is the "DIMACS" format?
    <ol>
     <li> Like the DIMACS cnf-format, we have first comment lines ("c"),
     and then a parameter line
     \verbatim
p edges 5 10
     \endverbatim
     (number of vertices, number of edges).
     This for undirected graphs, while for directed graphs "arcs" is used
     instead of "edges". </li>
     <li> Each edge is specified by a line like
     \verbatim
e 1 2
     \endverbatim
     so apparently vertices are always numbered from 1 to n. </li>
     <li> For undirected graphs, likely "e 1 2" as well as "e 2 1"
     is possible? </li>
    </ol>
   </li>
   <li> What is the "graph6" format?
    <ol>
     <li> None of these functions seem to work! </li>
    </ol>
   </li>
   <li> What is the "sparse6" format? 
    <ol>
     <li> None of these functions seem to work! </li>
    </ol>
   </li>
  </ul>


  \todo Other graph-formats
  <ul>
   <li> The dot-format seems to be the main format used in the OKlibrary. </li>
   <li> Perhaps we should not provide other format at the Maxima/Lisp level,
   but we provide a series of conversion *programs* (in C++). </li>
  </ul>

*/

