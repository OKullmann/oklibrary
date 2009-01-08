// Oliver Kullmann, 1.1.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/CombinatorialMatrices/Lisp/AssociationSchemes/plans/general.hpp
  \brief General plans regarding association schemes


  \todo Connections to other modules
  <ul>
   <li> See ComputerAlgebra/docus/CourseCombinatorics_LintWilson/Chapter30.hpp.
   </li>
  </ul>


  \todo Other tools
  <ul>
   <li> R should have support for association schemes? </li>
   <li> Hanaki and Miyamoto are classifying association scheme in GAP,
   so that there should be support in it. </li>
  </ul>


  \todo Basic notions
  <ul>
   <li> Value associations (arbitrary matrices)
    <ol>
     <li> Association classes </li>
     <li> Value incidence matrices </li>
     <li> Value adjacency matrices for square matrices </li>
    </ol>
   </li>
   <li> Association products (arbitrary and square matrices)
    <ol>
     <li> The transposed association products for arbitrary matrices </li>
     <li> The association products for square matrices. </li>
    </ol>
   </li>
   <li> Parameters of the first kind ("intersection numbers")
    <ol>
     <li> For square matrices only, but we consider general square matrices
     and symmetric matrices. </li>
     <li> Symmetry for undirected value-associated matrices </li>
    </ol>
   </li>
   <li> The problems of different levels of "equipment"
    <ol>
     <li> Just as a matrix </li>
     <li> Together with value range, association classes and incidence
     matrices </li>
     <li> Additionally also with (transposed) association products </li>
     <li> With "neutral element" </li>
     <li> With parameters of the first kind </li>
    </ol>
   </li>
   <li> Tests for the various properties (only for square matrices)
    <ol>
     <li> directed value-associated (the intersection number exist) </li>
     <li> undirected value-associated (additionally the matrix is symmetric)
     </li>
     <li> association scheme (undirected value-associated with "neutral
     element") </li>
    </ol>
   </li>
  </ul>


  \todo Basic examples
  <ul>
   <li> Trivial association schemes </li>
   <li> Divisible association schemes </li>
   <li> Rectangular association schemes </li>
   <li> Triangular association schemes </li>
   <li> Johnson association schemes </li>
   <li> Association schemes of latin square type </li>
   <li> Association schemes from distance-regular graphs </li>
   <li> Hamming association schemes </li>
   <li> Association schemes from groups </li>
  </ul>

*/

