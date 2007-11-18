// Oliver Kullmann, 20.11.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Transformers/SATModuloTheories/plans/general.hpp
  \brief General Plans for SAT "modulo theories"


  \todo Overview on SMT
  <ul>
   <li> Easiest access should be incorporating some open source SMT
   libraries into the OKlibrary (-> ExternalSources), and study them. </li>
   <li> Of course, with accompanying literature. </li>
  </ul>
  

  \todo Overview on main types of constraints
  <ul>
   <li> Difference constraints
    <ul>
     <li> See[Armando,Castellini,Giunchiglia,Maratea; A {SAT}-Based Decision
     Procedure for the Boolean Combination of Difference Constraints]. </li>
     <li> For the active clause-sets the Bellman-Ford algorithm is applied.
     </li>
    </ul>
   </li>
  </ul>


  \todo How to translate these problems into our domain
  <ul>
   <li> Once we have some overview, we need to get ideas how to handle
   SMT in our library. </li>
  </ul>


  \todo Create new milestones

*/

/*!
  \namespace OKlib::Satisfiability::Transformers::SATModuloTheories
  \brief Components for active clause-sets with an underlying
  logical theory

  Alias "SmT".
*/

namespace OKlib {
  namespace Satisfiability {
    namespace Transformers {
      namespace SATModuloTheories {
      }
      namespace SmT = SATModuloTheories;
    }
  }
}

