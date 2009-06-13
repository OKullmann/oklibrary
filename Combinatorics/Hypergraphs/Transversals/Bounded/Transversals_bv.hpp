// Oliver Kullmann, 6.6.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Combinatorics/Hypergraphs/Transversals/Bounded/Transversals_bv.hpp
  \brief Computing (basically) all transversals of size up to B, by branching on vertices

  The role model is transversals_bv (and transversals_bvs) in
  ComputerAlgebra/Hypergraphs/Lisp/Transversals/Transversals.mac
*/

#ifndef TRANSVERSALSBV_jsBgf4wY
#define TRANSVERSALSBV_jsBgf4wY

#include <list>
#include <vector>
#include <iterator>

#include <OKlib/Structures/Sets/SetAlgorithms/BasicSetOperations.hpp>

namespace OKlib {
 namespace Combinatorics {
  namespace Hypergraphs {
   namespace Transversals {
    namespace Bounded {

      /*!
        \class Bounded_transversals_bv
        \brief Functor, which for a given set system G and a bound B
        computes (essentially) all transversals of G of size at most B.
        
        Recursive splitting on the first vertex in the first hyperedge,
        first including that vertex in the transversal, then excluding it.
        <ul>
         <li> This is algorithm transversals_bv from
         ComputerAlgebra/Hypergraphs/Lisp/Transversals/Transversals.mac.
         </li>
         <li> By using sets which are length-sorted one also obtains
         algorithm transversals_bvs. </li>
        </ul>
      */
      
      template <class SetSystem>
      struct Bounded_transversals_bv {
        
        typedef SetSystem set_system_type;
        typedef typename set_system_type::const_iterator iterator;
        typedef typename set_system_type::value_type hyperedge_type;
        typedef typename hyperedge_type::value_type vertex_type;
        typedef typename hyperedge_type::size_type size_type;
        
        typedef std::list<hyperedge_type> transversal_list_type;
        typedef typename transversal_list_type::iterator result_iterator;
        
        const set_system_type& G_orig;
        size_type bound;
        
        Bounded_transversals_bv(const set_system_type& G, const size_type B)
          : G_orig(G), bound(B) {}
        
        transversal_list_type operator() () const {
          return operator()(G_orig, bound);
        }
        transversal_list_type iterated() {
          transversal_list_type result(operator()(G_orig, bound));
          while (result.empty())
            result = operator()(G_orig, ++bound);
          return result;
        }
        
        transversal_list_type operator()
          (set_system_type G, const size_type B) const {
          transversal_list_type result;
          
          if (G.empty()) { result.push_back(hyperedge_type()); return result; }
          if (B == 0) return result;
          const hyperedge_type H(*G.begin());
          if (H.empty()) return result;
          if (B == 1) {
            typedef std::vector<vertex_type> vertex_list_t;
            vertex_list_t I;
            OKlib::SetAlgorithms::intersection_sets(G.begin(), G.end(), std::back_inserter(I));
            typedef typename vertex_list_t::const_iterator iterator_t;
            const iterator_t& end(I.end());
            for (iterator_t i = I.begin(); i != end; ++i) {
              hyperedge_type S; S.insert(*i);
              result.push_back(S);
            }
            return result;
          }
          
          const vertex_type a(*H.begin());
          if (H.size() == 1) {
            for (iterator i = G.begin(); i != G.end(); ) {
              const hyperedge_type& K(*i);
              const iterator j(i); ++i;
              if (K.find(a) != K.end()) G.erase(j);
            }
            result = operator()(G,B-1);
            const result_iterator& end(result.end());
            for (result_iterator i = result.begin(); i != end; ++i)
              i -> insert(a);
            return result;
          }
          else {
            set_system_type G_a;
            { iterator old_insert(G_a.begin());
              for (iterator i = G.begin(); i != G.end(); ) {
                hyperedge_type K(*i);
                const iterator j(i); ++i;
                if (K.erase(a) != 0) {
                  old_insert = G_a.insert(old_insert,K);
                  G.erase(j);
                }
              }
            }
            result = operator()(G,B-1);
            const result_iterator& end(result.end());
            for (result_iterator i = result.begin(); i != end; ++i)
              i -> insert(a);
            {
              iterator old_insert(G.begin());
              const iterator& end(G_a.end());
              for (iterator i = G_a.begin(); i != end; ++i)
                old_insert = G.insert(old_insert, *i);
            }
            transversal_list_type temp_res(operator()(G, B));
            result.splice(result.end(), temp_res);
            return result;
          }
        }
        
      };
      
    }
   }
  }
 }
}

#endif
