// Oliver Kullmann, 10.11.2007 (Swansea)
/* Copyright 2007, 2009, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Interfaces/InputOutput/ExtendedDimacsStatistics.cpp
  \brief Application to compute simple statistics for extended Dimacs format

  Reads from standard input, and writes to standard output (and standard
  error). Extended Dimacs format allows identifiers as variable names.

*/

#include <iostream>
#include <string>

#include <OKlib/Satisfiability/Interfaces/InputOutput/Dimacs.hpp>
#include <OKlib/Satisfiability/Interfaces/InputOutput/ClauseSetAdaptors.hpp>

namespace {

  const std::string program = "ExtendedDimacsStatistics";

  const std::string version = "0.1.1";

}

int main() {
  typedef OKlib::InputOutput::CLSAdaptorStatistics<> CLSAdaptor;
  CLSAdaptor output;
  OKlib::InputOutput::StandardDIMACSInput<CLSAdaptor, OKlib::InputOutput::LiteralReadingExtended>(std::cin, output);
  std::cout << output.stat << "\n";
}

