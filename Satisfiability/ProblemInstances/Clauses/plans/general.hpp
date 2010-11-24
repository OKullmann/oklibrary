// Oliver Kullmann, 23.9.2007 (Swansea)
/* Copyright 2007, 2009, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ProblemInstances/Clauses/plans/general.hpp
  \brief General plans on clauses, their concepts and implementations


  \todo Update namespaces: DONE


  \todo Create milestones


  \todo Connections
  <ul>
   <li> See
   Satisfiability/Reductions/UnitClausePropagation/plans/ClsAdaptorUCP.hpp
   for classes which read clause-sets and then perform unit-clause propagation.
   </li>
  </ul>


  \todo Organisation
  <ul>
   <li> Move the related concepts and plans from OKlib/Concepts here. </li>
  </ul>


  \todo Updates
  <ul>
   <li> Update ProblemInstances/Clauses/Clause.hpp, so that it becomes
   a very simple implementation of the generic concepts. </li>
  </ul>


  \todo Head-tail clauses
  <ul>
   <li> Similar to ProblemInstances/Clauses/ClausesWithRemoval.hpp, we
   need an implementation of clauses for initial UCP, which uses two
   watched literals in the head-tail form. </li>
   <li> That is, the two watched literals are initially the first and
   the last clause, and then they move inward; this is faster for finding
   the next watched literal (since we don't consider again the literals
   we already touched, and we can stop once the two watched literals
   touch), while we do not have here the update-problem (for initial
   UCP there is no backtracking). </li>
   <li> The concept could perhaps be just that for (general) watched literals;
   see "Watched literals" in ProblemInstances/Clauses/concepts/general.hpp.
   </li>
  </ul>

*/

/*!
  \namespace OKlib::Satisfiability::ProblemInstances::Clauses
  \brief General plans on clauses

  Alias "Cla".
*/

namespace OKlib {
  namespace Satisfiability {
    namespace ProblemInstances {
      namespace Clauses {
      }
      namespace PrI = ProblemInstances;
    }
  }
}

