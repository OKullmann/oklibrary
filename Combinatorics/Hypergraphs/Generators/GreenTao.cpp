// Oliver Kullmann, 17.10.2009 (Swansea)
/* Copyright 2009, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKlib/Combinatorics/Hypergraphs/Generators/GreenTao.cpp
  \brief Application for generating GreenTao hypergraphs

  Called with parameters k and n, outputs the Green-Tao hypergraph
  in Dimacs format.

  \todo Use InputOutput::CLSAdaptorDIMACSOutput
  \todo Use Messages
*/

#include <iostream>
#include <string>

#include <boost/lexical_cast.hpp>

#include <OKlib/Combinatorics/Hypergraphs/Generators/GreenTao.hpp>

namespace {

#  ifndef UINT_TYPE_GT
#    define UINT_TYPE_GT unsigned long  
#  endif

  typedef UINT_TYPE_GT uint_type;

  enum { errcode_parameter = 1, errcode_parameter_values = 2 };
        
  const std::string program = "GreenTao";
  const std::string err = "ERROR[" + program + "]: ";

  const std::string version = "0.2";

}

int main(const int argc, const char* const argv[]) {

  if (argc != 3) {
    std::cerr << err << "Exactly two arguments are needed,\n"
    "the length k of arithmetic progressions, and the number n of primes.\n";
    return errcode_parameter;
  }

  try {

    const uint_type k = boost::lexical_cast<uint_type>(argv[1]);
    const uint_type n = boost::lexical_cast<uint_type>(argv[2]);

    typedef OKlib::Combinatorics::Hypergraphs::Generators::GreenTao<uint_type> gt_generator;
    typedef gt_generator::set_system_type set_system_type;
    typedef gt_generator::hyperedge_type hyperedge_type;
    const gt_generator g(k,n);

    const set_system_type G(g.hyperedge_set());

    std::cout << "c Green-Tao hypergraph with arithmetic-progression length " << g.k << " and " << g.nver() << " vertices.\n";
    std::cout << "c The prime numbers are used directly as vertices.\n";
    std::cout << "p cnf " << g.max_index() << " " << G.size() << "\n";

    for (set_system_type::const_iterator i = G.begin(); i != G.end(); ++i) {
      const hyperedge_type H = *i;
      for (hyperedge_type::const_iterator j = H.begin(); j != H.end(); ++j)
        std::cout << *j << " ";
      std::cout << "0\n";
    }

  }
  catch (boost::bad_lexical_cast&) {
    std::cerr << err << "Both parameters must be non-negative integers\n"
    "in the range given by UINT_TYPE_GT (default = unsigned long).\n";
    return(errcode_parameter_values);
  }

}
