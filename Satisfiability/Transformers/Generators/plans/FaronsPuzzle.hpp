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


  \todo Test.cpp
  <ul>
   <li> This is just "FaronsPuzzle", and thus the
     commented code should be removed, and the file renamed
     to the application FaronsPuzzle.cpp. </li>
  </ul>


  \todo Write docus.


*/

