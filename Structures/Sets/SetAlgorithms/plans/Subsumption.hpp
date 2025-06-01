// Oliver Kullmann, 8.11.2011 (Swansea)
/* Copyright 2011, 2025 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Structures/Sets/SetAlgorithms/plans/Subsumption.hpp
  \brief Plans regarding algorithms for subsumption-elimination


  \todo Update structure
  <ul>
   <li> In Satisfiability/Transformers/Generators/Bicliques/Algorithms.hpp
          we have the (also simple) algorithm
          Algorithms::forward_strictsubsumption_by_erase . </li>
   <li> A better algorithm is in
          ComputerAlgebra/Hypergraphs/Lisp/SetSystems.mac . </li>
   <li> Important to restart the test-system in this module (and elsewhere).
  </ul>


  \todo Move todos
  <ul>
   <li> Move the todos in Structures/Sets/SetAlgorithms/Subsumption.hpp here.
   </li>
  </ul>


  \todo Better algorithms
  <ul>
   <li> Literature:
    <ol>
     <li> [Zhang SAT 2005]. </li>
    </ol>
   </li>
   <li> min_elements_unique_fast_l_ in
   ComputerAlgebra/Hypergraphs/Lisp/SetSystems.mac. </li>
  </ul>

*/
