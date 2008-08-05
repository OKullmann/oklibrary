// Oliver Kullmann, 4.8.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Graphs/Lisp/Trees/plans/Representations.hpp
  \brief Plans regarding the creation and usage of tree representations


  \todo tree2pruefercode_og
  <ul>
   <li> This function is very slow. </li>
   <li> We need also a more efficient version. The basic ideas are as follows:
    <ol>
     <li> First we compute a standardised version T' of the tree T. </li>
     <li> From T' we compute the representation P as a polar hypergraph via
     stdg2phg (see "Polar hypergraphs" in
     ComputerAlgebra/Hypergraphs/Lisp/plans/Basics.hpp). </li>
     <li> From P we obtain the array d of vertex degrees from T'. </li>
     <li> Additionally we have an integer array E, where E(v)=1 means
     that vertex v (of T') has been eliminated (E is 0-initialised). </li>
     <li> Scanning through d, we compute the set M of monovalent vertices
     in T'. </li>
     <li> Now the loop:
      <ol>
       <li> Using "first_element", we obtain the smallest element x of M. </li>
       <li> Via P and E we find the current (single) neighbour y of x. </li>
       <li> y is appended to the code pc (initially the empty list). </li>
       <li> E(x) : 1, and x is removed from M. </li>
       <li> The degree of y in d is reduced by one, and if the degree of y
       becomes 1, then y is added to M. </li>
      </ol>
     <li>
    </ol>
   </li>
   <li> Except of not using a dedicated data-structure for M, this looks
   like a natural representation of the natural efficient implementation.
    <ol>
     <li> The main imported datastructure (and "point of view") is given by
     P; d is naturally connected to it, and it doesn't seem worth the effort
     to have some special function for computing d from P. </li>
     <li> The use of a priority queue for M is indicated by using
     "first_element" (which is also needed, since "first" is not guaranteed
     to work, and should not be used). </li>
     <li> There is no need to update P, but instead we use E: This just
     performs the minimal work (nothing is repeated in this way). </li>
    </ol>
   </li>
   <li> Later we might implement it also using Boost::Graphs. </li>
   <li> Then perhaps the old version is renamed to "tree2pruefercode_bydef_og".
   </li>
  </ul>

*/

