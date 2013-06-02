// Oliver Kullmann, 23.1.2008 (Swansea)
/* Copyright 2008, 2009, 2013 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/docus/Tutorial.hpp
  \brief Tutorial on SAT in Maxima/Lisp by MH


  <h1> SAT via Maxima in the OKlibrary </h1>


  <h2> From variables to clause-sets </h2>

  <ol> 
   <li> A "boolean variable" can be any expression x, such that
    <ul>
     <li> "x" is positive (i.e., <code>is(x > 0)</code> yields true); </li>
     <li> "- x" is negative; </li>
     <li> <code>--x = x</code>. </li>
    </ul>
   </li>
   <li> A literal is either a variable or the negation of a variable. To
   obtain the underlying variable of a literal l, the function var_l(l) is
   provided.
   \verbatim
oklib_load_all();

(%i12) var_l(-1);
(%o12)                                 1
   \endverbatim
   </li>
   <li> For a set S of variables, by literals_v(S) we compute the
   set of all literals with underlying variable in S.
   \verbatim
(%i13) literals_v({1,2,3});
(%o13)                     {- 3, - 2, - 1, 1, 2, 3}
   \endverbatim
   </li>
   <li> A "boolean clause" is a set C of boolean literals such that C and the
   complement of C, comp_sl(C), have empty intersection.
   \verbatim
(%i14) C1:{1,-2};
(%o14)                             {- 2, 1}
(%i15) intersect(C1,comp_sl(C1));
(%o15)                                {}
   \endverbatim
   </li>
   <li> A "boolean clause-set" is a set of boolean clauses.
   \verbatim
(%i16) C2:{-1,3}$
(%i17) C3:{2,3,-4}$
(%i18) F:{C1,C2,C3};
(%o18)                   {{-4, 2, 3}, {- 2, 1}, {- 1, 3}}
   \endverbatim
   </li>
   <li> The variables of a clause-set can be obtained by the var_cs function:
   \verbatim
(%i19) var_cs(F);
(%o19)                             {1, 2, 3, 4}
   \endverbatim
   </li>
   <li> The number of variables of a clause-set can be obtained by the
   nvar_cs function.
   \verbatim
(%i20) nvar_cs(F);
(%o20)                                 4
   \endverbatim
   </li>
   <li> The number of clauses of a clause-set is given by the ncl_cs function.
   \verbatim
(%i21) ncl_cs(F);
(%o21)                                 3
   \endverbatim
   </li>
   <li> The number of literal occurences in a clause-set is given by the
   nlitocc_cs function.
   \verbatim
(%i22) nlitocc_cs(F);
(%o22)                                 7
   \endverbatim
   </li>
   <li> A "formal boolean clause-set" is a pair [V, F], where V is a set of
   variables, while F is a clause-set using only variables from V.
   The function cs2fcs(F) returns the formal clause-set of a clause-set F.
   \verbatim
(%i23) FF:cs2fcs(F);
(%o23)             [{1, 2, 3}, {{-4, 2, 3}, {- 2, 1}, {- 1, 3}}]
   \endverbatim
   </li>
   <li> The variables of a formal clause-set can be obtained by the
   var_fcs function.
   \verbatim
(%i24) var_fcs(FF);
(%o24)                             {1, 2, 3, 4}
   \endverbatim
   Note that the point of "formal clause-sets" is to allow that variables
   might not occur in the clause-set.
   </li>
   <li> The number of variables of a formal clause set is given by the
   nvar_fcs function.
   \verbatim
(%i25) nvar_fcs(FF);
(%o25)                                 4
   \endverbatim
   </li>
   <li> The number of clauses of a formal clause set is given by the
   ncl_fcs function.
   \verbatim
(%i26) ncl_fcs(FF);
(%o26)                                 3
   \endverbatim
   </li>
   <li> A formal clause-set can be printed in Dimacs format using the
   print_fcs function. The first argument is a comment.
   \verbatim
(%i27) print_fcs("Sample boolean clause set #001",FF)$
c Sample boolean clause set #001
p cnf 3 3
-4 2 3 0
-2 1 0
-1 3 0
   \endverbatim
   </li>
   <li> For output to a file, use the output_fcs function. By default, the
   file is located in the directory where you started Maxima.
   \verbatim
(%i28) output_fcs("Sample boolean clause set #001",FF,"001.cnf")$
   \endverbatim
   </li>
  </ol>


  <h2> SAT problems </h2>

  <ol>
   <li> The pigeonhole principle for 2 pigeons in 2 pigeonholes. Here the
   variable php(i,j) (note the usage of function "php" for variables) means
   that pigeon i is in pigeonhole j and -php(i,j) means that pigeon i is not
   in pigeonhole j.
   \verbatim
(%i29)  php22:weak_php_fcs(2,2);
(%o29) [{php(1, 1), php(1, 2), php(2, 1), php(2, 2)},
{{- php(1, 1), - php(2, 1)}, {php(1, 1), php(1, 2)},
{- php(1, 2), - php(2, 2)}, {php(2, 1), php(2, 2)}}]
   \endverbatim
   </li>
   <li> To solve a pigeonhole principle problem, one possibility is to use the
   various DLL solvers available in the Backtracking module. The simplest of
   these has input a formal clause-set and returns either true or false. 
   \verbatim
(%i30) dll_simplest_first_shortest_clause(php22);
(%o30)                                true
   \endverbatim
   </li>
   <li> For each DLL solver there is also a version which returns the splitting
   tree:
   \verbatim
(%i31) split_tree:dll_simplest_st_first_shortest_clause(php22);
(%o31) [php(1, 1), [- php(1, 2), [php(2, 2), [- php(2, 1), [true], [false]],
[false]], [false]], [php(2, 1), [- php(2, 2), [php(1, 2), [true], [false]],
[false]], [false]]]
   \endverbatim
   </li>
   <li> From a splitting tree the satisfying assignments can be extracted
   using the sat_pass_st() function:
   \verbatim
(%i32) sat_pass_st(split_tree);
(%o32) [{- php(1, 1), php(1, 2), php(2, 1), - php(2, 2)},
                              {php(1, 1), - php(1, 2), - php(2, 1), php(2, 2)}]
   \endverbatim
   </li>
   <li> For output, we can create a splitting tree diagram in PStree format
   (the file "tree.tex" is placed in the directory from where you started Maxima).
   \verbatim
(%i33) tex_st_f("tree.tex", split_tree)$
   \endverbatim
   </li>
  </ol>


  <h2> Special generators </h2>

  <ol>
   <li> vanderwaerden2_fcs(k,n) is the Boolean clause-set whose solutions
   are the partitionings of {1,...,n} into two parts such that none of them
   contains an arithmetic progression of size k.
   \verbatim
(%i34) vdw38:vanderwaerden2_fcs(3,8);
(%o34) [{1, 2, 3, 4, 5, 6, 7, 8}, {{- 8, - 7, - 6}, {- 8, - 6, - 4},
{- 8, - 5, - 2}, {- 7, - 6, - 5}, {- 7, - 5, - 3}, {- 7, - 4, - 1},
{- 6, - 5, - 4}, {- 6, - 4, - 2}, {- 5, - 4, - 3}, {- 5, - 3, - 1},
{- 4, - 3, - 2}, {- 3, - 2, - 1}, {1, 2, 3}, {1, 3, 5}, {1, 4, 7}, {2, 3, 4},
{2, 4, 6}, {2, 5, 8}, {3, 4, 5}, {3, 5, 7}, {4, 5, 6}, {4, 6, 8}, {5, 6, 7},
{6, 7, 8}}]
(%i35) dll_simplest_first_shortest_clause(vdw38);
(%o35)                               true
(%i36) vdw39:vanderwaerden2_fcs(3,9);
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

*/

