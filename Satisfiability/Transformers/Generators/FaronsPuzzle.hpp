// Oliver Kullmann, 28.2.2004 (Swansea)
/* Copyright 2004 - 2007, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Transformers/Generators/FaronsPuzzle.hpp
  \brief Tools for coding "Enigma 1277" as CNF.
  \deprecated Needs a complete update.

  \todo See script for CS_342, week 05.
*/

#ifndef FARONSPUZZLEWAECHTER_dh3524Rda1
#define FARONSPUZZLEWAECHTER_dh3524Rda1

#include <string>
#include <ostream>
#include <stdexcept>
#include <algorithm>
#include <cassert>

#include <boost/lexical_cast.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>


namespace FaronsPuzzle {

  class Test_score {

  public :
    typedef unsigned int Index;

  protected :
    const Index d; // dimension
    const Index k; // number components
    std::ostream& out;
    // derived:
    const Index n; // number variables
    const Index c; // number clauses

  public :
    struct wrong_dimension : std::invalid_argument {
      wrong_dimension(Index d) : invalid_argument("Dimension must be at least 2, but is " + boost::lexical_cast<std::string>(d)) {}
    };
    struct zero_components : std::invalid_argument {
      zero_components() : invalid_argument("The number of components required must be at least 1, but is 0") {}
    };
    struct too_many_components : std::invalid_argument {
      too_many_components(Index d, Index k) : invalid_argument("The number of components required must be at most " + boost::lexical_cast<std::string>(d*d) + ", but is " + boost::lexical_cast<std::string>(k)) {}
    };

    Test_score(const Index d, const Index k, std::ostream& out) : d(d), k(k), out(out), n(d * d * (1 + k)), c(d * d * ((k * (k-1)) / 2) + k + (4 * 3 + 4 * (d-2) * 5 + (d-2) * (d-2) * 8) * k * 2) {

      if (d <= 1) throw wrong_dimension(d);
      if (k == 0) throw zero_components();
      if (k > d*d) throw too_many_components(d, k);
    }

    void operator() () const {
      comment();
      ensure_different_components();
      ensure_component_chaining();
      ensure_enough_components();
      ensure_components_for_fields();
      initial_symmetry_breaking();
    }

    virtual ~Test_score() {}

  protected :
    virtual std::string b_var(const Index i, const Index j) const {
      assert(i != 0 and i <= d and j != 0 and j <= d);
      return "B" + boost::lexical_cast<std::string>(i) + "B" + boost::lexical_cast<std::string>(j);
    };
    virtual std::string c_var(const Index i, const Index j, const Index p) const {
      assert(i != 0 and i <= d and j != 0 and j <= d and p != 0 and p <= k);
      return "C" + boost::lexical_cast<std::string>(i) + "C" + boost::lexical_cast<std::string>(j) + "C" + boost::lexical_cast<std::string>(p);
    };
    virtual std::string neg(const std::string& var) const {
      return "-" + var;
    }
    virtual std::string eoc() const {
      return " 0\n";
    }
    virtual void comment() const {
      out << "c Enigma 1277; generator written by Oliver Kullmann, Swansea, February 2004\n";
      out << "c Dimension = " << boost::lexical_cast<std::string>(d) << "\n";
      out << "c Required score = " << boost::lexical_cast<std::string>(k) << "\n";
      out << "p cnf " << boost::lexical_cast<std::string>(n) << " " << boost::lexical_cast<std::string>(c) << "\n";
    }
    
  private :
    virtual void ensure_different_components() const {
      for (Index i = 1; i <= d; ++i)
	for (Index j = 1; j <= d; ++j)
	  for (Index p = 1; p < k; ++p)
	    for (Index p2 = p+1; p2 <= k; ++p2)
	      out << neg(c_var(i,j,p)) << " " << neg(c_var(i,j,p2)) << eoc();
    }
    virtual void ensure_component_chaining() const {
      for (Index i = 1; i <= d; ++i)
	for (Index j = 1; j <= d; ++j)
	  for (Index i2 = std::max(i-1, (Index)1); i2 <= std::min(i+1,d); ++i2)
	    for (Index j2 = std::max(j-1, (Index)1); j2 <= std::min(j+1,d); ++j2)
	      for (Index p = 1; p <= k; ++p)
		if (i2 == i and j2 == j) continue;
		else {
		  out << neg(c_var(i,j,p)) << " " << neg(b_var(i,j)) << " " << neg(b_var(i2,j2)) << " " << c_var(i2,j2,p) << eoc();
		  out << neg(c_var(i,j,p)) << " " << b_var(i,j) << " " << b_var(i2,j2) << " " << c_var(i2,j2,p) << eoc();
		}
    }
    virtual void ensure_enough_components() const {
      for (Index p = 1; p <= k; ++p) {
	for (Index i = 1; i <= d; ++i)
	  for (Index j = 1; j <= d; ++j)
	    out << c_var(i,j,p) << " ";
	out << eoc();
      }
    }
    virtual void ensure_components_for_fields() const {}
    virtual void initial_symmetry_breaking() const {}

  };


