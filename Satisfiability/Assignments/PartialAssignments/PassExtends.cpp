// Matthew Gwynne, 23.1.2012 (Swansea)
/* Copyright 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKlib/Satisfiability/Assignments/PartialAssignments/PassExtends.cpp
  \brief Application for checking whether one partial assignment extends another

    PassExtends phi_sup phi_sub

  Takes two parameters phi_sup and phi_sub, files with partial assignments,
  and returns with exit code 0 if phi_sup extends phi_sub (i.e., phi_sup
  is a superset of phi_sub), and exit code 1 otherwise. Error codes for
  unexpected behaviour start at error code 2.

  Note that this application expects *valid* partial assignments without
  clashing literals.

*/

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cassert>

#include<boost/range/algorithm/set_algorithm.hpp>

#include <OKlib/Satisfiability/ProblemInstances/Literals/TrivialLiterals.hpp>
#include <OKlib/Satisfiability/Assignments/PartialAssignments/Boolean.hpp>
#include <OKlib/Satisfiability/Interfaces/InputOutput/PartialAssignments.hpp>
#include <OKlib/Satisfiability/Interfaces/InputOutput/ClauseSetAdaptors.hpp>
#include <OKlib/Satisfiability/Interfaces/InputOutput/Dimacs.hpp>

namespace {

  enum { errcode_parameter = 2, errcode_ifile = 3,
         errcode_ofile = 4 };

  typedef OKlib::Literals::Literals_int literal_type;
  typedef OKlib::Satisfiability::Assignments::PartialAssignments::BPass0<literal_type> pass_type;
  typedef OKlib::Satisfiability::Interfaces::InputOutput::ReadPass<literal_type, pass_type> readpass_t;

  const std::string program = "PassExtends";
  const std::string err = "ERROR[" + program + "]: ";

  const std::string version = "0.1";

}

int main(const int argc, const char* const argv[]) {
  if (argc != 3) {
    std::cerr << err << "Two arguments are needed,\n"
      " the two files with the partial assignments, where the first should\n"
      " extend the second.\n";
    return errcode_parameter;
  }

  const std::string filename_sup = argv[1];
  std::ifstream file_pa_sup(filename_sup.c_str());
  if (not file_pa_sup) {
    std::cerr << err << "Can not open file \"" << filename_sup << "\"\n"
      " (the file with the partial assignment).\n";
    return errcode_ifile;
  }

  const std::string filename_sub = argv[2];
  std::ifstream file_pa_sub(filename_sub.c_str());
  if (not file_pa_sub) {
    std::cerr << err << "Can not open file \"" << filename_sub << "\"\n"
      " (the file with the partial assignment).\n";
    return errcode_ifile;
  }

  readpass_t rpa_sup(file_pa_sup);
  file_pa_sup.close();
  readpass_t rpa_sub(file_pa_sub);
  file_pa_sub.close();
  if (boost::includes(rpa_sup.litset, rpa_sub.litset))
    exit(0);
  else
    exit(1);
}
