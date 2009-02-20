// Oliver Kullmann, 23.9.2007 (Swansea)
/* Copyright 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Algorithms/Backtracking/plans/general.hpp
  \brief General plans on generic backtracking algorithms, their concepts and
  implementations.


  \todo Update namespace usage.


  \todo Concepts
  <ul>
   <li> We have now two initial approaches:
    <ol>
     <li> Satisfiability/Algorithms/Backtracking/DLL_Algorithms.cpp </li>
     <li> Satisfiability/Algorithms/Backtracking/GenericBacktracking.hpp
     (see Satisfiability/Algorithms/Backtracking/plans/GenericBacktracking.hpp) </li>
    </ol>
   </li>
   <li> These attempts need to be merged. </li>
   <li> Considers OK's 3 schemes in [Kullmann 2007; Present and future of
   practical SAT solving]. </li>
   <li> The purpose of this module is to deliver algorithmic skeletons; one question
   is whether there should be only one, most general, or we should have several ones,
   according to the current paradigms (and generalisations)? </li>
  </ul>


  \todo New milestones
  <ul>
   <li> Once the approach towards generic backtracking algorithm has been
   clarified, further milestones need to be created. </li>
  </ul>


  \todo Move the content of OKlib/SATAlgorithms here : DONE


  \todo Update namespaces : DONE

*/

/*!
  \namespace OKlib::Satisfiability::Algorithms::Backtracking
  \brief Components for (generalised) backtracking SAT algorithms

  Alias "Bkt".
*/

namespace OKlib {
  namespace Satisfiability {
    namespace Algorithms {
      namespace Backtracking {
      }
    namespace Bkt = Backtracking;
    }
  }
}

