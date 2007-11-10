// Oliver Kullmann, 26.9.2007 (Swansea)
/* Copyright 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Interfaces/plans/general.hpp
  \brief General plans on interfaces for satisfiability


  \todo Update namespaces. DONE


  \todo Create milestones. DONE


  \todo Ladder logic
  <ul>
   <li> Is the super-module the right place for transformations from
   ladder logic to propositional logic? </li>
   <li> It seems Satisfiability/Transformers is more suitable. But perhaps
   here we have some corresponding input/output. </li>
   <li> Which other modules then shall go to Satisfiability/Interfaces ? </li>
  </ul>

*/

/*!
  \namespace OKlib::Satisfiability::Interfaces
  \brief The supermodule with components for satisfiability interfaces

  Alias "Ifs".
*/

namespace OKlib {
  namespace Satisfiability {
    namespace Interfaces {
    }
    namespace Ifs = Interfaces;
  }
}

