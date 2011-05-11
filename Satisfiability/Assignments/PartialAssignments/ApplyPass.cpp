// Oliver Kullmann, 11.5.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKlib/Satisfiability/Assignments/PartialAssignments/ApplyPass.cpp
  \brief Application for applying partial assignments

*/

#include <iostream>
#include <string>

#include <OKlib/Satisfiability/Assignments/PartialAssignments/Boolean.hpp>
#include <OKlib/Satisfiability/Interfaces/InputOutput/PartialAssignments.hpp>

namespace {

  enum { errcode_parameter = 1 };

  const std::string program = "ApplyPass";
  const std::string err = "ERROR[" + program + "]: ";

  const std::string version = "0.0.1";

}

int main(const int argc, const char* const argv[]) {
  if (argc != 2) {
    std::cerr << err << "Exactly one argument is needed, the filename for the partial assignment.\n";
    return errcode_parameter;
  }

}
