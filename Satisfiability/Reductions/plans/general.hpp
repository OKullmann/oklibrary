// Oliver Kullmann, 29.1.2008 (Swansea)
/* Copyright 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Reductions/plans/general.hpp
  \brief Plans for the development of SAT reductions


  \todo DONE Update namespaces.


  \todo Preprocessing wrappers
  <ul>
   <li> Likely we should create a module Preprocessing. </li>
   <li> Yet it (only) shall contain tools for writing wrappers,
   wrapping given solvers with given preprocessors. </li>
   <li> See the OKsolver_2002 wrapper as well as the satz215 wrapper. </li>
   <li> In that module of course we shall also develop our own preprocessors.
   </li>
   <li> Perhaps this is yet the easiest way (besides updating the
   OKsolver_2002) to enter the solver-business. </li>
   <li> See ComputerAlgebra/Satisfiability/Lisp/Preprocessing/plans/general.hpp.
   </li>
   <li> Actually, according to the plans there module Preprocessing should
   be a supermodule, directly located in part Satisfiability. </li>
  </ul>

*/

/*!
  \namespace OKlib::Satisfiability::Reductions
  \brief Super-module for reductions

  Alias "Red".
*/

namespace OKlib {
  namespace Satisfiability {
    namespace Reductions {
    }
    namespace Red = Reductions;
  }
}

