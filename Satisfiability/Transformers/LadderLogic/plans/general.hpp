// Oliver Kullmann, 16.11.2007 (Swansea)
/* Copyright 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Transformers/LadderLogic/plans/general.hpp
  \brief General plans for the module on transforming ladder logic in propositional logic


  \todo Create milestones


  \todo Literature overview on boolean translations
  <ul>
   <li> Hollinghouse's dissertation. </li>
  </ul>


  \todo Ladder logic standard
  <ul>
   <li> There are several forms of ladder logic. </li>
   <li> And there is a standard. </li>
   <li> We need to decide on a solid basic form of ladder
   logic. </li>
   <li> The transformation needs to be able to handle this
   basic form of ladder logic, and likely should be open
   to extensions. </li>
  </ul>


  \todo Input format
  <ul>
   <li> Is there some standard file format to express
   ladder logic? </li>
  </ul>


  \todo Output format
  <ul>
   <li> What types of boolean connectives are used? </li>
  </ul>


  \todo Update namespaces : DONE

*/

/*!
  \namespace OKlib::Satisfiability::Transformers::LadderLogic
  \brief Components for transforming ladder logic into propositional logic

  Alias "LLo".
*/

namespace OKlib {
  namespace Satisfiability {
    namespace Transformers {
      namespace LadderLogic {
      }
      namespace LLo = LadderLogic;
    }
  }
}

