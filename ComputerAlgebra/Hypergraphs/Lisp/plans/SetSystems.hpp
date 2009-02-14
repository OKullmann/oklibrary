// Oliver Kullmann, 3.9.2008 (Swansea)
/* Copyright 2008, 2009 Oliver Kullmann
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
   <li> DONE (we supply "lunion" and "lintersection", which, like "lmin", can
   be applied to lists and sets)
   Naming conventions:
    <ol>
     <li> Yet, with tree_reduce, one doesn't need to make a distinction between
     union_ls and union_ss. </li>
     <li> However for the future one might choose different implementations. </li>
     <li> One could of course just supply a generic function, which checks whether
     as set of sets or a list of sets is given. </li>
     <li> Or one could always apply "listify"; this seems easiest. </li>
    </ol>
   </li>
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
   <li> DONE (implemented uapply in
   ComputerAlgebra/DataStructures/Lisp/Lists.mac)
   About the argument-length restriction to "apply":
    <ol>
     <li> We could assume that "apply" takes arbitrarily many
     arguments. </li>
     <li> This rules out to use ECL as the underlying Lisp. </li>
     <li> The only alternative would be to write dedicated implementations
     of union_listset etc.; perhaps at some point in the future we'll do
     so. </li>
     <li> Then actually we should encourage the use of "union_listset" etc.
     </li>
     <li> The current implementation of union_listset=list_sets_union
     queries maximal_argument_length_union: The underlying algorithm
     can be generalised to obtain an unrestricted version of apply.
    </ol>
   </li>
   <li> DONE (in the case of mostly disjoint sets tree_reduce is still
   considerably faster)
   A simple alternative algorithm for union_listset:
   \verbatim
union_ls_alt(L) := setify(uaapply(append, map(listify,L)))$
   \endverbatim
   This is much faster if the sets of mostly disjoint (and apparently
   not much slower if not). </li>
   <li> DONE
   However, here actually we encounter the upper-bound of 4095 on
   the argument-length! So "apply(union,x)" is simply handled differently
   than "apply(append,x)" ! </li>
   <li> DONE (the choice is between apply and tree_reduce)
   "xreduce" should not be used, as
   \verbatim
(%i632) uuapply(union,create_list({},i,1,100000));
Evaluation took 7.4890 seconds (7.5370 elapsed)
(%o632) {}
(%i633) xreduce(union,create_list({},i,1,100000));
Evaluation took 686.2800 seconds (692.9140 elapsed)
(%o633) {}
   \endverbatim
   shows (with Ecl, on csltok); with CLisp we have
   \verbatim
(%i2) uuapply(union,create_list({},i,1,100000));
Evaluation took 10.1505 seconds (10.1909 elapsed) using 41.200 MB.
(%o2) {}
(%i3) xreduce(union,create_list({},i,1,100000));
Evaluation took 10.2634 seconds (10.3040 elapsed) using 45.777 MB.
   \endverbatim
   </li>
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


  \todo DONE (it seems that this bug won't be corrected in Maxima, so we are
  using our own functions)
  Bug in Maxima: corr_cartesian_product
  <ul>
   <li> DONE
   Report the bug corrected by corr_cartesian_product to the Maxima
   mailing list and to the bug-reporting system. </li>
   <li> Once corrected, remove all applications of corr_cartesian_product.
   </li>
   <li> See "Document important programming techniques" in
   ComputerAlgebra/plans/Maxima.hpp. </li>
  </ul>

*/

