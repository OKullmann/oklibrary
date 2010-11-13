// Oliver Kullmann, 15.11.2009 (Swansea)
/* Copyright 2009, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Transformers/Generators/GreenTaoGCNF.cpp
  \brief Application for creating SAT instances expressing Green-Tao problems with more than 2 colours

  For the specification see output_greentao in
  Satisfiability/Lisp/Generators/RamseyTheory/GreenTaoProblems.mac.

  \todo Compare with specification
  <ul>
   <li> Make sure that the order of clauses coincides with the Maxima-function
   output_greentao. </li>
   <li> Provide similar comments as output_greentao is doing. </li>
  </ul>

  \todo Provide other translations
  <ul>
   <li> Currently we need at least output_greentao_standnest_stdname and
   output_greentao_logarithmic_stdname. </li>
   <li> This shall be controlled by a string as additional parameter,
   "standnest" and "log". </li>
   <li> We need to think in general about naming schemes for the translations,
   and their abbreviations, including what to do with the value-orderings.
   </li>
   <li> Once this extended functionality is available, it needs to be
   incorporated into the script GTSat and GTdSat. </li>
  </ul>

*/

#include <string>
#include <iostream>
#include <vector>
#include <sstream>

#include <boost/lexical_cast.hpp>

#include <OKlib/Satisfiability/Interfaces/InputOutput/Colouring.hpp>
#include <OKlib/Satisfiability/Interfaces/InputOutput/ClauseSetAdaptors.hpp>
#include <OKlib/Combinatorics/Hypergraphs/Generators/GreenTao.hpp>

namespace {

  typedef unsigned int uint_type;
  typedef int int_type;

  enum { errcode_parameter = 1, errcode_neg_parameter = 2,
         errcode_parameter_values = 3 };
        
  const std::string program = "GreenTaoGCNF";
  const std::string err = "ERROR[" + program + "]: ";

}

int main(const int argc, const char* const argv[]) {
  if (argc <= 2) {
    std::cerr << err << "At least two arguments are needed, one arithmetic-progression length and the number of vertices.\n";
    return errcode_parameter;
  }

  try {

    typedef std::vector<uint_type> vector_ap_sizes_t;
    vector_ap_sizes_t ap_sizes;
    const uint_type num_colours = argc-2;
    ap_sizes.reserve(num_colours);

    for (uint_type i = 1; i <= num_colours; ++i) {
      if (boost::lexical_cast<int_type>(argv[i]) < 0) {
        std::cerr << err << "Lengths of arithmetic progresssions must be non-negative.\n";
        return errcode_neg_parameter;
      }
      ap_sizes.push_back(boost::lexical_cast<uint_type>(argv[i]));
    }
    if (boost::lexical_cast<int_type>(argv[argc-1]) < 0) {
      std::cerr << err << "The number of vertices must be non-negative.\n";
      return errcode_neg_parameter;
    }
    const uint_type n = boost::lexical_cast<uint_type>(argv[argc-1]);

    typedef OKlib::Combinatorics::Hypergraphs::Generators::GreenTao<uint_type> gt_generator;
    typedef gt_generator::set_system_type set_system_type;
    typedef std::vector<set_system_type> vector_hypergraphs_t;
    vector_hypergraphs_t vector_hypergraphs;
    vector_hypergraphs.reserve(num_colours);
    typedef std::vector<const set_system_type*> vector_pointers_t;
    vector_pointers_t vector_pointers;
    vector_pointers.reserve(num_colours);

    const gt_generator g0(ap_sizes[0],n);
    vector_hypergraphs.push_back(g0.hyperedge_set());
    vector_pointers.push_back(& vector_hypergraphs[0]);
    uint_type last_index = 0;
    uint_type last_ap_size = ap_sizes[0];

    for (uint_type i = 1; i < num_colours; ++i) {
      if (ap_sizes[i] == last_ap_size)
        vector_pointers.push_back(& vector_hypergraphs[last_index]);
      else {
        const gt_generator g(ap_sizes[i],n,g0.vertex_set());
        vector_hypergraphs.push_back(g.hyperedge_set());
        vector_pointers.push_back(& vector_hypergraphs[++last_index]);
        last_ap_size = ap_sizes[i];
      }
    }

    typedef OKlib::InputOutput::CLSAdaptorDIMACSOutput<int_type> cls_adaptor_type;
    cls_adaptor_type out(std::cout);
    std::stringstream comment;
    comment << "Green-Tao problem with " << num_colours << " colours,\nc and with arithmetic-progression lengths (";
    comment << argv[1];
    for (uint_type i = 2; i <= num_colours; ++i)
      comment << "," << argv[i];
    comment << "), while n = " << n << ".";
    OKlib::Satisfiability::Interfaces::InputOutput::GeneralisedHypergraphColouring<set_system_type, int_type> gg(vector_pointers, g0.vertex_set());
    gg.transfer(out, comment.str());
  }
  catch (boost::bad_lexical_cast&) {
    std::cerr << err << "All parameters must be integers fitting into type int.\n";
    return(errcode_parameter_values);
  }

}

