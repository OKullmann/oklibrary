// Oliver Kullmann, 31.5.2004 (Swansea)
/* Copyright 2004 - 2007, 2009, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Transformers/Generators/VanderWaerden.hpp
  \brief Old components for creating SAT instances expressing van-der-Waerden problems
  \deprecated Needs a complete update.

  \todo Update according to new hypergraph generator
*/

#ifndef VANDERWAERDENWAECHTER_3199GeQa
#define VANDERWAERDENWAECHTER_3199GeQa

#include <stdexcept>
#include <cassert>
#include <string>
#include <ostream>
#include <vector>

#include <boost/lexical_cast.hpp>


namespace VanderWaerden {

  // "Quick and dirty" --- to be improved later (via using a standard output interface)

  template <typename Int>
  class Arithmetical_progressions {
    // ToDo: Integration with classes in Combinatorics/Hypergraphs/Generators/plans/VanderWaerden.hpp and General/IteratorHandling.hpp
  public :
    typedef Int Index;
    const Index n;
    const Index m; // size of arithmetical progression in {1, ..., n}
    const Index max_element;
    const Index count; // total number of arithmetical progressions

  private :
    Index current_element;
    Index current_distance;

  public :

    Arithmetical_progressions(const Index m, const Index n) : n(n), m(m), max_element(n-m+1), count(h()), current_element(1), current_distance(1) {}

    typedef std::vector<Index> Arithmetical_progression;

    Arithmetical_progression next() {
      Arithmetical_progression ap;
      ap.reserve(m);
      for (Index i = 0; i < m; ++i)
	ap.push_back(current_element + i * current_distance);
      if (current_element + (m-1) * (current_distance + 1) <= n and m >= 2)
	++current_distance;
      else {
	++current_element; current_distance = 1;
      }
      return ap;
    } // shall be called only count many times

  private :

    Index h() const {
      assert(m >= 1);
      assert(n >= 2);
      assert(n >= m);
      if (m == 1) return n;
      const Index q = (n-1) / (m-1);
      return q * (n - ((m - 1) * (q + 1)) / 2);
    }
  };


  class VanderWaerden_TwoParts_1 {

    // Clause-sets \FvdW(2,m,n) according to "BuchKombinatorik"

  public :

    typedef unsigned int Index;

  public :
    const Index m; // size of arithmetical progression
    const Index n; // size of counter example and number of variables
  private :
    std::ostream& out;
    typedef Arithmetical_progressions<Index> AP;
    AP ap;
    const Index number_ap;
  public :
    const Index c; // number clauses

  public :
  
    VanderWaerden_TwoParts_1(const Index m, const Index n, std::ostream& out) : m(m), n(n), out(out), ap(m, n), number_ap(ap.count), c(number_ap * 2) {}
  
    void operator() () {
      comment();
      clauses();
    }

    virtual ~VanderWaerden_TwoParts_1() {}

  private :

    void virtual comment() const {
      out << "c Van der Waerden numbers with partitioning into k = 2 parts; generator written by Oliver Kullmann, Swansea, May 2004\n";
      out << "c Arithmetical progression size m = " << boost::lexical_cast<std::string>(m) << "\n";
      out << "c Number of elements n = " << boost::lexical_cast<std::string>(n) << "\n";
      out << "p cnf " << boost::lexical_cast<std::string>(n) << " " << boost::lexical_cast<std::string>(c) << "\n";
    }
    void clauses() {
      for (Index i = 0; i < number_ap; ++i) {
	const AP::Arithmetical_progression p = ap.next();
	for (AP::Arithmetical_progression::const_iterator i = p.begin(); i != p.end(); ++i)
	  out << " " << var(*i);
	out << eoc();
	for (AP::Arithmetical_progression::const_iterator i = p.begin(); i != p.end(); ++i)
	  out << " " << neg(var(*i));
	out << eoc();
      }
    }

  protected :

    std::string var(const Index v) const {
      assert(v != 0);
      return boost::lexical_cast<std::string>(v);
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
