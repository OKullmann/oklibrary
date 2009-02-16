// Oliver Kullmann, 16.2.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/Transversals.hpp
  \brief Investigating the transversal hypergraph of van-der-Waerden hypergraphs

  The transversal hypergraph T(n,k) of arithprog_hg(k,n) for fixed k and
  running n has at least been implicitly studied for over 70 years in the
  form of r_k(n), the independence number of arithprog_hg(k,n).
  <ul>
   <li> It seems more natural to study the transversal number of
   arithprog_hg(k,n), that is, the minimal length of a hyperedge of T(n,k).
   </li>
   <li> The literature seems to concentrate asymptotic results on r_k(n).
   </li>
   <li> This we want to complement by detailed studies on T(n,k). </li>
   <li> Considering the independence number instead of the transversal number,
   there are results in the literature (see below). </li>
  </ul>


  \todo Naming the transversal hypergraph of arithprog_hg(k,n)
  <ul>
   <li> Calling it tr_arithprog_hg(k,n). </li>
   <li> Perhaps tr_arithprog_hg(k,n) is a function which calls the special
   algorithms for computing transversal hypergraphs, according to which is
   best. </li>
   <li> For a hypergraph G, tau(G) is the transversal number. How to call
   the transversal number of arithprog_hg(k,n)? tau_arithprog_hg(k,n). </li>
   <li> While the independence number shall be called alpha_arithprog_hg(k,n).
   </li>
  </ul>


  \todo Relations to vanderwaerden_k(n_1, ..., n_k)
  <ul>
   <li> Let [2]_k denotes the list of k 2's. </li>
   <li> Then we have vanderwaerden_{k+1}([2]_k, m) > n iff
   tau_arithprog_hg(m,n) <= k. </li>
   <li> vanderwaerden_{k+1}([2]_k, m) is the smallest n such that
   however k numbers are picked, an arithmetic progression of size m
   must be contained. </li>
   <li> A related sequence is A065825, which for given k is the smallest
   n such that alpha_arithprog_hg(3,n) = k. </li>
   <li> The known values are
   1, 2, 4, 5, 9, 11, 13, 14, 20, 24, 26, 30, 32, 36, 40, 41, 51, 54, 58, 63,
   71, 74, 82, 84, 92, 95, 100, 104, 111, 114, 121, 122, 137, 145, 150, 157,
   starting with k=1. </li>
   <li> For given m, the sequences vanderwaerden_{k+1}([2]_k, m),
   tau_arithprog_hg(m,n), alpha_arithprog_hg(3,n), A065825 are related as
   follows:
    <ol>
     <li> Given the sequence vanderwaerden_{k+1}([2]_k, m), one obtains
     tau_arithprog_hg(m,n) by running through k=0,1,..., and observing
     when first the value is strictly greater than n --- for this k we
     have tau_arithprog_hg(m,n) = k.
     <li> Given the sequence vanderwaerden_{k+1}([2]_k, m), one obtains
     A065825 XXX </li>
    </ol>
   </li>
  </ul>


  \todo Elementary statistics for k=3
  <ul>
   <li> Just using clause-set statistics-functions:
   \verbatim
L_0_20 : create_list(transversal_hg_rs(arithprog_hg(3,n)),n,0,20)$
for i : 0 thru 20 do (print(i, statistics_fcs(arithprog_hg(3,i)), 
 statistics_fcs(L_0_20[i+1]), ncl_list_fcs(L_0_20[i+1])));
   \endverbatim
   we obtain
   \verbatim
0 [0,0,0,-1,inf] [0,1,0,0,0] [[0,1]]
1 [1,0,0,-1,inf] [1,1,0,0,0] [[0,1]]
2 [2,0,0,-1,inf] [2,1,0,0,0] [[0,1]]
3 [3,1,3,3,3] [3,3,3,1,1] [[1,3]]
4 [4,2,6,3,3] [4,3,4,2,1] [[1,2],[2,1]]
5 [5,4,12,3,3] [5,3,5,2,1] [[1,1],[2,2]]
6 [6,6,18,3,3] [6,4,8,2,2] [[2,4]]
7 [7,9,27,3,3] [7,10,30,3,3] [[3,10]]
8 [8,12,36,3,3] [8,25,100,4,4] [[4,25]]
9 [9,16,48,3,3] [9,38,186,5,4] [[4,4],[5,34]]
10 [10,20,60,3,3] [10,48,264,6,5] [[5,24],[6,24]]
11 [11,25,75,3,3] [11,58,362,7,5] [[5,7],[6,30],[7,21]]
12 [12,30,90,3,3] [12,73,498,8,6] [[6,25],[7,36],[8,12]]
13 [13,36,108,3,3] [13,86,642,9,6] [[6,6],[7,41],[8,32],[9,7]]
14 [14,42,126,3,3] [14,109,878,10,6] [[6,1],[7,14],[8,76],[9,14],[10,4]]
15 [15,49,147,3,3] [15,146,1284,11,7] [[7,4],[8,32],[9,102],[10,6],[11,2]]
16 [16,56,168,3,3] [16,210,2018,12,8] [[8,14],[9,60],[10,131],[11,4],[12,1]]
17 [17,64,192,3,3] [17,285,2959,12,9] [[9,43],[10,92],[11,148],[12,2]]
18 [18,72,216,3,3] [18,411,4578,12,10] [[10,97],[11,160],[12,154]]
19 [19,81,243,3,3] [19,614,7294,13,11] [[11,220],[12,248],[13,146]]
20 [20,90,270,3,3] [20,957,12090,14,11] [[11,2],[12,451],[13,400],[14,104]]
   \endverbatim
   </li>
   <li> So the sequence tau_arithprog_hg(3,n) (starting with n=0) begins with
   0,0,0,1,1,1,2,3,4,4,5,5,6,6,6,7,8,9,10,11,11. </li>
   <li> The sequence n - tau_arithprog_hg(3,n) = r_3(n):
   0,1,2,2,3,4,4,4,4,5,5,6,6,7,8,8,8,8,8,8,9. </li>
   <li> Excluding the first term, this is sequence A003002;
   see [S. S. Wagstaff, Jr., On k-free sequences of integers, Math. Comp., 26
   (1972), 767-771]. </li>
   <li> Another sequence of interest is the number of hyperedges:
   1,1,1,3,3,3,4,10,25,38,48,58,73,86,109,146,210,285,411,614,957. </li>
   <li> Doesn't seem to be in the eis-database (also after excluding the first
   terms). </li>
   <li> Perhaps also the maximum size of a hyperedge is of interest:
   0,0,0,1,2,2,2,3,4,5,6,7,8,9,10,11,12,12,12,13,14. </li>
   <li> Doesn't seem to be in the eis-database (also after excluding the first
   terms). </li>
  </ul>


  \todo Elementary statistics for k=4
  <ul>
   <li> The sequence r_4(n) (starting with n=1) is A003003. </li>
  </ul>


  \todo Elementary statistics for k=5
  <ul>
   <li> The sequence r_5(n) (starting with n=1) is A003004. </li>
  </ul>


  \todo Elementary statistics for k=6
  <ul>
   <li> The sequence r_6(n) (starting with n=1) is A003005. </li>
  </ul>

*/

