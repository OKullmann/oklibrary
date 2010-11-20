// Oliver Kullmann, 15.11.2009 (Swansea)
/* Copyright 2009, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Transformers/Generators/GreenTaoCNF.hpp
  \brief Application for creating SAT instances expressing 2-coloured Green-Tao problems

  <ul>
   <li> Only 2-coloured Green-Tao-problems can be generated; for more than
   two colours see Satisfiability/Transformers/Generators/GreenTaoGCNF.hpp.
   </li>
   <li> The three command-line parameters are the two progression size and the
   number of vertices. </li>
   <li> For the Maxima specification see output_greentao2nd_stdname(k1,k2,n)
   in ComputerAlgebra/Satisfiability/Lisp/Generators/RamseyTheory/GreenTaoProblems.mac.
   </li>
  </ul>

  \todo Improve implementation
  <ul>
   <li> The name-map likely can be implemented much faster using a hash-map
   (in C++0x). </li>
   <li> Make the comments the same as with output_greentao2nd_stdname(k1,k2,n).
   </li>
  <ul>

  \todo Write application tests

*/

#include <string>
#include <iostream>
#include <map>
#include <cassert>
#include <utility>

#include <boost/lexical_cast.hpp>

#include <OKlib/Satisfiability/Interfaces/InputOutput/Colouring.hpp>
#include <OKlib/Satisfiability/Interfaces/InputOutput/ClauseSetAdaptors.hpp>
#include <OKlib/Combinatorics/Hypergraphs/Generators/GreenTao.hpp>

namespace {

  typedef unsigned int uint_type;
  typedef int int_type;

  enum { errcode_parameter = 1, errcode_neg_parameter = 2,
         errcode_parameter_values = 3 };
        
  const std::string program = "GreenTaoCNF";
  const std::string err = "ERROR[" + program + "]: ";

  const std::string version = "0.2";

  struct NameMap {
    typedef std::map<uint_type, int_type> map_type;
    map_type m;
    int_type operator()(const uint_type v) const {
      return (m.find(v)) -> second;
    }
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

    typedef OKlib::Combinatorics::Hypergraphs::Generators::GreenTao<uint_type> gt_generator;
    const gt_generator g1(k1,n);
    const gt_generator g2(k2,n,g1.vertex_set());
    typedef gt_generator::set_system_type set_system_type;
    const set_system_type& G1(g1.hyperedge_set());
    const set_system_type& G2(k1 == k2 ? G1 : g2.hyperedge_set());
    NameMap M;
    {
      typedef gt_generator::hyperedge_type hyperedge_type;
      const hyperedge_type primes(g1.vertex_set());
      assert(primes.size() == n);
      typedef hyperedge_type::const_iterator iterator;
      const iterator end(primes.end());
      int_type count = 1;
      for (iterator i = primes.begin(); i != end; ++i, ++count)
        M.m.insert(std::make_pair(*i,count));
    }
    typedef OKlib::InputOutput::CLSAdaptorDIMACSOutput<int_type> cls_adaptor_type;
    cls_adaptor_type out(std::cout);
    const std::string comment = "Green-Tao problem with two colours, where k1 = " + std::string(argv[1]) + ", k2 = " + std::string(argv[2]) + ", and n = " + std::string(argv[3]) + ".";
    OKlib::Satisfiability::Interfaces::InputOutput::HypergraphTwoColouring<set_system_type, cls_adaptor_type, int_type>(G1, G2, M, n, comment, out);

  }
  catch (boost::bad_lexical_cast&) {
    std::cerr << err << "All parameters must be integers fitting into type int.\n";
    return(errcode_parameter_values);
  }

}
