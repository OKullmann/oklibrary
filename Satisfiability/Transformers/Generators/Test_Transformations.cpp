// Oliver Kullmann, 12.8.2004 (Swansea)
/* Copyright 2004 - 2007, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Transformers/Generators/Test_Transformations.cpp
  \brief Old test application
  \deprecated Needs to be replaced by the new test system.
*/

#include <iostream>
#include <exception>

#include <OKlib/General/ErrorHandling.hpp>

#include <OKlib/Satisfiability/Transformers/Generators/Ramsey.hpp>
#include <OKlib/Satisfiability/Transformers/Generators/Tests_Ramsey.hpp>

int main() {
  try {

    Tests_Ramsey::test_enumeration_hyperedges<Ramsey::Enumerate_hyperedges<> >();
  }
  catch (const ErrorHandling::Error& e) {
    std::cerr << ErrorHandling::Error2string(e) << "\n";
    return 1;
  }
  catch (const std::exception& e) {
    std::cerr << ErrorHandling::Error2string(e) << "\n";
    return 1;
  }
  catch (...) {
    std::cerr << "UNKNOWN EXCEPTION CAUGHT (neither derived from ErrorHandling::Error nor from std::exception)!\n";
    return 1;
  }
  std::cout << std::endl;

}
