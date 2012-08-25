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
   <li> See RamseyTheory/SchurProblems/plans/Symmetry.hpp for questions
   regarding symmetries. </li>
   <li> See "What is known" below for a quick overview. </li>
   <li> See below for the main research questions. </li>
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


  \todo Main research questions
  <ul>
   <li> Determine schur(5) and wschur(5).
    <ol>
     <li> Conjectures are schur(5) = 161 and wschur(5) = 196. </li>
     <li> Also pdschur(5) and pdwschur(5) are relevant (and more accessible).
     </li>
     <li> See RamseyTheory/SchurProblems/plans/Schur5/general.hpp. </li>
     <li> Regarding palindromic problems, there are conjectures related to
     the shape of the partitions; see "Forbidden elements for palindromic
     problems" in SchurProblems/plans/Schur5/CompleteSolvers/general.hpp. </li>
    </ol>
   </li>
   <li> Is there are palindromic gap? That is, is pdschur(r)=schur(r) for all
   r (taking the different representations into account)?
    <ol>
     <li> See "Palindromic problems" below. </li>
     <li> For weak problems we have a (clear) gap. </li>
    </ol>
   </li>
   <li> Is the restriction by "full symmetry-breaking"
   satisfiability-equivalent?
    <ol>
     <li> See "Full symmetry-breaking" in
     RamseyTheory/SchurProblems/plans/Symmetry.hpp. </li>
    </ol>
   </li>
   <li> Are there relations for different n's between palindromic problems?
    <ol>
     <li> Open for standard and weak problems. </li>
     <li> See "Relations between palindromic hypergraphs" in
     RamseyTheory/SchurProblems/plans/Symmetry.hpp. </li>
    </ol>
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
  [lambda([x],is(x < 1)),
   lambda([x],is(x < 2)),
   lambda([x],is(x < 5)),
   lambda([x],is(x < 14)),
   lambda([x],is(x < 45))]
   \endverbatim
   </li>
   <li> It seemed too complicated to use some implicit representations (via
   numbers), and so "palindromic Schur-numbers" are in fact functionals,
   yielding which cases are satisfiable. </li>
   <li> We see that we have no "palindromic gaps" here: This is the first and
   main open question --- is there always no palindromic gap?! See
   "Main research questions" above. </li>
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
  [lambda([x],is(x < 1)),
   lambda([x],is(x < 3)),
   lambda([x],is(x < 6)),
   lambda([x],is(x < 15)),
   lambda([n],if n >= 48 or n = 45 or n = 46 then false else true)]
   \endverbatim
   </li>
   <li> It seems that there is only little deviation from pdschur(r). </li>
  </ul>


  \todo Modular form of Schur numbers
  <ul>
   <li> The number of hyperedges of mschurtriples_ohg and its altogether
   6 variations (where we know 2 of them; see
   ComputerAlgebra/Hypergraphs/Lisp/Generators/Schur.mac) should be of
   interest to OEIS. The 2 known are in OEIS, but not the unknown ones. </li>
   <li> Similar for the 6 forms of numbers of hyperedges of
   symmetrictriples_ohg. Again we know two cases, but now none of them is in
   the OEIS:
   \verbatim
create_list(nhyp_ohg(symmetrictriples_ohg(n)),n,0,5);
  [0,0,0,20,506,14042]
create_list(nhyp_hg(symmetrictriples_hg(n)),n,0,5);
  [0,0,0,7,305,11003]
create_list(nhyp_ohg(symmetrictriples_me_ohg(n)),n,0,5);
  [0,0,0,7,302,10924]
create_list(nhyp_hg(symmetrictriples_me_hg(n)),n,0,5);
  [0,0,0,7,302,10924]
create_list(nhyp_ohg(wsymmetrictriples_ohg(n)),n,0,5);
  [0,0,0,18,492,13948]
create_list(nhyp_hg(wsymmetrictriples_hg(n)),n,0,5);
  [0,0,0,6,295,10919]

create_list(nhyp_symmetrictriples_ohg(n),n,0,5);
  [0,0,0,20,506,14042]
create_list(nhyp_wsymmetrictriples_ohg(n),n,0,5);
  [0,0,0,18,492,13948]
   \endverbatim
   </li>
   <li> Via
   \verbatim
> CRunMSchur 1 2 minisat-2.2.0
> CRunMSchur 2 5 minisat-2.2.0
> CRunMSchur 3 14 minisat-2.2.0
> CRunMSchur 4 45 minisat-2.2.0
   \endverbatim
   we determined mschur(r) for 0 <= r <= 4:
   \verbatim
create_list(mschur(r),r,0,4);
  [lambda([x],is(x < 1)),
   lambda([x],is(x < 2)),
   lambda([x],is(x < 5)),
   lambda([x],is(x < 14)),
   lambda([x],is(x < 45))]
   \endverbatim
   which is the same as schur(r) and pdschur(r) for these values. </li>
   <li> Via
   \verbatim
> CRunWMSchur 1 3 minisat-2.2.0
> CRunWMSchur 2 9 minisat-2.2.0
> CRunWMSchur 3 24 minisat-2.2.0
> CRunWMSchur 4 67 minisat-2.2.0
   \endverbatim
   we determined wmschur(r) for 0 <= r <= 4:
   \verbatim
create_list(wmschur(r),r,0,4);
  [lambda([x],is(x < 1)),
   lambda([x],is(x < 3)),
   lambda([x],is(x < 7)),
   lambda([x],is(x < 15)),
   lambda([n],if n < 45 or n = 47 then true elseif n > 51 then unknown)]
   \endverbatim
   We see that for r=2 we have one satisfiable instance more compared to the
   palindromic case (and two less than the ordinary case). </li>
  </ul>

*/
