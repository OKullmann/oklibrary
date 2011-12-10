// Matthew Gwynne, 11.1.2011 (Swansea)
/* Copyright 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/FiniteFunctions/QuineMcCluskeySubsumptionHypergraphWithFullStatistics.cpp
  \brief Application for computing the subsumption hypergraph of the prime clauses of a full clause-set as well as statistics

  <ul>
   <li> One parameter is needed, the file containing the clause-set F in DIMACS
   format. </li>
   <li> The subsumption hypergraph is printed to standard output, a hypergraph
   in DIMACS format, in lexicographical order, without duplicated clauses.
   </li>
   <li> The prime clauses of F are output (in DIMACS format) to
   basename(input_filename)_primes. </li>
   <li> Additionally the clause-set statistics for the prime clauses and
   the subsumption hypergraph is output to
   basename(input_filename)_primes_stats and
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

  const std::string version = "0.2.1";

  using namespace OKlib::InputOutput;

  inline output_options translate_option(const std::string& opt) {
    if (opt == "n") return no_zeros;
    if (opt == "ni") return no_initfinal_zeros;
    if (opt == "f") return full_output;
    return (output_options) 0;
  }
  const output_options default_option = no_initfinal_zeros;

}

int main(const int argc, const char* const argv[]) {

  if (argc != 2 and argc != 3) {
    std::cerr << err << "Exactly one input is required,\n"
      " the name of the file with the clause-set in DIMACS-format.\n"
      "One may also specify additionally \"n\", \"ni\" or \"f\" for\n"
      " no zero-counts, no initial and final zero-counts, or full output (\"ni\" is default).\n"
      "However, the actual number of input parameters was " << argc-1 << ".\n";
    return error_parameters;
  }

  const std::string shg_input_filepath = argv[1];
  std::ifstream shg_inputfile(shg_input_filepath.c_str());
  if (not shg_inputfile) {
    std::cerr << err << "Failure opening input file " << shg_input_filepath << ".\n";
    return error_openfile;
  }

  typedef OKlib::InputOutput::RawDimacsCLSAdaptor<> CLSAdaptor;
  CLSAdaptor cls_F;
  typedef OKlib::InputOutput::StandardDIMACSInput<CLSAdaptor> CLSInput;
  const CLSInput input_F(shg_inputfile, cls_F);
  shg_inputfile.close();

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
  
  // Do we want full output or to remove leading and trailing zeroes.
  StatsCLSAdaptor shg_stats;
  StatsCLSAdaptor prime_stats;
  if (argc > 2) {
    const output_options opt = translate_option(argv[2]);
    if (opt != (output_options) 0) {
      shg_stats.stat.option = opt;
      prime_stats.stat.option = opt;
    }
    else {
      std::cerr << err << "If an option is given, it must be one of \"n\", \"ni\" or \"f\".\n";
      return(error_parameters);
    }
  }
  else {
    shg_stats.stat.option = default_option;
    prime_stats.stat.option = default_option;
  }

  List2Statistics(subsumption_hg, shg_stats, "");
  const std::string shg_input_filename = boost::filesystem::path(shg_input_filepath).filename().string();
  const std::string shg_stats_filename = shg_input_filename + "_shg_stats";
  std::ofstream shg_stats_outputfile(shg_stats_filename.c_str());
  shg_stats_outputfile << shg_stats.stat << "\n";   
    
  List2Statistics(prime_imp_F, prime_stats, "");
  
  const std::string primes_stats_filename = 
    shg_input_filename + "_primes_stats";
  std::ofstream primes_stats_outputfile(primes_stats_filename.c_str());
  primes_stats_outputfile << prime_stats.stat << "\n";

  // Output subsumption hypergraph to STDOUT.
  const std::string shg_comment("Subsumption hypergraph for the minimisation problem for " + shg_input_filepath);
  OKlib::InputOutput::List2DIMACSOutput(subsumption_hg,std::cout,shg_comment.c_str());
  // Output of prime clauses if needed to the correct file.
  const std::string primes_output_filepath = shg_input_filename + "_primes";
  std::ofstream primes_outputfile(primes_output_filepath.c_str());
  if (not primes_outputfile) {
    std::cerr << err << "Failure opening output file " << 
      primes_output_filepath  << ".\n";
    return error_openfile;
  }
  const std::string primes_comment("All prime implicates for " + shg_input_filepath);
  OKlib::InputOutput::List2DIMACSOutput(prime_imp_F,primes_outputfile,primes_comment.c_str());

}
