// Oliver Kullmann, 22.7.2008 (Swansea)
/* Copyright 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Algebra/Lisp/Groupoids/Actions/plans/general.hpp
  \brief Plans in general regarding actions of sets


  \todo Connections
  <ul>
   <li> See ComputerAlgebra/Algebra/Lisp/Groupoids/Operations/plans/general.hpp
   for operations of groupoids on structures. </li>
  </ul>
  
  
  \todo Notions and notations
  <ul>
   <li> "act", "ract" as the principal abbreviations for "acts" and "right
   acts". </li>
   <li> "mp" for map, "sm" for set-map, "hm" for hash-map (compare with
   ComputerAlgebra/DataStructures/Lisp/plans/HashMaps.hpp) </li>
   <li> "trs_sm" for transformations as set-maps, and "trs_l" for
   transformations as lists (compare with
   ComputerAlgebra/Algebra/Lisp/Groupoids/Semigroups/plans/general.hpp). </li>
   <li> Yet we only have the actions isolated, as pure maps; but we also
   need actions as structures, that is, as triples [act,A,X]. </li>
   <li> While an A-act would be a pair [act,X]. </li>
   <li> Or should we follow the normal convention, putting the operation
   to the tail of the list: [A,X,act], [X,act] ? </li>
   <li> A reasonable abbreviation would be "act_set" for the action on a set
   (of course, of a set). Then perhaps the predicates like "act_set_p" should
   have one argument, namely the triple [A,X,act] (not, like yet, three
   arguments). </li>
  </ul>


  \todo Homomorphisms

*/

