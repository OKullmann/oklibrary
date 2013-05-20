// Oliver Kullmann, 19.4.2009 (Swansea)
/* Copyright 2009, 2010, 2011, 2013 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Solvers/Satz/plans/general.hpp
  \brief Plans for maintaining solvers from the Satz family

  Yet only satz215 is considered.


  \todo Improve the statistics output
  <ul>
   <li> DONE
   The output should confirm to the Dimacs-output form. </li>
   <li> The labels for the various numbers should be improved resp.
   introduced at all:
    <ol>
     <li> "NB_BRANCHE" should mean the number of branches, that is,
     the number of nodes - 1; but what then is NB_BACK? It is always
     a bit more than half of NB_BRANCHE. </li>
     <li> Discussion with Chu Min Li:
     \verbatim
> Now from the code
>
> int choose_and_instantiate_variable_in_clause() {
>    int var, nb=0, chosen_var=NONE;
>    long  i, posi, nega;
>    unsigned long nb_clauses, max_nb_clauses = 0;
>    my_type pos2, neg2;
>    long saved_nb_back;
>    struct var_node *pvar_node;
>    NB_BRANCHE++;
>
> I would assume that NB_BRANCHE is exatly the total number of nodes in
> the search tree, or?

Yes, if you also count the nodes having only one child and the nodes
which are leaves.
There are three possible situations when this function is called:

(i) an inconsistency is found (when lookahead finds a contradiction),
the node is a leaf.
(ii) a branching variable x is chosen, but x=true and/or x=false leads
to a contradiction
via the following unit propagation. This is possible, because the
lookahead is not complete.
In this case, the node is either a leaf or only has one child.
(iii) a branching variable x is chosen and the node has two children
(neither x=true nor x=false
leads to a contradiction via the following unit propagation).
     \endverbatim
     </li>
     <li> Regarding NB_BACK:
     \verbatim
>
> And from
>
> int backtracking() {
>   int var, index;
>   UNITCLAUSE_STACK_fill_pointer = 0;
>   NB_BACK++;
>
> For unsatisfiable problems:
> I would guess that NB_BACK either is the number of edges in the search
> tree (if a backtrack only backtracks to the immediate predecessor), or
> is the number of leaves in the tree (if one backtracking goes as
> far back
> as possible in one step).

NB_BACK is the number of nodes having two children (corresponding to
situation (iii)
above), since there is exactly one backtracking per such node.
However, since the number
of leaves of a binary tree is roughly equal to the number
of internal nodes (+1), you also can say NB_BACK is the number of
leaves (-1).
     \endverbatim
     </li>
     <li> Regarding other data:
     \verbatim
> However, e.g.,
>
>> satz215 Schur/Schur_3_13.cnf
> **** The instance is satisfiable. *****
> NB_MONO= 0, NB_UNIT= 117, NB_BRANCHE= 13, NB_BACK= 5
> Program terminated in 0.000 seconds.
> satz215 Schur/Schur_3_13.cnf 0.000 13 5 468 15 1 39 178 -12 15 6
>
>
> If I understand it right, then your tree should be binary, that is,
> every
> inner node should have exactly two children. Now NB_BRANCHE could be
> the full tree of depth 3, but where one node at level 2 is also a
> leaf.
> Then we have 15-2 = 13 nodes altogether --- but we have 7 leaves (and
> 6 inner nodes) ?
>
> Furthermore, what is "-12" here? You said it's the number of
> added resolvents of length <= 3 --- so do you perform
> also subsumption, and so also clauses are removed?

Yes.
     \endverbatim
     </li>
    </ol>
   </li>
   <li> Our current analysis in ExtractSatz.awk is:
   \verbatim
BEGIN {
  rn=0; rc=0; t=0; sat=2; nds=0; r1=0; file=""; bnds=0; r2la=0
  r2=0; prpr=0; r3la=0; r3=0
}
/^satz215 ([^ ]+ )+/ {
  file=$2
  t=$3
  nds=$4
  bnds=$5
  r2la=$6
  r2=$7
  sat=$8
  rn=$9
  rc=$10
  prpr=$11 # preprocessing: number of added resolvents of lengths 2,3 minus number of clauses eliminated by subsumption
  r3la=$12
  r3=$13
}
/^NB_MONO= / {
  pls=$2; sub(/,/, "",pls)
  r1=$4; sub(/,/, "",r1)
}
END {
  print rn " " rc " " t " " sat " " nds " " r1 " " r2 " " pls " \"" file "\" " bnds " " \
    r2la " " r3 " " r3la " " prpr
}
   \endverbatim
   Clause elimination (in prpr) happens also by pure-literals; it is now called
   "dc" (for "delta c"). </li>
   <li> And we need additional documentation on these numbers. </li>
   <li> DONE
   We should also update the output to the file "satz215_timetable":
    <ol>
     <li> It should be just like the extracted statistics file. </li>
     <li> So when it is created, the header-line is output. </li>
     <li> And no output of the leading "satz215". </li>
    </ol>
   </li>
  </ul>


  \todo Improve range of statistic measurements : DONE
  <ul>
   <li> Yet for the four basic statistics variables, <code> NB_UNIT, NB_MONO,
   NB_BRANCHE, NB_BACK </code> just type <code>long</code> is used. </li>
   <li> A typedef should be introduced. </li>
   <li> The type should become unsigned. </li>
   <li> And it should be a 64-bit type. </li>
  </ul>


  \todo Improve file output
  <ul>
   <li> We should use the same conventions as with the OKsolver_2002. </li>
  </ul>


  \todo Correct memory handling
  <ul>
   <li> With Sudoku problems of box-dimension 6 we get a memory access error:
   \verbatim
> satz215 Strong_Sudoku_Box_dim_6.cnf
/h/21/GemeinsameBasis/SAT-Algorithmen/OKplatform/bin/satz215: line 12: 13463 Speicherzugriffsfehler  (core dumped) /home/kullmann/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/Installations/SAT/Satz/satz215/satz215 $*
   \endverbatim
   </li>
  </ul>


  \todo Use signals as with OKsolver_2002
  <ul>
   <li> The solver should react as OKsolver_2002 to the signals SIGINT and
   SIGUSR1. </li>
  </ul>


  \todo Provide application tests


  \todo Update to C99
  <ul>
   <li> Improve naming (functions, variables, types, macros). </li>
   <li> Improve comments. </li>
   <li> Introduce const. </li>
   <li> Make variables more local. </li>
   <li> Use type bool. </li>
   <li> Add least some macros can be replaced by constants. </li>
  </ul>


  \todo Add Doxygen documentation

*/

