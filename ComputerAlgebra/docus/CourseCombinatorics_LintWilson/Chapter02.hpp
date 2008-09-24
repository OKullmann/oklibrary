// Oliver Kullmann, 23.7.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/docus/CourseCombinatorics_LintWilson/Chapter02.hpp
  \brief Chapter "Trees" from [A Course in %Combinatorics]


  <h1> Counting, enumerating and sampling trees </h1>

  See ComputerAlgebra/Graphs/Lisp/Trees/plans/Generators.hpp.

  <ul>
   <li> The Pruefer code for a given tree is given by, e.g.,
   \verbatim
tree2pruefercode_og([[1,2,3,4,5],[{1,2},{3,2},{2,4},{4,5}]]);
 [2,2,4]
   \endverbatim
   Conversely, given a pruefer code, and the vertex list, the tree can be
   reconstructed by 
   \verbatim
pruefercode2tree_og([1,2,3,4,5],[2,2,4]);
 [[1,2,3,4,5],[{1,2},{2,3},{2,4},{4,5}]])
   \endverbatim
   </li>
   <li> pruefercode2tree_og is the inverse of tree2pruefercode_og w.r.t. the
   underlying graph, while the edge-order in general is not preserved, e.g.,
   \verbatim
pruefercode2tree_og([1,2,3,4,5,6],tree2pruefercode_og([[1,2,3,4,5,6],[{1,2},{3,2},{4,5},{4,6},{2,4}]]));
 [[1,2,3,4,5,6],[{1,2},{2,3},{2,4},{4,5},{4,6}]]
   \endverbatim 
   </li>
  </ul>


  <h1> Spanning trees </h1>


  <h1> Connectivity </h1>
  

*/

