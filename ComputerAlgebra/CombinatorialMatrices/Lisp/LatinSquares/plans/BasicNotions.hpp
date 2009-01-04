// Oliver Kullmann, 4.1.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/CombinatorialMatrices/Lisp/LatinSquares/plans/BasicNotions.hpp
  \brief Plans regarding the basic notions about latin squares


  \todo The notion of a "latin square"
  <ul>
   <li> </li>
  </ul>


  \todo Properties of latin squares
  <ul>
   <li> Regarding considering a latin square L as a quasigroup Q (via ls2qgrp),
   we have the following basic properties:
    <ol>
     <li> Q can be unital (this is automatically given when L is fully
     standardised). </li>
     <li> Q can be a associative (that is, a group; this includes unitality).
     </li>
     <li> Q can be commutative (that is, L is symmetric). </li>
    </ol>
   </li>
   <li> If we consider a latin square L over a set H (and thus we allow
   arbitrary isomorphisms, which independently can permute rows and columns):
    <ol>
     <li> L is always isomorphic to a fully standardised latin square, and
     thus (if not empty) yields a unital quasigroup. </li>
     <li> For the other properties of quasigroups (which only arise once
     we fix a (general) matrix isomorphism to an observed square matrix),
     associativity (yielding a group) and commutativity (yielding a
     commutative quasigroup), it seems unlikely that they can always
     be realised. </li>
     <li> And combination of commutativity with unitality might again give
     a new property (yielding a commutative unital quasigroup). </li>
     <li> Whether L can yield a commutative quasigroup is exactly whether
     L is self-polar (see
     ComputerAlgebra/CombinatorialMatrices/Lisp/Isomorphisms.mac); also the
     weaker notion of self-duality seems of interest here. </li>
     <li> As
     \verbatim
selfpolar_bydef_m_p(scom2m(stdls2qgrp(nassoc_fstdls)));
false
selfdual_bydef_m_p(scom2m(stdls2qgrp(nassoc_fstdls)));
false
     \endverbatim
     shows, the latin square nassoc_fstdls is not even isomorphic to
     its transposed (as a general matrix). </li>
    </ol>
   </li>
  </ul>

*/

