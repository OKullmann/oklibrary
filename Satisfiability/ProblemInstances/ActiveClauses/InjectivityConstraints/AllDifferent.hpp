// Matthew Henderson, 24.4.2006 (Paderborn)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ActiveClauses/InjectivityConstraints/AllDifferent.hpp
  \brief Algorithms for all-different constraint.
*/

#ifndef ALLDIFFERENT_08s7dg8O2
#define ALLDIFFERENT_08s7dg8O2

#include <vector>
#include <set>
#include <cassert>

#include <boost/logic/tribool.hpp>

namespace OKlib {
  namespace InjectivityConstraints {

    /*!
      \class TrivialAllDifferent
      \brief Very first prototype for an all-different constraint (just returning true or false).
    */

    template <class PASS>
    struct TrivialAllDifferent {
      typedef PASS partial_assignments_type;
      typedef typename partial_assignments_type::variables_type variables_type;
      typedef typename partial_assignments_type::value_type value_type;
      typedef typename partial_assignments_type::domain_type domain_type;

    private :
      typedef std::vector<variables_type> vector_type;
      vector_type variable_vector;

    public :

      template <typename InputIterator>
      TrivialAllDifferent(const InputIterator begin, const InputIterator end) : variable_vector(begin,end) {}

      boost::logic::tribool eval(const partial_assignments_type& phi) {

        typedef std::set<value_type> set_type;
        set_type collected_values;
        typedef typename set_type::size_type size_type;
        size_type set_size = 0;

        typedef typename vector_type::const_iterator vector_iterator;
        const vector_iterator& end(variable_vector.end());
        for (vector_iterator i = variable_vector.begin(); i != end; ++i) {
          const domain_type& D(phi(*i));
          if (D.size() == 0) return false;
          if (D.size() == 1) {
            collected_values.insert(*(i -> second.begin()));
            if (collected_values == set_size++)
              return false;
          }
        }
        assert(set_size == collected_values.size());
        if (set_size == variable_vector.size())
          return true;
        else
          return boost::logic::indeterminate;
      }
      
    };

  }
}

#endif
