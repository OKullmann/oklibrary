// Oliver Kullmann, 21.11.2006 (Swansea)

/*!
  \file PartialAssignments/MultivaluedPartialAssignments.hpp
  \brief Components for set-valued partial assignments
*/

#ifndef MULTIVALUEDPARTIALASSIGNMENTS_7474haalaoi8
#define MULTIVALUEDPARTIALASSIGNMENTS_7474haalaoi8

#include <set>
#include <vector>
#include <utility>
#include <cassert>

#include <tr1/array>

#include <Transitional/Variables/TrivialVariables.hpp>

namespace OKlib {
  namespace PartialAssignments {

    /*!
      \class MultiPASS
      \brief Very first prototype for a set-valued partial assignment

      \todo Use literals from module Literals.

      \todo Test it.
    */

    template <class Values, int NumVar>
    struct MultiPASS {

      typedef Values value_type;
      typedef std::set<value_type> domain_type;
      static const int n = NumVar;

      typedef std::tr1::array<domain_type, n+1> array_type;
      array_type phi;

      typedef OKlib::Variables::Variables_int variables_type;
      typedef std::pair<variables_type, domain_type> literal_type;

      const domain_type& operator[](const variables_type v) const {
        assert(v > 0);
        assert(v <= n);
        return phi[n];
      }

      void set(const variables_type v, domain_type& S) {
        assert(v > 0);
        assert(v <= n);
        action_list.push_back(literal_type(v, phi[v]));
        phi[v].swap(S);
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
          assert(old_value.first > 0);
          assert(old_value.first <= n);
          phi[old_value.first] = old_value.second;
          action_list.pop_back();
        }
      }
      
    };


  }
}


#endif


