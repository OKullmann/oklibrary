// Oliver Kullmann, 26.9.2007 (Swansea)
/* Copyright 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ProblemInstances/ActiveClauses/plans/general.hpp
  \brief General plans on active clauses

  "Active clauses" provide polynomial-time functionality for many
  fundamental services, at least deciding satisfiability w.r.t. the
  given partial (multi)assignment.


  \todo Update namespaces. DONE


  \todo Create milestones.

*/

/*!
  \namespace OKlib::Satisfiability::ProblemInstances::ActiveClauses
  \brief The supermodule with components for problem instances

  Alias "ACl".
*/

namespace OKlib {
  namespace Satisfiability {
    namespace ProblemInstances {
      namespace ActiveClauses {
        namespace ACl = ActiveClauses;
      }
    }
  }
}

