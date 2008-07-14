// Oliver Kullmann, 5.4.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/plans/ComputerAlgebra.hpp
  \brief Plans regarding building of computer algebra systems

  See also
  <ul>
   <li> Buildsystem/ExternalSources/SpecialBuilds/plans/Axiom.hpp </li>
   <li> Buildsystem/ExternalSources/SpecialBuilds/plans/GAP.hpp </li>
   <li> Buildsystem/ExternalSources/SpecialBuilds/plans/Maxima.hpp </li>
   <li> Buildsystem/ExternalSources/SpecialBuilds/plans/Sage.hpp </li>
  </ul>


  \todo General systems
  <ul>
   <li> 
   <li> What about Octave? http://www.gnu.org/software/octave/
   It has (only) numerical capabilities, and we should evaluate
   which could be useful for us (sparse matrices and optimisation). </li>
  </ul>


  \todo Computational commutative algebra
  <ul>
   <li> CoCoA http://cocoa.dima.unige.it/ </li>
   <li> Singular: http://www.singular.uni-kl.de </li>
  </ul>


  \todo Vicinity of computational group theory
  <ul>
   <li> Is the computational (open-source) semigroup theory outside of GAP?
   </li>
   <li> Magnus http://www.grouptheory.org/magnus; perhaps we postpone this,
   since it might be more specialised </li>
   <li> Sims' Rutgers Knuth-Bendix package rkbp
   http://users.ox.ac.uk/~vlee/4engel/rkbp/
    <ol>
     <li> Doesn't contain a licence, and thus likely can't be distributed.
     </li>
    </ol>
   </li>
   <li> DONE (contained in GAP)
   MeatAxe http://www.math.rwth-aachen.de/homes/MTX/ (perhaps also
   postponed, since somewhat more specialised) </li>
   <li> DONE (proprietary) Magma </li>
   <li> DONE (included with GAP) ACE3 </li>
   <li> DONE (included with GAP) KBMAG </li>
  </ul>


  \todo Polyeders, matroids etc.
  <li> PORTA and SMAPO at
  http://www.iwr.uni-heidelberg.de/iwr/comopt/soft </li>
  <li> Code for [Bokowski, Computational Oriented Matroids] at
  http://juergen.bokowski.de, and other software. </li>

*/

