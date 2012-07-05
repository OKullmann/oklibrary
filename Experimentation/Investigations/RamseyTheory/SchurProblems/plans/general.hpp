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
  for the "injective" versions.

  The numbers are called genschur_m(k_1, ..., k_m) resp.
  genschuri_m(k_1, ..., k_m). For the standard Schur-numbers we have
  schur(r) = genschur_r(3,...,3) and
  schuri(r) = genschuri_r(3,...,3).

  SAT-translations are
  output_schur_stdname(r,n) for the SAT-problem "schur(r) > n ?", using
  the (strong) direct translation for the Schur-hypergraph, and
  output_schur_sb_stdname(r,n), additionally using symmetry-breaking
  (putting vertex 1 into the first partition (colour-class).

  The underlying hypergraph is schurtriples_hg(n), e.g.
  schurtriples_hg(5) =
    [{1,2,3,4,5},{{1,2},{1,2,3},{1,3,4},{1,4,5},{2,3,5},{2,4}}].


  \todo What is known
  <ul>
   <li> For a natural number r >= 1 the Schur-number schur(r) is the smallest
   n such that for every partition {1,...,n} into r parts at least one part
   contains a triple (x,y,z) in {1,...,n}^3 with x+y=z. </li>
   <li> So schur(r) = genschur_r(3, ..., 3). </li>
   <li> And schuri(r) = genschuri_r(3, ..., 3). </li>
   <li> Only schur(1)=2, schur(2)=5, schur(3)=14 and schur(4)=45
   are known; this is A030126 in the OEIS. </li>
   <li> See http://mathworld.wolfram.com/SchurNumber.html . </li>
   <li> While schur(5)=160 is conjectured (research problem 8.1 in
   [Landman, Robertson, 2003]), where 160 <= schur(5) <= 316 is known. </li>
   <li> Furthermore schur(6) >= 536 and schur(7) >= 1680. </li>
   <li> At http://www.di.univaq.it/~formisano/CLPASP/node3.html one finds
   some results (mchaff, simo, relsat, zchaff) obtained by answer-set
   translations (likely irrelevant here). </li>
   <li> There is also a little talk
   http://www.rumathphysics.org/brandt/schur.pdf ; we could contact the
   author (once we can enumerate all solutions). </li>
   <li> Also the strict versions, where x,y are required to be different,
   are true. Let's call the numbers schuri(r) ("i" for "injective"). </li>
   <li> The generalised Schur-numbers genschur_m(k_1, ..., k_m) for m >= 0
   and k_i >= 2 asks for the existence of 1 <= i <= m such that part i
   contains a tuple (x_1, ..., x_{k_i}) with x_{k_i} = x_1 + ... + x_{k_i-1}.
   </li>
  </ul>


  \todo OKsolver_2002
  <ul>
   <li> Performance looks reasonable:
   \verbatim
OKplatform> OKsolver_2002-O3-DNDEBUG Schur_2_5.cnf
s UNSATISFIABLE
c sat_status=0 initial_maximal_clause_length=3 initial_number_of_variables=10 initial_number_of_clauses=22 initial_number_of_literal_occurrences=52 running_time(s)=0.0 number_of_nodes=1 number_of_single_nodes=0 number_of_quasi_single_nodes=0 number_of_2-reductions=1 number_of_pure_literals=0 number_of_autarkies=0 number_of_missed_single_nodes=0 max_tree_depth=0 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=0 reduced_number_of_clauses=0 reduced_number_of_literal_occurrences=0 number_of_1-autarkies=0 number_of_initial_unit-eliminations=0 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=14 file_name=Schur_2_5.cnf
OKplatform> OKsolver_2002-O3-DNDEBUG Schur_2_4.cnf
s SATISFIABLE
c sat_status=1 initial_maximal_clause_length=3 initial_number_of_variables=8 initial_number_of_clauses=16 initial_number_of_literal_occurrences=36 running_time(s)=0.0 number_of_nodes=1 number_of_single_nodes=0 number_of_quasi_single_nodes=0 number_of_2-reductions=0 number_of_pure_literals=0 number_of_autarkies=1 number_of_missed_single_nodes=0 max_tree_depth=0 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=0 reduced_number_of_clauses=0 reduced_number_of_literal_occurrences=0 number_of_1-autarkies=0 number_of_initial_unit-eliminations=0 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=12 file_name=Schur_2_4.cnf
OKplatform> OKsolver_2002-O3-DNDEBUG Schur_3_13.cnf
s SATISFIABLE
c sat_status=1 initial_maximal_clause_length=3 initial_number_of_variables=39 initial_number_of_clauses=178 initial_number_of_literal_occurrences=477 running_time(s)=0.0 number_of_nodes=4 number_of_single_nodes=0 number_of_quasi_single_nodes=0 number_of_2-reductions=5 number_of_pure_literals=0 number_of_autarkies=1 number_of_missed_single_nodes=0 max_tree_depth=3 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=0 reduced_number_of_clauses=0 reduced_number_of_literal_occurrences=0 number_of_1-autarkies=102 number_of_initial_unit-eliminations=0 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=57 file_name=Schur_3_13.cnf
OKplatform> OKsolver_2002-O3-DNDEBUG Schur_3_14.cnf
s UNSATISFIABLE
c sat_status=0 initial_maximal_clause_length=3 initial_number_of_variables=42 initial_number_of_clauses=203 initial_number_of_literal_occurrences=546 running_time(s)=0.0 number_of_nodes=23 number_of_single_nodes=0 number_of_quasi_single_nodes=0 number_of_2-reductions=84 number_of_pure_literals=0 number_of_autarkies=0 number_of_missed_single_nodes=0 max_tree_depth=4 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=0 reduced_number_of_clauses=0 reduced_number_of_literal_occurrences=0 number_of_1-autarkies=387 number_of_initial_unit-eliminations=0 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=63 file_name=Schur_3_14.cnf
OKplatform> OKsolver_2002-O3-DNDEBUG Schur_4_44.cnf
s SATISFIABLE
c sat_status=1 initial_maximal_clause_length=4 initial_number_of_variables=176 initial_number_of_clauses=2244 initial_number_of_literal_occurrences=6424 running_time(s)=0.1 number_of_nodes=243 number_of_single_nodes=0 number_of_quasi_single_nodes=0 number_of_2-reductions=1727 number_of_pure_literals=0 number_of_autarkies=2 number_of_missed_single_nodes=0 max_tree_depth=18 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=0 reduced_number_of_clauses=0 reduced_number_of_literal_occurrences=0 number_of_1-autarkies=14747 number_of_initial_unit-eliminations=0 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=352 file_name=Schur_4_44.cnf
OKplatform> OKsolver_2002-O3-DNDEBUG Schur_4_45.cnf
s UNSATISFIABLE
c sat_status=0 initial_maximal_clause_length=4 initial_number_of_variables=180 initial_number_of_clauses=2339 initial_number_of_literal_occurrences=6704 running_time(s)=110.6 number_of_nodes=681344 number_of_single_nodes=4045 number_of_quasi_single_nodes=0 number_of_2-reductions=4195926 number_of_pure_literals=0 number_of_autarkies=22317 number_of_missed_single_nodes=10760 max_tree_depth=36 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=0 reduced_number_of_clauses=0 reduced_number_of_literal_occurrences=0 number_of_1-autarkies=33684148 number_of_initial_unit-eliminations=0 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=358 file_name=Schur_4_45.cnf
OKplatform> OKsolver_2002-O3-DNDEBUG Schur_5_159.cnf
s UNKNOWN
c sat_status=2 initial_maximal_clause_length=5 initial_number_of_variables=795 initial_number_of_clauses=33349 initial_number_of_literal_occurrences=98380 running_time(s)=1000.8 number_of_nodes=43992 number_of_single_nodes=5 number_of_quasi_single_nodes=0 number_of_2-reductions=692072 number_of_pure_literals=0 number_of_autarkies=0 number_of_missed_single_nodes=146 max_tree_depth=52 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=0 reduced_number_of_clauses=0 reduced_number_of_literal_occurrences=0 number_of_1-autarkies=12321670 number_of_initial_unit-eliminations=0 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=1985 file_name=Schur_5_159.cnf
OKplatform> OKsolver_2002-O3-DNDEBUG Schur_5_119.cnf
s SATISFIABLE
c sat_status=1 initial_maximal_clause_length=5 initial_number_of_variables=595 initial_number_of_clauses=19009 initial_number_of_literal_occurrences=55780 running_time(s)=74.7 number_of_nodes=16042 number_of_single_nodes=0 number_of_quasi_single_nodes=0 number_of_2-reductions=282401 number_of_pure_literals=0 number_of_autarkies=13 number_of_missed_single_nodes=1 max_tree_depth=47 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=0 reduced_number_of_clauses=0 reduced_number_of_literal_occurrences=0 number_of_1-autarkies=2874235 number_of_initial_unit-eliminations=0 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=1485 file_name=Schur_5_119.cnf
   \endverbatim
   </li>
   <li> On the unsatisfiable instances according to
   http://www.di.univaq.it/~formisano/CLPASP/node3.html
   only relsat is faster, while on the satisfiable instances
   only simo is faster. </li>
   <li> The minisat2-preprocessor doesn't seem to help:
   \verbatim
OKplatform> OKsolver_2002-m2pp Schur_5_119.cnf

s UNKNOWN
c sat_status=2 initial_maximal_clause_length=12 initial_number_of_variables=476 initial_number_of_clauses=18219 initial_number_of_literal_occurrences=84980 running_time(s)=560.1 number_of_nodes=132039 number_of_single_nodes=269 number_of_quasi_single_nodes=0 number_of_2-reductions=1694425 number_of_pure_literals=0 number_of_autarkies=1 number_of_missed_single_nodes=449 max_tree_depth=59 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=0 reduced_number_of_clauses=0 reduced_number_of_literal_occurrences=0 number_of_1-autarkies=47 number_of_initial_unit-eliminations=0 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=950 file_name=Schur_5_119.cnf_m2pp_7169
   \endverbatim
   </li>
  </ul>


  \todo march_pl
  <ul>
   <li> Satisfiability of Schur_4_44.cnf is also trivial. </li>
   <li> And unsatisfiability of Schur_4_45.cnf is established with
   91862 nodes (in ~ 52s). </li>
   <li> Satisfiability of Schur_5_119.cnf established with 65945 nodes (in
   ~ 204s). </li>
  </ul>


  \todo minisat2
  <ul>
   <li> Satisfiability of Schur_4_44.cnf is also trivial. </li>
   <li> But unsatisfiability of Schur_4_45.cnf is more problematic,
   needs 7091007 conflicts (26 restarts, 290s). </li>
   <li> So apparently minisat2 is not very strong here. </li>
  </ul>
   
*/


