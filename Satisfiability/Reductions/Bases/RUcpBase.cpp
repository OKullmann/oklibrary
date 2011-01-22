// Oliver Kullmann, 30.12.2010 (Swansea)
/* Copyright 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Reductions/Bases/RUcpBase.cpp
  \brief Application for computing a random UCP-base of a clause-set

  For the specification see rand_rbase_cs in
  ComputerAlgebra/Satisfiability/Lisp/Reductions/RBases.mac.

  <ul>
   <li> The input is a clause-list in DIMACS format on standard input. </li>
   <li> An r_1-base is output to standard output. </li>
  </ul>

*/

#include <vector>
#include <list>
#include <string>
#include <iostream>
#include <cassert>
#include <sstream>
#include <limits>
#include <algorithm>

#include <boost/lexical_cast.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/random/random_number_generator.hpp>

#include <OKlib/Satisfiability/ProblemInstances/Literals/TrivialLiterals.hpp>
#include <OKlib/Satisfiability/Interfaces/InputOutput/ClauseSetAdaptors.hpp>
#include <OKlib/Satisfiability/Interfaces/InputOutput/Dimacs.hpp>
#include <OKlib/Satisfiability/ProblemInstances/Clauses/WatchedLiterals.hpp>
#include <OKlib/Satisfiability/Assignments/TotalAssignments/AssignmentsWithBuffer.hpp>
#include <OKlib/Satisfiability/Reductions/UnitClausePropagation/ClsAdaptorUCP.hpp>

namespace {

  const std::string program = "RUcpBase";

  const std::string version = "0.1.1";

}

int main() {

  typedef OKlib::Literals::Literals_int literal_type;
  typedef std::vector<literal_type> clause_type;
  typedef std::vector<clause_type> clause_set1_type;
  typedef OKlib::InputOutput::RawDimacsCLSAdaptor<literal_type, clause_set1_type> InputClsadaptor;
  InputClsadaptor F1;
  OKlib::InputOutput::StandardDIMACSInput<InputClsadaptor>(std::cin, F1);
  const InputClsadaptor::int_type n = F1.stat.parameter_n;

  typedef std::list<clause_type> clause_set2_type;
  clause_set2_type F2(F1.clause_set.begin(), F1.clause_set.end());
  F1.clause_set.clear();
  if (F2.size() >= 2) {
    clause_set2_type F_removed;
    typedef clause_set2_type::iterator clause_iterator;
    const clause_iterator F2end = F2.end();
    for (struct {clause_iterator Ci, next;} l = {F2.begin(), boost::next(F2.begin())}; l.Ci != F2end; l.Ci = l.next) {
      l.next = boost::next(l.Ci);
      F_removed.splice(F_removed.begin(), F2, l.Ci);
      typedef OKlib::Satisfiability::Reductions::UnitClausePropagation::CLSAdaptorUcpW<
          OKlib::Satisfiability::ProblemInstances::Clauses::WatchedLiterals_mono<literal_type>,
          OKlib::Satisfiability::Assignments::TotalAssignments::BAssignmentWithQueue<literal_type> >
        Ucp;
      Ucp U;
      OKlib::InputOutput::ListTransfer<Ucp>(F2, U, n);
      const Ucp::assignment_type f(U.assignment());
      bool removable = true;
      const clause_iterator Frend = F_removed.end();
      for (clause_iterator Di = F_removed.begin(); Di != Frend; ++Di) {
        U.set_assignments(f);
        const clause_type& D = *Di;
        typedef clause_type::const_iterator literal_iterator;
        for (literal_iterator xi = D.begin(); xi != D.end(); ++xi)
          U.push_unit_clause(-*xi);
        if (not U.perform_ucp()) { removable = false; break; }
      }
      if (not removable) F2.splice(l.next, F_removed, F_removed.begin());
    }
  }

  {
   typedef OKlib::InputOutput::CLSAdaptorDIMACSOutput<literal_type> OutputClsadaptor;
   OutputClsadaptor out(std::cout);
   OKlib::InputOutput::ListTransfer<OutputClsadaptor>(F2, out, "");
  }

}

 
