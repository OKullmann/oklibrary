// Oliver Kullmann, 19.8.2007 (Swansea)

/*!
  \file GeneralInputOutput/MatchFiles.cpp
  \brief Application for checking whether one file matches a regular expresssion

  Two input parameters are needed, which are the names of two files P and F.
  P (the "pattern") is a regular expression, and the return value
  is 0 if the content of file F matches exactly P, and 10 otherwise.
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
    std::cerr << "ERROR: Either one or two input parameters are required, the file-name "
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
      std::cerr << "ERROR: File \"" << P << "\" is not readable.\n";
      return EXIT_FAILURE;
    }
    Ps << Pf.rdbuf();
    if (not Pf or not Ps) {
      std::cerr << "ERROR when reading file \"" << P << "\".\n";
      return EXIT_FAILURE;
    }

    if (argc == 2)
      Fs << std::cin.rdbuf();
    else {
      const std::string F(argv[2]);
      std::ifstream Ff(F.c_str());
      if (not Ff) {
        std::cerr << "ERROR: File \"" << F << "\" is not readable.\n";
        return EXIT_FAILURE;
      }
      Fs << Ff.rdbuf();
      if (not Ff or not Fs) {
        std::cerr << "ERROR when reading file \"" << F << "\".\n";
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

