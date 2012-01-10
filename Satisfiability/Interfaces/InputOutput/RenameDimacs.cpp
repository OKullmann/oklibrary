// Matthew Gwynne, 9.1.2012 (Swansea)
/* Copyright 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Interfaces/InputOutput/RenameDimacs.cpp
  \brief Application to apply a variable renaming to a clause-set

  <ul>
   <li> Reads a Dimacs clause-set from standard input, and writes to standard
   output (and standard error). </li>
   <li> The only argument is a path to a file containing a list of
   non-zero (signed) integers, terminated by zero. There should be a non-zero
   integer in this list for each variable in the input clause-set. </li>
   <li> The list of integers defines a renaming of the variables from
   the input clause-set. </li>
   <li> The variable with index i in the input clause-set is mapped to
   the i-th literal given in the list of integers (literals). </li>
   <li> The result, given on standard output, is the input Dimacs file
   having applied this renaming. </li>
   <li> The literals in each clause are output in numerical order, not in the
   order they are input (i.e., "4 1 -3 2 0" becomes "-3 1 2 4 0"). </li>
   <li> The maximum variable index on the p-line of the output
   is the maximum variable index of the output clause-set. </li>
   <li> The specification for this function is given by
   rename_fcs in
   ComputerAlgebra/Satisfiability/Lisp/ClauseSets/Substitutions.mac
   at the Maxima level. </li>
  </ul>


  \todo Move CLSAdaptorRename
  <ul>
   <li> CLSAdaptorRename should likely go to
   Interfaces/InputOutput/ClauseSetAdaptors.hpp. </li>
  </ul>

*/

#include <string>
#include <fstream>
#include <iostream>
#include <iterator>
#include <list>
#include <map>

#include <boost/foreach.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/range/algorithm.hpp>

#include <OKlib/Satisfiability/Interfaces/InputOutput/Dimacs.hpp>
#include <OKlib/Satisfiability/Interfaces/InputOutput/ClauseSetAdaptors.hpp>


namespace OKlib {
  namespace InputOutput {

    template <typename Map, typename Int = int, class String = std::string,
              class CLSAdaptor = OKlib::InputOutput::CLSAdaptorDIMACSOutput<> >
    class CLSAdaptorRename {

    public :

      typedef Int int_type;
      typedef OKlib::Literals::Literals_int lit_type;
      typedef String string_type;
      typedef CLSAdaptor cls_adaptor_type;

      const Map renaming;

    private :

      cls_adaptor_type cls_adaptor;

    public :


      CLSAdaptorRename(const Map& renaming_arg, cls_adaptor_type& cls_adaptor_arg) : renaming(renaming_arg), cls_adaptor(cls_adaptor_arg) {
        /* BOOST_CONCEPT_ASSERT(( ReadablePropertyMapConcept<Map, lit_type> )); */
      }

      void comment(const string_type& s) { cls_adaptor.comment(s); }
      void n(const int_type pn) {
        int_type new_pn = 0;
        for (int i = 1; i <= pn; ++i)
          new_pn = std::max(boost::get(renaming,i), new_pn);
        cls_adaptor.n(new_pn);
      }
      void c(const int_type pc) { cls_adaptor.c(pc);  }
      void finish() { cls_adaptor.finish(); }
      void tautological_clause(const int_type t) { cls_adaptor.tautological_clause(t); }
      template <class ForwardRange>
      void clause(const ForwardRange& r, const int_type t) {
        std::set<lit_type> new_r;
        BOOST_FOREACH( lit_type l, r) {
          if (l > 0) new_r.insert(boost::get(renaming,l));
          else new_r.insert(-boost::get(renaming,-l));
        }
        cls_adaptor.clause(new_r, t);
      }
    };
  }
}


namespace {

  enum {
    error_parameters = 1,
    error_openfile = 2
  };

  const std::string program = "RenameDimacs";
  const std::string err = "ERROR[" + program + "]: ";

  const std::string version = "0.0.4";

}

int main(const int argc, const char* const argv[]) {

  if (argc != 2) {
    std::cerr << err << "Exactly one inputs is "
      "required, the  name of the file containing the variable renaming, "
      "as a list of literals. The Dimacs file to be renamed should be given "
      "on standard input.\n";
    return error_parameters;
  }

  std::ifstream f_in(argv[1]);
  if (not f_in) {
    std::cerr << err << "Failure opening file " << argv[1] << ".\n";
    return error_openfile;
  }

  typedef int int_type;
  typedef std::map<int, int> raw_map_type; /* Vars -> Lits */
  typedef boost::associative_property_map< raw_map_type > map_type;
  typedef OKlib::InputOutput::CLSAdaptorRename<map_type> CLSAdaptorRename;


  raw_map_type raw_renaming_map;
  map_type renaming_map(raw_renaming_map);
  CLSAdaptorRename::int_type var_index = 0;
  while (not f_in.eof()) {
    CLSAdaptorRename::int_type renamed_var;
    f_in >> renamed_var;
    if (renamed_var == 0) break;
    raw_renaming_map[++var_index] = renamed_var;
  }
  f_in.close();

  CLSAdaptorRename::cls_adaptor_type output(std::cout);
  CLSAdaptorRename renaming(renaming_map, output);
  OKlib::InputOutput::StandardDIMACSInput<CLSAdaptorRename>(std::cin, renaming);
}
