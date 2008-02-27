// Oliver Kullmann, 4.2.2006 (Swansea)
/* Copyright 2006 - 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file AutarkySearch/plans/general.hpp
  \brief Plans for the module on autarky search

  Searching for (single) autarkies, using complete or incomplete methods.


  \todo Update namespaces : DONE


  \todo Update plans : DONE
  <ul>
   <li> DONE (now in Satisfiability/Algorithms/Autarkies/plans/general.hpp)
   Is this sub-module only for "direct" search for autarkies,
   while the algorithms performing iterated SAT-search with
   variable-cross-out (see [Kullmann, Lynce, Marques-Silva 2006])
   goes to a different module? </li>
   <li> DONE (no) Should we make an organisation difference between full
   autarky search and partial autarky search? </li>
  </ul>


  \todo Update namespace usage.


  \todo Link to ComputerAlgebra/Satisfiability/Lisp/Autarkies/plans/general.hpp
  <ul>
   <li> We need to combine plans. </li>
   <li> First the "procedural specifications" at the Lisp/Maxima level have to
   be established. </li>
  </ul>

*/

/*!
  \namespace OKlib::Satisfiability::Algorithms::Autarkies::Search
  \brief Components for (general) autarky search

  Alias "Sea".
*/

namespace OKlib {
  namespace Satisfiability {
    namespace Algorithms {
      namespace Autarkies {
        namespace Search {
        }
        namespace Sea = Search;
      }
    }
  }
}

