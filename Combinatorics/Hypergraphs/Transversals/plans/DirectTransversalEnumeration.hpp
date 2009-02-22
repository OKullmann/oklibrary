// Oliver Kullmann, 26.8.2005 (Swansea)
/* Copyright 2005 - 2007, 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Combinatorics/Hypergraphs/Transversals/plans/DirectTransversalEnumeration.hpp
  \brief Implementations of enumerations of hypergraph
  transversals based on the direct method.


  \todo Concepts
  <ul>
   <li> Creating the concepts (the output, the transversal
   hypergraph, as container; for incrementell methods the
   iterator in it creates the next transversal "on the fly"). </li>
  </ul>


  \todo %Implementation
  <ul>
   <li> The general implementation as discussed in
   AllSolutions/plans/MinimalAssignments.hpp ("The basic algorithm") should
   be generic enough to obtain the hypergraph case as a special case. </li>
   <li> Worth having a dedicated hypergraph-implementation? </li>
   <li> For prototypes see transversal_hg_rs in
   ComputerAlgebra/Hypergraphs/Lisp/Transversals.mac. </li>
  </ul>


  \todo Connections
  <ul>
   <li> See ComputerAlgebra/Hypergraphs/Lisp/plans/Transversals.hpp. </li>
  </ul>

*/

