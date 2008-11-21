// Oliver Kullmann, 21.11.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/FiniteFunctions/plans/general.hpp
  \brief Plans regarding boolean functions and generalisations


  \todo Connections to other modules
  <ul>
   <li> See
   Transitional/Satisfiability/FiniteFunctions/plans/general.hpp. </li>
   <li> Links within part ComputerAlgebra:
    <ol>
     <li> We have something in Cryptology (MG). </li>
    </ol>
   </li>
  </ul>


  \todo Basic concepts
  <ul>
   <li> The main point is the construction of representations of finite
   functions via generalised CNF/DNF, propositional logic, CSPs, alliances
   of active clause-sets etc. </li>
   <li> And also general computational representations need to be supported,
   together with conversions to other forms. </li>
   <li> Such "conversions" can involve themselves satisfiability problems in
   various forms. </li>
  </ul>


  \todo Boolean circuits
  <ul>
   <li> Of interest finding boolean circuits (w.r.t. different bases) for
   given boolean functions. </li>
   <li> See the work of Alexander S. Kulikov for example. </li>
  </ul>


  \todo Representations of fundamental functions:
  <ul>
   <li> Cardinality constraints:
    <ol>
     <li> At least k out of n boolean variables are true. </li>
     <li> At most k out of n boolean variables are true. </li>
     <li> Exactly k out of n boolean variables are true. </li>
    </ol>
   </li>
  </ul>

*/

