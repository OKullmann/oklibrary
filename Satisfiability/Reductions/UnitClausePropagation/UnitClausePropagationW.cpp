// Oliver Kullmann, 26.12.2009 (Swansea)
/* Copyright 2009, 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKlib/Satisfiability/Reductions/UnitClausePropagation/UnitClausePropagationW.cpp
  \brief Application for performing UCP on Dimacs input, based on watched literals

  \todo Write application tests : DONE
*/

#include <iostream>

#include <OKlib/Satisfiability/ProblemInstances/Literals/TrivialLiterals.hpp>
#include <OKlib/Satisfiability/Interfaces/InputOutput/Dimacs.hpp>
#include <OKlib/Satisfiability/Interfaces/InputOutput/ClauseSetAdaptors.hpp>

#include <OKlib/Satisfiability/ProblemInstances/Clauses/WatchedLiterals.hpp>
#include <OKlib/Satisfiability/Assignments/TotalAssignments/AssignmentsWithBuffer.hpp>
#include <OKlib/Satisfiability/Reductions/UnitClausePropagation/ClsAdaptorUCP.hpp>

#ifndef LIT_TYPE
# define LIT_TYPE std::int32_t
#endif

int main() {
  typedef LIT_TYPE literal_type;
  using namespace OKlib::Satisfiability;
  typedef Reductions::UnitClausePropagation::CLSAdaptorUcpW<
    ProblemInstances::Clauses::WatchedLiterals_mono<literal_type>,
    Assignments::TotalAssignments::BAssignmentWithQueue<literal_type> >
   ucp_type;

  ucp_type U;
  typedef OKlib::InputOutput::StandardDIMACSInput<ucp_type, OKlib::InputOutput::LiteralReadingStrict, literal_type> dimacs_in_t;
  dimacs_in_t(std::cin, U);
  U.perform_ucp();
  typedef OKlib::InputOutput::CLSAdaptorDIMACSOutput<literal_type> dimacs_out_t;
  dimacs_out_t out(std::cout);
  U.output(out);
}
