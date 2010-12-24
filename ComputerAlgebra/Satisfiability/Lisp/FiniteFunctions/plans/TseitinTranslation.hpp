// Oliver Kullmann, 24.12.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/FiniteFunctions/plans/TseitinTranslation.hpp
  \brief Plans regarding the Tseitin translation from boolean circuits to CNF


  \todo Translation of boolean circuits into boolean CNF-clause-sets
  <ul>
   <li> It seems there is basically only one translation to CNF known, the
   "Tseitin translation". And for every other form of representations of
   finite functions the CNF-translation seems always to work via
   first constructing a circuit, and then representing this circuit
   as CNF. </li>
   <li> See
   Satisfiability/Transformers/Generators/plans/TseitinTransformation.hpp
   for the general considerations. </li>
   <li> See
   ComputerAlgebra/Satisfiability/Lisp/PropositionalLogic/plans/TseitinTranslation.hpp
   for the special case of propositional formulas. </li>
   <li> For each gate g in a circuit C, one represents the inputs i
   by variables v_i, the output by variable o, and then represents
   the equivalence o <-> g( ... v_i ...) by a CNF. </li>
   <li> The conjunction of all these CNFs yields the whole CNF, where
   once the values for all inputs are given, all other variables have
   a unique value so that the whole CNF becomes true. </li>
   <li> This also works for non-boolean circuits and various generalised
   forms of non-boolean CNF-clause-sets. </li>
  </ul>

*/
