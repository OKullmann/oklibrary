// Oliver Kullmann, 18.8.2007 (Swansea)
/* Copyright 2007, 2009, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Solvers/plans/general.hpp
  \brief Plans for the development of SAT solvers in the OKlibrary

  Besides the OKsolver family, there will be other more specialised or
  experimental solvers, and interfaces to external solvers.

  A "solver" here is a special composition of components from the library,
  based on some guiding ideas.


  \todo External solvers
  <ul>
   <li> Develop first basic ideas what to do about external solvers. </li>
   <li> They are installed under ExternalSources, but at least for some
   we should enable interfacing, or simulate them (as a performance test,
   for example). </li>
   <li> All current sub-modules for external solvers should have milestones.
   </li>
  </ul>


  \todo Special solvers
  <ul>
   <li> Develop a kind of conceptual framework for specialised solvers etc.
   (the areas, the directions to move). </li>
   <li> One might have solvers specialised for application areas. </li>
  </ul>


  \todo DONE Update namespaces.

*/

/*!
  \namespace OKlib::Satisfiability::Solvers
  \brief Super-module on solvers

  Alias "Sol".
*/

namespace OKlib {
  namespace Satisfiability {
    namespace Solvers {
    }
    namespace Sol = Solvers;
  }
}

