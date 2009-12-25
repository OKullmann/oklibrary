// Oliver Kullmann, 25.12.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file  OKlib/Satisfiability/ProblemInstances/Literals/concepts/general.hpp
  \brief Plans regarding general concepts for literals


  \todo Move the related concepts and plans from OKlib/Concepts here.


  \todo Boolean literals
  <ul>
   <li> See "Prototypes" in ProblemInstances/Literals/plans/general.hpp
   for first concepts. </li>
   <li> We have FullyConstructibleEq (see Concepts/Basics.hpp). </li>
   <li> For a literal type Lit we have the following type-operations:
    <ol>
     <li> OKlib::Literals::traits::var_type<Lit>::type </li>
     <li> OKlib::Literals::traits::cond_type<Lit>::type (a typedef for bool).
     </li>
    </ol>
   </li>
   <li> For a literal x we have the following value-operations:
    <ol>
     <li> -x for negation </li>
     <li> OKlib::Literals::var(x) for the underlying variable </li>
     <li> OKlib::Literals::cond(x) for the underlying condition, a boolean
     which is true iff x is positive. </li>
    </ol>
   </li>
  </ul>

*/

