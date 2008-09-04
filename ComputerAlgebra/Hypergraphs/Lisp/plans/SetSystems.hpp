// Oliver Kullmann, 3.9.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
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
  </ul>


  \todo Naming conventions
  <ul>
   <li> We need abbreviations for "set systems" (or "sets of sets"),
   "lists of sets" etc. </li>
   <li> Perhaps simply "setset" and "listset". </li>
   <li> Then "set_sets_union" should become "union_setset" etc. </li>
  </ul>


  \todo Unions
  <ul>
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
   <li> A simple alternative algorithm for union_listset:
   \verbatim
union_listset_alt(L) := setify(uaapply(append, map(listify,L)))$
   \endverbatim
   This is much faster if the sets of mostly disjoint (and apparently
   not much slower if not). </li>
   <li> DONE
   However, here actually we encounter the upper-bound of 4095 on
   the argument-length! So "apply(union,x)" is simply handled differently
   than "apply(append,x)" ! </li>
   <li> "xreduce" should not be used, as
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

