// Oliver Kullmann, 6.12.2009 (Swansea)
/* Copyright 2009, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Interfaces/InputOutput/plans/Colouring.hpp
  \brief Plans regarding translating colouring problems to SAT problems


  \todo Improve design
  <ul>
   <li> In Interfaces/InputOutput/Colouring.hpp we have the two class templates
   InputOutput::HypergraphTwoColouring and
   InputOutput::GeneralisedHypergraphColouring. </li>
   <li> They follow different design approaches. </li>
   <li> Reflecting on the experiences made, possibly we should unify the
   designs. </li>
  </ul>


  \todo Improve implementations
  <ul>
   <li> Transfer the todos from Interfaces/InputOutput/Colouring.hpp here.
   </li>
  </ul>


  \todo Generic translation
  <ul>
   <li> We have gcol2sat_ohg2nbfclud in
   ComputerAlgebra/Hypergraphs/Lisp/Colouring.mac, and then the various
   translations in
   ComputerAlgebra/Satisfiability/Lisp/Generators/NonBooleanTranslations.mac.
   </li>
   <li> This functionality needs to be provided. </li>
  </ul>

*/
