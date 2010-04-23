// Oliver Kullmann, 16.2.2009 (Swansea)
/* Copyright 2009, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/Transversals/plans/general.hpp
  \brief Investigating the transversal hypergraph of van-der-Waerden hypergraphs

  The transversal hypergraph T(n,k) of arithprog_hg(k,n) for fixed k and
  running n has at least been implicitly studied for over 70 years in the
  form of r_k(n), the independence number of arithprog_hg(k,n).
  <ul>
   <li> It seems more natural to study the transversal number of
   arithprog_hg(k,n), that is, the minimal length of a hyperedge of T(n,k).
   </li>
   <li> The literature seems to concentrate on asymptotic results for r_k(n).
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
   the transversal number of arithprog_hg(k,n)? tau_arithprog(k,n). </li>
   <li> While the independence number shall be called alpha_arithprog(k,n).
   </li>
  </ul>


  \todo Relations to vanderwaerden_m(k_1, ..., k_m)
  <ul>
   <li> Let [2]_m denotes the list of m 2's. </li>
   <li> Then we have vanderwaerden_{m+1}([2]_m, k) > n iff
   tau_arithprog(k,n) <= m. </li>
   <li> vanderwaerden_{m+1}([2]_m, k) is the smallest n such that
   however m numbers are picked, an arithmetic progression of size k
   must be contained. </li>
   <li> In other words, vanderwaerden_{m+1}([2]_m, k) is the smallest n such
   that tau_arithprog(k,n) > m. </li>
   <li> It seems that for computing vanderwaerden_{m+1}([2]_m, k), we best
   search for the smallest n with tau_arithprog(k,n) >= m+1 --- in this way
   we have only to decide whether or not a transversal of size at most m
   exists. This amounts to the same as computing the numbers
   tau_arithprog(k,n) in succession, just using that these numbers are
   non-decreasing and each step is at most 1. </li>
   <li> A related sequence is A065825, which for given m is the smallest
   n such that alpha_arithprog(3,n) = m.
    <ol>
     <li> The known values are
     1, 2, 4, 5, 9, 11, 13, 14, 20, 24, 26, 30, 32, 36, 40, 41, 51, 54, 58, 63,
     71, 74, 82, 84, 92, 95, 100, 104, 111, 114, 121, 122, 137, 145, 150, 157,
     163, 165, 169, 174, 194;
     starting with m=1 (added data from Bestk3.txt). </li>
     <li> This sequence is the same as obtained by computing the sequence of
     transversal numbers via VdWTransversalsInc and listing those n for which
     we obtained a satisfiable instance. </li>
     <li> This is because the independence number always increases by one at
     the satisfiable instances, while for the unsatisfiable instances the
     transversal numbe increases, i.e., the independence number stays the
     same. </li>
     <li> The satisfying assignments yield the complements of the minimum
     independent sets here. </li>
     <li> From this sequence we obtain values for alpha_arithprog(3,n) for
     n <= 194, quite a bit more what A003002 contains. </li>
     <li> We need to have a look at
     http://www.math.uni.wroc.pl/~jwr/non-ave.htm , where perhaps we obtain
     more information on how these numbers where obtained. </li>
     <li> Also of interest here are unique solutions, which yet we have for
     n in {1,2,5,14,30,41}. </li>
    </ol>
   </li>
   <li> For given k, the four sequences vanderwaerden_{m+1}([2]_m, k),
   tau_arithprog(k,n), alpha_arithprog(k,n), A065825 are related as
   follows:
    <ol>
     <li> Given the sequence vanderwaerden_{m+1}([2]_m, k), one obtains
     tau_arithprog(k,n) by running through m=0,1,..., and observing
     when first the value is strictly greater than n --- for this m we
     have tau_arithprog(k,n) = m. </li>
     <li> The associated transformation function, where L is a strictly
     increasing sequence of natural numbers (the indices here start with 0),
     is transform_threshold_l(L) in
     ComputerAlgebra/RamseyTheory/Lisp/Sequences.mac. </li>
     <li> Given the sequence tau_arithprog(k,n) (n running), one obtains
     vanderwaerden_{m+1}([2]_m, k) for m=0,1,..., by observing when first the
     value is strictly greater than m --- for this index n we have
     vanderwaerden_{m+1}([2]_m, k) = n. </li>
     <li> In other words, the values of vanderwaerden_{m+1}([2]_m, k) are
     exactly the step-indices, where the value of tau_arithprog(k,n)
     increases by one. </li>
     <li> The transformer (where L is a non-decreasing sequence of natural
     numbers, and the indices here start with 0), is transform_steps_l(L)
     (see ComputerAlgebra/RamseyTheory/Lisp/Sequences.mac). </li>
     <li> Given the sequence vanderwaerden_{m+1}([2]_m, k), one obtains
     A065825 by XXX </li>
     <li> Given a sequence L like A065825, which contains the independence
     numbers in compressed form, only containing the changes (a "steplist"),
     one obtains alpha_arithprog(k,n) by returning the first k where
     L[k] >= n --- this is already achieved by transform_threshold_l(L),
     starting with n=0. </li>
    </ol>
   </li>
  </ul>


  \todo Studying the transversal hypergraph
  <ul>
   <li> If the transversal-number t(n) increases (by one, i.e., it becomes
   t(n+1) = t(n) + 1), then for the set MTr(n) of minimum transversals we
   have |MTr(n+1)| >= |MTr(n)|, since for T in MTr(n) we have T + {n+1}
   in MTr(n+1). </li>
   <li> The other elements T in MTr(n+1) (in this case) are exactly the
   transversals T of G(n) of length t(n)+1 (using only vertices 1,...,n)
   which are also transversals of G(n+1), that is, they are (exactly) the
   minimum transversals of G(n) where those arithmetic progressions P of
   length k-1 are added which yield an arithmetic progressions P + {n+1}
   of length k. </li>
   <li> It seems possible that there are no such other elements T, i.e.,
   |MTr(n+1)| = |MTr(n)| seems possible?
    <ol>
     <li> At least when considering only prime numbers (see
     Investigations/RamseyTheory/GreenTaoProblems/Transversals/plans/Datak3.hpp)
     this happenes for k=3, namely for n=28 we have tau=11 and 19 minimum
     transversals, and for n=29 we have tau=12 and 19 minimum transversals.
     </li>
     <li> And three arithmetic progressions were added in this case; for
     higher k we get more and more cases where no arithmetic progressions
     are added, but still there are cases with additions but all old
     transversals are still valid. </li>
     <li> Of course, the question is whether this can happen when we consider
     the full intervals. </li>
    </ol>
   </li>
   <li> If on the other hand t(n) = t(n+1), then MTr(n+1) is exactly the
   (non-empty) set of T in MTr(n) which happen to be also transversals
   of MTr(n+1). </li>
   <li> So here we have |MTr(n+1)| <= |MTr(n)|. Equality seems possible?
    <ol>
     <li> At least when considering only prime numbers (see
     Investigations/RamseyTheory/GreenTaoProblems/Transversals/plans/Datak3.hpp)
     this happenes:
     \verbatim
5 1 2
6 1 2
11 3 2
12 3 2
     \endverbatim
     </li>
    </ol>
   </li>
   <li> One can ask whether the count 1 of minimum transversals occurs
   infinitely often? Or more generally, whether every number occurs as
   count, and this infinitely often? </li>
   <li> One should have a look at the cases where we have a unique
   transversal. Perhaps these sets, as sequences, are known? </li>
  </ul>


  \todo Exploiting "self-similarity"
  <ul>
   <li> Yet the fastest algorithm is minimum_transversals_mongen, based on
   the simple algorithm transversals_bvs. </li>
   <li> This is a generic algorithm not using any special knowledge. </li>
   <li> We know that the arithmetic progressions of length k in any
   subset S of {1, ..., n} are contained in arithprog_hg(k,n). </li>
   <li> If S is itself an arithmetic progression of length K, then
   the hypergraph of arithmetic progressions of size k within S, computed
   by arithprog_list_hg, is isomorphic to arithprog_hg(k, |S|). </li>
   <li> Thus we can consider any arithmetic progression S in {1,...,n}
   (arbitrary length s = length(S) < n): a transversal T of arithprog_hg(k,n)
   is also a transversal of arithprog_hg(k, s) (according to transversal_p,
   which ignores irrelevant vertices) under the order-preserving bijection
   S -> {1,...,s}, and so we can exploit knowledge we have on
   arithprog_hg(k, s). </li>
   <li> Yet the only knowledge we have on the transversals of arithprog_hg(k,s)
   is the transversal number tau_arithprog_hg(k,s), which means a lower
   bound on their size: length(T cap S) >= tau_arithprog_hg(k,s). </li>
   <li> How can such a lower bound be exploited?
    <ol>
     <li> In the context of the algorithm transversals_bvs perhaps it's best
     to consider the resulting upper bound
     |T - S| <= B - tau_arithprog_hg(k,s), where B is the bound considered,
     which might contradict the requirements for traversing {1,...,n} - S.
     </li>
     <li> If {1,...,n} - S is itself an arithmetic progression, then we can
     use |T - S| >= tau_arithprog_hg(k, n-s). </li>
     <li> This yields
     tau_arithprog_hg(k, n-s) <= |T - S| <= B - tau_arithprog_hg(k,s). </li>
     <li> If T is not yet a transversal, but only an attempted one (during the
     course of the algorithm), then we can use
     |T - S| <= B - tau_arithprog_hg(k,s). </li>
     <li> So for partitions (S_0,S_1) of {1,...,n}, where both parts are
     arithmetic progressions, we must always have
     |T cap S_i| <= B - tau_arithprog_hg(k,|S_{1-i}|). </li>
     <li> So if a vertex v is added to the current T, then for each considered
     pair (S_0,S_1) exactly one of two sets T cap S_0, T cap C_1 is enlarged,
     and whether the upper bound still is obeyed needs to be checked. </li>
     <li> We only needed to check such constraints which are "active", that is,
     where the upper bound had been reached already. </li>
     <li> Of course, it can be that just
     tau_arithprog_hg(k, n-s) <= B - tau_arithprog_hg(k,s) does not hold,
     in which case B is to be rejected from the beginning. </li>
     <li> However, in our given application, B is always exact, so this
     situation won't arise. </li>
    </ol>
   </li>
   <li> What are arithmetic progressions S_0 (arbitrary length) in {1,..,n}
   such that the complement S_1 is again an arithmetic progression (arbitrary
   length)?
    <ol>
     <li> S_0 = {1, ..., j}, S_1 = {j+1, ..., n} </li>
     <li> S_0 = even numbers, S_1 = odd numbers </li>
     <li> That seems to be it? </li>
    </ol>
   </li>
   <li> Of course, one could use look-ahead. What we have are just additional
   active clauses, controlling the "local sizes" of T. </li>
   <li> More generally, applicable for arbitrary hypergraphs, we have given
   an upper bound B on the size of transversal T for hypergraph G, and
   for certain partitions S_0,S_1 of the vertex set we have given lower bounds
   t_0, t_1 on the transversal numbers of G_{S_0}, G_{S_1}, obtained by
   considering only hyperedges completely contained in S_0 resp. S_1. Then we
   must have max(|T cap S_0|,t_0) + max(|T cap S_1|,t_1) <= B for arbitrary
   partial transversals T. </li>
   <li> As constraints, using boolean variables v in V(G), we get
   sum V(G) = B, sum S_1 >= t_0, sum S_1 >= t_1. </li>
   <li> This can be used in SAT solvers supporting cardinality constraints,
   or in constraint solvers. </li>
  </ul>

*/

