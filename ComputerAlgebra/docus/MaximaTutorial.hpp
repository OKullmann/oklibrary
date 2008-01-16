// Oliver Kullmann, 16.1.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/docus/MaximaTutorial.hpp
  \brief Tutorial Maxima/Lisp


  <h1> Tutorial for Maxima in the OKlibrary </h1>

  <ol>
    <li> Load all OKlibrary functions.
      \verbatim
(%i1) oklib_load_all()$
0 errors, 0 warnings
      \endverbatim
    </li> 
    <li> %Graphs
      <ol>
        <li> Creating a graph.
          \verbatim
(%i2) g:[{1,2,3},{{1,2},{1,3}}];
(%o2)                    [{1, 2, 3}, {{1, 2}, {1, 3}}]
          \endverbatim
        </li>
        <li> Basic graph operations.
          <ol>
            <li> Neighbourhood of a vertex:
              \verbatim
(%i3) neighbours(1,g);
(%o3)                               {2, 3}
              \endverbatim
            </li>
	    <li> Remove a vertex:
              \verbatim
(%i4) remove_vertices_graph({2},g);
(%o4)                        [{1, 3}, {{1, 3}}]
              \endverbatim
            </li>
          </ol>
        </li>
        <li> Conversion to/from Maxima graph.
              \verbatim
(%i5) mg:g2mg(g);
(%o5)                                GRAPH
(%i6) mg2g(mg);
(%o6)                    [{1, 2, 3}, {{1, 2}, {1, 3}}]
              \endverbatim
        </li>
        <li> Graph generators
          \verbatim
(%i7) k:g2mg(Kneser_graph(5,2))$
          \endverbatim
        </li>
        <li> Graph output for Maxima-graphs
          \verbatim
(%i8) print_graph(k)$

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
    </li>
    <li> Satisfiability
      <ol>
        <li> The pigeonhole principle for 2 pigeons in 2 pigeonholes:
          \verbatim
(%i9)  php22:weak_php(2,2);
(%o9) [{php(1, 1), php(1, 2), php(2, 1), php(2, 2)},
{{- php(1, 1), - php(2, 1)}, {php(1, 1), php(1, 2)},
{- php(1, 2), - php(2, 2)}, {php(2, 1), php(2, 2)}}]
          \endverbatim
          (note the usage of function "php" for variables).
        </li>
        <li> Satisfiability decision by DLL solver:
          \verbatim
(%i10) dll_simplest_first_shortest_clause(php22);
(%o10)                                true
          \endverbatim
        </li>
        <li> Splitting tree by DLL solver:
          \verbatim
(%i11) split_tree:dll_simplest_st_first_shortest_clause(php22);
(%o11) [php(1, 1), [- php(1, 2), [php(2, 2), [- php(2, 1), [true], [false]],
[false]], [false]], [php(2, 1), [- php(2, 2), [php(1, 2), [true], [false]],
[false]], [false]]]
          \endverbatim
        </li>
        <li> Extract satisfying assignments from splitting tree.
          \verbatim
(%i12) sat_pass_st(split_tree);
(%o12) [{- php(1, 1), php(1, 2), php(2, 1), - php(2, 2)},
                              {php(1, 1), - php(1, 2), - php(2, 1), php(2, 2)}]
          \endverbatim
        </li>
        <li> Output splitting tree to file in PStree format.
          \verbatim
(%i13) tex_st_f("tree.tex", split_tree)$
          \endverbatim
         (this file is placed in the directory from where you started Maxima).
        </li>
        <li> VanDerWaerden2_cs_f(k,n) is the Boolean clause-set whose solutions are the partitionings of {1,...,n} into two parts such that none of them contains an arithmetic progression of size k.
          \verbatim
(%i14) vdw38:VanDerWaerden2_cs_f(3,8);
(%o14) [{1, 2, 3, 4, 5, 6, 7, 8}, {{- 8, - 7, - 6}, {- 8, - 6, - 4},
{- 8, - 5, - 2}, {- 7, - 6, - 5}, {- 7, - 5, - 3}, {- 7, - 4, - 1},
{- 6, - 5, - 4}, {- 6, - 4, - 2}, {- 5, - 4, - 3}, {- 5, - 3, - 1},
{- 4, - 3, - 2}, {- 3, - 2, - 1}, {1, 2, 3}, {1, 3, 5}, {1, 4, 7}, {2, 3, 4},
{2, 4, 6}, {2, 5, 8}, {3, 4, 5}, {3, 5, 7}, {4, 5, 6}, {4, 6, 8}, {5, 6, 7},
{6, 7, 8}}]
(%i15) dll_simplest_first_shortest_clause(vdw38);
(%o15)                               true
(%i16) vdw39:VanDerWaerden2_cs_f(3,9);
(%o16) [{1, 2, 3, 4, 5, 6, 7, 8, 9}, {{- 9, - 8, - 7}, {- 9, - 7, - 5},
{- 9, - 6, - 3}, {- 9, - 5, - 1}, {- 8, - 7, - 6}, {- 8, - 6, - 4},
{- 8, - 5, - 2}, {- 7, - 6, - 5}, {- 7, - 5, - 3}, {- 7, - 4, - 1},
{- 6, - 5, - 4}, {- 6, - 4, - 2}, {- 5, - 4, - 3}, {- 5, - 3, - 1},
{- 4, - 3, - 2}, {- 3, - 2, - 1}, {1, 2, 3}, {1, 3, 5}, {1, 4, 7}, {1, 5, 9},
{2, 3, 4}, {2, 4, 6}, {2, 5, 8}, {3, 4, 5}, {3, 5, 7}, {3, 6, 9}, {4, 5, 6},
{4, 6, 8}, {5, 6, 7}, {5, 7, 9}, {6, 7, 8}, {7, 8, 9}}]
(%i17) dll_simplest_first_shortest_clause(vdw39);
(%o17)                               false
          \endverbatim
        </li>
      </ol>
    </li>
  </ol>

*/

