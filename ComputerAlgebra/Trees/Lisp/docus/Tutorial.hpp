// Rui Wang, 21.11.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Trees/Lisp/docus/Tutorial.hpp
  \brief Tutorial on trees in Maxima/Lisp


  <h1> %Trees via Maxima in the OKlibrary </h1>

  <ol>
   <li> For the OKlibrary, An "unlabelled rooted tree" ("rt") is recursively 
   defined as a list [T_1, ..., T_n], where the T_i are the subtrees (the case 
   n=0, i.e., the empty list, represents the trivial tree).
   \verbatim
(%i1) T:[[],[[],[]]];
(%o1) [[],[[],[]]]
   \endverbatim
   </li>
   <li> Similarly, a "labelled rooted tree" ("lrt") is recursively defined as a
   list [L, T_1, ..., T_n], n >= 0, where L is the label, and the T_i are the
   subtrees.
   \verbatim
(%i2) T:[1,[2],[3,[4],[5]]];
(%o2) [1,[2],[3,[4],[5]]]
   \endverbatim
   </li>
  </ol>

*/

