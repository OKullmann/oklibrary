// Oliver Kullmann, 31.5.2004 (Swansea)
/* Copyright 2004 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <iostream>

#include <OKlib/General/ErrorHandling.hpp>

#include <OKlib/Satisfiability/Transformers/Generators/VanderWaerden.hpp>

int main(const int argc, const char* const argv[]) {
    if (argc <= 2) {
    std::cerr << "Two arguments are needed: The size of the arithmetic progression, and the number of elements.\n";
    return 1;
  }
  try {
    typedef unsigned int Index;
    const Index m = boost::lexical_cast<unsigned int>(argv[1]);
    const Index n = boost::lexical_cast<unsigned int>(argv[2]);
    VanderWaerden::VanderWaerden_TwoParts_1 vdW(m,n, std::cout);
    vdW();
  }
  catch (const std::exception& e) {
    std::cerr << ErrorHandling::Error2string(e);
  }

}
