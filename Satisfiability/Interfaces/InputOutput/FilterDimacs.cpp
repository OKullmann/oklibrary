// Matthew Gwynne, 4.1.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Interfaces/InputOutput/FilterDimacs.cpp
  \brief Application to filter out the given numbered clauses in a clause-set.


  <ul>
   <li> Reads a Dimacs clause-set from standard input, and writes to standard 
   output (and standard error). </li>
   <li> The only argument is the path to a file containing integer clause 
   positions in the clause-set. </li>
   <li> The file containing clause positions should simply be a list of 
   positive integers between 1 and the number of clauses given on standard
   input. </li>
   <li> The result, given on standard output is a Dimacs file containing 
   the i-th clauses for all i occurring in the clause positions file. </li>
   <li> Any non-integer characters (including "-") in the clause positions 
   file will be ignored. </li>
   <li> Negative integers are also ignored. This allows v-lines from SAT 
   solvers put to a file and used directly without removing any characters. 
   </li>
  </ul>


  \todo Add application tests

  
  \todo Move CLSAdaptorFilter
  <ul>
   <li> CLSAdaptorFilter should likely go to 
   Interfaces/InputOutput/ClauseSetAdaptors.hpp. </li>
   <li> Additionally, one should consider renaming it, as it is not a general
   filter, but a very specific one. </li>
   <li> On the other hand, a general filter which takes a predicate, similar
   to functions like std::remove_if etc might in fact be better. </li>
  </ul>

*/

#include <string>
#include <ostream>
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
      
      typedef std::set<Int> clause_numbers_container_type;
      
      CLSAdaptor cls_adaptor;
      clause_numbers_container_type clause_numbers;
      Int current_clause;

    public :
      
      typedef Int int_type;
      typedef String string_type;
      template <typename Range>
      CLSAdaptorFilter(Range clause_numbers_arg, CLSAdaptor cls_adaptor_arg) : cls_adaptor(cls_adaptor_arg), current_clause(0) {
        boost::copy(clause_numbers_arg, 
                    std::inserter(clause_numbers, 
                                  boost::begin(clause_numbers)));
      }
 
      void comment(const string_type& s) {
        cls_adaptor.comment(s);
      }
      void n(const int_type pn) {
        cls_adaptor.n(pn);
      }
      void c(const int_type pc) {
        // Remove any line numbers which are too large
        clause_numbers_container_type temp_clause_numbers;
        std::remove_copy_if(boost::begin(clause_numbers),
                            boost::end(clause_numbers),
                            std::inserter(temp_clause_numbers, 
                                          boost::begin(temp_clause_numbers)),
                            std::bind2nd(std::greater<int_type>(), pc));
        clause_numbers.clear();
        std::remove_copy_if(boost::begin(temp_clause_numbers),
                            boost::end(temp_clause_numbers),
                            std::inserter(clause_numbers, 
                                          boost::begin(clause_numbers)),
                            std::bind2nd(std::less<int_type>(), 1));
        
        cls_adaptor.c(clause_numbers.size());
      }
      void finish() {
        cls_adaptor.finish();
      }
      void tautological_clause(const int_type t) {
        ++current_clause;
        if (clause_numbers.find(current_clause) != clause_numbers.end()) {
          cls_adaptor.tautological_clause(t);
        }
      }
      template <class ForwardRange>
      void clause(const ForwardRange& r, const int_type t) {
        ++current_clause;
        if (clause_numbers.find(current_clause) != clause_numbers.end()) {
          cls_adaptor.clause(r,t);
        }
      }
    };
  }
}


namespace {

  enum {
    error_parameters = 1,
    error_openfile = 2,
    error_readfile = 3
  };

  const std::string version = "0.0.1";

}

int main(const int argc, const char* const argv[]) {
  typedef OKlib::InputOutput::CLSAdaptorDIMACSOutput<> CLSAdaptor;
  typedef OKlib::InputOutput::CLSAdaptorFilter<> CLSAdaptorFilter;
  typedef int int_type;
  typedef std::set<int_type> clause_numbers_container_type;

  

  // Read in list of line numbers
  if (argc != 2) {
    std::cerr << "ERROR[FilterDimacs]: Exactly one inputs is "
      "required, the  name of the file containing the list of "
      "clause positions. The Dimacs file should be given on "
      "standard input.\n";
    return error_parameters;
  }

  std::ifstream f_in(argv[1]);
  if (not f_in) {
    std::cerr << "ERROR[FilterDimacs]: Failure opening file " << 
      argv[1] << ".\n";
    return error_openfile;
  }

  clause_numbers_container_type clause_numbers;
  while (not f_in.eof()) {
    while ( (((char)f_in.peek() > '9') or 
            ((char)f_in.peek() < '0')) and
            ((char)f_in.peek() != '-') and 
            not f_in.eof()) {
      f_in.seekg(1,f_in.cur);
    }
    int_type i; 
    f_in >> i; 
    if (not f_in.fail() and (i > 0)) {
      clause_numbers.insert(i);
    }
  }
  f_in.close();

  // Filter clause set and output only those clauses which are numbered.
  CLSAdaptor output(std::cout);
  CLSAdaptorFilter filter(clause_numbers, output);
  OKlib::InputOutput::StandardDIMACSInput<CLSAdaptorFilter>(std::cin, filter);
}
