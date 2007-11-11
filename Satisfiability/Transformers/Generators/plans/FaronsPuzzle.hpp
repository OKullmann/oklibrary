// Oliver Kullmann, 10.11.2007 (Swansea)
/* Copyright 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Transformers/Generators/plans/FaronsPuzzle.hpp
  \brief Plans for the generator on "Enigma1277"


  \bug When using symmetry breaking, the number of variables and clauses is not correctly computed.
  <ul>
   <li> The "comment" member functions needs to be redefined. </li>
   <li> n and c needs to be computed differently. </li>
  </ul>


  \todo Comments in output
  <ul>
   <li> The nature of the symmetry-breaking clauses should be explained. </li>
  </ul>


  \todo Create doxygen-documentation.


  \todo Use an CLSAdaptor for output
  <ul>
   <li> See Interfaces/InputOutput/ClauseSetAdaptors.hpp. </li>
   <li> Supported for the extended Dimacs-format needs to be added. </li>
  </ul>


  \todo Write docus
  <ul>
   <li> Link to the CS-342-script. </li>
   <li> Link to module Applications/Configurations/MaxComponents. </li>
  </ul>


  \todo Test.cpp
  <ul>
   <li> Use Messages. </li>
   <li> Use ProgramOptions. </li>
   <li> DONE This is just "FaronsPuzzle", and thus the
     commented code should be removed, and the file renamed
     to the application FaronsPuzzle.cpp. </li>
  </ul>


  \todo %Tools
  <ul>
   <li> We need a little programs which shows the black-white pattern
   found by a solution (the extracted B-variables, shown just as a
   text-{0,1}-matrix of dimension d). Especially for strict Dimacs
   format. </li>
  </ul>


  \todo Symmetries
  <ul>
   <li> The basic symmetry breaking should be generalised, using arbitrary
   fields, so that one can experiment whether these placement have an
   influence. </li>
   <li> Are there other symmetries exploitable? </li>
  </ul>


  \todo Alternative translations
  <ul>
   <li> See Configurations/MaxComponents/plans/general.hpp,
   "Alternative formulations". </li>
  </ul>


  \todo Generalisations
  <ul>
   <li> See Configurations/MaxComponents/plans/general.hpp,
   "Generalisation for arbitrary graphs". </li>
  </ul>

*/

