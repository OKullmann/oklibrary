// Oliver Kullmann, 4.5.2008 (Guangzhou)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/DataStructures/Lisp/plans/HashMaps.hpp
  \brief Plans for Maxima-components regarding maps


  \todo New naming conventions
  <ul>
   <li> Renaming old functions:
    <li> "setmapp" -> sm_p </li>
    <li> XXX </li>
   </li>
  </ul>


  \todo Concept of a set-theoretical map
  <ul>
   <li> DONE (see ComputerAlgebra/DataStructures/Lisp/HashMaps.mac)
   We should institutionalise the concept of a "set-theoretic"
   map as a set of pairs. </li>
   <li> Operations:
    <ol>
     <li> DONE check for right-uniqueness </li>
     <li> extract domain </li>
     <li> extract range </li>
     <li> DONE compute value for a single argument </li>
     <li> compute value set for a set of arguments </li>
    </ol>
   </li>
   <li> We need the same functinality also for ordered set-maps. </li>
   <li> We also should have the concept of a (set-theoretical)
   relation as set of pairs!
    <ol>
     <li> Perhaps then we create a new sub-module "Relations.mac",
     which also contains the set-maps. </li>
    </ol>
   </li>
  </ul>


  \todo Improving the hash-maps
  <ul>
   <li> Determining whether key x is contained in hash-map h:
    <ol>
     <li> Yet we can use ev_hm(h,x) if values are never "false". </li>
     <li> Otherwise one has to use a value y which can never occur,
     and one has to check via ev_hm_d(h,x,y). </li>
     <li> So well, should be doable, but it would be nice to have a direct
     way of querying whether x is already present. </li>
    </ol>
   </li>
   <li> Perhaps we provide specialised hash-maps for cases where we know
   a good hash-function for the key, and then we do not need to use the
   conversion to strings:
    <ol>
     <li> Especially for integer arguments this seems interesting. </li>
     <li> The underlying Lisp-functions seem to have already the ability
     to use different hash-functions; we need to understand this better. </li>
    </ol>
   </li>
  </ul>

*/
