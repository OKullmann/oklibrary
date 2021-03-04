// Oliver Kullmann, 20.11.2010 (Swansea)
/* Copyright 2010, 2011, 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Transformers/Generators/PdVanderWaerdenCNF.cpp
  \brief Application for creating SAT instances expressing 2-coloured palindromised van der Waerden problems

  <ul>
   <li> Only 2-coloured vdW-problems can be generated; more than
   two colours have to be yet established. </li>
   <li> The three command-line parameters are the two progression size and the
   number of vertices. </li>
   <li> The output by default is put into file
   "VanDerWaerden_pd_2-k1-k2_n.cnf". </li>
   <li> An optional fourth argument S is treated as follows:
    <ol>
     <li> If S="-", then output is put to standard output. </li>
     <li> Otherwise the output is put into a file named S. </li>
    </ol>
   </li>
   <li> For the Maxima specification see
   output_pd_vanderwaerden2nd_stdname(k1,k2,n) in
   ComputerAlgebra/Satisfiability/Lisp/Generators/RamseyTheory/VanderWaerdenProblems.mac.
   </li>
  </ul>

  \todo Update to new system

*/

#include <string>
#include <iostream>
#include <fstream>

#include <boost/lexical_cast.hpp>

#include <OKlib/Satisfiability/Interfaces/InputOutput/Colouring.hpp>
#include <OKlib/Satisfiability/Interfaces/InputOutput/ClauseSetAdaptors.hpp>
#include <OKlib/Combinatorics/Hypergraphs/Generators/VanderWaerden.hpp>

namespace {

  typedef unsigned int uint_type;
  typedef int int_type;

  enum {
    errcode_parameter = 1,
    errcode_neg_parameter = 2,
    errcode_parameter_values = 3,
    errcode_file = 4
  };
        
  const std::string program = "PdVanderWaerdenCNF";
  const std::string err = "ERROR[" + program + "]: ";

  const std::string version = "0.1.1";

  struct NameMap {
    int_type operator()(const uint_type v) const { return v; }
  };

}

int main(const int argc, const char* const argv[]) {
    if (argc <= 3) {
    std::cerr << err << "Three arguments are needed: The two sizes of the arithmetic progressions, and the number of elements.\n";
    return errcode_parameter;
  }

  try {

    if (boost::lexical_cast<int_type>(argv[1]) < 0 or
        boost::lexical_cast<int_type>(argv[2]) < 0 or
        boost::lexical_cast<int_type>(argv[3]) < 0) {
      std::cerr << err << "All parameters must be non-negative.\n";
      return errcode_neg_parameter;
    }

    const uint_type k1 = boost::lexical_cast<uint_type>(argv[1]);
    const uint_type k2 = boost::lexical_cast<uint_type>(argv[2]);
    const uint_type n = boost::lexical_cast<uint_type>(argv[3]);
    const uint_type nvar = (n+1)/2;

    const std::string filename = (argc == 5) ?
      (std::string(argv[4]) == "-" ? "" : argv[4]) :
      std::string("VanDerWaerden_pd_2-") + argv[1] + "-" + argv[2] + "_" + argv[3] + ".cnf";

    typedef OKlib::Combinatorics::Hypergraphs::Generators::Pd_arithprog_ohg<uint_type> vdw_generator;
    typedef vdw_generator::set_system_type set_system_type;
    const set_system_type& G1(vdw_generator()(k1,n));
    const set_system_type& G2(k1 == k2 ? G1 : vdw_generator()(k2,n));
    typedef OKlib::InputOutput::CLSAdaptorDIMACSOutput<int_type> cls_adaptor_type;
    const std::string comment = "Palindromic van der Waerden problem: 2 parts, arithmetic progressions of size " + std::string(argv[1]) + " and " + std::string(argv[2]) + ", and " + std::string(argv[3]) + " elements, yielding " + boost::lexical_cast<std::string>(nvar) + " variables.";
    if (filename.empty()) {
      cls_adaptor_type out(std::cout);
      OKlib::Satisfiability::Interfaces::InputOutput::HypergraphTwoColouring<set_system_type, cls_adaptor_type, int_type>(G1, G2, NameMap(), nvar, comment, out);
    }
    else {
      std::ofstream fout(filename.c_str());
      if (not fout) {
        std::cerr << err << "Couldn't open file " << filename << " for writing.\n";
        return errcode_file;
      }
      cls_adaptor_type out(fout);
      OKlib::Satisfiability::Interfaces::InputOutput::HypergraphTwoColouring<set_system_type, cls_adaptor_type, int_type>(G1, G2, NameMap(), nvar, comment, out); 
    }

  }
  catch (boost::bad_lexical_cast&) {
    std::cerr << err << "All parameters must be integers fitting into type int.\n";
    return(errcode_parameter_values);
  }

}
