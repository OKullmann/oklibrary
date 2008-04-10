// Oliver Kullmann, 10.4.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/DataStructures/Lisp/plans/Lists.hpp
  \brief Plans for Maxima-components regarding operations on lists


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
     <li> Perhaps all the basic operations are offerend in "accumulator-mode",
     which provides in-place modification of the main operand. </li>
    </ol>
   </li>
  </ul>

*/
