// Oliver Kullmann, 3.9.2008 (Swansea)
/* Copyright 2008, 2009, 2010, 2011 Oliver Kullmann
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
     <li> With Ecl: For weak_php(n,n) no difference, while with full_fcs(n)[2] "apply"
     is by 50% faster than "tree". On the other hand, for van-der-Waerden problems
     "tree" is with a growing factor faster (2 s instead of 15.5 s for
     vanderwaerden2_fcs(5,300)[2]). </li>
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
   <li> Currently just choose_element_ = first. A random choice should be
   better. </li>
   <li> Perhaps for small lists the simpler algorithms are faster. </li>
  </ul>


  \todo Function max_elements
  <ul>
   <li> Can we transfer the idea of min_elements_unique_fast_l_ ? </li>
  </ul>

*/

