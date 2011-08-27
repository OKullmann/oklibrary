// Oliver Kullmann, 10.11.2007 (Swansea)
/* Copyright 2007, 2009, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Interfaces/InputOutput/ExtendedDimacsStatistics.cpp
  \brief Application to compute the basic statistics for extended Dimacs format

  Reads from standard input, and writes to standard output (and standard
  error). Extended Dimacs format allows identifiers as variable names.

*/

#include <iostream>
#include <string>

#include <OKlib/Satisfiability/Interfaces/InputOutput/Dimacs.hpp>
#include <OKlib/Satisfiability/Interfaces/InputOutput/ClauseSetAdaptors.hpp>

namespace {

  enum {
    errcode_parameter = 1,
    errcode_parameter_value = 2
  };

  const std::string program = "ExtendedDimacsStatistics";
  const std::string err = "ERROR[" + program + "]: ";

  const std::string version = "0.1.5";

  const std::string pv1 = "use-p";
  const std::string pv2 = "ignore-p";

}

int main(const int argc, const char* const argv[]) {
  if (argc >= 3) {
    std::cerr << err << "At most one parameter is allowed, either the string\n"
     " \"use-p\" (same as without parameter) or \"ignore-p\".\n";
    return errcode_parameter;
  }
  const std::string parameter = (argc == 1) ? pv1 : argv[1];
  if (parameter != pv1 and parameter != pv2) {
    std::cerr << err << "The parameter must be either the string\n \""
     << pv1 << "\" or \"" << pv2 << "\", but it is \"" << argv[1] << "\".\n";
    return errcode_parameter_value;
  }
  const bool use_p = (parameter == pv1) ? true : false ;
  if (use_p) {
    typedef OKlib::InputOutput::CLSAdaptorStatistics<> CLSAdaptor;
    CLSAdaptor output;
    OKlib::InputOutput::StandardDIMACSInput<CLSAdaptor, OKlib::InputOutput::LiteralReadingExtended>(std::cin, output);
    std::cout << output.stat << "\n";
  }
  else {
    typedef OKlib::InputOutput::CLSAdaptorPreciseStatistics<> CLSAdaptor;
    CLSAdaptor output;
    OKlib::InputOutput::StandardDIMACSInput<CLSAdaptor, OKlib::InputOutput::LiteralReadingExtended>(std::cin, output, false);
    std::cout << output.stat << "\n";
  }
}

