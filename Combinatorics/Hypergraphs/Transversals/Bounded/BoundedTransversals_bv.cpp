// Oliver Kullmann, 6.6.2009 (Swansea)
/* Copyright 2009, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Combinatorics/Hypergraphs/Transversals/Bounded/BoundedTransversals_bv.cpp
  \brief Application for computing all size-bounded transversals of a hypergraph

  <ul>
   <li> The hypergraph is read from standard input, in DIMACS format. </li>
   <li> The output is the list of transversals, also in DIMACS format. </li>
   <li> The role model is transversals_bvs in
   ComputerAlgebra/Hypergraphs/Lisp/Transversals/Bounded/MaintainingBound.mac.
   </li>
   <li> To perform this computation requires (exactly) one parameter, of the
   form "=B". </li>
   <li> This is a simple recursive search (by branching on vertices) for
   computing basically all minimal transversals of size at most B. </li>
   <li> With an argument ">=B" (a lower bound), or with no argument
   (corresponding to ">=0"), minimum_transversals_lbbvs_hg in
   ComputerAlgebra/Hypergraphs/Lisp/Transversals/Bounded/MaintainingBound.mac
   is realised, i.e., incremental search. </li>
   <li> So "=B" is used if the size of minimum transversals is known, while
   otherwise starting from a known lower bound the size of minimum transversals
   is computed by the algorithm. </li>
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
#include <OKlib/Combinatorics/Hypergraphs/Transversals/Bounded/VertexBranching.hpp>

namespace {

  enum {
    error_parameters = 1,
    error_empty = 2,
    error_symbol1 = 3,
    error_symbol2 = 4,
    error_empty_hyperedge = 5
  };

  const std::string version = "0.0.5";

}

int main(const int argc, const char* const argv[]) {

  if (argc > 2) {
    std::cerr << "ERROR[BoundedTransversals_bv]:\n"
      " Either zero or one parameter is required, which is of the form\n"
      " \"=n\" or \">=n\" for some natural number n >= 0.\n"
      " However, the actual number of input parameters was " << argc-1 << ".\n";
    return error_parameters;
  }

  typedef unsigned int vertex_type; // currently ignored
  typedef int literal_type; // necessary since (currently) we are using clause-set input- and output-facilities
  typedef std::set<literal_type> hyperedge_type;
  typedef hyperedge_type::size_type size_type;

  bool iterated_; size_type B_;
  if (argc == 1) { iterated_ = true; B_ = 0; }
  else {
    const std::string par(argv[1]);
    if (par.empty()) {
      std::cerr << "ERROR[BoundedTransversals_bv]:\n"
        "The parameter is the empty string.\n";
      return error_empty;
    }
    if (par[0] == '=') {
      iterated_ = false;
      B_ = boost::lexical_cast<size_type>(par.substr(1));
    }
    else {
      if (par[0] != '>') {
        std::cerr << "ERROR[BoundedTransversals_bv]:\n"
          "Unrecognised leading symbol " << par[0] << ".\n";
        return error_symbol1;
      }
      if (par.size() == 1 or par[1] != '=') {
        std::cerr << "ERROR[BoundedTransversals_bv]:\n"
          "After \">\" the symbol \"=\" is expected.\n";
        return error_symbol2;
      }
      iterated_ = true;
      B_ = boost::lexical_cast<size_type>(par.substr(2));
    }
  }
  const bool iterated(iterated_); const size_type B(B_);

  typedef OKlib::Programming::Utilities::OrderRelations::SizeLessThan<std::less<hyperedge_type> > hyperedge_ordering_type;
  typedef std::set<hyperedge_type, hyperedge_ordering_type> set_system_type;
  typedef OKlib::InputOutput::RawDimacsCLSAdaptorSets<literal_type, set_system_type> dimacs_adaptor_type;
  typedef OKlib::InputOutput::StandardDIMACSInput<dimacs_adaptor_type> dimacs_input_type;
  
  dimacs_adaptor_type in;
  dimacs_input_type(std::cin, in);
  
  typedef OKlib::Combinatorics::Hypergraphs::Transversals::Bounded::Bounded_transversals_bv<set_system_type> transversal_enumerator_type;
  typedef transversal_enumerator_type::transversal_list_type transversal_list_type;
  
  transversal_enumerator_type t_e(in.clause_set, B);
  if (iterated and not t_e.G_orig.empty() and t_e.G_orig.begin() -> empty()) {
    std::cerr << "ERROR[BoundedTransversals_bv]:\n"
      "The iteration would not terminate due to the presence of the empty hyperedge.\n";
    return error_empty_hyperedge;
  }
  const transversal_list_type transversals(iterated ? t_e.iterated() : t_e());
  
  typedef OKlib::InputOutput::CLSAdaptorDIMACSOutput<> dimacs_adaptor2_type;
  typedef OKlib::InputOutput::ListTransfer<dimacs_adaptor2_type> dimacs_output_type;
  dimacs_adaptor2_type out(std::cout);
  dimacs_output_type(transversals, out, "Bounded transversals of size " + boost::lexical_cast<std::string>(t_e.bound));

}

