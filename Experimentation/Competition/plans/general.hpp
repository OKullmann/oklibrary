// Oliver Kullmann, 2.3.2006 (Swansea)
/* Copyright 2006 - 2007, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Competition/plans/general.hpp
  \brief Plans for the module on evaluations of SAT competitions (in general, including "competitions" in articles)

  Especially we have code to evaluate according to the SAT-2005 rules.


  \todo Update namespaces
  <ul>
   <li> Introduce new namespaces and namespace-aliases according
   to the new standard. </li>
   <li> Update all files. </li>
  </ul>


  \todo Update
  <ul>
   <li> doxygen documentation for all files </li>
   <li> new plans files, transferring todos </li>
   <li> Use anonymous namespaces in implementation files. </li>
   <li> General updates of code according to our coding standard. </li>
  </ul>


  \todo Concepts
  <ul>
   <li> Move the three files Concepts/ResultElement* to subdirectory
   Competition/concepts, once the build system can compile and run
   the tests. </li>
  </ul>


  \todo SET ("SAT Evaluation Toolkit")
  <ul>
   <li> Implement at R level the SAT-2005 competition as specified in this
   module. </li>
   <li> Implement at R level the methods for evaluating R competitions from
   [Pulina, Empirical Evaluation of Scoring Methods, 2006]. </li>
   <li> DONE (moved to Experimentation/Competition/TwoSolverComparison)
   Implement at R level the methods from Mladen Nikolic 2010; see
   Competition/TwoSolverComparison/plans/TwoSolverComparison.hpp. </li>
  </ul>


  \todo New tests
  Transfer to the new test system


  \todo Messages
  Use Messages.

*/

/*!
  \namespace OKlib::SATCompetition
  \brief Tools for evaluating SAT competition (under various circumstances)
*/

namespace OKlib {
  namespace SATCompetition {
  }
}

