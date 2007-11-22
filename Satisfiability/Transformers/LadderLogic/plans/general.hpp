// Oliver Kullmann, 16.11.2007 (Swansea)
/* Copyright 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Transformers/LadderLogic/plans/general.hpp
  \brief General plans for the module on transforming ladder logic in propositional logic


  \todo Literature overview on boolean translations
  <ul>
   <li> Hollinghouse's dissertation. </li>
  </ul>


  \todo Ladder logic standard
  <ul>
   <li> There are several forms of ladder logic. </li>
   <li> And there is the standard IEC 61131-3 (now in second edition). </li>
   <li> We need to decide on a solid basic form of ladder
   logic. </li>
   <li> The transformation needs to be able to handle this
   basic form of ladder logic, and likely should be open
   to extensions. </li>
   <li> Likely this module should move to Satisfiability/Interfaces,
   and it just translates ladder logic into propositional logic. </li>
  </ul>


  \todo Input format
  <ul>
   <li> Is there some standard file format to express
   ladder logic? </li>
  </ul>


  \todo Output format
  <ul>
   <li> What types of boolean connectives are used? </li>
   <li> See Interfaces/InputOutput/plans/general.hpp. </li>
  </ul>


  \todo Iteration
  <ul>
   <li> We need the ability to run through a "ladder device"
   ("ladder diagram" ?) k times. </li>
   <li> Or perhaps this should happen at the level of the
   boolean functions? </li>
  </ul>


  \todo Properties
  <ul>
   <li> We need to show properties of ladder logics. </li>
   <li> Either by fixing an iteration depth, or by an induction
   proof (encoded propositionally). </li>
   <li> For the induction step strengthenings need to be considered
   ("invariants"). </li>
   <li> Again, perhaps this is best expressed at the level of
   boolean functions? </li>
   <li> So the main gist is moved to two modules, on boolean functions
   and on iterating boolean functions?! </li>
  </ul>

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