  class Test_score_simple_symmetry_breaking : public Test_score {
  public :
    Test_score_simple_symmetry_breaking(const Index d, const Index k, std::ostream& out) : Test_score(d, k, out) {}
  private :
    void initial_symmetry_breaking() const {
      out << c_var(1,1,1) << eoc();
      out << b_var(1,1) << eoc();
    }
  };

  class Test_score_extended_symmetry_breaking : public Test_score {
  public :
    Test_score_extended_symmetry_breaking(const Index d, const Index k, std::ostream& out) : Test_score(d, k, out), num_comp(d+1, d+1) {
      determine_restricted_fields();
    }
  private :
    typedef boost::numeric::ublas::matrix<Index> Matrix;
    mutable Matrix num_comp;
    // num_comp(i,j) is the number of different component numbers available for field (i, j)
    void determine_restricted_fields() const {
      Index akt_k = 1;
      for (Index i = 1; i <= d; ++i)
	for (Index j = 1; j <= d; ++j) {
	  num_comp(i,j) = akt_k;
	  akt_k = std::min(k, akt_k+1);
	}
    }
  protected :
    std::string c_var(const Index i, const Index j, const Index p) const {
      assert(i != 0 and i <= d and j != 0 and j <= d and p != 0 and p <= num_comp(i,j));
      return "C" + boost::lexical_cast<std::string>(i) + "C" + boost::lexical_cast<std::string>(j) + "C" + boost::lexical_cast<std::string>(p);
    };
    
  private :
    void ensure_different_components() const {
      for (Index i = 1; i <= d; ++i)
	for (Index j = 1; j <= d; ++j)
	  for (Index p = 1; p < num_comp(i,j); ++p)
	    for (Index p2 = p+1; p2 <= num_comp(i,j); ++p2)
	      out << neg(c_var(i,j,p)) << " " << neg(c_var(i,j,p2)) << eoc();
    }
    void ensure_component_chaining() const {
      for (Index i = 1; i <= d; ++i)
	for (Index j = 1; j <= d; ++j)
	  for (Index i2 = std::max(i-1, (Index)1); i2 <= std::min(i+1,d); ++i2)
	    for (Index j2 = std::max(j-1, (Index)1); j2 <= std::min(j+1,d); ++j2)
	      for (Index p = 1; p <= std::min(num_comp(i,j), num_comp(i2,j2)); ++p)
		if (i2 == i and j2 == j) continue;
		else {
		  out << neg(c_var(i,j,p)) << " " << neg(b_var(i,j)) << " " << neg(b_var(i2,j2)) << " " << c_var(i2,j2,p) << eoc();
		  out << neg(c_var(i,j,p)) << " " << b_var(i,j) << " " << b_var(i2,j2) << " " << c_var(i2,j2,p) << eoc();
		}
    }
    void ensure_enough_components() const {
      for (Index p = 1; p <= k; ++p) {
	for (Index i = 1; i <= d; ++i)
	  for (Index j = 1; j <= d; ++j)
	    if (p <= num_comp(i,j))
	      out << c_var(i,j,p) << " ";
	out << eoc();
      }
    }
    void ensure_components_for_fields() const {
      Index akt_k = 1;
      for (Index i = 1; i <= d; ++i)
	for (Index j = 1; j <= d; ++j) {
	  for (Index p = 1; p <= akt_k; ++p)
	    out << c_var(i,j,p) << " ";
	  out << eoc();
	  if (++akt_k > k)
	    goto End;
	}
    End: ;
    }

    void initial_symmetry_breaking() const {
      out << b_var(1,1) << eoc();
    }
  };

  enum Versions {basic, simple_symmetry_breaking, extended_symmetry_breaking };


}

#endif
