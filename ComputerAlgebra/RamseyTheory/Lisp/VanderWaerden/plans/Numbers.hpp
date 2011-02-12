// Oliver Kullmann, 12.4.2009 (Swansea)
/* Copyright 2009, 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/RamseyTheory/Lisp/VanderWaerden/plans/Numbers.hpp
  \brief Plans regarding van der Waerden numbers


  \todo Connections to other modules
  <ul>
   <li> In RamseyTheory/Lisp/VanderWaerden/Numbers.mac we need to provide
   everything known about van-der-Waerden numbers. </li>
   <li> See
   "Architectures of the systems for showing/computing Ramsey-type numbers" in
   ComputerAlgebra/RamseyTheory/Lisp/plans/general.hpp. </li>
   <li> And see
   Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/general.hpp.
   </li>
  </ul>


  \todo General organisation
  <ul>
   <li> We have several, redundant sources for vdW-numbers: it seems best to
   actually use them, since they provide additional certificates. </li>
  </ul>


  \todo Improving exactf_tau_arithprog
  <ul>
   <li> DONE (package boolsimp does this job)
   How do we get Maxima to evaluate simple terms?
   \verbatim
exactf_tau_arithprog(1,n);
 if n < 1 then 0 elseif 1 = 1 then n elseif 1 = 2 then n-1 elseif n <= 1 and evenp(1)
           then (if n = 1 then 2 else 1) elseif n <= 2 and oddp(1) then (if n = 2 then 2 else 1)
           else unknown
   \endverbatim
   --- this should yield "if n < 1 then 0 else n" ?! </li>
  </ul>


  \todo Improving initial_sequence_vdWt
  <ul>
   <li> initial_sequence_vdWt(k) should proceed until the first result returned
   by greentaot(m,k) appears which is not an integer. </li>
   <li> The computation is very slow: greentaot(m,k) needs a more efficient
   algorithm. </li>
  </ul>


  \todo The LRC-formula
  <ul>
   <li> DONE (the "m" from the paper was not correctly used)
   Using
   \verbatim
testf(M,K) := 
 for m : 1 thru M do
  for k : m+2 thru K do
   block([res1 : vanderwaerdent_lrc(m,k), res2 : vanderwaerdent(m,k)],
     if not listp(res1) and res2 # unknown and res1 # res2 then
       print(m,k,res1,res2)
   )$
   \endverbatim
   we get
   \verbatim
testf(10,30);
2 4 10 11
4 6 18 27
6 8 26 51
   \endverbatim
   </li>
   <li> DONE (see above)
   So for r in {3,5,7} and for the minimal allowed k the formula
   is not correct. Contact the authors. </li>
  </ul>


  \todo Transversal and independence numbers
  <ul>
   <li> tau_arithprog_hg(k,n) is the transversal number of arithprog_hg(k,n),
   while alpha_arithprog_hg(k,n) is the independence number of
   arithprog_hg(k,n). </li>
   <li> For fixed k, we want to easily extract the sequences
   tau_arithprog_hg(k,-) and alpha_arithprog_hg(k,-), and likely it's also
   best to specify the values by providing the known initial sequences
   (and potentially sporadic values). </li>
   <li> Perhaps we provide the initial sequences as values of
   tau_arithprog_seq[k] and alpha_arithprog_seq[k] (perhaps best
   starting with index 1):
   \verbatim
tau_arithprog_seq[3] : [
0,0,1,1,1,2,3,4,4,5,
5,6,6,6,7,8,9,10,11,11,
12,13,14,14,15,15,16,17,18,18,
19,19,20,21,22,22,23,24,25,25,
25,26,27,28,29,30,31,32,33,34,
34,35,36,36,37,38,39,39,40,41
]$
   \endverbatim
   </li>
   <li> Perhaps these lists are then transferred into a hash-map, which
   additionally contains the other "sporadic" values. </li>
   <li> Methods for lower bounds on tau_arithprog_hg(k,n):
    <ul>
     <li> For natural numbers 0 <= p, q with p+q=n we have
     tau_arithprog_hg(k,n) >= tau_arithprog_hg(k,p) + tau_arithprog_hg(k,q).
     </li>
     <li> This method yields for the above sequence tau_arithprog_seq[3]:
     \verbatim
lb(n) := block([L : tau_arithprog_seq[3], l, m:0],
 l : length(L),
 for k : max(1,n-l) thru min(n-1,l) do block(
  [v : L[k] + L[n-k]],
   if v > m then m:v),
 m)$
map(lb,create_list(i,i,1,60)) - tau_arithprog_seq[3];
[
0,0,-1,0,0,0,-1,-1,0,-1,
0,-1,0,0,0,0,-1,-1,-1,0,
-1,-1,-1,0,-1,0,-1,-1,-1,0,
-1,0,-1,-1,-1,0,-1,-1,-1,0,
0,0,-1,-1,-1,-1,-1,-1,-1,-1,
0,-1,-1,0,-1,-1,-1,0,-1,-1
]
     \endverbatim
     </li>
     <li> At exactly the indices 15,42 do we get a non-trivial lower bound
     (at these places we actually have exactly one minimum-transversal 
     before, thus the transversal number should(?) increase by one). </li>
     <li> Namely, L[15] = 7 = L[12] + L[3] = L[8] + L[7], and
     L[42] = 26 = L[39] + L[3]. </li>
     <li> Otherwise the bound is trivial. </li>
    </ul>
   </li>
   <li> Methods for upper bounds on tau_arithprog_hg(k,n):
    <ul>
     <li> tau_arithprog_hg(k,n) <= n. </li>
     <li> More generally, for 0 <= p <= n we have tau_arithprog_hg(k,n) <=
     tau_arithprog_hg(k,p) + (n - p). </li>
     <li> Alternatively, for p >= 0 we have tau_arithprog_hg(k,n) <=
     tau_arithprog_hg(k,n+p) - tau_arithprog_hg(k,p)
     (from this by tau_arithprog_hg(k,n+p) <= tau_arithprog_hg(k,p)+n
     we obtain the upper bound n). </li>
     <li> It seems that the last bound is hardly useful; so all what we get
     is the trivial bound that advancing one steps costs at most one. </li>
     <li> For natural numbers n >= 0 and k >= 2 the following functions
     checks whether the base-k representation of n contains the digit k-1:
     \verbatim
maxdigitp(k,n) := 
 some_s(lambda([d],is(d=k-1)),int2polyadic(n,k))$
     \endverbatim
     while
     \verbatim
maxdigits(k,n) := subset(setmn(0,n-1),lambda([x],maxdigitp(k,x)))$
     \endverbatim
     is the set of all such numbers from 0 to n-1. The basic fact here is,
     that for prime numbers k, maxdigits(k,n) is a transversal of
     arithprog_hg(k,n), but where the vertex set is {0,...,n-1}
     (instead of {1,...,n}, as it is). </li>
     <li> Thus
     \verbatim
transdig_ap(k,n) := map(lambda([x],x+1), maxdigits(k,n))$
     \endverbatim
     yields a transversal of arithprog_hg(k,n) for n >= 0. </li>
     <li> A counter-example for non-prime k is given by
     transdig_ap(10,19) = {10}, which is not a transversal of
     arithprog_hg(10,19), since the hyperedge {1,3,5,7,9,11,13,15,17,19}
     is missed:
     \verbatim
transversal_p(transdig_ap(10,19),arithprog_hg(10,19));
 false
     \endverbatim
     </li>
     <li> In other words, tau_arithprog_hg(k,n) <= ubmd(k,n))
     for prime numbers k, where
     \verbatim
ubmd(k,n) := length(transdig_ap(k,n))$
     \endverbatim
     </li>
     <li> For k=3 and n <= 60 this upper bound coincides with the above
     lower bound lb exactly for [1,2,4,5,6,11,13,14,15,16,40,41,42]. </li>
     <li> For natural numbers k and a we have
     ubmd(k,((k-2)*k^a+1)/(k-1)) = ((k-2)*k^a+1)/(k-1) - (k-1)^a. </li>
     <li> Using
     \verbatim
ubmda(k,a) := block([n : ((k-2)*k^a+1)/(k-1)], [n, n-(k-1)^a])$
     \endverbatim
     for [n,b] = ubmda(k,a) we have tau_arithprog_hg(k,n) <= b. </li>
     <li> For k=3 this yields tau_arithprog_hg(3,(3^a+1)/2) <= 
     (3^a+1)/2 - 2^a.
     \verbatim
map(lambda([a],ubmda(3,a)),create_list(i,i,1,6));
 [[2,0],[5,1],[14,6],[41,25],[122,90],[365,301]]
     \endverbatim
     (one sees that for a <= 4 the upper bound is sharp, and coincides with
     the lower bound). </li>
     <li> It should be possible to at least compute tau_arithprog_hg(3,122),
     so that we can determine whether this bound is still sharp (in general
     it is not). </li>
     <li> But perhaps we should use the general function ubmd(k,n), and not
     just the computation of special values via ubmda(k,a). </li>
    </ul>
   </li>
   <li> For our stored values, we should always have that they are best
   relatively to the current knowledge (so that no further calculations
   are needed for them). </li>
  </ul>

*/

