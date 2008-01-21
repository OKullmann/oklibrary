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
    <li> To make use of the special OKlibrary Maxima functions, first they have to be loaded. One possibility is to load individual files seperately using:
      \verbatim
(%i1) oklib_load(filename)
      \endverbatim
Alternatively, every function from the OKlibrary can be loaded simultaneously by using the oklib_load_all() function.
      \verbatim
(%i2) oklib_load_all()$
0 errors, 0 warnings
      \endverbatim
    </li> 
    <li> %Graphs
      <ol>
        <li> For the OKlibrary, a graph is a simply a 2-element list. The first element is a set of vertices and the second is a set of 2-element vertex sets.
          \verbatim
(%i3) g:[{1,2,3},{{1,2},{1,3}}];
(%o3)                    [{1, 2, 3}, {{1, 2}, {1, 3}}]
          \endverbatim
        </li>
        <li> Some basic graph operations, which are not provided by the Maxima graph library, are available in the OKlibrary
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
        <li> For most real work we can exploit the Maxima graph library by converting a 2-element list graph to a Maxima graph using the g2mg function.
              \verbatim
(%i7) mg:g2mg(g);
(%o7)                               GRAPH
(%i8) chromatic_number(mg);
(%o8)                                  2
              \endverbatim
        </li>
          <li> We can also do the reverse, and convert a Maxima graph into a 2-element list graph using the mg2g function.
              \verbatim
(%i9) mg2g(mg);
(%o9)                    [{1, 2, 3}, {{1, 2}, {1, 3}}]
              \endverbatim
        </li>
        <li> There are many graph generators available in the Maxima graph library. The OKlibrary provides additional generators. For example, Kneser graphs - graphs where vertices are the m-element subets of {1,..,n} and edges join disjoint vertices.
          \verbatim
(%i10) k:g2mg(Kneser_graph(5,2))$
          \endverbatim
        </li>
        <li> 
          \verbatim
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
    </li>
    <li> Satisfiability
      <ol>
        <li> %Variables and literals.
          <ol> 
            <li> A "boolean variable" can be any expression x, such that "- x" is defined and yields a different object, and such that abs(x) = x. 
            </li>
            <li> A literal is either a variable or the negation of a variable. To obtain the underlying variable of a literal l, the function var_l(l) is provided.
              \verbatim
(%i12) var_l(-1);
(%o12)                                 1
              \endverbatim
            </li>
            <li> For a set of variables S, the literals_v(S) function returns the set of all literals with underlying variable in S.
              \verbatim
(%i13) literals_v({1,2,3});
(%o13)                     {- 3, - 2, - 1, 1, 2, 3}
              \endverbatim
            </li>
          </ol>
        </li>
        <li> %Clauses and clause-sets.
          <ol> 
            <li> A "boolean clause" is a set C of boolean literals such that C and the complement of C, comp_sl(C), have empty intersection.
              \verbatim
(%i14) c1:{1,-2};
(%o14)                             {- 2, 1}
(%i15) intersect(c1,comp_sl(c1));
(%o15)                                {}
              \endverbatim
            </li>
            <li> A "boolean clause-set" is a set of boolean clauses.
              \verbatim
(%i16) c2:{-1,3}$
(%i17) c3:{2,3}$
(%i18) c:{c1,c2,c3};
(%o18)                   {{- 2, 1}, {- 1, 3}, {2, 3}}
              \endverbatim
            </li>
	      <li> The variables of a clause-set can be obtained by the var_cs function:
                \verbatim
(%i19) var_cs(c);
(%o19)                             {1, 2, 3}
                \endverbatim
              </li>
              <li> The number of variables of a clause-set can be obtained by bthe nvar_cs function.
                \verbatim
(%i20) nvar_cs(c);
(%o20)                                 3
                \endverbatim
              </li>
              <li> The number of clauses of a clause-set is given by the ncl_cs function.
                \verbatim
(%i21) ncl_cs(c);
(%o21)                                 3
                \endverbatim
              </li>
              <li> The number of literal occurences in a clause-set is given by the nlitocc function.
                \verbatim
(%i22) nlitocc(c);
(%o22)                                 6
                \endverbatim
              </li>
	    <li> A "formal boolean clause-set" is a pair [V, F], where V is a set of variables, while F is a clause-set using only variables from V. The function cs_to_fcs(F) returns the formal clause-set of a clause-set F.
              \verbatim
(%i23) f:cs_to_fcs(c);
(%o23)             [{1, 2, 3}, {{- 2, 1}, {- 1, 3}, {2, 3}}]
              \endverbatim
            </li>
            <li> The variables of a formal clause-set can be obtained by the var_cs_f function.
              \verbatim
(%i24) var_cs_f(f);
(%o24)                             {1, 2, 3}
              \endverbatim
            </li>
            <li> The number of variables of a formal clause set is given by the nvar_f function.
              \verbatim
(%i25) nvar_f(f);
(%o25)                                 3
              \endverbatim
            </li>
            <li> The number of clauses of a formal clause set is given by the ncl_f function.
              \verbatim
