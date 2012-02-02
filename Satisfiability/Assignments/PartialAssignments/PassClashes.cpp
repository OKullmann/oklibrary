// Matthew Gwynne, 26.1.2012 (Swansea)
/* Copyright 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKlib/Satisfiability/Assignments/PartialAssignments/PassClashes.cpp
  \brief Application for checking whether two partial assignments clash

    PassClashes phi1 phi2

  Takes two parameters phi1 and phi2, files with partial assignments,
  and returns with exit code 0 if the two clash in some variable and
  false otherwise. Error codes for unexpected behaviour start at error
  code 2.

*/

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cassert>
#include <iterator>
#include <functional>

#include<boost/range/algorithm/set_algorithm.hpp>
#include<boost/range/algorithm/transform.hpp>

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
  typedef readpass_t::litset_t litset_t;

  const std::string program = "PassClashes";
  const std::string err = "ERROR[" + program + "]: ";

  const std::string version = "0.1";

}

int main(const int argc, const char* const argv[]) {
  if (argc != 3) {
    std::cerr << err << "Two arguments are needed,\n"
      " the two files with the partial assignments\n";
    return errcode_parameter;
  }

  const std::string filename1 = argv[1];
  std::ifstream file_pa1(filename1.c_str());
  if (not file_pa1) {
    std::cerr << err << "Can not open file \"" << filename1 << "\"\n"
      " (the file with the partial assignment).\n";
    return errcode_ifile;
  }

  const std::string filename2 = argv[2];
  std::ifstream file_pa2(filename2.c_str());
  if (not file_pa2) {
    std::cerr << err << "Can not open file \"" << filename2 << "\"\n"
      " (the file with the partial assignment).\n";
    return errcode_ifile;
  }

  readpass_t rpa1(file_pa1);
  file_pa1.close();
  readpass_t rpa2(file_pa2);
  file_pa2.close();


  typedef std::insert_iterator< litset_t > litset_insertor_t;

  litset_t rpa2_litset_comp;
  litset_insertor_t rpa2_litset_comp_insertor(rpa2_litset_comp, rpa2_litset_comp.begin());
  boost::transform(rpa2.litset, rpa2_litset_comp_insertor, std::negate<literal_type>());

  litset_t lit_inter;
  litset_insertor_t lit_inter_insertor(lit_inter, lit_inter.begin());
  boost::set_intersection(rpa1.litset, rpa2_litset_comp, lit_inter_insertor);

  if (boost::empty(lit_inter))
    exit(1);
  else
    exit(0);

}
