// Oliver Kullmann, 16.6.2007 (Swansea)
/* Copyright 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKlib/Satisfiability/plans/general.hpp
  \brief Contains all super-modules regarding (generalised) satisfiability
  issues


  \todo New supermodule on autarkies
  <ul>
   <li> The old plan was to have autarkies inside supermodule Assignments.
   </li>
   <li> But due to the basic scape, it seems better to have a supermodule
   Autarkies (namespace-alias "Aut"). </li>
  </ul>


  \todo New supermodule on CSP
  <ul>
   <li> This module would consider mostly the traditional CSP
   approaches (like in
   ComputerAlgebra/Satisfiability/Lisp/Backtracking/ConstraintSatisfaction.mac)
   </li>
   <li> How to name it: "ConstraintSatisfaction" ? Alias "CoS" ? </li>
  </ul>

*/

/*!
  \namespace OKlib::Satisfiability
  \brief The part of the OKlibrary concerned with (generalised) satisfiability

  Alias "Sat".
*/

namespace OKlib {
  namespace Satisfiability {
  }
  namespace Sat = Satisfiability;
}

