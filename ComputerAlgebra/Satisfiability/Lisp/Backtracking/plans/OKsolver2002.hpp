// Oliver Kullmann, 19.1.2008 (Swansea)
/* Copyright 2008, 2009, 2011, 2013 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/Backtracking/plans/OKsolver2002.hpp
  \brief Plans for the Maxima/Lisp specification of the original OKsolver


  \todo Differences to the original OKsolver2002
  <ul>
   <li> On weak_php_fcs(5,4) the C-OKsolver needs 17 nodes and depth 6,
   \verbatim
Pigeonhole> OKsolver_2002-O3-DNDEBUG php54_w.cnf
s UNSATISFIABLE
c sat_status=0 initial_maximal_clause_length=4 initial_number_of_variables=20 initial_number_of_clauses=45 initial_number_of_literal_occurrences=100 running_time(s)=0.0 number_of_nodes=17 number_of_single_nodes=0 number_of_quasi_single_nodes=6 number_of_2-reductions=23 number_of_pure_literals=5 number_of_autarkies=0 number_of_missed_single_nodes=0 max_tree_depth=6 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=0 reduced_number_of_clauses=0 reduced_number_of_literal_occurrences=0 number_of_1-autarkies=72 number_of_initial_unit-eliminations=0 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=40 file_name=php54_w.cnf
   \endverbatim
   while the Maxima-OKsolver needs 15 nodes and depth 5. </li>
   <li> The Maxima-tree is (labelled):
   \verbatim
okt_php_54 : OKsolver_2002_st(weak_php_fcs(5,4));
[- php(1, 1), [- php(2, 2), [false], [false]],
 [- php(1, 2), [- php(2, 1), [false], [false]],
  [- php(2, 3), [false], 
   [- php(2, 4), [false],
    [- php(3, 1), [false], [false]]]]]];
   \endverbatim
   This tree seems correct.
   </li>
   <li> We need to get the tree also with the distances, so that we can
   collapse the inf-branches.
   \verbatim
okat_php_54 : OKsolver_2002_ast(weak_php_fcs(5,4));
[[- php(1, 1), [[0.2, 0.8], [1, 5]]], 
  [[- php(2, 2), [[0, inf]]], [false], [false]],
  [[- php(1, 2), [[1, 0.8], [1, 5]]], 
    [[- php(2, 1), [[0, inf]]], [false], [false]],
    [[- php(2, 3), [[0, inf]]], [false], 
      [[- php(2, 4), [[0, inf]]], [false],
        [[- php(3, 1), [[0, inf]]], [false], [false]]]]]]
count_inf_branches(okat_php_54, 0);
5
okcat_php_54 : collapse_inf_branches(okat_php_54, 0);
[[- php(1, 1), [[0.2, 0.8], [1, 5]]], [false],
  [[- php(1, 2), [[1, 0.8], [1, 5]]], [false], [false]]]
   \endverbatim  
   </li>
   <li> It seems that at the deep right end the C-OKsolver added
   a further branching, namely a quasi-single node (the Maxima-solver
   has only 5 quasi-single nodes, while the C-solver has 6). </li>
   <li> Could be chance (due to the quasi-single-nodes). </li>
   <li> But we must check; we need the C-OKsolver to output the
   branching literals. See "OUTPUTTREEDATAXML" in
   Solvers/OKsolver/SAT2002/plans/general.hpp. </li>
   <li> And we need to develop methods for analysing trees like
   the above; adorning it with further data like
    <ol>
     <li> the number of 2-reductions (obtained by amending
     the tree as r_2-splitting tree and adding the sizes of partial
     assignments). </li>
     <li> the number of quasi-single-nodes  </li>
    </ol>
    etc. </li>
   <li> For weak_php_fcs(6,5) the problem gets further pronounced:
   Here we have 79 nodes versus 89 nodes, and a depth of 9 versus 10. </li>
   <li> And with weak_php_fcs(7,6) we have 479 nodes and height 14 against 539
   nodes and height 15. </li>
   <li> So there seems to be a systematic problem. </li>
   <li> By the way, we should also collect data on weak (and strong) php:
    <ol>
     <li> Such data should perhaps be systematically collected somewhere.
     Perhaps in part Experimentation. </li>
     <li> First weak_php_fcs(m+1,m), for m >= 4. </li>
     <li> For m = 3 we have 3 nodes, height = 1 = levelled height. Likely
     we should exclude m <= 3 due to "irregularity". </li>
     <li> The levelled height of the trees found by the Maxima-OKsolver is
     2,3,4 for the above three formulas. Since the levelled height of
     weak_php_fcs(m+1,m) is m, and the OKsolver uses r_2, this seems optimal.
     It should be m-2 in general. </li>
     <li> Height is 5, 9, 14. Could be quadratic (then (8,7) -> 20).
     And yes, the C-OKsolver yields 6,10,15,21,28,36,45. So the guess is 
     that the first differences are 4,5,6, ... </li>
     <li> For the number of nodes we have 17, 79, 479. One would expect
     these numbers to be somewhat "random". </li>
     <li> The optimal numbers of nodes for r_2-splitting trees are:
      <ol>
       <li> weak_php_fcs(3,2) : 1 node (height 0) </li>
       <li> weak_php_fcs(4,3) : 3 nodes (height 1) </li>
       <li> weak_php_fcs(5,4) : 11 nodes (height 3); tree is
       \verbatim
[php(1, 1), 
  [php(1, 2), [php(2, 1), [false], [false]], 
    [php(2, 1), [false], [false]]],
  [php(2, 2), [false], [false]]]
       \endverbatim
       (The OKsolver-tree above needed more nodes since it used
       the implicit reductions when 2-CLSs were detected; after collapsing
       these reductions then the tree only has 5 nodes.)
       </li>
      </ol>
     </li>
    </ol>
   </li>
  </ul>


  \todo Annotated tree output
  <ul>
   <li> The tree output by function OKsolver_2002_ast (an annotated splitting
   tree) should mirror the information output by the OKsolver_2002 when
   compile-time option OUTPUTTREEDATAXML is used. </li>
  </ul>


  \todo Improving the implementation
  <ul>
   <li> Currently, the Maxima implementation of the OKsolver is basically
   unusable on just a bit bigger instances, since it's so slow. </li>
   <li> The main bottleneck should be the generalised_ucp_ple1, together
   with the look-ahead heuristics. </li>
   <li> Using clause-lists instead of clause-sets should be the first
   step. </li>
   <li> And then likely we need an instance of the general DPLL algorithm
   using the clause-variable-graph datastructure (so that UCP becomes
   linear time). </li>
  </ul>


  \todo Tree pruning ("intelligent backtracking")
  <ul>
   <li> We need to get the variables used in a refutation. </li>
   <li> Then tree-pruning follows straight. </li>
  </ul>


  \todo Local learning
  <ul>
   <li> The basic problem with local learning is, which binary clauses to add.
   </li>
   <li> One possibility is, of course, all possible binary clauses. </li>
   <li> We need also to figure out the scheme which was to be intended
   finally, a kind of "canonically minimal scheme". </li>
   <li> The added binary clauses are best kept in an additional clause-set
   (for each recursion). </li>
  </ul>


  \todo Adding look-ahead autarky clauses
  <ul>
   <li> An experimental feature of the OKsolver-2002 is the use of
   "near-autarkies", that is, if a partial assignment phi_x, obtained by 
   extending x -> 1, yields exactly one new clause C, then for each a in C
   the binary clause {-a,x} is added. </li>
   <li> This addition is a form of "local learning" (happens only at the
   residual clause-sets). </li>
   <li> Thus it can be combined with local learning (see above). </li>
   <li> Problematic the non-confluence (i.e., the dependency on the order
   we run through the variables). </li>
   <li> Apparently this addition cannot be combined with tree pruning. </li>
   <li> Also with counting satisfying assignments this is not compatible.
   </li>
   <li> One could allow more new clauses, analysing for example whether a
   literal occurs in all of them (then again we would obtain a binary clause).
   </li>
  </ul>


  \todo Counting satisfying assignments
  <ul>
   <li> At this level it should be easy to add counting of satisfying
   assignments. </li>
  </ul>

*/
