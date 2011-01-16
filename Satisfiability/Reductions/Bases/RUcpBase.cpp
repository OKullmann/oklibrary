// Oliver Kullmann, 30.12.2010 (Swansea)
/* Copyright 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Reductions/Bases/RUcpBase.cpp
  \brief Application for computing a random UCP-base of a clause-set

  For the specification see  rand_rbase_cs in
  ComputerAlgebra/Satisfiability/Lisp/Reductions/RBases.mac.

  <ul>
   <li> The input is a clause-list in DIMACS format on standard input. </li>
   <li> An r_1-base is output to standard output. </li>
   <li> One optional argument is allowed, an integer used as seed for the
   random-number generator; the default-seed is 1. </li>
  </ul>

  \todo Provide implementation

  \todo Improve implementation
  <ul>
   <li> The algorithm should go into a (reusable) class in RUcpBase.hpp. </li>
   <li> Instead of constantly refilling the clause-set for UCP, only the
   changes needed (removing one clause, readding some clause) should be
   performed. </li>
   <li> Use messages. </li>
   <li> DONE (removed it)
   The Boost-error-message is not very informative. </li>
  </ul>

*/

#include <vector>
#include <list>
#include <string>
#include <iostream>
#include <cassert>

#include <boost/lexical_cast.hpp>

#include <OKlib/Satisfiability/ProblemInstances/Literals/TrivialLiterals.hpp>
#include <OKlib/Satisfiability/Interfaces/InputOutput/ClauseSetAdaptors.hpp>
#include <OKlib/Satisfiability/Interfaces/InputOutput/Dimacs.hpp>
#include <OKlib/Satisfiability/ProblemInstances/Clauses/WatchedLiterals.hpp>
#include <OKlib/Satisfiability/Assignments/TotalAssignments/AssignmentsWithBuffer.hpp>
#include <OKlib/Satisfiability/Reductions/UnitClausePropagation/ClsAdaptorUCP.hpp>

namespace {

  enum { errcode_parameter = 1, errcode_parameter_value = 2 };
        
  const std::string program = "RUcpBase";
  const std::string err = "ERROR[" + program + "]: ";

  const std::string version = "0.0.3";

  int convert_seed(const char* const arg) {
    int seed;
    try { seed = boost::lexical_cast<int>(arg); }
    catch (boost::bad_lexical_cast&) { return 0; }
    if (seed < 0) return 0;
    else return seed;
  }

}

int main(const int argc, const char* const argv[]) {
  if (argc > 2) {
    std::cerr << err << "At most one arguments is allowed "
      "(the seed for the random-number generator).\n";
    return errcode_parameter;
  }

  const int seed = (argc == 1) ? 1 : convert_seed(argv[1]);
  if (seed == 0) {
    std::cerr << err << "The seed \"" << argv[1] << "\" must be an integer >= 1 fitting into type int.\n";
    return(errcode_parameter_value);
  }
  assert(seed >= 1);

  typedef OKlib::Literals::Literals_int literal_type;
  typedef std::vector<literal_type> clause_type;
  typedef std::list<clause_type> clause_set_type;
  typedef OKlib::InputOutput::RawDimacsCLSAdaptor<literal_type, clause_set_type> InputClsadaptor;
  typedef InputClsadaptor::int_type int_type;
  typedef InputClsadaptor::string_type string_type;

  typedef OKlib::Satisfiability::Reductions::UnitClausePropagation::CLSAdaptorUcpW<
      OKlib::Satisfiability::ProblemInstances::Clauses::WatchedLiterals_mono<literal_type>,
      OKlib::Satisfiability::Assignments::TotalAssignments::BAssignmentWithQueue<literal_type> >
    Ucp;

  typedef OKlib::InputOutput::ListTransfer<Ucp> TransferClsadaptor;

}

 
