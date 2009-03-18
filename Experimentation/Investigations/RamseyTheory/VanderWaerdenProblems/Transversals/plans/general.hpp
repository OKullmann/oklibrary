// Oliver Kullmann, 16.2.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
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
   <li> In other words, vanderwaerden_{k+1}([2]_k, m) is the smallest n such
   that tau_arithprog_hg(m,n) > k. </li>
   <li> It seems that for computing vanderwaerden_{k+1}([2]_k, m), we best
   search for the smallest n with tau_arithprog_hg(m,n) >= k+1 --- in this way
   we have only to decide whether or not a transversal of size at most k
   exists. This amounts to the same as computing the numbers
   tau_arithprog_hg(m,n) in succession, just using that these numbers are
   non-decreasing and each step is at most 1. </li>
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
     have tau_arithprog_hg(m,n) = k. </li>
     <li> The associated transformation function, where L is a strictly
     increasing sequence of natural numbers (the indices here start with 0):
     \verbatim
transform_threshold_l(L) := 
if emptyp(L) then [] else block(
 [n : 0, k : 0, R : []],
  for x in endcons(last(L)+1,L) do (
    if x > n then (
      R : append(R,create_list(k,i,1,x-n)), 
      n : x
    ),
    k : k+1
  ),
  return(R))$
     \endverbatim
     </li>
     <li> Given the sequence tau_arithprog_hg(m,n), one obtains
     vanderwaerden_{k+1}([2]_k, m) by running through n=0,1,..., and observing
     when first the value is strictly greater than k --- for this n we have
     vanderwaerden_{k+1}([2]_k, m) = n. </li>
     <li> In other words, the values of tau_arithprog_hg(m,n) are exactly the
     step-indices, where the value of tau_arithprog_hg(m,n) increases by one. </li>
     <li> The transformer (where L is a non-decreasing sequence of natural
     numbers, and the indices here start with 0):
     \verbatim
transform_steps_l(L) := if length(L) <= 1 then [] else
 block([a : first(L), i : 1, R : []],
  for b in rest(L) do (
    if b > a then (R : endcons(i,R), a : b),
    i : i + 1
  ),
  return(R))$
     \endverbatim
     </li>
     <li> Given the sequence vanderwaerden_{k+1}([2]_k, m), one obtains
     A065825 XXX </li>
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
   length 3-1=2 are added which yield an arithmetic progressions P + {n+1}
   of length 3. </li>
   <li> It seems possible that there are no such other elements T, i.e.,
   |MTr(n+1)| = |MTr(n)| seems possible? </li>
   <li> If on the other hand t(n) = t(n+1), then MTr(n+1) is exactly the
   (non-empty) set of T in MTr(n) which happen to be also transversals
   of MTr(n+1). </li>
   <li> So here we have |MTr(n+1)| <= |MTr(n)|. Equality seems possible?
   </li>
   <li> One can ask whether the count 1 of minimum transversals occurs
   infinitely often? Or more generally, whether every number occurs as
   count, and this infinitely often? </li>
   <li> One should have a look at the cases where we have a unique
   transversal. Perhaps these sets, as sequences, are known? </li>
  </ul>


  \todo Efficient computation of the transversal number
  <ul>
   <li> Yet the fastest algorithm is minimum_transversals_mongen, based on
   the simple algorithm transversals_bes.
    <ol>
     <li> It could be improved by incorporating the upper bounds coming
     from tau_arithprog_hg(k,x+y) <=  sum_i i in {x,y} tau_arithprog_hg(k,i).
     </li>
     <li> This should be done by first pre-computing for i in
     {1,...,n-1} the transversal number for the hypergraphs restricted
     to {1,...,i} and {i+1,...,n}, with i and n-i vertices, that is
     tau_arithprog_hg(k,i) and tau_arithprog_hg(k,n-i); lets call these
     numbers a_i and b_i. </li>
     <li> For the current partial transversal T these numbers are updated
     as new-a_i = max(old-a_i, T intersect {1,...,i}) and
     new-b_i = max(old-b_i, T intersect {i+1,...,n}). </li>
     <li> We must always have a_i + b_i <= B (where B is the current upper
     bound, which in our case is actually the (precise) transversal number).
     </li>
     <li> So each time a new element is added to the T, the numbers a_i, b_i
     are updated, and the branch is aborted when a_i + b_i > B. </li>
     <li> Of course, one could use look-ahead. What we have is just an
     additional active clause, controlling the "local sizes" of T (including
     the total size). </li>
     <li> More generally, applicable for arbitrary hypergraphs, we have given
     an upper bound B on the size of transversal T for hypergraph G, and
     for certain partitions A, B of the vertex set we have given lower bounds
     a, b on the transversal number of G_A, G_B, obtained by considering only
     hyperedges completely contained in A resp. B. Then we must have
     max(|T intersect A|,a) + max(|T intersect B|,b) <= B. </li>
     <li> As constraints, using boolean variables v in V(G), we get
     sum V(G) = B, sum A >= a, sum B >= b. </li>
     <li> This can be used in SAT solvers supporting cardinality constraints,
     or in constraint solvers. It seems, though, that only the above
     implications can be drawn (w.r.t. partial assignments!). </li>
     <li> No, actually also the lower bounds themselves can be used, either
     when too many vertices are crossed out, so that the lower bound can not
     be reached, or, just before that, when all remaining vertices have
     to be used to reach the lower bound. </li>
     <li> Regarding the lower bounds, we can consider arbitrary intervals
     {i, ..., j} <= {1,...,n}, where we must have
     |T intersect {i,...,j}| >= tau_transversal_hg(k,j-i+1). </li>
     <li> As above, using B we obtain also an upper bound on
     |T intersect {i,...,j}|, where this time in general the complement is
     the (disjoint) union of two intervals {1,...,i-1} and {j+1,...,n}. </li>
    </ol>
   </li>
   <li> A C++ implementation should be able to go quite beyond the known
   numbers:
    <ol>
     <li> See Hypergraphs/Transversals/plans/SizeParameter.hpp. </li>
    </ol>
   </li>
  </ul>

*/

