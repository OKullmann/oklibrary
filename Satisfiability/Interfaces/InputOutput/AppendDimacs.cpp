// Matthew Gwynne, 3.2.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Interfaces/InputOutput/AppendDimacs.cpp
  \brief Application to select clauses from a clause-set


  <ul>
   <li> Takes an arbitrary number of Dimacs files as arguments, and outputs on
   standard output a Dimacs CNF containing the clauses from all of the given
   clause-lists, in the order the arguments are given and in their original
   order in their respective inputs. </li>
   <li> First all comments are output, from each input clause-set in the
   order they are given as arguments, then the p-line, and then the
   clauses. </li>
   <li> All formatting (spacing, indentation etc) from input files is lost.
   </li>
   <li> The literals in each clause are output in numerical order, not in the
   order they are input (i.e., "4 1 -3 2 0" becomes "-3 1 2 4 0"). </li>
   <li> Tautological clauses and repeated literals are removed. </li>
   <li> The maximal variable-index given on the p-line of the output is
   the maximum of all variable-indices across the given clause-lists. </li>
   <li> The number of clauses given on the p-line of the output is the
   sum of the number of clauses for each clause-list given as an argument.
   </li>
   <li> Both (output) p-line values are computed after the removal of
   tautological clauses and repeated literals. </li>
  </ul>


  \todo Move CLSAdaptorAppend
  <ul>
   <li> CLSAdaptorAppend should likely go to 
   Interfaces/InputOutput/ClauseSetAdaptors.hpp. </li>
   <li> Additionally, one should consider renaming it, as it is not a general
   filter, but a very specific one. </li>
   <li> On the other hand, a general filter which takes a predicate, similar
   to functions like std::remove_if etc might in fact be better. </li>
  </ul>


  \bug Incomplete specification : DONE
  <ul>
   <li> What does "maximum number of variables" mean?? </li>
   <li> It should be the maximal variable-*index*. </li>
   <li> What is the strange requirement "that the variable set of every
   argument clause-list is a subset of the variable set of the clause-list
   with the most variables" ?? </li>
   <li> This is very likely not meant. </li>
  </ul>

*/

#include <string>
#include <fstream>
#include <iostream>

#include <OKlib/Satisfiability/Interfaces/InputOutput/Dimacs.hpp>
#include <OKlib/Satisfiability/Interfaces/InputOutput/ClauseSetAdaptors.hpp>


namespace {

  enum { error_openfile = 1 };

  const std::string program = "AppendDimacs";
  const std::string err = "ERROR[" + program + "]: ";

  const std::string version = "0.0.8";

  template <typename Int = int, class String = std::string,
            class OutputCLSAdaptor = OKlib::InputOutput::CLSAdaptorDIMACSOutput<> >
  class CLSAdaptorAppend {
  public :
    typedef Int int_type;
    typedef String string_type;
    typedef OutputCLSAdaptor output_cls_adaptor_type;

  private :
    typedef OKlib::InputOutput::RawDimacsCLSAdaptor<> stored_cls_adaptor_type;
    typedef stored_cls_adaptor_type::clause_type stored_clause_type;
    output_cls_adaptor_type output_cls_adaptor;
    stored_cls_adaptor_type stored_cls_adaptor;

  public :
    CLSAdaptorAppend(output_cls_adaptor_type& cls_adaptor) :
      output_cls_adaptor(cls_adaptor) {}

    void comment(const string_type& s) { output_cls_adaptor.comment(s); }

    void n(const int_type pn) {}
    void c(const int_type pc) {}
    void finish() {}
    void tautological_clause(const int_type t) {}

    template <class ForwardRange>
    void clause(const ForwardRange& r, const int_type t) {
      stored_cls_adaptor.clause(r,t);
    }

    void flush() {
      OKlib::InputOutput::ListTransfer<output_cls_adaptor_type>(
        stored_cls_adaptor.clause_set,output_cls_adaptor,"Result of append-operation.");
    }
  };

}

int main(const int argc, const char* const argv[]) {
  typedef CLSAdaptorAppend<> AdaptorAppend;
  typedef AdaptorAppend::output_cls_adaptor_type OutputAdaptor;
  OutputAdaptor output(std::cout);
  AdaptorAppend append_cls(output);

  for (int i = 1; i < argc; ++i) {
    std::ifstream f_in(argv[i]);
    if (not f_in) {
      std::cerr << err << "Failure opening file " << argv[i] << ".\n";
      return error_openfile;
    }
    OKlib::InputOutput::StandardDIMACSInput<AdaptorAppend>(f_in, append_cls);
  }
  append_cls.flush();
}
