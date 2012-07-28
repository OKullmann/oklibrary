// Oliver Kullmann, 11.5.2011 (Swansea)
/* Copyright 2011, 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKlib/Satisfiability/Assignments/PartialAssignments/ApplyPass.cpp
  \brief Application for applying partial assignments

    ApplyPass phi
  or
    ApplyPass phi Out

  Takes at least one parameter phi, a file with a partial assignment, reads a
  clause-list F from standard input, and outputs the result of applying phi to
  F, either to standard output or to file Out.

   - The old comments are kept, only before the parameter-line a comment is
     added showing phi.
   - The application uses only one pass, and does not store the clause-list.
   - Thus in case of output to standard output, it can not update the
     parameter-line, but keeps it as is.
   - And so then the number of clauses is then only an upper bound.
   - However, in case of output to file Out, then (in-place) the parameter-line
     is updated (padded with spaces to maintain the old length).
   - Variables, the order of clauses and the order of literals are kept.
*/

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cassert>

#include <OKlib/Satisfiability/ProblemInstances/Literals/TrivialLiterals.hpp>
#include <OKlib/Satisfiability/Assignments/PartialAssignments/Boolean.hpp>
#include <OKlib/Satisfiability/Interfaces/InputOutput/PartialAssignments.hpp>
#include <OKlib/Satisfiability/Interfaces/InputOutput/ClauseSetAdaptors.hpp>
#include <OKlib/Satisfiability/Interfaces/InputOutput/Dimacs.hpp>

namespace {

  enum { errcode_parameter = 1, errcode_ifile = 2,
         errcode_ofile = 3, errcode_write = 4 };

  typedef OKlib::Literals::Literals_int literal_type;
  typedef OKlib::Satisfiability::Assignments::PartialAssignments::BPass0<literal_type> pass_type;
  typedef OKlib::Satisfiability::Interfaces::InputOutput::ReadPass<literal_type, pass_type> readpass_t;

  const std::string program = "ApplyPass";
  const std::string err = "ERROR[" + program + "]: ";

  const std::string version = "0.1.1";

}

int main(const int argc, const char* const argv[]) {
  if (argc != 2 and argc != 3) {
    std::cerr << err << "Either one or two arguments are needed,\n"
      " the file with the partial assignment, and optionally the output file.\n";
    return errcode_parameter;
  }
  const bool with_file = argc == 3;

  const std::string filename = argv[1];
  std::ifstream file_pa(filename.c_str());
  if (not file_pa) {
    std::cerr << err << "Can not open file \"" <<filename << "\"\n"
      " (the file with the partial assignment).\n";
    return errcode_ifile;
  }

  if (not with_file) {
    typedef OKlib::InputOutput::CLSAdaptorDIMACSOutput<literal_type> output_t;
    typedef OKlib::Satisfiability::Assignments::PartialAssignments::ApplyPassAdaptor<literal_type, readpass_t,  output_t> applypass_t;
    typedef OKlib::InputOutput::StandardDIMACSInput<applypass_t, OKlib::InputOutput::LiteralReadingStrict, literal_type> input_t;
    readpass_t rpa(file_pa);
    file_pa.close();
    output_t out(std::cout);
    applypass_t apply(rpa, out);
    input_t in(std::cin, apply);
  }
  else {
    typedef OKlib::InputOutput::CLSAdaptorDIMACSFileOutput<literal_type> output_t;
    typedef OKlib::Satisfiability::Assignments::PartialAssignments::ApplyPassAdaptor<literal_type, readpass_t,  output_t> applypass_t;
    typedef OKlib::InputOutput::StandardDIMACSInput<applypass_t, OKlib::InputOutput::LiteralReadingStrict, literal_type> input_t;
    const std::string ofilename = argv[2];
    std::fstream f(ofilename.c_str(), std::ios::in | std::ios::out | std::ios::trunc);
    if (not f) {
      std::cerr << err << "Can not create file \"" << filename << "\".\n";
      return errcode_ofile;
    }
    readpass_t rpa(file_pa);
    file_pa.close();
    output_t out(f);
    applypass_t apply(rpa, out);
    input_t in(std::cin, apply);

    std::stringstream new_pline_;
    new_pline_ << "p cnf " << apply.new_n() << " " << apply.new_c();
    if (new_pline_.str().size() < out.pline_length())
      new_pline_ << std::string(out.pline_length() - new_pline_.str().size(), ' ');
    const std::string new_pline = new_pline_.str();
    assert(new_pline.size() == out.pline_length());
    f.seekp(out.pline_position());
    f << new_pline;
    if (not f) {
      std::cerr << err << "Write error with file \"" << ofilename << "\".\n";
      return errcode_write;
    }
  }
}
