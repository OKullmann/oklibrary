// Oliver Kullmann, 11.5.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKlib/Satisfiability/Assignments/PartialAssignments/ApplyPass.cpp
  \brief Application for applying partial assignments

  Takes one parameter, a file with a partial assignment phi, reads a
  clause-list F from standard input, and prints to standard output the result
  of applying phi to F.

   - The old comments are kept, only before the parameter-line a comment is
     added showing phi.
   - The application uses only one pass, and does not store the clause-list.
   - Thus it can not update the parameter-line, but keeps it as is.
   - So the number of clauses is then only an upper bound.
   - Variables, the order of clauses and the order of literals are kept.
*/

#include <iostream>
#include <string>
#include <fstream>

#include <OKlib/Satisfiability/ProblemInstances/Literals/TrivialLiterals.hpp>
#include <OKlib/Satisfiability/Assignments/PartialAssignments/Boolean.hpp>
#include <OKlib/Satisfiability/Interfaces/InputOutput/PartialAssignments.hpp>
#include <OKlib/Satisfiability/Interfaces/InputOutput/ClauseSetAdaptors.hpp>
#include <OKlib/Satisfiability/Interfaces/InputOutput/Dimacs.hpp>

namespace {

  enum { errcode_parameter = 1, errcode_file = 2 };

  typedef OKlib::Literals::Literals_int literal_type;
  typedef OKlib::Satisfiability::Assignments::PartialAssignments::BPass0<literal_type> pass_type;
  typedef OKlib::Satisfiability::Interfaces::InputOutput::ReadPass<literal_type, pass_type> readpass_t;
  typedef OKlib::InputOutput::CLSAdaptorDIMACSOutput<literal_type> output_t;
  typedef OKlib::Satisfiability::Assignments::PartialAssignments::ApplyPassAdaptor<literal_type, readpass_t,  output_t> applypass_t;
  typedef OKlib::InputOutput::StandardDIMACSInput<applypass_t, OKlib::InputOutput::LiteralReadingStrict, literal_type> input_t;

  const std::string program = "ApplyPass";
  const std::string err = "ERROR[" + program + "]: ";

  const std::string version = "0.0.6";

}

int main(const int argc, const char* const argv[]) {
  if (argc != 2) {
    std::cerr << err << "Exactly one argument is needed, the filename\n"
      " for the partial assignment.\n";
    return errcode_parameter;
  }

  const std::string filename = argv[1];
  std::ifstream file_pa(filename.c_str());
  if (not file_pa) {
    std::cerr << err << "Can not open file \"" <<filename << "\"\n"
      " (the file with the partial assignment).\n";
    return errcode_file;
  }

  readpass_t rpa(file_pa);
  file_pa.close();
  output_t out(std::cout);
  applypass_t apply(rpa, out);
  input_t in(std::cin, apply);

}
