// Oliver Kullmann, 23.1.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Graphs/Lisp/docus/Tutorial.hpp
  \brief Tutorial on graphs in Maxima/Lisp by MH


  <h1> %Graphs via Maxima in the OKlibrary </h1>

  <ol>
   <li> For the OKlibrary, a graph is a simply a 2-element list. The first
   element is a set of vertices and the second is a set of 2-element vertex
   sets.
   \verbatim
(%i3) g:[{1,2,3},{{1,2},{1,3}}];
(%o3)                    [{1, 2, 3}, {{1, 2}, {1, 3}}]
   \endverbatim
   </li>
   <li> Basic graph operations are available in the OKlibrary.
    <ol>
     <li> For instance, to find the neighbourhood of a vertex in a graph:
     \verbatim
(%i4) neighbours(1,g);
(%o4)                               {2, 3}
     \endverbatim
     </li>
     <li> Or, to remove a vertex from a graph:
     \verbatim
(%i5) remove_vertices_graph({2},g);
(%o5)                        [{1, 3}, {{1, 3}}]
(%i6) g;
(%o6)                   [{1, 2, 3}, {{1, 2}, {1, 3}}]
     \endverbatim
     </li>
    </ol>
   </li>
   <li> For most real work we can exploit the Maxima graph library by converting
   a 2-element list graph to a Maxima graph using the g2mg function.
   \verbatim
(%i7) mg:g2mg(g);
(%o7)                               GRAPH
(%i8) chromatic_number(mg);
(%o8)                                  2
   \endverbatim
   </li>
   <li> We can also do the reverse, and convert a Maxima graph into a 2-element
   list graph using the mg2g function.
   \verbatim
(%i9) mg2g(mg);
(%o9)                    [{1, 2, 3}, {{1, 2}, {1, 3}}]
   \endverbatim
   </li>
   <li> There are many graph generators available in the Maxima graph library.
   </li>
   <li> The OKlibrary provides additional generators. For example, 
   Kneser graphs - graphs where vertices are the m-element subets of {1,..,n} and 
   edges join disjoint vertices. For example the Peterson-graph:
   \verbatim
(%i10) k:g2mg(knesergraph_g(5,2))$
(%i11) print_graph(k)$

Graph on 10 vertices with 15 edges.
Adjacencies:
 10 :  5  2  1
  9 :  6  3  1
  8 :  7  4  1
  7 :  8  3  2
  6 :  9  4  2
  5 : 10  4  3
  4 :  8  6  5
  3 :  9  7  5
  2 : 10  7  6
  1 : 10  9  8
   \endverbatim
   </li>
  </ol>

*/

