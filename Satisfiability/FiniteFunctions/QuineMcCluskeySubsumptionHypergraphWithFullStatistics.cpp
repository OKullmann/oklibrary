// Matthew Gwynne, 11.1.2011 (Swansea)
/* Copyright 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/FiniteFunctions/QuineMcCluskeySubsumptionHypergraphWithFullStatistics.cpp
  \brief Application for computing the subsumption hypergraph of the prime clauses of a full clause-set as well as statistics.

  <ul>
   <li> One parameter is needed, the file containing the clause-set F in DIMACS
   format. </li>
   <li> The result is printed to standard output (a hypergraph in DIMACS
   format). </li>
   <li> The subsumption hypergraph is output in lexicographical order, without
   duplicate clauses. </li>
   <li> Additionally the clause-set statistics for the prime implicates and 
   the subsumption hypergraph (with duplicate clauses removed) are output to 
   basename(input_filename)_all_primes_stats and 
   basename(input_filename)_shg_stats. </li>
  </ul>

*/

#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>

#include <boost/filesystem/path.hpp>

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

  const std::string program = "QuineMcCluskeySubsumptionHypergraphWithFullStatistics";
  const std::string err = "ERROR[" + program + "]: ";

  const std::string version = "0.1.1";

}

int main(const int argc, const char* const argv[]) {

  if (argc != 2) {
    std::cerr << err << "Exactly one input is required,\n"
      " the name of the file with the clause-set in DIMACS-format.\n"
      "However, the actual number of input parameters was " << argc-1 << ".\n";
    return error_parameters;
  }

  const std::string shg_input_filepath = argv[1];
  typedef boost::filesystem::basic_path<std::string, boost::filesystem::path_traits> Path;
  const std::string shg_input_filename = Path(shg_input_filepath).filename();
  const std::string primes_output_filepath = Path(argv[1]).filename() + "_primes";
  std::ifstream inputfile(shg_input_filepath.c_str());
  if (not inputfile) {
    std::cerr << err << "Failure opening input file " << shg_input_filepath << ".\n";
    return error_openfile;
  }

  typedef OKlib::InputOutput::RawDimacsCLSAdaptor<> CLSAdaptor;
  CLSAdaptor cls_F;
  typedef OKlib::InputOutput::StandardDIMACSInput<CLSAdaptor> CLSInput;
  const CLSInput input_F(inputfile, cls_F);
  inputfile.close();

  // Compute the prime clauses:
  typedef OKlib::Satisfiability::FiniteFunctions::QuineMcCluskey<num_vars>::clause_set_type clause_set_type;
  const clause_set_type prime_imp_F = OKlib::Satisfiability::FiniteFunctions::quine_mccluskey<num_vars>(cls_F.clause_set);

  // Compute the subsumption hypergraph:
  typedef OKlib::SetAlgorithms::Subsumption_hypergraph<clause_set_type, CLSAdaptor::clause_set_type>::set_system_type subsumption_hg_type;
  subsumption_hg_type subsumption_hg = 
    OKlib::SetAlgorithms::subsumption_hypergraph(prime_imp_F, cls_F.clause_set);
  std::sort(subsumption_hg.begin(), subsumption_hg.end());
  subsumption_hg.erase(std::unique(subsumption_hg.begin(), subsumption_hg.end()), subsumption_hg.end());

  // Compute statistics and output to file
  
  typedef OKlib::InputOutput::CLSAdaptorFullStatistics<> StatsCLSAdaptor;
  typedef OKlib::InputOutput::ListTransfer<StatsCLSAdaptor > List2Statistics;
  
  StatsCLSAdaptor shg_stats;
  List2Statistics(subsumption_hg, shg_stats, "");
  const std::string shg_stats_filename = shg_input_filename + "_shg_stats";
  std::ofstream shg_stats_outputfile(shg_stats_filename.c_str());
  shg_stats_outputfile << shg_stats.stat << "\n";   
    
  if (argc > 2) {
    StatsCLSAdaptor prime_stats;
    List2Statistics(prime_imp_F, prime_stats, "");
    
    const std::string primes_stats_filename = 
      shg_input_filename + "_primes_stats";
    std::ofstream primes_stats_outputfile(primes_stats_filename.c_str());
    primes_stats_outputfile << prime_stats.stat << "\n";
  }

  // Output:
  const std::string comment1("Subsumption hypergraph for the minimisation problem for " + shg_input_filepath);
  OKlib::InputOutput::List2DIMACSOutput(subsumption_hg,std::cout,comment1.c_str());
  // Output of prime clauses if needed:
  std::ofstream outputfile(primes_output_filepath.c_str());
  if (not outputfile) {
    std::cerr << err << "Failure opening output file " << 
      primes_output_filepath  << ".\n";
    return error_openfile;
  }
  const std::string comment2("All prime implicates for " + shg_input_filepath);
  OKlib::InputOutput::List2DIMACSOutput(prime_imp_F,outputfile,comment2.c_str());

}
