// Matthew Gwynne, 16.7.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/Generators/plans/CardinalityConstraints.hpp
  \brief Plans for Maxima-generators (of SAT-problems) related to cardinality constraints


  \todo Provide complete specifications
  <ul>
   <li> For cardinality_totalizer_cs, cardinality_comparator_cs and
   cardinality_cs *combinatorial* specifications are needed. </li>
   <li> Then two levels of tests are needed: One which checks the
   (precise) combinatorial specification, and another one which checks
   the (general) specification as a presentation of some constraint. </li>
  </ul>


  \todo Add statistics functions
  <ul>
   <li> These statistics functions rely on precise (combinatorial)
   specifications. </li>
  </ul>
   

  \todo Docus


  \todo Add adder circuit translation
  <ul>
   <li> Also provide the translation centred around the Tseitin translation of
   the boolean adder circuits, for comparison. </li>
   <li> See Satisfiability/Transformers/Generators/plans/LinInequal.hpp for 
   a C++ implementation. </li>
  </ul>

*/

