// Oliver Kullmann, 2.10.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/Groupoids/plans/Transformations.hpp
  \brief On investigations regarding transformation semigroups


  \todo Hypergraph transformations I
  <ul>
   <li> Consider a natural number n >= 0, N = {1,...,n}, the powerset P =
   powerset(P), and the set MS_n =powerset(P) of all set-systems on N. </li>
   <li> Let c_0: MS -> MS be direct complementation: c_0(S) = P - S. </li>
   <li> Let c_1: MS -> MS be elementwise complementation: c_1(S): c_1(S) =
   { N - T : T in S }. </li>
   <li> Let s: MS -> MS be the closure under subsumption: s(S) = union_{T in S}
   powerset(T). </li>
   <li> What is the closure of {c_0, c_1, s} under composition? </li>
   <li>
   \verbatim
oklib_include("OKlib/ComputerAlgebra/Algebra/Lisp/Groupoids/Closures.mac")$
oklib_include("OKlib/ComputerAlgebra/Algebra/Lisp/Groupoids/Semigroups/TransformationMonoids.mac")$
oklib_include("OKlib/ComputerAlgebra/Hypergraphs/Lisp/SetSystems.mac")$
oklib_include("OKlib/ComputerAlgebra/DataStructures/Lisp/HashMaps.mac")$

det_cl(n) := block([N : setn(n), P, MS, c0, c0l, c1,c1l, s,sl, h, MSA, C],
 P : powerset(N),
 MS : powerset(P),
 c0 : lambda([S], setdifference(P,S)),
 c1 : lambda([S], ecomp(S,N)),
 s : lambda([S], subset_closure(S)),
 h : osm2hm(ll2osm(listify(MS),create_list(i,i,1,2^(2^n)))),
 MSA : l2ary(listify(MS)),
 c0l : create_list(ev_hm(h,c0(MSA[i])), i,1,2^(2^n)),
 c1l : create_list(ev_hm(h,c1(MSA[i])), i,1,2^(2^n)),
 sl : create_list(ev_hm(h,s(MSA[i])), i,1,2^(2^n)),
 C : closure_bydef_grd(trf_l_compo, {c0l,c1l,sl}),
 length(C)
)$
create_list(det_cl(n),n,0,3);
  [2,28,28,28]
   \endverbatim
   </li>
   <li> So it seems as soon as the base set contains at least one element,
   the closure contains exactly 28 elements. One now needs to determine
   them. </li>
  </ul>


  \todo Hypergraph transformations II
  <ul>
   <li> Additionally, let m: MS -> MS be subsumption elimination: m(S) =
   {T in S : there is no T' in S with T < T'}. </li>
   <li>
   \verbatim
det2_cl(n) := block([N : setn(n), P, MS, c0, c0l, c1,c1l, s,sl, m,ml, h, MSA, C],
 P : powerset(N),
 MS : powerset(P),
 c0 : lambda([S], setdifference(P,S)),
 c1 : lambda([S], ecomp(S,N)),
 s : lambda([S], subset_closure(S)),
 m : lambda([S], min_elements(S)),
 h : osm2hm(ll2osm(listify(MS),create_list(i,i,1,2^(2^n)))),
 MSA : l2ary(listify(MS)),
 c0l : create_list(ev_hm(h,c0(MSA[i])), i,1,2^(2^n)),
 c1l : create_list(ev_hm(h,c1(MSA[i])), i,1,2^(2^n)),
 sl : create_list(ev_hm(h,s(MSA[i])), i,1,2^(2^n)),
 ml : create_list(ev_hm(h,m(MSA[i])), i,1,2^(2^n)),
 C : closure_bydef_grd(trf_l_compo, {c0l,c1l,sl,ml}),
 length(C)
)$
:lisp (ext:set-limit 'ext:heap-size 1073741824)
for i : 0 thru 3 do print(i, det2_cl(i));
0 2
1 176
2 404
3 
   \endverbatim
   </li>
  </ul>

*/

