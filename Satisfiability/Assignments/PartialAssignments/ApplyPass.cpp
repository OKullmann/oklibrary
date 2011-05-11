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
#include <fstream>

#include <OKlib/Satisfiability/ProblemInstances/Literals/TrivialLiterals.hpp>
#include <OKlib/Satisfiability/Assignments/PartialAssignments/Boolean.hpp>
#include <OKlib/Satisfiability/Interfaces/InputOutput/PartialAssignments.hpp>

namespace {

  enum { errcode_parameter = 1, errcode_file = 2 };

  typedef OKlib::Literals::Literals_int literal_type;
  typedef OKlib::Satisfiability::Assignments::PartialAssignments::BPass0<literal_type> pass_type;
  typedef OKlib::Satisfiability::Interfaces::InputOutput::ReadPass<literal_type, pass_type> readpass_t;

  const std::string program = "ApplyPass";
  const std::string err = "ERROR[" + program + "]: ";

  const std::string version = "0.0.2";

}

int main(const int argc, const char* const argv[]) {
  if (argc != 2) {
    std::cerr << err << "Exactly one argument is needed, the filename for the partial assignment.\n";
    return errcode_parameter;
  }

  const std::string filename = argv[1];
  std::ifstream file_pa(filename.c_str());
  if (not file_pa) {
    std::cerr << err << "Can not open file \"" <<filename << "\" (the file with the partial assignment).\n";
    return errcode_file;
  }

  readpass_t rpa(file_pa);
  file_pa.close();

  // XXX

}
