// Oliver Kullmann, 6.6.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Combinatorics/Hypergraphs/Transversals/BoundedTransversals_bv.cpp
  \brief Application for computing all size-bounded transversals of a hypergraph

  <ul>
   <li> The role model is transversals_bvs in
   ComputerAlgebra/Hypergraphs/Lisp/Transversals/Transversals.mac. </li>
   <li> With an optional second argument minimum_transversals_lbbvs_hg in
   ComputerAlgebra/Hypergraphs/Lisp/Transversals/Transversals.mac is
   realised, i.e., incremental search. </li>
   <li> The algorithm does not terminate exactly iff incremental search
   is required, while the input contains the empty hyperedge. </li>
  </ul>

*/

#include <string>
#include <set>
#include <functional>
#include <iostream>

#include <boost/lexical_cast.hpp>

#include <OKlib/Programming/Utilities/OrderRelations/OrderConstructions.hpp>
#include <OKlib/Satisfiability/Interfaces/InputOutput/ClauseSetAdaptors.hpp>
#include <OKlib/Satisfiability/Interfaces/InputOutput/Dimacs.hpp>
#include <OKlib/Combinatorics/Hypergraphs/Transversals/Transversals_bv.hpp>

namespace {

  const int error_parameters = 1;

  const std::string version = "0.0.4";

}

int main(const int argc, const char* const argv[]) {

  if (argc != 2 and argc != 3) {
    std::cerr << "ERROR[BoundedTransversals_bv]:\n"
      " Exactly one input is required, the upper bound on the transversal size.\n"
      " A second parameter (whose value is ignored) indicates that this bound\n"
      " is to be increased until a transversal is found.\n"
      " However, the actual number of input parameters was " << argc-1 << ".\n";
    return error_parameters;
  }

  typedef unsigned int vertex_type;
  typedef int literal_type; // necessary since (currently) we are using clause-set input- and output-facilities

  typedef std::set<literal_type> hyperedge_type;

  typedef hyperedge_type::size_type size_type;
  const size_type B = boost::lexical_cast<size_type>(argv[1]);
  const bool iterated = (argc == 3);
  
  typedef OKlib::OrderRelations::SizeLessThan<std::less<hyperedge_type> > hyperedge_ordering_type;
  typedef std::set<hyperedge_type, hyperedge_ordering_type> set_system_type;

  typedef OKlib::InputOutput::RawDimacsCLSAdaptorSets<literal_type, set_system_type> dimacs_adaptor_type;
  typedef OKlib::InputOutput::StandardDIMACSInput<dimacs_adaptor_type> dimacs_input_type;
  
  dimacs_adaptor_type in;
  dimacs_input_type(std::cin, in);
  
  typedef OKlib::Combinatorics::Hypergraphs::Transversals::Bounded_transversals_bv<set_system_type> transversal_enumerator_type;
  typedef transversal_enumerator_type::transversal_list_type transversal_list_type;
  
  transversal_enumerator_type t_e(in.clause_set, B);
  const transversal_list_type transversals(iterated ? t_e.iterated() : t_e());
  
  typedef OKlib::InputOutput::CLSAdaptorDIMACSOutput<> dimacs_adaptor2_type;
  typedef OKlib::InputOutput::ListTransfer<dimacs_adaptor2_type> dimacs_output_type;
  dimacs_adaptor2_type out(std::cout);
  dimacs_output_type(transversals, out, "Bounded transversals of size " + boost::lexical_cast<std::string>(t_e.bound));

}

