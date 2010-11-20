// Oliver Kullmann, 20.11.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKlib/Combinatorics/Hypergraphs/Generators/PdVanderWaerden.cpp
  \brief Application for generating palindromic van der Waerden hypergraphs

  Called with parameters k and n, outputs the palindromised vdW-hypergraph
  in Dimacs format.

  \todo Use InputOutput::CLSAdaptorDIMACSOutput
  \todo Use Messages
*/

#include <iostream>
#include <string>

#include <boost/lexical_cast.hpp>

#include <OKlib/Combinatorics/Hypergraphs/Generators/VanderWaerden.hpp>

namespace {

#  ifndef UINT_TYPE_VDW
#    define UINT_TYPE_VDW unsigned long  
#  endif

  typedef UINT_TYPE_VDW uint_type;

  enum { errcode_parameter = 1, errcode_parameter_values = 2 };
        
  const std::string program = "VanderWaerden";
  const std::string err = "ERROR[" + program + "]: ";

  const std::string version = "0.1";

}

int main(const int argc, const char* const argv[]) {

  if (argc != 3) {
    std::cerr << err << "Exactly two arguments are needed,\n"
    "the length k of arithmetic progressions, and the number n of vertices.\n";
    return errcode_parameter;
  }

  try {

    const uint_type k = boost::lexical_cast<uint_type>(argv[1]);
    const uint_type n = boost::lexical_cast<uint_type>(argv[2]);

    typedef OKlib::Combinatorics::Hypergraphs::Generators::Pd_arithprog_ohg<uint_type> vdw_generator;
    typedef vdw_generator::set_system_type set_system_type;
    typedef vdw_generator::hyperedge_type hyperedge_type;

    const set_system_type G(vdw_generator()(k,n));

    std::cout << "c Palindromised hypergraph with arithmetic-progression length " << k << " and " << n << " vertices.\n";
    std::cout << "p cnf " << (n+1)/2 << " " << G.size() << "\n";

    for (set_system_type::const_iterator i = G.begin(); i != G.end(); ++i) {
      const hyperedge_type H = *i;
      for (hyperedge_type::const_iterator j = H.begin(); j != H.end(); ++j)
        std::cout << *j << " ";
      std::cout << "0\n";
    }

  }
  catch (boost::bad_lexical_cast&) {
    std::cerr << err << "Both parameters must be non-negative integers\n"
    "in the range given by UINT_TYPE_VDW (default = unsigned int).\n";
    return(errcode_parameter_values);
  }

}
