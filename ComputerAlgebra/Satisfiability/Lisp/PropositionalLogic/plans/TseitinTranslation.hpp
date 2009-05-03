// Oliver Kullmann, 27.3.2008 (Swansea)
/* Copyright 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/PropositionalLogic/plans/TseitinTranslation.hpp
  \brief Plans for Maxima-components translations from propositional logic to CNF/DNF via the Tseitin translations (and variations)


  \todo Connections to other modules
  <ul>
   <li> Compare with
   Satisfiability/Transformers/Generators/plans/TseitinTransformation.hpp.
   </li>
   <li> See "Translation of boolean circuits into boolean CNF-clause-sets" in
   ComputerAlgebra/Satisfiability/Lisp/FiniteFunctions/plans/StraightLinePrograms.hpp.
   </li>
   <li> Likely most developments should take place in module
   ComputerAlgebra/Satisfiability/Lisp/FiniteFunctions, while in this module
   only special considerations regarding formulas (not general circuits)
   and applications of the general methods take place. </li>
  </ul>


  \todo Translations to CNF/DNF
  <ul>
   <li> Write direct translation (without new variables). </li>
   <li> Complete the Tseitin translation. </li>
  </ul>


  \todo Reorganisation
  <ul>
   <li> Move respective code in
   OKlib/ComputerAlgebra/Satisfiability/Lisp/PropositionalLogic/Formulas.mac
   (see also the tests) to PropositionalLogic/TseitinTranslation.mac. </li>
  </ul>


  \todo Optimisation
  <ul>
   <li> Implement optimisation by sharing of subexpressions. </li>
   <li> But likely this has its proper home in
   ComputerAlgebra/Satisfiability/Lisp/FiniteFunctions/plans/StraightLinePrograms.hpp?
   </li>
  </ul>

*/

