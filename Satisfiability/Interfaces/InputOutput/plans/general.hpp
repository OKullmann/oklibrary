// Matthew Henderson, 25.1.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Interfaces/InputOutput/plans/general.hpp
  \brief Plans for the module on SAT-related input/output.


  \todo InputOutput/ExtendedToStrictDimacs.cpp
  <ul>
   <li> Really handle extended Dimacs format, using a policy for
   OKlib::InputOutput::StandardDIMACSInput.
   <ul>
    <li> But we should have an option for adding to the comment-section
    the map index -> original name. </li>
    <li> Should we have a command-line option to turn on extended Dimacs
    format? Seems useless: turn it always on (this can handle also standard
    Dimacs input). DONE </li>
   </ul>
   </li>
   <li> Complete doxygen-documentation. </li>
  </ul>


  \todo Rewrite code for the new namespaces.


  \todo Update:
  Get an overview on the current state of the module, create new
  plans-files for the sub-modules, move the todo's to there, update
  the doxygen-documentation.
  <ul>
   <li> InputOutput/ClauseSetAdaptors.hpp : </li>
   <li> InputOutput/Exceptions.hpp : </li>
   <li> InputOutput/ExtendedToStrictDimacs.cpp : DONE </li>
   <li> InputOutput/Dimacs.hpp : DONE </li>
  </ul>


  \todo See Transitional/Satisfiability/ProofSystems/DPv/Input_output.hpp


  \todo Move to the new test system.


  \todo Complete the tests.


  \todo Write docus.


  \todo Write demos.


  \todo Complete the functionality.


  \todo Move InputOutput/ExtendedToStrictDimacs.cpp to sub-directory
   apps, once buildsystem is ready.


  \todo Input and output of partial assignments.

*/

/*!
  \namespace OKlib::Satisfiability::Interfaces::InputOutput
  \brief Components for SAT-related input/output

  Alias "InO".
*/

namespace OKlib {
  namespace Satisfiability {
    namespace Interfaces {
      namespace InputOutput {
      }
      namespace InO = InputOutput;
    }
  }
}

