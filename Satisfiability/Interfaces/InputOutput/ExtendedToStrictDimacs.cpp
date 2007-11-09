// Oliver Kullmann, 8.11.2007 (Swansea)
/* Copyright 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Interfaces/InputOutput/ExtendedToStrictDimacs.cpp
  \brief Application to transfer extended Dimacs format to strict Dimacs format

  Reads from standard input, and writes to standard output (and standard
  error). Extended Dimacs format allows identifiers as variable names.


  \todo Really handle extended Dimacs format, using a policy for
  OKlib::InputOutput::StandardDIMACSInput.
  <ul>
   <li> Should we have a command-line option to turn on extended Dimacs
   format? </li>
   <li> Seems useless: turn it always on (this can handle also standard
   Dimacs input). </li>
   <li> But we should have an option for adding to the comment-section
   the map index -> original name. </li>
  </ul>


  \todo Complete doxygen-documentation.

*/


#include <iostream>

#include <Transitional/Satisfiability/Interfaces/InputOutput/Dimacs.hpp>
#include <Transitional/Satisfiability/Interfaces/InputOutput/ClauseSetAdaptors.hpp>

int main() {
  typedef OKlib::InputOutput::CLSAdaptorDIMACSOutput<> CLSAdaptor;
  CLSAdaptor output(std::cout);
  OKlib::InputOutput::StandardDIMACSInput<CLSAdaptor>(std::cin, output);
}

