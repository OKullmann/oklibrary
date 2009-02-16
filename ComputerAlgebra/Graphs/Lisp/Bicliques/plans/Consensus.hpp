// Oliver Kullmann, 15.1.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Graphs/Lisp/Bicliques/plans/Consensus.hpp
  \brief Plans regarding implementing the "consensus algorithm" from [XXX] for computing all maximal bicliques
  
  
  \todo Notions and notations
  <ul>
   <li> A kind of dictionary is needed. </li>
   <li> Compare with the functions for resolution. </li>
  </ul>


  \todo The basic "consensus" operation
  <ul>
   <li> Discuss the basic forms. </li>
   <li> Provide all basic definitions in
   ComputerAlgebra/Graphs/Lisp/Bicliques/Consensus.mac (avoiding "similar").
   </li>
  </ul>


  \todo Maximisation functions
  <ul>
   <li> It appears that the various maximisation functions should better
   be placed somewhere else. </li>
   <li> maximal_bc_gl belongs to general biclique functionality. </li>
   <li> For aux_con_alg1_abs compare
   max_elements in ComputerAlgebra/Hypergraphs/Lisp/SetSystems.mac. </li>
  </ul>


  \todo The consensus iteration
  <ul>
   <li> Discuss the basic forms. </li>
  </ul>

*/

