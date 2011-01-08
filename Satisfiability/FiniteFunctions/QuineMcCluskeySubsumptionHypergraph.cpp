// Matthew Gwynne, 5.10.2010 (Swansea)
/* Copyright 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/FiniteFunctions/QuineMcCluskeySubsumptionHypergraph.cpp
  \brief Application for computing the subsumption hypergraph of the prime clauses of a full clause-set

  One parameter is needed, the file containing the clause-set in DIMACS format.
  The result is printed to standard output (a hypergraph in DIMACS format).

  \todo Fully specify the hypergraph that is given.

  \todo Provide maxima specification.
  
*/

#include <fstream>
#include <iostream>
#include <string>

#include <OKlib/Satisfiability/Interfaces/InputOutput/Dimacs.hpp>
#include <OKlib/Satisfiability/Interfaces/InputOutput/ClauseSetAdaptors.hpp>
#include <OKlib/Satisfiability/FiniteFunctions/QuineMcCluskey.hpp>
#include <OKlib/Structures/Sets/SetAlgorithms/SubsumptionHypergraph.hpp>

namespace {
  
#ifdef NUMBER_VARIABLES
  const int num_vars = NUMBER_VARIABLES;
#else
  const int num_vars = 4;
#endif

  enum {
    error_parameters = 1,
    error_openfile = 2
  };

  const std::string version = "0.1.10";

}

int main(const int argc, const char* const argv[]) {

  if (argc < 2 || argc > 3) {
    std::cerr << "ERROR[QuineMcCluskey]: Either exactly one input is required,\n"
      "the name of the file with the clause-set in DIMACS-format, or\n"
      "exactly two inputs, the name of the file and the name of the file to\n"
      "output the intermediate prime computation.\n"
      "However, the actual number of input parameters was " << argc-1 << ".\n";
    return error_parameters;
  }

  const std::string filename = argv[1];
  std::ifstream inputfile(filename.c_str());
  if (not inputfile) {
    std::cerr << "ERROR[QuineMcCluskey]: Failure opening file " << filename << ".\n";
    return error_openfile;
  }

  typedef OKlib::InputOutput::RawDimacsCLSAdaptor<> CLSAdaptor;
  CLSAdaptor cls_F;
  typedef OKlib::InputOutput::StandardDIMACSInput<CLSAdaptor> CLSInput;
  const CLSInput input_F(inputfile, cls_F);
  inputfile.close();

  // Compute the prime clauses
  typedef OKlib::Satisfiability::FiniteFunctions::QuineMcCluskey<num_vars>::clause_set_type clause_set_type;
  const clause_set_type prime_imp_F = OKlib::Satisfiability::FiniteFunctions::quine_mccluskey<num_vars>(cls_F.clause_set);

  // Compute the subsumption hypergraph
  typedef std::list<std::list<boost::range_difference<CLSAdaptor::clause_set_type>::type> > subsumption_hg_type;
  const subsumption_hg_type subsumption_hg = 
    OKlib::SetAlgorithms::subsumption_hypergraph(prime_imp_F, cls_F.clause_set);

  // Output
  const std::string comment("Subsumption hypergraph for the minimisation problem for " + filename);
  OKlib::InputOutput::List2DIMACSOutput(subsumption_hg,std::cout,comment.c_str());
  // Output primes if necessary
  if (argc > 2) {
      const std::string filename_primes = argv[2];
      std::ofstream outputfile(filename_primes.c_str());
      if (not outputfile) {
        std::cerr << "ERROR[QuineMcCluskey]: Failure opening file " << filename_primes << ".\n";
        return error_openfile;
      }
      OKlib::InputOutput::List2DIMACSOutput(prime_imp_F,outputfile,comment.c_str());
  }
}
