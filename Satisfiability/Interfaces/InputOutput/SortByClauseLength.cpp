// Matthew Gwynne, 20.1.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Interfaces/InputOutput/SortByClauseLength.cpp
  \brief Application to sort clause-lists by clause length.

  <ul>
   <li> Reads a Dimacs clause-list from standard input, and writes to standard
   output (and standard error). </li>
   <li> The result, given on standard output, is a Dimacs file containing
   containing the input clause-list after applying a stable sort on the
   lengths of the clauses (in ascending order). </li>
  </ul>


  \todo Move CLSAdaptorSortByClauseLength
  <ul>
   <li> This should likely go to into ClauseSetAdaptors.hpp. </li>
  </ul>


  \todo Move AllEqual to OrderConstructions
  <ul>
    <li> The AllEqual functor should likely go to
    Programming/Utilities/OrderRelations/OrderConstructions.hpp .
    </li>
  </ul>


*/

#include <iostream>
#include <algorithm>
#include <functional>

#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/range/algorithm.hpp>

#include <OKlib/Satisfiability/Interfaces/InputOutput/Dimacs.hpp>
#include <OKlib/Satisfiability/Interfaces/InputOutput/ClauseSetAdaptors.hpp>
#include <OKlib/Programming/Utilities/OrderRelations/OrderConstructions.hpp>


namespace OKlib {
  namespace InputOutput {

      /*!
        \class AllEqual
        \brief Predicate-functor which is constant false for all inputs.

        This is useful as a comparison functor where you wish all objects
        to be considered equal.

      */
      template <class Object>
      struct AllEqual : std::binary_function<
        const Object, const Object, bool> {
        bool operator() (const Object& A, const Object& B) const {
          return false;
        }
      };

    /*!
       \class CLSAdaptorSortByClauseLength
       \brief Adaptor which sorts the clause-list given to it.

       Sorts the given clause-list in ascending order of the size of clauses,
       maintaining the order within each size group (i.e., stable sort),
       outputting to the given output.

    */
    template <typename Int = int, class String = std::string,
              class OutputCLSAdaptor = OKlib::InputOutput::CLSAdaptorDIMACSOutput<>,
              class OrderComparator = OKlib::Programming::Utilities::OrderRelations::SizeLessThan<AllEqual<RawDimacsCLSAdaptor<>::clause_type > > >
    class CLSAdaptorSortByClauseLength {
    public :

      typedef Int int_type;
      typedef String string_type;
      typedef OutputCLSAdaptor output_cls_adaptor_type;
      typedef RawDimacsCLSAdaptor<> stored_cls_adaptor_type;
      typedef stored_cls_adaptor_type::clause_type stored_clause_type;
      typedef OrderComparator order_comparator_type;

      CLSAdaptorSortByClauseLength(output_cls_adaptor_type& cls_adaptor_arg) :
        output_cls_adaptor(cls_adaptor_arg), stored_cls_adaptor() {}

      void comment(const string_type& s) { output_cls_adaptor.comment(s); }
      void n(const int_type pn) { stored_cls_adaptor.n(pn); }
      void c(const int_type pc) { stored_cls_adaptor.c(pc); }

      void finish() {
        stored_cls_adaptor.finish();
        typedef stored_cls_adaptor_type::clause_set_type cls_type;
        std::stable_sort(stored_cls_adaptor.clause_set.begin(),
                         stored_cls_adaptor.clause_set.end(),
                         order_comparator_type());
        ListTransfer<output_cls_adaptor_type>(stored_cls_adaptor.clause_set,
                                              output_cls_adaptor,
                                              "Sorted by length of clauses.");
      }

      void tautological_clause(const int_type t) {
        stored_cls_adaptor.tautological_clause(t);
      }
      template <class ForwardRange>
      void clause(const ForwardRange& r, const int_type t) {
        stored_cls_adaptor.clause(r,t);
      }

    private:

      output_cls_adaptor_type output_cls_adaptor;
      stored_cls_adaptor_type stored_cls_adaptor;

    };
  }
}


namespace {

  const std::string program = "SortByClauseLength";
  const std::string err = "ERROR[" + program + "]: ";

  const std::string version = "0.0.3";

}

int main(const int argc, const char* const argv[]) {
  typedef OKlib::InputOutput::CLSAdaptorDIMACSOutput<> CLSAdaptor;
  CLSAdaptor output(std::cout);
  typedef OKlib::InputOutput::CLSAdaptorSortByClauseLength<> CLSSortAdaptor;
  CLSSortAdaptor sorter(output);
  OKlib::InputOutput::StandardDIMACSInput<CLSSortAdaptor>(std::cin, sorter);
}
