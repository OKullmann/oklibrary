// Oliver Kullmann, 3.9.2008 (Swansea)
/* Copyright 2008, 2009, 2010, 2011, 2013 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Hypergraphs/Lisp/plans/SetSystems.hpp
  \brief Plans for Maxima-components for systems of sets
  
  
  \todo New module ComputerAlgebra/Sets
  <ul>
   <li> See "Module ComputerAlgebra/Sets" in ComputerAlgebra/plans/general.hpp.
   </li>
   <li> Functions like "kpermutations" connect to
   ComputerAlgebra/Combinatorics/Lisp/Enumeration/plans/general.hpp. </li>
  </ul>


  \todo Naming conventions
  <ul>
   <li> We need abbreviations for "set systems" (or "sets of sets"),
   "lists of sets" etc. </li>
   <li> Perhaps simply "setset" and "listset". </li>
   <li> But this is all too long --- just "ls" and "ss". </li>
   <li> DONE
   Then "set_sets_union" should become "union_setset" etc. </li>
  </ul>


  \todo Unions and intersections
  <ul>
   <li> Choosing between the different methods to apply union iteratively:
    <ol>
     <li> We have two basic extreme test cases: Disjoint union (by
     create_list({i},i,1,N)), and all sets identical (by
     create_list(S,i,1,N) for S : setn(n)). </li>
     <li> Basically, apply is faster by a factor of 2 for the second case,
     while tree_reduce is faster (much faster, growing with N) in the first
     case. </li>
     <li> First case with "apply":
     \verbatim
test1_apply(N) := length(uaapply(union,create_list({i},i,1,N)));
     \endverbatim
     </li>
     <li> With Ecl: N = 1000 around 0.8 s, with N = 10000 around 73 s. </li>
     <li> With CLisp: N = 1000 around 1.3 s, with N = 10000 around 46 s. </li>
     <li> First case with "tree_reduce":
     \verbatim
test1_tree(N) := length(tree_reduce(union,create_list({i},i,1,N)));
     \endverbatim
     </li>
     <li> With Ecl: N = 1000 around 0.1 s, with N = 10000 around 1.2 s. </li>
     <li> With CLisp: N = 1000 around 0.2 s, with N = 10000 around 2.2 s. </li>
     <li> Second case with "apply":
     \verbatim
test2_apply(N,n) := block([S : setn(n)], length(uaapply(union,create_list(S,i,1,N))));
     \endverbatim
     </li>
     <li> With Ecl: N=1000,n=10 around 0.03 s, with N=10000,n=10 around 0.3 s,
     with N=1000,n=100 around 0.35 s, with N=10000,n=100 around 3.5 s. </li>
     <li> With CLisp: N=1000,n=10 around 0.05 s, with N=10000,n=10 around 0.56 s,
     with N=1000,n=100 around 0.79 s, with N=10000,n=100 around 7.96 s. </li>
     <li> Second case with "tree_reduce":
     \verbatim
test2_tree(N,n) := block([S : setn(n)], length(tree_reduce(union,create_list(S,i,1,N))));
     \endverbatim
     </li>
     <li> With Ecl: N=1000,n=10 around 0.05 s, with N=10000,n=10 around 0.56 s,
     with N=1000,n=100 around 0.64 s, with N=10000,n=100 around 6.2 s. </li>
     <li> With CLisp: N=1000,n=10 around 0.1 s, with N=10000,n=10 around 1.05 s,
     with N=1000,n=100 around 1.52 s, with N=10000,n=100 around 15.7 s. </li>
     <li> Some "mixed cases":
     \verbatim
var_cs_apply(F) := var_sl(uaapply(union,listify(F)))$
var_cs_tree(F) := var_sl(tree_reduce(union,F))$
     \endverbatim
     </li>
     <li> With Ecl: For weak_php_fcs(n,n) no difference, while
     with full_fcs(n)[2] "apply" is by 50% faster than "tree". On the other
     hand, for van-der-Waerden problems "tree" is with a growing factor faster
     (2 s instead of 15.5 s for vanderwaerden2_fcs(5,300)[2]). </li>
     <li> So it seems that with tree_reduce one is on the safe side. </li>
    </ol>
   </li>
  </ul>


  \todo Computing all ordered tuples
  <ul>
   <li> The implementations of all_ord_tuples and all_ord_tuples_l are
   rather inefficient (especially for larger k). </li>
   <li> One possibility would be to use use meta-programming, that is, writing
   a macro which creates the k-times nested loop. </li>
  </ul>


  \todo Set creation
  <ul>
   <li> It would be nice to have "create_set", as we have "create_list". </li>
   <li> However
   \verbatim
create_set([A]) := setify(apply(create_list,A))$
   \endverbatim
   does not work, since "apply" evaluates the argument A. </li>
   <li> Or, perhaps it works? I (OK) can't find an example where it does
   not work? </li>
   <li> It should be possible to make this work, but perhaps this is not 
   worth the effort? </li>
   <li> Ask on the Maxima mailing list. </li>
   <li> This should belong to ComputerAlgebra/Sets. </li>
  </ul>


  \todo Function min_elements
  <ul>
   <li> Variation:
    <ol>
     <li> A similar idea is in [Lintao Zhang, On Subsumption Removal and
     On-the-Fly {CNF} Simplification, SAT 2005], under "forward subsumption
     checking". </li>
     <li> The task is to check for a given C whether there is D <= C. </li>
     <li> There however one runs through all the literals in C and all the
     clauses in their watched-lists. Thus the algorithms there is rather
     different. </li>
     <li> It is done there this way to avoid the subsumption check. This is
     achieved by moving the clauses which yet do not subsume C to the next
     watched-list. This works similar to setting all literals in C to false,
     and checking whether we obtain the empty clause. </li>
     <li> We need to implement this, and compare it with our implementation.
     It seems to me (OK), that our implementation should be faster in most
     cases. </li>
    </ol>
   </li>
   <li> Currently just choose_element_ = first. A random choice should be
   better. </li>
   <li> Perhaps for small lists the simpler algorithms are faster. </li>
   <li> Upper bound on the time-complexity:
    <ol>
     <li> Let m be the number of sets, and let k be the maximal size of
     a set. </li>
     <li> The trivial algorithm runs in time O(m^2*k). </li>
     <li> Can we prove a better bound for the improved algorithm? </li>
    </ol>
   </li>
  </ul>


  \todo Function max_elements
  <ul>
   <li> Can we transfer the idea of min_elements_unique_fast_l_ ? </li>
  </ul>


  \todo Minimal elements for convex set-systems
  <ul>
   <li> Consider a set-system S with the property, that for A, B in S with
   A <= B also for all A <= C <= B we have C in S. </li>
   <li> Then subsumption-elimination can be done by sorting and binary
   search as follows:
    <ol>
     <li> Let m := |S| and let n := union(S) (the size of the base-set). </li>
     <li> Copy S into an array A and sort A according to any linear ordering
     on sets. </li>
     <li> Now s in S is minimal iff for all x in s the set s-{x} is not
     in A, where (non-)elementship can be determined in time log_2(m)*n by
     binary search. </li>
     <li> Thus computation of the minimal elements of S can be done in time
     O(m*log_2(m)*n^2) (assuming comparison of two sets can be done in time
     O(n), and thus sorting of A can be done in time O(m*log_2(m)*n)). </li>
     <li> Similarly the maximal elements of S can be computed. </li>
    </ol>
   </li>
  </ul>

*/

