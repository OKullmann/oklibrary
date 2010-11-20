// Oliver Kullmann, 13.6.2009 (Swansea)
/* Copyright 2009, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Combinatorics/Hypergraphs/Transversals/Bounded/MinimumTransversalsMongen.cpp
  \brief Application for computing all minimum transversals of a monotone series of hypergraphs

  <ul>
   <li> Command line parameters are the maximal number of vertices and
   further parameters to be passed to the initialisation of the generator.
   </li>
   <li> Functions Transversals::Bounded::generator::initialise and
   Transversals::Bounded::generator need to be provided in another
   compilation unit (declared in the header file
   Bounded/MinimumTransversalsMongen.hpp). </li>
   <li> That generator has to compute for input n the list of hyperedges
   finishing in n (for example as a vector of vectors). </li>
   <li> Note that here vertices have to be natural numbers from 1 to n. </li>
   <li> The Maxima-specification is given by minimum_transversals_mongen in
   ComputerAlgebra/Hypergraphs/Lisp/Transversals/Bounded/MaintainingBound.mac
   (using [{}] for parameter MT0, since here we assume that we start with
   the hypergraph with no vertices and no hyperedges (n=0), which has thus
   exactly one transversal, the empty set). </li>
  </ul>

  \todo Making the types available
  <ul>
   <li> DONE
   In order that functions "initialise" and "generator" can be defined
   by clients, the involved types should be made available in a header-file.
   </li>
   <li> Better would be a mechanism so that clients can influence these three
   types. </li>
   <li>
  </ul>

*/

#include <iostream>
#include <set>
#include <vector>

#include <boost/lexical_cast.hpp>

#include <OKlib/Programming/Utilities/OrderRelations/OrderConstructions.hpp>
#include <OKlib/Combinatorics/Hypergraphs/Transversals/Bounded/VertexBranching.hpp>

#include <OKlib/Combinatorics/Hypergraphs/Transversals/Bounded/MinimumTransversalsMongen.hpp>

namespace {

  using OKlib::Combinatorics::Hypergraphs::Transversals::Bounded::vertex_type;
  using OKlib::Combinatorics::Hypergraphs::Transversals::Bounded::hyperedge_list_type;
  using OKlib::Combinatorics::Hypergraphs::Transversals::Bounded::parameter_type;

  template <class> struct GeneratorWrapper {
    typedef ::vertex_type vertex_type;
    typedef ::hyperedge_list_type hyperedge_list_type;
    hyperedge_list_type operator() (const vertex_type n) const {
      return ::OKlib::Combinatorics::Hypergraphs::Transversals::Bounded::generator(n);
    }
  };

  enum {
    error_parameters = 1
  };

  const std::string version = "0.0.3";

}

int main(const int argc, const char* const argv[]) {

  if (argc == 1) {
    std::cerr << "ERROR[MinimumTransversalsMongen]:\n"
      " At least one parameter is required, the maximal number of vertices.\n";
    return error_parameters;
  }

  const vertex_type N = boost::lexical_cast<vertex_type>(argv[1]);
  {
    parameter_type P; P.reserve(argc-2);
    for (int i = 2; i < argc; ++i)
      P.push_back(boost::lexical_cast<vertex_type>(argv[i]));
    ::OKlib::Combinatorics::Hypergraphs::Transversals::Bounded::initialise(N, P);
  }

  using OKlib::Combinatorics::Hypergraphs::Transversals::Bounded::hyperedge_type;
  typedef OKlib::Programming::Utilities::OrderRelations::SizeLessThan<std::less<hyperedge_type> > hyperedge_ordering_type;
  typedef std::set<hyperedge_type, hyperedge_ordering_type> set_system_type;

  using namespace OKlib::Combinatorics::Hypergraphs::Transversals;
  typedef Bounded::Minimum_transversals_mongen<set_system_type, GeneratorWrapper, Bounded::TrivialOutput> mtrans_mongen_type;
  typedef mtrans_mongen_type::generator_type generator_type;
  typedef mtrans_mongen_type::output_type output_type;
  output_type out(std::cout);

  mtrans_mongen_type()(N, generator_type(), out);

}

