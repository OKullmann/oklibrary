// Oliver Kullmann, 26.7.2004 (Turin)
/* Copyright 2004 - 2007, 2009, 2010, 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Transformers/Generators/Ramsey.hpp
  \brief Fundamental components for creating Ramsey instances
   \deprecated Needs a complete update.

  \details
  Currently the code in here is rather outdated.

*/

#ifndef RAMSEY_hhfzqwk15183Gfzx
#define RAMSEY_hhfzqwk15183Gfzx

#include <stdexcept>
#include <cassert>
#include <string>
#include <ostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <functional>
#include <cmath>
#include <sstream>

#include <boost/lexical_cast.hpp>

#include <OKlib/General/Combinatorics.hpp>
#include <OKlib/General/IteratorHandling.hpp>
#include <OKlib/General/Numerics.hpp>


namespace Ramsey {

  /*!
    \class Enumerate_hyperedges
    \brief Computing the hypergraphs underlying Ramsey-instances

    The procedural specification is given by ramsey_ohg(q,r,n) in
    ComputerAlgebra/Hypergraphs/Lisp/Generators/Ramsey.mac.

    That is, vertices are the r-subsets of {1, ..., n}, and for every
    q-subset T of {1, ..., n} there is the hyperedge of r-subsets of T.
    Precondition: n >= q >= r >= 0.

    \todo See ComputerAlgebra/Hypergraphs/Lisp/Generators/plans/Ramsey.hpp

    \todo Update the underlying notion of "hypergraph"
  */

  template <typename Int = int>
  class Enumerate_hyperedges {
  public :
    typedef Int size_type;
    const size_type q; // size of monochromatic subset
    const size_type r; // size of edges
    const size_type n; // number of vertices in underlying graph
    const size_type number_vertices_hypergraph; 
    const size_type size_hyperedges;
    const size_type number_hyperedges;
    typedef typename std::vector<size_type> edge_type; // const size of vectors = r
    typedef typename std::vector<edge_type> hyperedge_type; // const size of vectors = size_hyperedges
    hyperedge_type current_hyperedge;
    
  private :
    typedef std::vector<size_type> subset_type; // const size of vectors = q
    subset_type current_subset;

  public :
    Enumerate_hyperedges
    (const size_type q, const size_type r, const size_type n)
      : q(q), r(r), n(n), number_vertices_hypergraph(compute_number_vertices_hypergraph()), size_hyperedges(compute_size_hyperedges()), number_hyperedges(compute_number_hyperedges()), current_hyperedge(size_hyperedges, edge_type(r)), current_subset(IteratorHandling::count_iterator(size_type()), IteratorHandling::count_iterator(q)) {
      assert(current_hyperedge.size() == size_hyperedges);
      update_current_hyperedge();
    }

    void next() {
      if (Combinatorics::choose_next(n, q, current_subset.begin(), current_subset.end()) != Combinatorics::no_further_subsets)
	update_current_hyperedge();
    }

  private :
    void update_current_hyperedge() {
      typedef std::vector<typename subset_type::size_type> index_subset_type;
      index_subset_type s(IteratorHandling::count_iterator(size_type()), IteratorHandling::count_iterator(r));
      for (typename hyperedge_type::iterator i = current_hyperedge.begin(); i != current_hyperedge.end(); ++i, Combinatorics::choose_next(q, r, s.begin(), s.end()))
	for (typename edge_type::size_type j = 0; j != r; ++j) {
	  assert(i -> size() == r);
	  (*i)[j] = current_subset[s[j]];
	}
    }

    size_type compute_number_vertices_hypergraph() const {
      assert(n >= 0);
      assert(r >= 0);
      assert(n >= r);
      return Combinatorics::binom(n,r);    }

    size_type compute_size_hyperedges() const {
      assert(q >= r);
      return Combinatorics::binom(q,r);
    }

    size_type compute_number_hyperedges() const {
      assert(n >= q);
      return Combinatorics::binom(n,q);
    }
  };


  /*!
    \class Ramsey_TwoColours_1
    \brief Outputs a Ramsey SAT-instance for two colours and arbitrary
    hyperedge size

    For the diagonal case (q1=q2=q) this corresponds to
    output_ramsey2_stdname(q,r,N) in
    ComputerAlgebra/Satisfiability/Lisp/Generators/RamseyTheory/RamseyProblems.mac.

    \todo Use components from module InputOutput.
  */

