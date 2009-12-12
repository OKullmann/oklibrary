// Oliver Kullmann, 24.7.2005 (Swansea)
/* Copyright 2005 - 2007, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKlib/Satisfiability/Assignments/concepts/plans/general.hpp
  \brief Plans for concepts for (total) assignments.


  \todo The basic concept of a "total assignment"
  <ul>
   <li> The basic intuition for a "total assignment" is that of a "model",
   that is, a "structure" (which might satisfy some given condition, so
   that then it qualifies as a "model"). </li>
   <li> This is in contrast to "partial assignments" (see
   Assignments/PartialAssignments/concepts/plans/general.hpp), which are
   basically specifying sets of total assignments. </li>
   <li> A total assignment is furthermore a map, with range the base set
   of the structure. </li>
   <li> The arguments could be arbitrary "indices", but it seems that for the
   domain of (generalised) satisfiability variables are the appropriate
   arguments. </li>
   <li> For a total assignment f and a variable v, the basic operation is
   "f[v]", yielding a value; this syntax allows arrays and vectors as
   total assignments. </li>
   <li> Furthermore we have "value_type" for the type of values, generically as
   \code
OKlib::Satisfiability::Assignments::traits::value_type<T>::type
   \endcode
   or, if available, as T::value_type. </li>
   <li> The domain, the set of variables for which f is defined, is in general
   only implicitly defined. </li>
   <li> If we just speak of an "assignment", then f[v] is potentially
   undefined for v in the (implicit) domain, indicated by some special
   value in the domain. </li>
   <li> Although this is similar to partial assignments, the underlying ideas
   are different: assignments don't stand for sets of total assignments, but
   for total assignments on some sub-sets of variables. </li>
  </ul>

*/

