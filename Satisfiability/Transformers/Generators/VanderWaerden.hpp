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

namespace OKlib {
 namespace Satisfiability {
  namespace Transformers {
   namespace Generators {

   /*!
     \brief Computing the number of arithmetic progressions of length k in
     {1,...,n}

     The Maxima-specification is nhyp_arithprog_ohg(k,n) in
     ComputerAlgebra/Hypergraphs/Lisp/Generators/VanderWaerden.mac.
   */
   template <typename UInt>
   inline UInt nhyp_arithprog_hg(const UInt k, const UInt n) {
     if (k == 0) return 1;
     if (k == 1) return n;
     if (n < k) return 0;
     const UInt q = (n-1) / (k-1);
     return q * (n - ((k - 1) * (q + 1)) / 2);
   }

    /*!
      \class Arithmetical_progressions
      \brief All arithmetical progressions in {1,...,n}, in lexicographical order
      \deprecated Old, "quick and dirty" implementation, to be improved (via using a standard output interface).

      \detail

      Usage:
      <ul>
       <li> Construct an object
       <code>Arithmetical_progressions ap(k,n)</code>, where k is the length
       of the arithmetic progressions, to be considered in {1,...,n}. </li>
       <li> In ap.count the total number of ap's is to be found. </li>
       <li> Via calling ap.next() one then obtains the ap's, as vectors, in
       lexicographical order. </li>
       <li> It is an error if ap.next() is called more than ap.count many
       times. </li>
      </ul>

      \todo Integration
      <ul>
       <li> See classes in
       Combinatorics/Hypergraphs/Generators/plans/VanderWaerden.hpp and
       General/IteratorHandling.hpp. </li>
      </ul>
    */

    template <typename Int>
    class Arithmetical_progressions {
    public :
      typedef Int Index;
      const Index n;
      //! size of arithmetic progression in {1, ..., n}
      const Index k;
      //! maximal possible element of an arithmetic progression
      const Index max_element;
      //! total number of arithmetic progressions
      const Index count;

    private :
      //! first element in current arithmetic progression
      Index current_element;
      //! slope of current arithmetic progression
      Index current_distance;

    public :

      Arithmetical_progressions(const Index k, const Index n) :
          n(n), k(k),
          max_element(n-k+1),
          count(nhyp_arithprog_hg(k,n)),
          current_element(1),
          current_distance(1) {
        assert(k >= 1);
        assert(n >= 2);
        assert(n >= k);
      }

      typedef std::vector<Index> Arithmetical_progression;

      Arithmetical_progression next() {
          Arithmetical_progression ap;
        ap.reserve(k);
        for (Index i = 0; i < k; ++i)
	  ap.push_back(current_element + i * current_distance);
        if (current_element + (k-1) * (current_distance + 1) <= n and k >= 2)
	  ++current_distance;
        else {
	  ++current_element; current_distance = 1;
        }
        return ap;
      }
    };


    /*!
      \class VanderWaerden_TwoParts_1
      \brief Creation of vdW-SAT-problems for mixed binary problems
      \deprecated Old style

      \detail

      The Maxima-specification should be
      output_vanderwaerden2nd_stdname(k1,k2,n) in
      ComputerAlgebra/Satisfiability/Lisp/Generators/RamseyTheory/VanderWaerdenProblems.mac,
      however currently lexicographical order of the clauses is used here
      instead of colexicographical order.

      Usage:
      <ul>
       <li> Create an object <code>VanderWaerden_TwoParts_1 F(k1,k2,n,out).
       </li>
       <li> Via F() then the clause-set is output to stream out. </li>
      </ul>
    */
    class VanderWaerden_TwoParts_1 {
    public :
      typedef unsigned int Index;
      //! first size of arithmetic progression
      const Index k;
      //! second size of arithmetic progression
      const Index k2;
      //! number of vertices (size of counter-example)
      const Index n;
    private :
      std::ostream& out;
      typedef Arithmetical_progressions<Index> AP;
      AP ap, ap2;
      const Index number_ap, number_ap2;
    public :
      //! total number of clauses
      const Index c;

    public :
  
      VanderWaerden_TwoParts_1(
        const Index k,
        const Index k2,
        const Index n,
        std::ostream& out) :
          k(k), k2(k2), n(n), out(out), ap(k,n), ap2(k2,n), number_ap(ap.count), number_ap2(ap2.count), c(number_ap+number_ap2) {
          assert(k >= 1);
          assert(k2 >= 1);
          assert(n >= 2);
          }

      //! output of clause-set to out
      void operator() () {
        comment();
        clauses();
      }

      virtual ~VanderWaerden_TwoParts_1() {}

    private :

      void virtual comment() const {
        out << "c Van der Waerden numbers with partitioning into 2 parts; SAT generator written by Oliver Kullmann, Swansea, May 2004, October 2010.\n";
        if (k == k2)
          out << "c Arithmetical progression size k = " << boost::lexical_cast<std::string>(k) << ".\n";
        else
          out << "c Arithmetical progression sizes k1 = " << boost::lexical_cast<std::string>(k) << ", k2 = " << boost::lexical_cast<std::string>(k2) << ".\n";
        out << "c Number of elements n = " << boost::lexical_cast<std::string>(n) << ".\n";
        out << "p cnf " << boost::lexical_cast<std::string>(n) << " " << boost::lexical_cast<std::string>(c) << "\n";
      }
      void clauses() {
        for (Index i = 0; i < number_ap; ++i) {
	  const AP::Arithmetical_progression p = ap.next();
	  for (AP::Arithmetical_progression::const_iterator i = p.begin(); i != p.end(); ++i)
	    out << " " << var(*i);
	  out << eoc();
        }
        for (Index i = 0; i < number_ap2; ++i) {
	  const AP::Arithmetical_progression p = ap2.next();
	  for (AP::Arithmetical_progression::const_iterator i = p.begin(); i != p.end(); ++i)
	    out << " " << neg(var(*i));
	  out << eoc();
        }
      }

    protected :

      //! index to variable
      std::string var(const Index v) const {
        assert(v != 0);
        return boost::lexical_cast<std::string>(v);
      }
      //! negated variable
      virtual std::string neg(const std::string& var) const {
        return "-" + var;
      }
      //! end-of-clause
      virtual std::string eoc() const {
        return " 0\n";
      }

    };
   }
  }
 }
}

#endif
