// Oliver Kullmann, 21.11.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Assignments/PartialAssignments/MultivaluedPartialAssignments.hpp
  \brief Components for set-valued partial assignments
*/

#ifndef MULTIVALUEDPARTIALASSIGNMENTS_7474haalaoi8
#define MULTIVALUEDPARTIALASSIGNMENTS_7474haalaoi8

#include <set>
#include <vector>
#include <utility>
#include <cassert>

#include <tr1/array>
#include <tr1/type_traits>

#include <boost/static_assert.hpp>

#include <OKlib/Concepts/Variables.hpp>

namespace OKlib {
  namespace PartialAssignments {

    /*!
      \class MultiPASS
      \brief Very first prototype for a set-valued partial assignment

      MultiPASS<Values, NumVar, Var> :
       - Values is the value-type
       - NumVar is the number of variables
       - Var is a model of VariablesAsIndex.
    */

    template <class Values, unsigned int NumVar, class Var>
    struct MultiPASS {
      BOOST_STATIC_ASSERT((std::tr1::is_base_of<OKlib::Concepts::VariablesAsIndex_tag, typename OKlib::Concepts::traits::concept_tag<Var>::type>::value));

      typedef Values value_type;
      typedef std::set<value_type> domain_type; // this should come from module Literals; its an atomic condition as discussed in Concepts/plans/Literals.hpp

      typedef Var variables_type;
      typedef typename OKlib::Variables::traits::index_type<Var>::type index_type;
      
      static const unsigned int n = NumVar;

    private :
      typedef std::tr1::array<domain_type, n+1> array_type;
      array_type phi;

    public :
      typedef std::pair<variables_type, domain_type> literal_type;

      const domain_type& operator[](const variables_type v) const {
        const index_type i(v);
        assert(i > 0);
        assert(i <= n);
        return phi[i];
      }

      void set(const variables_type v, domain_type& S) { // likely the input should be a literal
        const index_type i(v);
        assert(i > 0);
        assert(i <= n);
        action_list.push_back(literal_type(v, phi[i]));
        phi[i].swap(S);
      }
      
    private :
      typedef std::vector<literal_type> stack_type;
      stack_type action_list;

    public :
      typedef typename stack_type::size_type token_type;

      token_type get_token() const { return action_list.size(); }

      void undo(const token_type last_token) {
        while (action_list.size() > last_token) {
          const literal_type& old_value(action_list.back());
          const index_type i(old_value.first);
          assert(i > 0);
          assert(i <= n);
          phi[i] = old_value.second;
          action_list.pop_back();
        }
      }
      
    };


  }
}


#endif


