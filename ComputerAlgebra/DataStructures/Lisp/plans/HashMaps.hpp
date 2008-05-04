// Oliver Kullmann, 4.5.2008 (Guangzhou)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/DataStructures/Lisp/plans/HashMaps.hpp
  \brief Plans for Maxima-components regarding maps


  \todo Concept of a set-theoretical map
  <ul>
   <li> DONE (see ComputerAlgebra/DataStructures/Lisp/HashMaps.mac)
   We should institutionalise the concept of a "set-theoretic"
   map as a set of pairs. </li>
   <li> Operations:
    <ol>
     <li> check for unique values </li>
     <li> extract domain </li>
     <li> extract range </li>
     <li> DONE compute value for a single argument </li>
     <li> compute value set for a set of arguments </li>
    </ol>
   </li>
   <li> We also should have the concept of a (set-theoretical)
   relation as set of pairs!
    <ol>
     <li> Perhaps then we create a new sub-module "Relations.mac",
     which also contains the set-maps. </li>
    </ol>
   </li>
  </ul>

*/
