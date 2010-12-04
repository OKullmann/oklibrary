// Oliver Kullmann, 18.10.2009 (Swansea)
/* Copyright 2009, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Applications/RamseyTheory/CountProgressions_GreenTao.cpp
  \brief Application for counting arithmetic progressions in the prime numbers

  Called with parameters k and N, outputs the number of hyperedges of the
  hypergraphs arithprog_primes_ohg(k,n) for n running from 1 to N,
  in a format readable by R (via read.table(filename, colClasses=
   c("integer","integer"), header=TRUE), creating a dataframe with columns
  "n" and "nhyp").
  More precisely, only those n are output for which a change happens (i.e.,
  new arithmetic progressions were found).

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
        
  const std::string program = "CountProgressions_GreenTao";
  const std::string err = "ERROR[" + program + "]: ";

}

int main(const int argc, const char* const argv[]) {

  if (argc != 3) {
    std::cerr << err << "Exactly two arguments are needed,\n"
    "the length k of arithmetic progressions, and the maximal number N of primes.\n";
    return(errcode_parameter);
  }

  try {

    const uint_type k = boost::lexical_cast<uint_type>(argv[1]);
    const uint_type N = boost::lexical_cast<uint_type>(argv[2]);

    typedef OKlib::Combinatorics::Hypergraphs::Generators::GreenTao<uint_type> gt_generator;
    typedef gt_generator::set_system_type set_system_type;
    typedef gt_generator::hyperedge_type hyperedge_type;
    const gt_generator g(k,N);

    const set_system_type G(g.hyperedge_set());
    const hyperedge_type V(g.vertex_set());

    typedef OKlib::Combinatorics::Hypergraphs::Generators::Sizes_strata_indmon<hyperedge_type, set_system_type> strata_gen_t;
    typedef strata_gen_t::result_type statistics_vector_t;

    statistics_vector_t cumulative = strata_gen_t()(V,G);
    OKlib::Combinatorics::Hypergraphs::Generators::accumulate_l(cumulative);

    std::cout << "n nhyp\n";
    for (statistics_vector_t::const_iterator i = cumulative.begin();
        i != cumulative.end(); ++i)
      std::cout << i -> first << " " << i -> second << "\n";

  }
  catch (boost::bad_lexical_cast&) {
    std::cerr << err << "Both parameters must be non-negative integers\n"
      "in the range given by UINT_TYPE_GT (default = unsigned long).\n";
    return(errcode_parameter_values);
  }

}

