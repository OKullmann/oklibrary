// Oliver Kullmann, 21.5.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/LinearAlgebra/Lisp/plans/general.hpp
  \brief Plans on linear algebra in Maxima


  \todo Create milestones


  \todo Write tests


  \todo Write docus


  \todo Indices
  <ul>
   <li> Also hermitian_rank_eig and hermitian_rank_charpoly should
   be changed to compute the indices as pnn_indices. </li>
   <li> So that we get pnn_indices_eig and pnn_indices_charpoly. </li>
   <li> Perhaps then we create a generic function for computing the
   hermitian rank from a version of pnn_indices. </li>
   <li> And we should have a function for computing the characteristic
   polynomial of a clause-set (then hermitian_rank_charpoly would use
   this function). </li>
  </ul>


  \todo Algorithms for the hermitian rank
  <ul>
   <li> Currently hermitian_rank_charpoly seems fastest. </li>
   <li> I (OK) would guess that the algorithm for hermitian_rank (the
   variation of Gaussian elimination) should be nevertheless the fastest:
    <ol>
     <li> Since at this level we don't want to go into implementation details,
     I (OK) don't see how to improve the implementation of "pnn_indices". </li>
     <li> Study the literature on computing the pnn-indices. </li>
     <li> And then there are the sparsity-considerations. </li>
    </ol>
   </li>
  </ul>

*/

