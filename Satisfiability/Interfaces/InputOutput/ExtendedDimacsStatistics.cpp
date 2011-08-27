// Oliver Kullmann, 10.11.2007 (Swansea)
/* Copyright 2007, 2009, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Interfaces/InputOutput/ExtendedDimacsStatistics.cpp
  \brief Application to compute the basic statistics for extended and strict Dimacs format

  Reads from standard input, and writes to standard output (and standard
  error). Extended Dimacs format allows identifiers as variable names.

  <ul>
   <li> An arbitrary number of parameters is allowed, each must be one of the
   strings up, ip, ed, sd. </li>
   <li> up means use the parameter of the p-line (also for checking). </li>
   <li> ip means to ignore these parameters: the line is skipped (thus there
   must be a line), and no checks are performed. </li>
   <li> ed means extended Dimacs format which allows identifiers as variable
   names. This means that variables are considered just as names, and are
   implicitly numbered (consecutively), and so automatically n=nmi holds. </li>
   <li> sd means to use strict Dimacs format, where variables now must be
   natural numbers (and here we have only n <= nmi). </li>
   <li> The default is up+ed. </li>
   <li> As usual, each parameter triggers an action, namely the corresponding
   setting of its flag. </li>
  </ul>

*/

#include <iostream>
#include <string>

#include <OKlib/Satisfiability/Interfaces/InputOutput/Dimacs.hpp>
#include <OKlib/Satisfiability/Interfaces/InputOutput/ClauseSetAdaptors.hpp>

namespace {

  enum { errcode_parameter = 1 };

  const std::string program = "ExtendedDimacsStatistics";
  const std::string err = "ERROR[" + program + "]: ";

  const std::string version = "0.2.1";

  const std::string pvt = "up"; // use parameter
  const std::string pvf = "ip"; // ignore parameter
  const std::string dime = "ed"; // extended Dimacs
  const std::string dims = "sd"; // strict Dimacs

  bool use_parameter = true;
  bool extended_dimacs = true;
}

int main(const int argc, const char* const argv[]) {
  for (int i = 1; i < argc; ++i)
    if (argv[i] == pvt) use_parameter = true;
    else if (argv[i] == pvf) use_parameter = false;
    else if (argv[i] == dime) extended_dimacs = true;
    else if (argv[i] == dims) extended_dimacs = false;
    else {
      std::cerr << err << "Parameters must be one of\n \""
     << pvt << "\", \"" << pvf << "\", \"" << dime << "\", \"" << dims << "\", but we have \"" << argv[i] << "\" at position " << i << ".\n";
    return errcode_parameter;
  }
  typedef OKlib::InputOutput::CLSAdaptorPreciseStatistics<> CLSAdaptor;
  CLSAdaptor output;
  if (extended_dimacs)
    OKlib::InputOutput::StandardDIMACSInput<CLSAdaptor, OKlib::InputOutput::LiteralReadingExtended>(std::cin, output, use_parameter);
  else if (not extended_dimacs)
    OKlib::InputOutput::StandardDIMACSInput<CLSAdaptor, OKlib::InputOutput::LiteralReadingStrict>(std::cin, output, use_parameter);
  std::cout << output.stat << "\n";
}