(%i26) ncl_f(f);
(%o26)                                 3
              \endverbatim
            </li>
            <li> A formal clause-set can be printed in Dimacs format using the print_cs_f function. The first argument is a comment.
              \verbatim
(%i27) print_cs_f("Sample boolean clause set #001",f)$
c Sample boolean clause set #001
p cnf 3 3
-2 1 0
-1 3 0
2 3 0
              \endverbatim
            </li>
            <li> For output to a file, use the output_cs_f function. By default, the file is located in the directory where you started Maxima.
              \verbatim
(%i28) output_cs_f("Sample boolean clause set #001",f,"001.cnf")$
              \endverbatim
            </li>
          </ol>
        </li>
        <li> The pigeonhole principle for 2 pigeons in 2 pigeonholes. Here the variable php(i,j) (note the usage of function "php" for variables) means that pigeon i is in pigeonhole j and -php(i,j) means that pigeon i is not in pigeonhole j.
          \verbatim
(%i29)  php22:weak_php(2,2);
(%o29) [{php(1, 1), php(1, 2), php(2, 1), php(2, 2)},
{{- php(1, 1), - php(2, 1)}, {php(1, 1), php(1, 2)},
{- php(1, 2), - php(2, 2)}, {php(2, 1), php(2, 2)}}]
          \endverbatim
        </li>
        <li> To solve a pigeonhole principle problem, one possibility is to use the various DLL solvers available in the Backtracking module. The simplest of these has input a formal clause-set and returns either true or false. 
          \verbatim
(%i30) dll_simplest_first_shortest_clause(php22);
(%o30)                                true
          \endverbatim
        </li>
        <li> For each DLL solver there is also a version which returns the splitting tree:
          \verbatim
(%i31) split_tree:dll_simplest_st_first_shortest_clause(php22);
(%o31) [php(1, 1), [- php(1, 2), [php(2, 2), [- php(2, 1), [true], [false]],
[false]], [false]], [php(2, 1), [- php(2, 2), [php(1, 2), [true], [false]],
[false]], [false]]]
          \endverbatim
        </li>
        <li> From a splitting tree the satisfying assignments can be extracted using the sat_pass_st() function:
          \verbatim
(%i32) sat_pass_st(split_tree);
(%o32) [{- php(1, 1), php(1, 2), php(2, 1), - php(2, 2)},
                              {php(1, 1), - php(1, 2), - php(2, 1), php(2, 2)}]
          \endverbatim
        </li>
        <li> For output, we can create a splitting tree diagram in PStree format (the file "tree.tex" is placed in the directory from where you started Maxima).
          \verbatim
(%i33) tex_st_f("tree.tex", split_tree)$
          \endverbatim
        </li>
        <li> VanDerWaerden2_cs_f(k,n) is the Boolean clause-set whose solutions are the partitionings of {1,...,n} into two parts such that none of them contains an arithmetic progression of size k.
          \verbatim
(%i34) vdw38:VanDerWaerden2_cs_f(3,8);
(%o34) [{1, 2, 3, 4, 5, 6, 7, 8}, {{- 8, - 7, - 6}, {- 8, - 6, - 4},
{- 8, - 5, - 2}, {- 7, - 6, - 5}, {- 7, - 5, - 3}, {- 7, - 4, - 1},
{- 6, - 5, - 4}, {- 6, - 4, - 2}, {- 5, - 4, - 3}, {- 5, - 3, - 1},
{- 4, - 3, - 2}, {- 3, - 2, - 1}, {1, 2, 3}, {1, 3, 5}, {1, 4, 7}, {2, 3, 4},
{2, 4, 6}, {2, 5, 8}, {3, 4, 5}, {3, 5, 7}, {4, 5, 6}, {4, 6, 8}, {5, 6, 7},
{6, 7, 8}}]
(%i35) dll_simplest_first_shortest_clause(vdw38);
(%o35)                               true
(%i36) vdw39:VanDerWaerden2_cs_f(3,9);
(%o36) [{1, 2, 3, 4, 5, 6, 7, 8, 9}, {{- 9, - 8, - 7}, {- 9, - 7, - 5},
{- 9, - 6, - 3}, {- 9, - 5, - 1}, {- 8, - 7, - 6}, {- 8, - 6, - 4},
{- 8, - 5, - 2}, {- 7, - 6, - 5}, {- 7, - 5, - 3}, {- 7, - 4, - 1},
{- 6, - 5, - 4}, {- 6, - 4, - 2}, {- 5, - 4, - 3}, {- 5, - 3, - 1},
{- 4, - 3, - 2}, {- 3, - 2, - 1}, {1, 2, 3}, {1, 3, 5}, {1, 4, 7}, {1, 5, 9},
{2, 3, 4}, {2, 4, 6}, {2, 5, 8}, {3, 4, 5}, {3, 5, 7}, {3, 6, 9}, {4, 5, 6},
{4, 6, 8}, {5, 6, 7}, {5, 7, 9}, {6, 7, 8}, {7, 8, 9}}]
(%i37) dll_simplest_first_shortest_clause(vdw39);
(%o37)                               false
          \endverbatim
        </li>
      </ol>
    </li>
  </ol>

*/

