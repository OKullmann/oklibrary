// Matthew Gwynne, 4.1.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Interfaces/InputOutput/FilterDimacs.cpp
  \brief Application to select clauses from a clause-set

  <ul>
   <li> Reads a Dimacs clause-set from standard input, and writes to standard
   output (and standard error). </li>
   <li> The only argument is a path to the file containing integer clause
   indices in the clause-set. </li>
   <li> These indices are positive integers; if they are too big then they
   are ignored. </li>
   <li> The result, given on standard output, is a Dimacs file containing
   the i-th clauses for all i occurring in the clause-positions-file. </li>
   <li> Any non-integer characters (including "-") in the
   clause-positions-file will be ignored. </li>
   <li> Negative integers are also ignored. This allows v-lines from SAT
   solvers put to a file and used directly without removing any characters.
   </li>
  </ul>


  \todo Add application tests

  \todo Weak implementation
  <ul>
   <li> The clause-index-container passed to the constructor obviously
   shouldn't be copied. </li>
   <li> Clause-indices should use an unsigned type. </li>
   <li> All indices passed to the constructor are assumed to be at least 1.
   </li>
   <li> Reading in of the clause-indices is awkward. </li>
  </ul>


  \todo Move CLSAdaptorFilter
  <ul>
   <li> CLSAdaptorFilter should likely go to
   Interfaces/InputOutput/ClauseSetAdaptors.hpp. </li>
   <li> Additionally, one should consider renaming it, as it is not a general
   filter, but a very specific one. </li>
   <li> On the other hand, a general filter which takes a predicate, similar
   to functions like std::remove_if etc might in fact be better. </li>
   <li> It is also not a "filter" (which would remove clauses) but a
   "selector". </li>
  </ul>

*/

#include <string>
#include <fstream>
#include <set>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <functional>

#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/range/algorithm.hpp>

#include <OKlib/Satisfiability/Interfaces/InputOutput/Dimacs.hpp>
#include <OKlib/Satisfiability/Interfaces/InputOutput/ClauseSetAdaptors.hpp>


namespace OKlib {
  namespace InputOutput {

    template <typename Int = int, class String = std::string,
              class CLSAdaptor = OKlib::InputOutput::CLSAdaptorDIMACSOutput<> >
    class CLSAdaptorFilter {

    public :

      typedef Int int_type;
      typedef String string_type;
      typedef CLSAdaptor cls_adaptor_type;

      typedef std::set<int_type> clause_index_container_type;

    private :

      cls_adaptor_type cls_adaptor;
      clause_index_container_type clause_index;
      int_type current_clause;

    public :

      template <typename Range>
      CLSAdaptorFilter(const Range& clause_index_arg, cls_adaptor_type& cls_adaptor_arg) : cls_adaptor(cls_adaptor_arg), current_clause(0) {
        boost::copy(clause_index_arg,
                    std::inserter(clause_index,
                                  boost::begin(clause_index)));
      }

      void comment(const string_type& s) { cls_adaptor.comment(s); }
      void n(const int_type pn) { cls_adaptor.n(pn); }
      void c(const int_type pc) {
        // Remove any clause-indices which are too large
        clause_index_container_type temp_clause_index;
        std::remove_copy_if(boost::begin(clause_index),
                            boost::end(clause_index),
                            std::inserter(temp_clause_index,
                                          boost::begin(temp_clause_index)),
                            std::bind2nd(std::greater<int_type>(), pc));
        clause_index.clear();
        std::remove_copy_if(boost::begin(temp_clause_index),
                            boost::end(temp_clause_index),
                            std::inserter(clause_index,
                                          boost::begin(clause_index)),
                            std::bind2nd(std::less<int_type>(), 1));

        cls_adaptor.c(clause_index.size());
      }
      void finish() { cls_adaptor.finish(); }
      void tautological_clause(const int_type t) {
        ++current_clause;
        if (clause_index.find(current_clause) != clause_index.end())
          cls_adaptor.tautological_clause(t);
      }
      template <class ForwardRange>
      void clause(const ForwardRange& r, const int_type t) {
        ++current_clause;
        if (clause_index.find(current_clause) != clause_index.end())
          cls_adaptor.clause(r,t);
      }
    };
  }
}


namespace {

  enum {
    error_parameters = 1,
    error_openfile = 2
  };

  const std::string program = "FilterDimacs";
  const std::string err = "ERROR[" + program + "]: ";

  const std::string version = "0.0.6";

}

int main(const int argc, const char* const argv[]) {

  if (argc != 2) {
    std::cerr << err << "Exactly one inputs is "
      "required, the  name of the file containing the list of "
      "clause positions. The Dimacs file should be given on standard input.\n";
    return error_parameters;
  }

  std::ifstream f_in(argv[1]);
  if (not f_in) {
    std::cerr << err << "Failure opening file " << argv[1] << ".\n";
    return error_openfile;
  }

  typedef OKlib::InputOutput::CLSAdaptorFilter<> CLSAdaptorFilter;

  CLSAdaptorFilter::clause_index_container_type clause_index;
  while (not f_in.eof()) {
    while ( (((char)f_in.peek() > '9') or
            ((char)f_in.peek() < '0')) and
            ((char)f_in.peek() != '-') and
            not f_in.eof()) {
      f_in.seekg(1,f_in.cur);
    }
    CLSAdaptorFilter::int_type i;
    f_in >> i;
    if (not f_in.fail() and (i > 0)) clause_index.insert(i);
  }
  f_in.close();

  CLSAdaptorFilter::cls_adaptor_type output(std::cout);
  CLSAdaptorFilter filter(clause_index, output);
  OKlib::InputOutput::StandardDIMACSInput<CLSAdaptorFilter>(std::cin, filter);
}
