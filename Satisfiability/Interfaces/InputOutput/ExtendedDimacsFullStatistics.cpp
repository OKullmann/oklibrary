// Oliver Kullmann, 15.12.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Interfaces/InputOutput/ExtendedDimacsFullStatistics.cpp
  \brief Application to compute statistics for extended Dimacs format

  Reads from standard input, and writes to standard output (and standard
  error). Extended Dimacs format allows identifiers as variable names.

  Optional option option:
   - "f" for full output (showing all clause-lengths from 0 to n)
   - "n" for no zeros (showing only the non-zero clause-lengths)
   - "ni" for no_initfinal_zeros (skipping initial and final zeros;
     the default).

*/

#include <iostream>
#include <string>

#include <OKlib/Satisfiability/Interfaces/InputOutput/Dimacs.hpp>
#include <OKlib/Satisfiability/Interfaces/InputOutput/ClauseSetAdaptors.hpp>

namespace {
  enum { errcode_parameter_values = 1 };
  const std::string program = "ExtendedDimacsFullStatistics";
  const std::string err = "ERROR[" + program + "]: ";

  const std::string version = "0.1";

  using namespace OKlib::InputOutput;

  inline output_options translate_option(const std::string& opt) {
    if (opt == "n") return no_zeros;
    if (opt == "ni") return no_initfinal_zeros;
    if (opt == "f") return full_output;
    return (output_options) 0;
  }
  const output_options default_option = no_initfinal_zeros;
}

int main(const int argc, const char* const argv[]) {
  typedef CLSAdaptorFullStatistics<> CLSAdaptor;
  CLSAdaptor output;
  if (argc > 1) {
    const output_options opt = translate_option(argv[1]);
    if (opt != (output_options) 0)
      output.stat.option = opt;
    else {
      std::cerr << err << "If an option is given, it must be one of \"n\", \"ni\" or \"f\".\n";
      return(errcode_parameter_values);
    }
  }
  else
    output.stat.option = default_option;
  StandardDIMACSInput<CLSAdaptor, LiteralReadingExtended>(std::cin, output);
  std::cout << output.stat;
}

