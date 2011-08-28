// Oliver Kullmann, 15.12.2010 (Swansea)
/* Copyright 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Interfaces/InputOutput/ExtendedDimacsFullStatistics.cpp
  \brief Application to compute statistics for extended Dimacs format

  Reads from standard input, and writes to standard output (and standard
  error). Extended Dimacs format allows identifiers as variable names.

  <ul>
   <li> An arbitrary number of parameters is allowed. </li>
   <li> Each must be one of the strings ed, sd, az, nz, nifz. </li>
   <li> ed means extended Dimacs format which allows identifiers as variable
   names. That is, variables are considered just as names, and are implicitly
   numbered (consecutively), and so automatically n=nmi holds. </li>
   <li> sd means to use strict Dimacs format, where variables now must be
   natural numbers (and here we have only n <= nmi). </li>
   <li> The parameters az, nz, nifz affect the handling of clause-lengths with
   zero occurence counts:
    <ul>
     <li> az for full output ("all zeros", showing all clause-lengths from 0
     to n) </li>
     <li> nz for no zeros (showing only the non-zero clause-lengths) </li>
     <li> nifz for no_initfinal_zeros (skipping initial and final zeros). </li>
    </ul
   <li> The default is ed+nz. </li>
   <li> As usual, each parameter triggers an action, namely the corresponding
   setting of its flag. </li>
  </ul>


  \todo Sharing with InputOutput/ExtendedDimacsStatistics.cpp
  <ul>
   <li> The basic input options are shared with
   Interfaces/InputOutput/ExtendedDimacsStatistics.cpp. </li>
   <li> Once we have (re-)established a system for handling command-line
   parameters, this sharing should be made explicit, via some commonly used
   components. </li>
  </ul>

*/

#include <iostream>
#include <string>

#include <OKlib/Satisfiability/Interfaces/InputOutput/Dimacs.hpp>
#include <OKlib/Satisfiability/Interfaces/InputOutput/ClauseSetAdaptors.hpp>

namespace {

  enum { errcode_parameter = 1 };
  const std::string program = "ExtendedDimacsFullStatistics";
  const std::string err = "ERROR[" + program + "]: ";

  const std::string version = "0.2.2";

  const std::string pnz = "nz"; // no_zeros
  const std::string pnifz = "nifz"; //no_initfinal_zeros
  const std::string paz = "az"; // full_output
  OKlib::InputOutput::output_options zeros_option = OKlib::InputOutput::no_zeros;

  // the following should coincide with
  // Interfaces/InputOutput/ExtendedDimacsStatistics.cpp
  const std::string dime = "ed"; // extended Dimacs
  const std::string dims = "sd"; // strict Dimacs
  bool extended_dimacs = true;

}

int main(const int argc, const char* const argv[]) {
  typedef OKlib::InputOutput::CLSAdaptorFullStatistics<> CLSAdaptor;
  CLSAdaptor output;
  for (int i = 1; i < argc; ++i)
    if (argv[i] == dime) extended_dimacs = true;
    else if (argv[i] == dims) extended_dimacs = false;
    else if (argv[i] == pnz) zeros_option = OKlib::InputOutput::no_zeros;
    else if (argv[i] == pnifz) zeros_option = OKlib::InputOutput::no_initfinal_zeros;
    else if (argv[i] == paz) zeros_option = OKlib::InputOutput::full_output;
    else {
      std::cerr << err << "Parameters must be one of\n \"" <<
        dime << "\", \"" << dims << "\", \"" <<
        pnz << "\", \"" << pnifz << "\", \"" << paz <<
        "\", but we have \"" << argv[i] << "\" at position " << i << ".\n";
    return errcode_parameter;
  }

  output.stat.option = zeros_option;
  if (extended_dimacs)
    OKlib::InputOutput::StandardDIMACSInput<CLSAdaptor, OKlib::InputOutput::LiteralReadingExtended>(std::cin, output);
  else
    OKlib::InputOutput::StandardDIMACSInput<CLSAdaptor, OKlib::InputOutput::LiteralReadingStrict>(std::cin, output);
  std::cout << output.stat;
}
