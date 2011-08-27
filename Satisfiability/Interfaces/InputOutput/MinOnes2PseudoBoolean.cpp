// Matthew Gwynne, 16.3.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Interfaces/InputOutput/MinOnes2PseudoBoolean.cpp
  \brief Application to translate a CNF into a pseudo-boolean encoding of the "minimum ones" problem for that CNF.

  <ul>
   <li> Reads from standard input, and writes to standard output (and standard
   error). </li>
   <li> The old parameter line is translated from:
   \verbatim
p cnf n c
   \endverbatim
   to
   \verbatim
* #variable= n #constraint= c
   \endverbatim
   </li>
   <li> Any comments occurring before the p-line are output as comments (i.e.,
   prefixed with "* ") after the pseudo-boolean parameter line. </li>
   <li> All clauses in the input are translated from CNF clauses of the form,
   e.g.,
   \verbatim
1 2 3 4 -5 0
   \endverbatim
   to non-linear pseudo-boolean constraints of the form, e.g.,
   \verbatim
+1 x1 +1 x2 +1 x3 +1 x4 +1 ~x5 >= 1;
   \endverbatim
   </li>
   <li> After the "* #variable ..." line, the application outputs the
   optimisation line
   \verbatim
min: +1 x1 ... +1 xn ;
   \endverbatim
   where n is the maximal possible variable limit. This specifies that
   the problem is to find an assignment to variables minimising the number
   of ones in the assignment.
   </li>
  </ul>


  \todo Improve specification
  <ul>
   <li> A full specification of what the "minimum ones" problem is,
   should be given. </li>
   <li> A full specification of the optimum solutions to the output
   pseudo-boolean formula should be given. </li>
   <li> A specification of the pseudo-boolean file format should be
   given. </li>
   <li> See http://www.cril.univ-artois.fr/PB11/format.pdf . </li>
  </ul>


  \todo Move to correct place
  <ul>
   <li> Satisfiability/Interfaces/InputOutput/ is likely not
   the right place for this application. </li>
  </ul>

*/

#include <string>
#include <iostream>
#include <cstdlib>

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/range/iterator.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/range/value_type.hpp>

#include <OKlib/Satisfiability/Interfaces/InputOutput/Dimacs.hpp>
#include <OKlib/Satisfiability/Interfaces/InputOutput/ClauseSetAdaptors.hpp>


namespace {

  const std::string program = "MinOnes2PseudoBoolean";
  const std::string err = "ERROR[" + program + "]: ";

  const std::string version = "0.0.10";

  /*!
    \class CLSAdaptorMinOnes2PseudoBoolean
    \brief Adaptor for clause-sets for output in DIMACS format

    Parameter n is considered as maximal possible variable index,
    while parameter c is considered as upper bound on the number of clauses.

    The role of the template parameters are as follows:
    <ol>
     <li> Int is the integral type for the Dimacs literals. </li>
     <li> String is the string type for comments. </li>
     <li> AdaptorStatistics is a CLSAdaptor which handles statistics. </li>
    </ol>

  */

  template <typename Int = int, class String = std::string, class AdaptorStatistics = OKlib::InputOutput::CLSAdaptorStatistics<Int, String> >
  class CLSAdaptorMinOnes2PseudoBooleanOutput {
  public:
    typedef String string_type;

  private:
    typedef AdaptorStatistics adaptor_statistics_type;
    std::ostream& out;
    adaptor_statistics_type adaptor_statistics;
    string_type comments;
    bool p_line_read;

  public :
    typedef Int int_type;
    typedef typename adaptor_statistics_type::statistics_type statistics_type;

    CLSAdaptorMinOnes2PseudoBooleanOutput(std::ostream& out) :
      out(out), p_line_read(false) {
      if (not out)
        throw OKlib::InputOutput::OStreamError("OKlib::InputOutput::CLSAdaptorMinOnes2PseudoBooleanOutput::CLSAdaptorMinOnes2PseudoBooleanOutput(std::ostream&):\n  cannot open the output stream");
    }

    void comment(const string_type& s) {
      adaptor_statistics.comment(s);
      if (s.empty()) comments = comments + "*\n";
      else
        if (boost::algorithm::is_space()(s[0]))
          comments = comments + "*" + s + "\n";
        else comments = comments + "* " + s + "\n";
      if (p_line_read) out << comments;
    }
    void n(const int_type pn) {
      adaptor_statistics.n(pn);
      if (pn < 0)
        throw OKlib::InputOutput::ParameterOutputError("OKlib::InputOutput::CLSAdaptorMinOnes2PseudoBooleanOutput::n:\n  maximal variable index is a negative quantity = " + boost::lexical_cast<std::string>(pn));
    }
    void c(const int_type pc) {
      adaptor_statistics.c(pc);
      if (pc < 0)
        throw OKlib::InputOutput::ParameterOutputError("OKlib::InputOutput::CLSAdaptorMinOnes2PseudoBooleanOutput::n:\n  number of clauses is a negative quantity = " + boost::lexical_cast<std::string>(pc));
      const int_type n = adaptor_statistics.stat.pn();
      out << "* #variable= " << n << " #constraint= " << adaptor_statistics.stat.pc() + n << "\n";
      out << comments;
      out << "min: ";
      for (int_type i = 1; i <= n; ++i) out << "+1 x" << i << " ";
      out << ";\n";
      p_line_read = true;
    }
    void finish() { adaptor_statistics.finish(); }
    void tautological_clause(const int_type t) {
      adaptor_statistics.tautological_clause(t);
    }
    template <class ForwardRange>
    void clause(const ForwardRange& r, const int_type t) {
      adaptor_statistics.clause(r, t);
      if (adaptor_statistics.stat.c() > adaptor_statistics.stat.pc())
        throw OKlib::InputOutput::ClauseOutputError("OKlib::InputOutput::CLSAdaptorMinOnes2PseudoBooleanOutput::clause:\n  number of non-tautological clauses exceeds specified total number of clauses = " + boost::lexical_cast<std::string>(adaptor_statistics.stat.pc()));
      typedef typename boost::range_const_iterator<ForwardRange>::type const_iterator;
      const const_iterator& end(boost::end(r));
      for (const_iterator i = boost::begin(r); i != end; ++i) {
        typedef typename boost::range_value<ForwardRange>::type value_type;
        const value_type& literal = *i;
        if (std::abs(literal) > adaptor_statistics.stat.pn())
          throw OKlib::InputOutput::ClauseOutputError("OKlib::InputOutput::CLSAdaptorMinOnes2PseudoBooleanOutput::clause:\n  variable index of literal = " + boost::lexical_cast<std::string>(literal) + " exceeds specified maximal index = " + boost::lexical_cast<std::string>(adaptor_statistics.stat.pn()));

        if (literal > 0) out << " +1 x" << literal << " ";
        else out << " +1 ~x" << literal << " ";
      }
      out << " >= 1;\n";
    }

  };

}

int main() {
  typedef CLSAdaptorMinOnes2PseudoBooleanOutput<> CLSAdaptor;
  CLSAdaptor output(std::cout);
  OKlib::InputOutput::StandardDIMACSInput<CLSAdaptor >(std::cin, output);
}
