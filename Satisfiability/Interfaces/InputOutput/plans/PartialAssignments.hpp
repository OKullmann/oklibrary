// Oliver Kullmann, 1.5.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Interfaces/InputOutput/plans/PartialAssignments.hpp
  \brief Plans for input and output of partial assignments


  \todo Exceptions
  <ul>
   <li> We should have a standard way of declaring exception types. </li>
  </ul>


  \todo Improvements of InputOutput::ReadPass
  <ul>
   <li> Perhaps in maxvar() we should use std::max_element together
   with an iterator-adaptor. </li>
   <li> Perhaps one could also provide a general literal2var iterator
   adaptor. </li>
   <li> Alternatively, it would be better to directly compute max_var
   in fill_litset; perhaps this is possible with C++0x? </li>
  </ul>


  \todo Write unit-tests

*/
