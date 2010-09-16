// Matthew Gwynne, 15.9.2010 (Swansea)
/* Copyright 2007, 2009, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Structures/Sets/SetAlgorithms/SubsumptionHypergraph.cpp
  \brief Application for computing the subsumption hypergraph of two given set systems

  Two parameters are needed, the file containing the subsuming set system in 
  DIMACS format, and the file containing the subsumed set system in DIMACS format.
  The result is printed to standard output (again a clause-set in DIMACS format).
*/

#include<iostream>
#include<fstream>

#include<boost/range.hpp>

#include <OKlib/Satisfiability/Interfaces/InputOutput/Dimacs.hpp>
#include <OKlib/Satisfiability/Interfaces/InputOutput/ClauseSetAdaptors.hpp>
#include <OKlib/Structures/Sets/SetAlgorithms/SubsumptionHypergraph.hpp>


namespace {

  enum {
    error_parameters = 1,
    error_openfile = 2,
    error_readfile = 3
  };

  const std::string version = "0.1.10";

}

int main(int argc, char** argv) {
  typedef OKlib::InputOutput::RawDimacsCLSAdaptor<> CLSAdaptor;
  typedef OKlib::InputOutput::StandardDIMACSInput<CLSAdaptor> CLSInput;
  typedef std::list<std::list<boost::range_difference<CLSAdaptor::clause_set_type>::type> > subsumption_hg_type;
  

  if (argc != 3) {
    std::cerr << "ERROR[SubsumptionHypergraph]: Exactly two inputs are"
      " required, the  name of the file containing the subsuming set system"
      "and the name of the file containing the subsumed set system\n with the clause-set in"
      "DIMACS-format.\n However, the actual number of input parameters was " << argc-1 << ".\n";
    return error_parameters;
  }

  CLSAdaptor set_system_F_atr;
  CLSAdaptor set_system_G_atr;

  std::ifstream f_in;
  std::ifstream g_in;

  f_in.open(argv[1], std::ios::in); 
  if (not f_in) {
    std::cerr << "ERROR[SubsumptionHypergraph]: Failure opening file " << argv[1] << ".\n";
    return error_openfile;
  }
  g_in.open(argv[2], std::ios::in);
  if (not g_in) {
    std::cerr << "ERROR[SubsumptionHypergraph]: Failure opening file " << argv[2] << ".\n";
    return error_openfile;
  }

  CLSInput input_F(f_in, set_system_F_atr);
  CLSInput input_G(g_in, set_system_G_atr);

  f_in.close(); g_in.close();

  subsumption_hg_type subsumption_hg = 
    OKlib::SetAlgorithms::subsumption_hypergraph(
                                                 set_system_F_atr.clause_set,
                                                 set_system_G_atr.clause_set);

  OKlib::InputOutput::List2DIMACSOutput(subsumption_hg,std::cout,"Test");

}
