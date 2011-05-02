// Oliver Kullmann, 23.9.2007 (Swansea)
/* Copyright 2007, 2009, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ProblemInstances/Literals/plans/general.hpp
  \brief General plans on literals, their concepts and implementations.


  \todo Update namespace-usage


  \todo Prototypes
  <ul>
   <li> What to do with ProblemInstances/Literals/Literal.hpp ? </li>
   <li> Very first prototype for "signed literals" (set-valued literals),
   based on the basic atomic condition for arbitrary value-types. What to
   do about the requirement that the atomic condition of a literal is not
   constant? Just an invariant? </li>
   <li> It seems using "-x" for the negation of a (boolean) literal x
   is easiest. </li>
   <li> While a boolean literal x is positive iff OKlib::Literals::cond(x)
   is true. </li>
   <li> A literal should be singular iff the underlying variable is singular,
   and the condition of a singular literal is implementation-defined. </li>
   <li> As with variables, literals are implicitly converted to bool, with the
   result true iff the literal is not singular. </li>
  </ul>


  \todo Write tests
  <ul>
   <li> OKlib/Satisfiability/ProblemInstances/Literals/Basics.hpp </li>
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

