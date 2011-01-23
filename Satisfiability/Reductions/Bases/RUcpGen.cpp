// Oliver Kullmann, 17.1.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Reductions/Bases/RUcpGen.cpp
  \brief Application for computing a Ucp-generating clause-set for a clause-set F and some given stock F0 of clauses to be included

  Input is clause-list F and optionally clause-list F0 (as filenames).
  Runs through F in the given order, adding those clauses C from F to F0 which
  don't follow from (the current) F0 by input-resolution modulo subsumption.
  Outputs the F0 obtained to standard output.

  Prerequisite is that all variables of F0 have at most the index n as given
  by the maxima variable-index in the parameter-line of the file for F.

  The Maxima-specification for empty F0 but general reduction r is
  rbase_cl(F,r) in ComputerAlgebra/Satisfiability/Lisp/Reductions/RBases.mac.

  \todo Update application tests
  <ul>
   <li> The application-tests for Bases/RUcpGen.cpp should coincide with the
   relevant parts of the unit-tests for the Maxima-function rbase_cl(F,r).
   </li>
  </ul>

*/

#include <string>
#include <iostream>
#include <vector>
#include <fstream>

#include <OKlib/Satisfiability/ProblemInstances/Literals/TrivialLiterals.hpp>
#include <OKlib/Satisfiability/Interfaces/InputOutput/ClauseSetAdaptors.hpp>
#include <OKlib/Satisfiability/Interfaces/InputOutput/Dimacs.hpp>
#include <OKlib/Satisfiability/ProblemInstances/Clauses/WatchedLiterals.hpp>
#include <OKlib/Satisfiability/Assignments/TotalAssignments/AssignmentsWithBuffer.hpp>
#include <OKlib/Satisfiability/Reductions/UnitClausePropagation/ClsAdaptorUCP.hpp>

namespace {

  enum {
    errcode_parameter1 = 1,
    errcode_parameter2 = 2,
    errcode_file1 = 3,
    errcode_file2 = 4
  };
        
  const std::string program = "RUcpGen";
  const std::string err = "ERROR[" + program + "]: ";

  const std::string version = "0.0.8";

}

int main(const int argc, const char* const argv[]) {

  if (argc == 1) {
    std::cerr << err << "At least one arguments is needed, the filename for F.\n";
    return errcode_parameter1;
  }
  if (argc > 3) {
    std::cerr << err << "At most one optional argument is allowed, the filename for F0.\n";
    return errcode_parameter2;
  }

  const std::string filename_F = argv[1];
  const std::string filename_F0 = (argc == 2) ? "" : argv[2];

  typedef OKlib::Literals::Literals_int literal_type;
  typedef std::vector<literal_type> clause_type;
  typedef std::vector<clause_type> clause_set_type;
  typedef OKlib::InputOutput::RawDimacsCLSAdaptor<literal_type, clause_set_type> InputClsadaptor;

  InputClsadaptor F_input;
  {
   std::ifstream F_inputfile(filename_F.c_str());
   if (not F_inputfile) {
     std::cerr << err << "Reading error with file \"" << filename_F << "\".\n";
     return errcode_file1;
   }
   OKlib::InputOutput::StandardDIMACSInput<InputClsadaptor>(F_inputfile, F_input);
  }
  const clause_set_type& F = F_input.clause_set;
  const InputClsadaptor::int_type n = F_input.stat.parameter_n;

  InputClsadaptor F0_input;
  if (not filename_F0.empty()) {
   std::ifstream F0_inputfile(filename_F0.c_str());
   if (not F0_inputfile) {
     std::cerr << err << "Reading error with file \"" << filename_F0 << "\".\n";
     return errcode_file2;
   }
   OKlib::InputOutput::StandardDIMACSInput<InputClsadaptor>(F0_inputfile, F0_input);
  }
  clause_set_type& F0 = F0_input.clause_set;

  F0.reserve(F0.size() + F.size());

  typedef clause_set_type::const_iterator clause_iterator;
  for (clause_iterator Ci = F.begin(); Ci != F.end(); ++Ci) {
    const clause_type& C = *Ci;
    typedef OKlib::Satisfiability::Reductions::UnitClausePropagation::CLSAdaptorUcpW<
      OKlib::Satisfiability::ProblemInstances::Clauses::WatchedLiterals_mono<literal_type>,
      OKlib::Satisfiability::Assignments::TotalAssignments::BAssignmentWithQueue<literal_type> >
      Ucp;
    Ucp U;
    OKlib::InputOutput::ListTransfer<Ucp>(F0, U, n);
    for (clause_type::const_iterator xi = C.begin(); xi != C.end(); ++xi)
      U.push_unit_clause(-*xi);
    if (not U.perform_ucp()) F0.push_back(C);
  }

  {
   typedef OKlib::InputOutput::CLSAdaptorDIMACSOutput<literal_type> OutputClsadaptor;
   OutputClsadaptor out(std::cout);
   std::string comment = "r_1-generating sequence from F = " + filename_F;
   if (filename_F0.empty()) comment += ".";
   else comment += " using all clauses from " + filename_F0 + ".";
   OKlib::InputOutput::ListTransfer<OutputClsadaptor>(F0, out, comment);
  }
 
}
