// Oliver Kullmann, 16.6.2007 (Swansea)
/* Copyright 2007, 2008, 2015 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Heuristics/Projections/plans/general.hpp
  \brief Module for projections (making branching tuples comparable)


  \todo Update
  <ul>
   <li> In OKlib/Satisfiability/Solvers/TawSolver there are various
   implementations. </li>
  </ul>


  \todo Incorporate the theory from OK's SAT-Handbook-article: planning
  <ul>
   <li> See module ComputerAlgebra/Satisfiability/Lisp/BranchingTuples,
   ComputerAlgebra/Satisfiability/Lisp/BranchingTuples/plans/general.hpp. </li>
   <li> The first goal is to be able to perform large experiments and
   optimise distances (Maxima/Lisp is very slow). </li>
   <li> Regarding the tau-function, we use a template for the floating point
   type; for all practical concerns double should be sufficient. </li>
   <li> Yet we ignore solvers, but we ask them for output in xml-format. </li>
   <li> So the question about xml arises: Is still xerces the right choice?
   </li>
   <li> We need also trees; see Trees/plans/general.hpp. </li>
   <li> According to the name of this module, here actually we would have
   only the tau-function and everything directly related to it, while
   the analysis of trees goes to module
  </ul>

*/

/*!
  \namespace OKlib::Satisfiability::Heuristics::Projections
  \brief Components for projections

  Alias "Prj".
*/

namespace OKlib {
  namespace Satisfiability {
    namespace Heuristics {
      namespace Projections {
      }
      namespace Prj = Projections;
    }
  }
}

