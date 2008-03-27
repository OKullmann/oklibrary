// Oliver Kullmann, 27.3.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/PropositionalLogic/plans/TseitinTranslation.hpp
  \brief Plans for Maxima-components translations from propositional logic to CNF/DNF via the Tseitin translations (and variations)


  \todo Translations to CNF/DNF
  <ul>
   <li> Compare with Transformers/Generators/plans/TseitinTransformation.hpp.
   </li>
   <li> Write direct translation (without new variables). </li>
   <li> Complete the Tseitin translation. </li>
  </ul>


  \todo Reorganisation
  <ul>
   <li> Move respective code in
   Transitional/ComputerAlgebra/Satisfiability/Lisp/PropositionalLogic/Formulas.mac
   (see also the tests) to PropositionalLogic/TseitinTranslation.mac.
  </ul>


  \todo Optimisation
  <ul>
   <li> Implement optimisation by sharing of subexpressions. </li>
  </ul>

*/

