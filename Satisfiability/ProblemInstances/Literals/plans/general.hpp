// Oliver Kullmann, 23.9.2007 (Swansea)
/* Copyright 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ProblemInstances/Literals/plans/general.hpp
  \brief General plans on literals, their concepts and
  implementations.


  \todo Move the related concepts and plans from OKlib/Concepts here.


  \todo Update namespace-usage.


  \todo Prototypes
  <ul>
   <li> What to do with ProblemInstances/Literals/Literal.hpp ? </li>
   <li> Very first prototype for "signed literals" (set-valued literals),
   based on the basic atomic condition for arbitrary value-types. What to
   do about the requirement that the atomic condition of a literal is not
   constant? Just an invariant? </li>
  </ul>

*/

/*!
  \namespace OKlib::Satisfiability::ProblemInstances::Literals
  \brief Module with components for (generalised) literals

  Alias "Lit".
*/

namespace OKlib {
  namespace Satisfiability {
    namespace ProblemInstances {
      namespace Literals {
      }
      namespace Lit = Literals;
    }
  }
}

