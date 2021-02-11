// Oliver Kullmann, 11.2.2021 (Swansea)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Programming/InputOutput/Matching.cpp
  \brief Application for checking whether one file matches a pattern given by regular expressions

  \todo Which regular expressions?
   - Is awk-style really best?
   - It seems the default ECMAScript is better; see
     https://en.cppreference.com/w/cpp/regex/ecmascript
     for documentation.
   - It seems more powerful, and the depth-first search is likely
     more intuitive (though we might not need this here).
   - However, MatchFiles is used with the old testsystem in many places.
   - So we need either to construct another program, or use ECMAScript only
     with an option.
   - Perhaps best to create another program, called "Matching.cpp".

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

#include <ProgramOptions/Environment.hpp>

namespace {

  const Environment::ProgramInfo proginfo{
        "0.0.1",
        "11.2.2021",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Programming/InputOutput/Matching.cpp",
        "GPL v3"};

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

}
