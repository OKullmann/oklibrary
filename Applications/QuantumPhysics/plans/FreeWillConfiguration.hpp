// Oliver Kullmann, 9.12.2007 (Swansea)
/* Copyright 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file QuantumPhysics/plans/FreeWillConfiguration.hpp
  \brief Plans regarding the point-configuration in the [Conway; Free Will]-paper


  \todo Detailed examination of Peres33PointConfiguration.cnf
  <ul>
   <li> W.r.t. the number of variables or number of clauses the smallest
   minimally unsatisfiable sub-clause-set. </li>
   <li> Also counting all minimally unsatisfiable and maximally satisfiable
   sub-clause-sets. </li>
   <li> And the largest satisfiable sub-clause-set (32 clauses) </li>
   <li> Should be lean. </li>
   <li> Necessary and potentially necessary clauses etc. ({-1,-2,-3} is
   necessary, and also the associated 2-clauses; removing the whole triple
   1,8,9 also renders it sat, and this seems to hold for all triples; it seems
   that also all binary clauses coming from the "free pairs" are necessary ---
   so perhaps all 33 variables are needed). </li>
   <li> Based on the symmetries of the underlying figure, it should be not too hard
   to determine the symmetries of the cnf. </li>
   <li> Resolution complexity </li>
   <li> Is there some special structure? </li>
  ,/ul>


  \todo Testing realisability
  <ul>
   <li> Test the conjecture that every graph with a circuit of length 4 is
   orthogonally realisable. </li>
   <li> A stronger conjecture is that after fulfilling all forced choices (given
   by triplets) via an arbitrary traversal (in the sense as developed in CS-232)
   and making always random choices exactly the realisable graphs are realised
   with high probability. For this we need to create random graphs without
   cycles of length 4. </li>
   <li> Of course, a first test is performed with the hypergraph from the
   Peres configuration. </li>
  </ul>


  \todo Minimal configuration
  <ul>
   <li> What is the minimal number of vertices in a realisable graph which
   is not 101-colourable? Develop plans to attack this problem. </li>
  </ul>

*/

