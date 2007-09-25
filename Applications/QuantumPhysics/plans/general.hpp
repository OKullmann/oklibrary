// Oliver Kullmann, 22.11.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file QuantumPhysics/plans/general.hpp
  \brief Plans regarding certain aspects of quantum physics (as applications
  of satisfiability)


  \todo Move this module to part Applications. DONE


  \todo Update namespaces. DONE


  \todo Detailed examination of Peres33PointConfiguration.cnf
   - w.r.t. the number of variables or number of clauses the smallest
     minimally unsatisfiable sub-clause-set
   - also counting all minimally unsatisfiable and maximally satisfiable
     sub-clause-sets
   - and the largest satisfiable sub-clause-set (32 clauses)
   - should be lean
   - necessary and potentially necessary clauses etc. ({-1,-2,-3} is necessary, and also the
     associated 2-clauses; removing the whole triple 1,8,9 also renders it sat, and this
     seems to hold for all triples; it seems that also all binary clauses coming from
     the "free pairs" are necessary --- so perhaps all 33 variables are needed)
   - based on the symmetries of the underlying figure, it should be not too hard
     to determine the symmetries of the cnf
   - resolution complexity
   - is there some special structure?


  \todo Testing realisability
  Test the conjecture that every graph with a circuit of length 4 is orthogonally
  realisable; a stronger conjecture is that after fulfilling all forced choices (given
  by triplets) via an arbitrary traversal (in the sense as developed in CS-232)
  and making always random choices exactly the realisable graphs are realised
  with high probability. For this we need to create random graphs without
  cycles of length 4.
  Of course, a first test is performed with the hypergraph from the Peres configuration.


  \todo Minimal configuration
  What is the minimal number of vertices in a realisable graph which
  is not 101-colourable? Develop plans to attack this problem.

*/

/*!
  \namespace OKlib::Applications::QuantumPhysics
  \brief Components related to certain aspects of quantum physics

  Alias "QuP".
*/

namespace OKlib {
  namespace Applications {
    namespace QuantumPhysics {
    }
    namespace QuP = QuantumPhysics;
  }
}

