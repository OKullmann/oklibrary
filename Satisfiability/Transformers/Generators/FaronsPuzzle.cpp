// Oliver Kullmann, 6.10.2002 (Swansea)
/* Copyright 2002 - 2007, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Transformers/Generators/FaronsPuzzle.cpp
  \brief Application for coding "Enigma 1277" as CNF.
  \deprecated Needs a complete update.
*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <exception>

#include <boost/lexical_cast.hpp>

#include <OKlib/General/ErrorHandling.hpp>

#include <OKlib/Satisfiability/Transformers/Generators/FaronsPuzzle.hpp>

int main(const int argc, const char* const argv[]) {

  if (argc <= 2) {
    std::cerr << "Two arguments are needed: The dimension and the score.\n";
    return 1;
  }
  try {
    using namespace FaronsPuzzle;
    const unsigned int d = boost::lexical_cast<unsigned int>(argv[1]);
    const unsigned int k = boost::lexical_cast<unsigned int>(argv[2]);
    const Versions version = (argc >= 4) ? static_cast<Versions>(boost::lexical_cast<int>(argv[3])) : basic;

    switch (version) {
    case basic :
      Test_score(d, k, std::cout)();
      break;
    case simple_symmetry_breaking :
      Test_score_simple_symmetry_breaking(d, k, std::cout)();
      break;
    case extended_symmetry_breaking :
      Test_score_extended_symmetry_breaking(d, k, std::cout)();
      break;
    }
  }
  catch (const std::exception& e) {
    std::cerr << ErrorHandling::Error2string(e);
  }
}

