// Oliver Kullmann, 13.7.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/OrientedMatroids/Lisp/plans/general.hpp
  \brief Plans regarding oriented matroids in Maxima/Lisp


  \todo Fundamental notions
  <ul>
   <li> While the basic view of a matroid is that of a hypergraph
   (see ComputerAlgebra/Matroids/Lisp/plans/general.hpp), the basic
   view of an oriented matroid is that of a *boolean clause-set*. </li>
   <li> [Bachem, Kern; Linear Programming Duality] is quite readable,
   but the focus on the more "loose" concept of an oriented matroid
   as a pair (with "built-in duality"). </li>
   <li> Abbreviations:
    <ol>
     <li> "omtr" for "oriented matroid" </li>
     <li> "sgnv" for "sign vector" </li>
     <li> "sgnvs" for "sign vector set" </li>
    </ol>
   </li>
  </ul>

*/

