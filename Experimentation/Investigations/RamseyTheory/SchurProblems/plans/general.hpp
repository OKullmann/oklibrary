// Oliver Kullmann, 17.4.2009 (Swansea)
/* Copyright 2009, 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/SchurProblems/plans/general.hpp
  \brief General plans regarding investigations on Schur problems (and generalisations)

  The hyperedges of "generalised Schur problems" are of the form
  {x_1, ..., x_{k-1}, x_k}, where x_k is the sum of x_1, ..., x_{k-1}.
  The (original) Schur problems have k=3. For the (standard) versions
  the x_1, ..., x_{k-1} don't need to be distinct, while this is required
  for the "weak" versions.

  The numbers are called genschur_m(k_1, ..., k_m) resp.
  wgenschur_m(k_1, ..., k_m). For the standard Schur-numbers we have
  schur(r) = genschur_r(3,...,3) and
  wschur(r) = wgenschur_r(3,...,3).

  The standard Schur-numbers are available at Maxima-level via schur(r):
  create_list(schur(r),r,1,8);
    [2,5,14,45,[161,306],[537,1837],[1681,12860],[3916,109601]].
  The weak Schur-numbers are available at Maxima-level via wschur(r):
  create_list(wschur(r),r,1,8);
    [3,9,24,67,[190,1631],[573,11743],[1681,95901],[3916,876809]].

  SAT-translations are
  output_schur_stdname(r,n) for the SAT-problem "schur(r) > n ?", using
  the (strong) direct translation for the Schur-hypergraph, and
  output_schur_sb_stdname(r,n), additionally using symmetry-breaking
  (putting vertex 1 into the first partition (colour-class).

  The underlying hypergraph is schurtriples_hg(n), e.g.
  schurtriples_hg(5) =
    [{1,2,3,4,5},{{1,2},{1,2,3},{1,3,4},{1,4,5},{2,3,5},{2,4}}],
  while
  schurtriples_ohg(5) =
    [[1,2,3,4,5],[{1,2},{1,2,3},{1,3,4},{2,4},{1,4,5},{2,3,5}]].
  Subsumption-eliminiation removes the hyperedge {1,2,3} here:
  schurtriples_me_hg(5) =
    [{1,2,3,4,5},{{1,2},{1,3,4},{1,4,5},{2,3,5},{2,4}}],
  schurtriples_me_ohg(5) =
    [[1,2,3,4,5],[{1,2},{1,3,4},{2,4},{1,4,5},{2,3,5}]].
  The SAT-translation uses the subsumption-eliminated hypergraph.

  Good basic information is available in Chapter 8 of [Ramsey Theory on the
  Integers; Landman, Robertson].

  The palindromic problems are created by output_pd_schur_stdname(r,n),
  with underlying hypergraph schurtriples_pd_hg(n).


  \todo Overview
  <ul>
   <li> See RamseyTheory/SchurProblems/plans/Schur4/general.hpp for
   investigations on schur(4). </li>
   <li> See RamseyTheory/SchurProblems/plans/Schur5/general.hpp for
   investigations on schur(5). </li>
   <li> See RamseyTheory/SchurProblems/plans/Schur6/general.hpp for
   investigations on schur(6). </li>
   <li> See "What is known" below for a quick overview. </li>
   <li> See below for a naive quick overview on the performance of complete
   SAT solvers. </li>
  </ul>


  \todo What is known
  <ul>
   <li> For a natural number r >= 1 the Schur-number schur(r) is the smallest
   n such that for every partition {1,...,n} into r parts at least one part
   contains a triple (x,y,z) in {1,...,n}^3 with x+y=z. </li>
   <li> So schur(r) = genschur_r(3, ..., 3). </li>
   <li> And wschur(r) = wgenschur_r(3, ..., 3). </li>
   <li> Only schur(1)=2, schur(2)=5, schur(3)=14 and schur(4)=45
   are known; this is A030126 in the OEIS. </li>
   <li> And only wschur(1)=3, wschur(2)=9, wschur(3)=24 and wschur(4)=67
   are known; this is A118771 in the OEIS. </li>
   <li> The determination of (w)schur(r) for 1 <= r <= 3 is trivial for
   any SAT solver. </li>
   <li> See http://mathworld.wolfram.com/SchurNumber.html . </li>
   <li> While schur(5)=160 is conjectured as research problem 8.1 in
   [Landman, Robertson, 2003]), where 160 <= schur(5) <= 316 is known.
   However the conjecture must be schur(5) = 161, since since 1994 a
   certificate showing schur(5)>160 is known. </li>
   <li> Furthermore schur(6) >= 537 and schur(7) >= 1681. </li>
   <li> At http://www.di.univaq.it/~formisano/CLPASP/node3.html one finds
   some results (mchaff, simo, relsat, zchaff) obtained by answer-set
   translations (likely irrelevant here). </li>
   <li> There is also a little talk
   http://www.rumathphysics.org/brandt/schur.pdf ; we could contact the
   author (once we can enumerate all solutions). </li>
   <li> Also the strict versions, where x,y are required to be different,
   are true. Let's call the numbers wschur(r) ("w" for "weak"). </li>
   <li> The generalised Schur-numbers genschur_m(k_1, ..., k_m) for m >= 0
   and k_i >= 2 asks for the existence of 1 <= i <= m such that part i
   contains a tuple (x_1, ..., x_{k_i}) with x_{k_i} = x_1 + ... + x_{k_i-1}.
   </li>
  </ul>


  \todo Palindromic problems
  <ul>
   <li> Via
   \verbatim
> CRunPdSchur 1 2 minisat-2.2.0
> CRunPdSchur 2 5 minisat-2.2.0
> CRunPdSchur 3 14 minisat-2.2.0
> CRunPdSchur 4 45 minisat-2.2.0
   \endverbatim
   we determined pdschur(r) for 0 <= r <= 4:
   \verbatim
create_list(pdschur(i),i,0,4);
  [[{},1],[{},2],[{},5],[{},14],[{},45]]
   \endverbatim
   </li>
   <li> A pair [S,k], where S is a set of natural numbers and k a natural
   number,means that k is the minimum such that for all n >= k the problems
   are unsatisfiable, while S is the set of exceptions below that, that is,
   where the problems are also unsatisfiable for n < k. </li>
   <li> We see that we have no "palindromic gaps" here: This is the first and
   main open question --- is there always no palindromic gap?! </li>
   <li> For pdschur(5) see "pdschur(5)" in
   RamseyTheory/SchurProblems/plans/Schur5/CompleteSolvers/general.hpp. </li>
   <li> Via
   \verbatim
> CRunPdWSchur 1 3 minisat-2.2.0
> CRunPdWSchur 2 9 minisat-2.2.0
> CRunPdWSchur 3 24 minisat-2.2.0
> CRunPdWSchur 4 67 minisat-2.2.0
   \endverbatim
   we determined pdwschur(r) for 0 <= r <= 4:
   \verbatim
create_list(pdwschur(i),i,0,4);
  [[{},1],[{},3],[{},6],[{},14],[{45,46},48]]
   \endverbatim
   </li>
   <li> It seems that there is only little deviation from pdschur(r). </li>
  </ul>


  \todo OKsolver_2002
  <ul>
   <li>
   \verbatim
OKplatform> OKsolver_2002-O3-DNDEBUG Schur_5_119.cnf
s SATISFIABLE
c sat_status=1 initial_maximal_clause_length=5 initial_number_of_variables=595 initial_number_of_clauses=19009 initial_number_of_literal_occurrences=55780 running_time(s)=74.7 number_of_nodes=16042 number_of_single_nodes=0 number_of_quasi_single_nodes=0 number_of_2-reductions=282401 number_of_pure_literals=0 number_of_autarkies=13 number_of_missed_single_nodes=1 max_tree_depth=47 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=0 reduced_number_of_clauses=0 reduced_number_of_literal_occurrences=0 number_of_1-autarkies=2874235 number_of_initial_unit-eliminations=0 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=1485 file_name=Schur_5_119.cnf
   \endverbatim
   </li>
   <li> The minisat2-preprocessor doesn't seem to help:
   \verbatim
OKplatform> OKsolver_2002-m2pp Schur_5_119.cnf
s UNKNOWN
c sat_status=2 initial_maximal_clause_length=12 initial_number_of_variables=476 initial_number_of_clauses=18219 initial_number_of_literal_occurrences=84980 running_time(s)=560.1 number_of_nodes=132039 number_of_single_nodes=269 number_of_quasi_single_nodes=0 number_of_2-reductions=1694425 number_of_pure_literals=0 number_of_autarkies=1 number_of_missed_single_nodes=449 max_tree_depth=59 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=0 reduced_number_of_clauses=0 reduced_number_of_literal_occurrences=0 number_of_1-autarkies=47 number_of_initial_unit-eliminations=0 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=950 file_name=Schur_5_119.cnf_m2pp_7169
   \endverbatim
   </li>
  </ul>

*/
