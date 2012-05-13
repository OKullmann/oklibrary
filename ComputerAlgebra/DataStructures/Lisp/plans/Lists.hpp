// Oliver Kullmann, 10.4.2008 (Swansea)
/* Copyright 2008, 2011, 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/DataStructures/Lisp/plans/Lists.hpp
  \brief Plans for Maxima-components regarding operations on lists


  \todo Generating list of elements 1 to n
  <ul>
   <li> One must often generate the list of elements 1 to n:
   \verbatim
create_list(i,i,1,n);
   \endverbatim
   </li>
   <li> "create_list" is too long for such a simple task. </li> 
   <li> Also, it makes the simple statement of "1..n" hard to read. The reader
   must apply more thought to realise what is meant. </li>
   <li> For sets, we have setn and setmn. These functions provide the 
   functionality for sets. </li>
   <li> We should provide some function like setn for lists. </li>
   <li> There is a Maxima-function for this task:
    <ol>
     <li> This function is not documented and not included by default.
     It needs to be identified. Ask Maxima mailing-list. </li>
     <li> Then we need to check whether it is not slower than create_list.
     </li>
     <li> If it is slower, then we do not use it, but possibly write our own
     wrapper. </li>
     <li> Finally, once a replacement is in place, all occurrences of
     create_list have to be sifted and possibly replaced (but only if a test
     exists --- otherwise a test needs to be provided). </li>
    </ol>
   </li>
  </ul>


  \todo Apply-functionality
  <ul>
   <li> Better if Maxima would correct "apply" so that we don't need
   to worry about it --- ask on the mailing list! </li>
   <li> We need docus on the use of apply, uaapply:
    <ol>
     <li> "apply" iff the operator is like "+", or we have at most
     63 arguments (guaranteed). </li>
     <li> DONE (for this better use the dedicated functions in SetSystems.mac)
     "uuapply" iff the operator is like "union, intersection,
     cartesian_product". </li>
     <li> "uaapply" otherwise. </li>
    </ol>
   </li>
  </ul>


  \todo Correct implementation of some_s and every_s
  <ul>
   <li> We can get erroneous results if for example "_x" is used in
   the application context. </li>
   <li> Can this be avoided? </li>
  </ul>


  \todo In-place modification
  <ul>
   <li> It seems that for example handling of loops would often be
   simplified if in-place modifications of lists would be available. </li>
   <li> This needs to be provided at the Lisp-level.
    <ol>
     <li> Stavros Macrakis on the Maxima mailing list showed
     \verbatim
(defun $set_rest (l new)
  (cond ((or (atom l)
             (not (eq (caar l) 'mlist))
             (not (cdr l)))
         (merror "Can only replace rest of non-null lists: ~:M" l))
        ((or (atom new) (not (eq (caar new) 'mlist)))
         (merror "Can only replace rest with a list: ~:M" new))
        (t (rplacd (cdr l) (cdr new))
           l)))
     \endverbatim
     for replacing the rest of a list by another list. </li>
    </ol>
   </li>
   <li> On the other hand, we would introduce some performance-oriented
   aspects, which should be avoided. </li>
   <li> Nevertheless, a sort of an "abstract data type" for lists should
   be considered.
    <ol>
     <li> One needs to get an overview on the list-functionality. </li>
     <li> Providing clear guidelines for all the basic usage scenarios. </li>
     <li> Better support for index-oriented operations (instead of
     value-oriented) seems to be needed. </li>
    </ol>
   </li>
   <li> More problematic: What to do with sets?
    <ol>
     <li> Here it would require a rather large effort to enable
     more powerful and efficient operations, since the set-structure
     needs to be maintained. </li>
     <li> Making list-operations more efficient than set-operations would
     introduce an incentive to use the former even if the latter are
     more appropriate. </li>
     <li> So careful planning is needed, always with emphasise on providing
     meaningful operations, which actually also improve readability of the
     programs. </li>
     <li> The removal of an element from a set should be doable, and
     should also increase readability. </li>
     <li> Perhaps all the basic operations are offered in "accumulator-mode",
     which provides in-place modification of the main operand. </li>
    </ol>
   </li>
  </ul>


  \todo Improving sort_length
  <ul>
   <li> Functionality should be merged with partition_list_eq and
   partition_list (where the notion of "partitioning" is not appropriate).
   </li>
   <li> Is using the array A improving efficiency? </li>
   <li> In case we have many different lengths, then likely just applying a
   a *stable* sorting w.r.t. length should be more efficient. </li>
  </ul>


  \todo Improve partition_list_eq
  <ul>
   <li> This function is very slow. </li>
   <li> Since only internal hash-maps are needed, the use of hash-arrays needs
   to be investigated. </li>
   <li> If they are faster here, then they should be used. </li>
   <li> And the use of such hash-arrays must be documents at
   ComputerAlgebra/plans level. </li>
  </ul>

*/
