// Oliver Kullmann, 13.12.2009 (Swansea)
/* Copyright 2009, 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKlib/Satisfiability/Reductions/UnitClausePropagation/UnitClausePropagation.cpp
  \brief Application for performing UCP on Dimacs input

  The full clause-literal graph is used here. If a command-line value is used
  of value "set", then the implementation uses (std::)sets for implementing
  clauses.

  BUGS:

  1. A version with 64-bit literals is needed.
      - Using a macro, as with tawSolver.
      - Having two versions then, UnitClausePropagation, and UnitClausePropagation64.
      - dimacs_input_type needs then to have this 64-bit int-type as third parameter.

*/

#include <iostream>

#include <OKlib/Satisfiability/Interfaces/InputOutput/Dimacs.hpp>
#include <OKlib/Satisfiability/Values/StatusTypes.hpp>
#include <OKlib/Satisfiability/Reductions/UnitClausePropagation/ClsAdaptorUCP.hpp>

namespace {

  typedef OKlib::Literals::Literals_int default_Lit;
  typedef OKlib::Satisfiability::ProblemInstances::Clauses::RClausesAsVectors<default_Lit> default_Clauses;
  // The default values of template parameters should be the same as for
  // OKlib::Satisfiability::Reductions::UnitClausePropagation::CLSAdaptorUcp.

  template <class Clauses = default_Clauses>
  struct read_ucp_output {
    typedef typename Clauses::value_type literal_type;
    typedef OKlib::Satisfiability::Reductions::UnitClausePropagation::CLSAdaptorUcp<literal_type,Clauses> adaptor_type;

    read_ucp_output(std::istream& in, std::ostream& out) {
      adaptor_type A;
      typedef OKlib::InputOutput::StandardDIMACSInput<adaptor_type>  ;
      dimacs_input_type(in, A);
      typedef typename adaptor_type::int_type int_type;
      typedef std::vector<int_type> cl_t;
      typedef std::vector<cl_t> cls_t;
      cls_t F;
      using namespace OKlib::Satisfiability::Values;
      switch (A.perform_ucp()) {
      case falsified :
        F.push_back(cl_t());
        OKlib::InputOutput::List2DIMACSOutput(F,out,A.orig_comment()+A.add_comment());
        break;
      case satisfied :
        OKlib::InputOutput::List2DIMACSOutput(F,out,A.orig_comment()+A.add_comment());
        break;
      case open :
        OKlib::InputOutput::List2DIMACSOutput(A,out,A.orig_comment()+A.add_comment());
        break;
      }
    }
  };
}

int main(const int argc, const char* const argv[]) {

  const std::string arg_set = "set";
  if (argc == 1 or argv[1] != arg_set)
    read_ucp_output<>(std::cin, std::cout);
  else
    read_ucp_output<OKlib::Satisfiability::ProblemInstances::Clauses::RClausesAsSets<default_Lit> >(std::cin, std::cout);
  
}
