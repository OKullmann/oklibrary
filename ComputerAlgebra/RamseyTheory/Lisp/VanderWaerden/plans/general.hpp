// Oliver Kullmann, 20.9.2008 (Swansea)
/* Copyright 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/VanderWaerdenTheory/Lisp/VanderWaerden/plans/general.hpp
  \brief Plans regarding Van der Waerden problems


  \todo Relations to other modules
  <ul>
   <li> See ComputerAlgebra/Hypergraphs/Lisp/plans/Generators.hpp for
   the function ramsey_hg(q,r,n) and its variations. </li>
   <li> See ComputerAlgebra/Satisfiability/Lisp/Generators/plans/VanderWaerdenProblems.hpp
   for the SAT translation. </li>
   <li> See Experimentation/Investigations/plans/VanderWaerdenProblems.hpp for
   investigations. </li>
  </ul>


  \todo Automorphisms of Van-der-Waerden hypergraphs
  <ul>
   <li> We need to investigate experimentally the symmetries of these
   hypergraphs. </li>
  </ul>


  \todo Automorphisms of Van-der-Waerden clause-sets
  <ul>
   <li> The obvious automorphisms of diagonal Van-der-Waerden clause-sets are
   given by the inner product of automorphisms for the underlying
   Van-der-Waerden hypergraph and the S_m when using m parts. </li>
   <li> Are there more? </li>
  </ul>


  \todo Van-der-Waerden numbers
  <ul>
   <li> In RamseyTheory/Lisp/VanderWaerden/Numbers.mac we need to provide
   everything known about Van-der-Waerden numbers. </li>
   <li> See "Systematic notations for the numbers in %Ramsey theory" in
   ComputerAlgebra/RamseyTheory/plans/general.hpp. </li>
   <li> And see
   Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/Transversals.hpp.
   </li>
   <li> Transversal and independence numbers
    <ol>
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
34,35,36,36,37,38,39,39,40
]
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
map(lb,create_list(i,i,1,59)) - tau_arithprog_seq[3];
[
0,0,-1,0,0,0,-1,-1,0,-1,
0,-1,0,0,0,0,-1,-1,-1,0,
-1,-1,-1,0,-1,0,-1,-1,-1,0,
-1,0,-1,-1,-1,0,-1,-1,-1,0,
0,0,-1,-1,-1,-1,-1,-1,-1,-1,
0,-1,-1,0,-1,-1,-1,0,-1
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
     <li> Methods fur upper bounds on tau_arithprog_hg(k,n):
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
       <li> For prime numbers k and natural numbers a we have
       tau_arithprog_hg(k,((k-2)*k^a+1)/(k-1)) <=
       ((k-2)*k^a+1)/(k-1) - (k-1)^a. </li>
       <li> For k=3 this yields  tau_arithprog_hg(3,(3^a+1)/2) <= 
       (3^a+1)/2 - 2^a.
       \verbatim
ub(a) := block([b : (3^a + 1)/2], [b, b-2^a])$
map(ub,create_list(i,i,1,6));
 [[2,0],[5,1],[14,6],[41,25],[122,90],[365,301]]
       \endverbatim
       (one sees that for a <= 4 the upper bound is sharp, and coincides with
       the lower bound). </li>
       <li> It should be possible to at least compute tau_arithprog_hg(3,122),
       so that we can determine whether this bound is still sharp (in general
       it is not). </li>
      </ul>
     </li>
     <li> For our stored values, we should always have that they are best
     relatively to the current knowledge (so that no further calculations
     are needed for them). </li>
    </ol>
   </li>
  </ul>

*/

