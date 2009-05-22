// Oliver Kullmann, 19.8.2007 (Swansea)
/* Copyright 2007, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Programming/InputOutput/MatchFiles.cpp
  \brief Application for checking whether one file matches a regular expresssion

  Two input parameters are needed, which are the names of two files P and F.
  P (the "pattern") is a regular expression (using awk-syntax), and the return
  value is 0 if the content of file F matches exactly P, and 10 otherwise.
  If only one parameter is given, then it is taken for P, and F is read from
  standard input.

  \todo Replace use of boost-regex by the facility from the standard library.

  \todo Use module ProgramOption (once available).

*/

#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <sstream>
//#include <tr1/regex>

#include <boost/regex.hpp>


namespace {
  const int return_no_match = 10;
}

int main(const int argc, const char* const argv[]) {
  if (argc != 2 and argc != 3) {
    std::cerr << "ERROR[MatchFiles]: Either one or two input parameters are required, the file-name "
      "for the pattern, and the file-name of the file to check,\nwhere the latter"
      " can be supplied by standard input.\n However the "
      "number of actual arguments was " << argc-1 << ".\n";
    return EXIT_FAILURE;
  }

  std::stringstream Ps, Fs;
  {
    const std::string P(argv[1]);
    std::ifstream Pf(P.c_str());
    if (not Pf) {
      std::cerr << "ERROR[MatchFiles]: Pattern file \"" << P << "\" is not readable.\n";
      return EXIT_FAILURE;
    }
    Ps << Pf.rdbuf();
    if (not Pf or not Ps) {
      std::cerr << "ERROR[MatchFiles] when reading pattern file \"" << P << "\".\n";
      return EXIT_FAILURE;
    }

    if (argc == 2)
      Fs << std::cin.rdbuf();
    else {
      const std::string F(argv[2]);
      std::ifstream Ff(F.c_str());
      if (not Ff) {
        std::cerr << "ERROR[MatchFiles]: Matching-file \"" << F << "\" is not readable.\n";
        return EXIT_FAILURE;
      }
      Fs << Ff.rdbuf();
      if (not Ff or not Fs) {
        std::cerr << "ERROR[MatchFiles] when reading matching-file \"" << F << "\".\n";
        return EXIT_FAILURE;
      }
    }
  }

  const boost::regex Pr(Ps.str(), boost::regex_constants::awk);
  if (boost::regex_match(Fs.str(), Pr))
    return 0;
  else
    return return_no_match;
  
}

