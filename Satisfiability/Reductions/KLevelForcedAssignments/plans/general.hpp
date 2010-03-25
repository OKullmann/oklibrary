// Oliver Kullmann, 17.12.2005 (Swansea)
/* Copyright 2005 - 2007, 2008, 2009, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Reductions/KLevelForcedAssignments/plans/general.hpp
  \brief Plans for the module KLevelForcedAssignments

  The special case K=2 (and extensions) is handled in module
  FailedLiteralReduction (see FailedLiteralReduction/plans/general.hpp),
  while "real" resolution reductions are handled in module Resolution
  (see Resolution/plans/Reductions.hpp). This module considers
  algorithms and data structures for the general case (arbitrary K)
  of generalised unit-clause-propagation.


  \todo DONE Update namespaces.


  \todo Connections
  <ul>
   <li> See ComputerAlgebra/Satisfiability/Lisp/Reductions/GeneralisedUCP.mac
   for Maxima-specifications. </li>
  </ul>


  \todo Create milestones.

*/

/*!
  \namespace OKlib::Satisfiability::Reductions::KLevelForcedAssignments
  \brief Components for generalised unit-clause elimination

  Alias "Klfa".
*/

namespace OKlib {
  namespace Satisfiability {
    namespace Reductions {
      namespace KLevelForcedAssignments
      }
      namespace Klfa = KLevelForcedAssignments;
    }
  }
}

