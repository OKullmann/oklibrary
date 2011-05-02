// Oliver Kullmann, 2.5.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKlib/Satisfiability/Assignments/PartialAssignments/plans/Boolean.hpp
  \brief Plans for for boolean partial assignments


  \todo Class BPass0
  <ul>
   <li> BPass0::fill_map should be made superfluous:
    <ol>
     <li> By using a transform-iterator, directly filling the map with
     the pairs (var, cond). </li>
    </ol>
   </li>
   <li> We need tests. </li>
   <li> Update the general concepts, and create a suitable concept for
   BPass0. </li>
  </ul>

*/
