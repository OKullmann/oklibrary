// Oliver Kullmann, 19.8.2007 (Swansea)
/* Copyright 2007, 2009, 2021 Oliver Kullmann
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

  \todo Update to new standard
   - DONE At least first, use the free-standing Makefile.
   - DONE Use <regex> from the C++ library.
   - Use Environment.hpp.

  \todo Which regular expressions?
   - Is awk-style really best?
   - It seems the default ECMAScript is better; see
     https://en.cppreference.com/w/cpp/regex/ecmascript
     for documentation.
   - It seems more powerful, and the depth-first search is likely
     more intuitive (though we might not need this here).

  \todo More styles for matching
   - The pattern-file P should also allow for line-wise matching.
   - Then it is an error if the numbers of lines are different.
   - In case of error, output the line-number and the two differing lines.
   - This is regulated by an optional third parameter (the option for the
     matching-style).
   - Always going for a full match (line per line, or for the full file),
     via std::regex_match (returning a boolean).
   - The only flat for regex_match possibly of interest might be
     std::regex_constants::match_any (but not really clear what that means).
     It seems that without that flag, the match must be unique -- perhaps
     this is actually what we need?
   - For the construction of the regular expression, std::regex::ECMAScript
     together with std::regex::multiline seems appropriate (only the latter
     needs to be given for the constructor).

  \todo More/better output
   - The regular expressions themselves could be faulty, and the
     corresponding error should be output.
   - In case of a matching-error, a precise output is needed (including
     spaces). Obtaining more information on the error is likely not possible.

*/

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <regex>

#include <cstdlib>

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

  const std::regex Pr(Ps.str(), std::regex_constants::awk);
  if (std::regex_match(Fs.str(), Pr))
    return 0;
  else
    return return_no_match;
  
}

