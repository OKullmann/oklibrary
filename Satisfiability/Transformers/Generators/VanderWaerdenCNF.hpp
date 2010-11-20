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

  \todo Use InputOutput::CLSAdaptorDIMACSOutput
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
      \class VanderWaerden_TwoParts_1
      \brief Creation of vdW-SAT-problems for mixed binary problems
      \deprecated Old style

      \detail

      The Maxima-specification is
      output_vanderwaerden2nd_stdname(k1,k2,n) in
      ComputerAlgebra/Satisfiability/Lisp/Generators/RamseyTheory/VanderWaerdenProblems.mac,
      when using APit = Combinatorics::Hypergraphs::Generators::Arithmetical_progressions_colex.

      Usage:
      <ul>
       <li> Create an object <code>VanderWaerden_TwoParts_1 F(k1,k2,n,out).
       </li>
       <li> Via F() then the clause-set is output to stream out. </li>
      </ul>
    */
    template <typename UInt, template <typename> class APit>
    class VanderWaerden_TwoParts_1 {
    public :
      typedef UInt Index;
      typedef APit<Index> ArithProg;
      //! first size of arithmetic progression
      const Index k;
      //! second size of arithmetic progression
      const Index k2;
      //! number of vertices (size of counter-example)
      const Index n;
    private :
      std::ostream& out;
      ArithProg ap, ap2;
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
        out << "c " << ArithProg::message() << "\n";
        out << "p cnf " << boost::lexical_cast<std::string>(n) << " " << boost::lexical_cast<std::string>(c) << "\n";
      }
      void clauses() {
        typedef typename ArithProg::Arithmetical_progression ap_type;
        typedef typename ap_type::const_iterator ap_iterator;
        for (Index i = 0; i < number_ap; ++i) {
	  const ap_type p = ap.next();
	  for (ap_iterator i = p.begin(); i != p.end(); ++i)
	    out << var(*i) << " ";
	  out << eoc();
        }
        for (Index i = 0; i < number_ap2; ++i) {
	  const ap_type p = ap2.next();
	  for (ap_iterator i = p.begin(); i != p.end(); ++i)
	    out << neg(var(*i)) << " ";
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
        return "0\n";
      }

    };
   }
  }
 }
}

#endif