  template <typename Int = unsigned int>
  class Ramsey_TwoColours_1 {

    typedef Enumerate_hyperedges<Int> enumeration_type;
  public :
    typedef typename enumeration_type::size_type size_type;
    const size_type q1, q2;
    const size_type r;
    const size_type N;
  private :
    std::ostream& out;
    enumeration_type H1, H2;
  public :
    const size_type number_vertices_hypergraph; 
    const size_type size_hyperedges1, size_hyperedges2;
    const size_type number_hyperedges1, number_hyperedges2;
    const size_type n;
    const size_type c;
 
    Ramsey_TwoColours_1
    (const size_type q1, const size_type q2, const size_type r, const size_type N, std::ostream& out) :
      q1(q1), q2(q2), r(r), N(N), out(out), H1(q1, r, N), H2(q2, r, N), number_vertices_hypergraph(H1.number_vertices_hypergraph), size_hyperedges1(H1.size_hyperedges), size_hyperedges2(H2.size_hyperedges), number_hyperedges1(H1.number_hyperedges), number_hyperedges2(H2.number_hyperedges), n(number_vertices_hypergraph), c(number_hyperedges1 + number_hyperedges2) {
      assert(H1.number_vertices_hypergraph == H2.number_vertices_hypergraph);
    }

    void operator() () {
      comment();
      clauses();
    }

    virtual ~Ramsey_TwoColours_1() {}

  private :

    void virtual comment() const {
      out << "c Ramsey numbers with partitioning into s = 2 parts; generator written by Oliver Kullmann, Swansea, July 2004\n";
      out << "c Size of first monochromatic set q_1 = " << boost::lexical_cast<std::string>(q1) << "\n";
      out << "c Size of second monochromatic set q_2 = " << boost::lexical_cast<std::string>(q2) << "\n";
      out << "c Hyperedge size r = " << boost::lexical_cast<std::string>(r) << "\n";
      out << "c Number of vertices N = " << boost::lexical_cast<std::string>(N) << "\n";
      out << "c (The clause-set is satisfiable iff the hyperedges of the complete hypergraph K_N^r (N vertices, every hyperedge has size r) can be labelled red or blue such that neither there is a red set of q_1 many vertices nor a blue set of q_2 many vertices, where a set of vertices is called \"red\" respectively \"blue\" iff all hyperedges consisting only of vertices out of this set are marked red respectively blue. For all fixed q1, q2, r there exists a natural number N_0, such that for all N >= N_0 the clause-sets with parameters q_1, q_2, r and N are unsatisfiable; the smallest such N_0 is the Ramsey-number N(q_1, q_2, r).)\n";
      out << "p cnf " << boost::lexical_cast<std::string>(n) << " " << boost::lexical_cast<std::string>(c) << "\n";
    }
    void clauses() {
      if (c == 0) return;
      // excluding "red"
      for (size_type i = 0; i < number_hyperedges1; ++i, H1.next()) {
	for (typename hyperedge_type::const_iterator j = H1.current_hyperedge.begin(); j != H1.current_hyperedge.end(); ++j)
	  out << " " << var(*j);
	out << eoc();
      }
      // excluding "blue"
      for (size_type i = 0; i < number_hyperedges2; ++i, H2.next()) {
	for (typename hyperedge_type::const_iterator j = H2.current_hyperedge.begin(); j != H2.current_hyperedge.end(); ++j)
	  out << " " << neg(var(*j));
	out << eoc();
      }
    }

  protected :

    typedef typename enumeration_type::hyperedge_type hyperedge_type;
    typedef typename enumeration_type::edge_type edge_type;
    std::string var(const edge_type& v) const {
      assert(v.size() == r);
      std::stringstream s("V");
      typename edge_type::const_iterator i = v.begin();
      s << *(i++) + 1;
      for (; i != v.end(); ++i)
	s << "S" << *i + 1;
      return s.str();
    }
    virtual std::string neg(const std::string& var) const {
      return "-" + var;
    }
    virtual std::string eoc() const {
      return " 0\n";
    }

  };

}

#endif


