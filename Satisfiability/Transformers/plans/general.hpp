// Oliver Kullmann, 13.10.2007 (Swansea)
/* Copyright 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Transformers/plans/general.hpp
  \brief General plans for the general module on transformers (creating SAT problems from other problems)


  \todo Update namespaces : DONE


  \todo Create milestones : DONE


  \todo Update namespace-usage.


  \todo Travelling salesman
  <ul>
   <li> What are the fileformats for TSPs ? </li>
   <li> And then there are the different versions of TSP ? </li>
   <li> Of course, the first thing is are translations into CNF; first via
   Lisp/Maxima. </li>
   <li> Compare "Organisation" in Graphs/plans/general.hpp. </li>
   <li> Can translations of Hamiltonian-path-problems into SAT be understood as
   special cases? </li>
  </ul>

*/

/*!
  \namespace OKlib::Satisfiability::Transformers
  \brief The supermodule with components for transformers into SAT

  Alias "Tra".
*/

namespace OKlib {
  namespace Satisfiability {
    namespace Transformers {
    }
    namespace Tra = Transformers;
  }
}

