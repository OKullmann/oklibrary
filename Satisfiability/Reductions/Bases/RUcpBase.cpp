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
   <li> Instead of the boost-classes for random numbers the C++0x components
   should be used, once we move away from gcc-4.1.2. </li>
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

  enum { errcode_parameter = 1, errcode_parameter_value = 2 };
        
  const std::string program = "RUcpBase";
  const std::string err = "ERROR[" + program + "]: ";

  const std::string version = "0.0.4";

  inline int convert_seed(const char* const arg) {
    int seed;
    try { seed = boost::lexical_cast<int>(arg); }
    catch (boost::bad_lexical_cast&) { return 0; }
    if (seed < 0) return 0;
    else return seed;
  }

  typedef boost::mt19937 base_generator_type;
  base_generator_type base_rand_gen;
  inline void set_random(const int seed) {
    assert(seed >= 1);
    base_rand_gen.seed(seed);
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
  set_random(seed);

  typedef OKlib::Literals::Literals_int literal_type;
  typedef std::vector<literal_type> clause_type;
  typedef std::vector<clause_type> clause_set_type;
  typedef OKlib::InputOutput::RawDimacsCLSAdaptor<literal_type, clause_set_type> InputClsadaptor;
  typedef InputClsadaptor::int_type int_type;
  typedef InputClsadaptor::string_type string_type;

  typedef OKlib::Satisfiability::Reductions::UnitClausePropagation::CLSAdaptorUcpW<
      OKlib::Satisfiability::ProblemInstances::Clauses::WatchedLiterals_mono<literal_type>,
      OKlib::Satisfiability::Assignments::TotalAssignments::BAssignmentWithQueue<literal_type> >
    Ucp;

  typedef OKlib::InputOutput::ListTransfer<Ucp> TransferClsadaptor;


  InputClsadaptor F;
  OKlib::InputOutput::StandardDIMACSInput<InputClsadaptor>(std::cin, F);
  {
   typedef boost::uniform_int<> uniform_distribution_type;
   uniform_distribution_type uniform_distribution(0,std::numeric_limits<int>::max()); // is this correct???
   typedef boost::variate_generator<base_generator_type&, uniform_distribution_type> generator_type;
   generator_type rand_gen(base_rand_gen, uniform_distribution);
   typedef boost::random_number_generator<generator_type> RandomNumberGenerator;
   RandomNumberGenerator rg(rand_gen);
   std::random_shuffle(F.clause_set.begin(), F.clause_set.end(), rg);
  }
  
  clause_set_type F_removed;
  // XXX

  {
   typedef OKlib::InputOutput::CLSAdaptorDIMACSOutput<int_type, string_type> OutputClsadaptor;
   OutputClsadaptor out(std::cout);
   std::stringstream comment;
   comment << "r_1 base for seed = " << seed;
   OKlib::InputOutput::ListTransfer<OutputClsadaptor>(F.clause_set, out, comment.str());
  }

}

 
