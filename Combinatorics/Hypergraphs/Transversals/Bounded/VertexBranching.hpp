// Oliver Kullmann, 6.6.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Combinatorics/Hypergraphs/Transversals/Bounded/VertexBranching.hpp
  \brief Computing (basically) all transversals of size up to B, by branching on vertices

  The role model is transversals_bv (and transversals_bvs) in
  ComputerAlgebra/Hypergraphs/Lisp/Transversals/Bounded/MaintainingBound.mac
*/

#ifndef TRANSVERSALSBV_jsBgf4wY
#define TRANSVERSALSBV_jsBgf4wY

#include <list>
#include <vector>
#include <iterator>
#include <cassert>
#include <algorithm>
#include <functional>
#include <ostream>
#include <iomanip>

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
         ComputerAlgebra/Hypergraphs/Lisp/Transversals/Bounded/MaintainingBound.mac.
         </li>
         <li> By using sets which are length-sorted one also obtains
         algorithm transversals_bvs. </li>
        </ul>
      */
      
      template <class SetSystem>
      struct Bounded_transversals_bv {
        
        typedef SetSystem set_system_type;
        typedef typename set_system_type::const_iterator const_iterator;
        typedef typename set_system_type::iterator iterator;
        typedef typename set_system_type::value_type hyperedge_type;
        typedef typename hyperedge_type::value_type vertex_type;
        typedef typename hyperedge_type::size_type size_type;
        
        typedef std::list<hyperedge_type> transversal_list_type;
        typedef typename transversal_list_type::const_iterator const_result_iterator;
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
            for (const_iterator i = G.begin(); i != G.end(); ) {
              const hyperedge_type& K(*i);
              const const_iterator j(i); ++i;
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
            { const_iterator old_insert(G_a.begin());
              for (iterator i = G.begin(); i != G.end(); ) {
                hyperedge_type K(*i);
                const const_iterator j(i); ++i;
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
              const_iterator old_insert(G.begin());
              const const_iterator& end(G_a.end());
              for (const_iterator i = G_a.begin(); i != end; ++i)
                old_insert = G.insert(old_insert, *i);
            }
            transversal_list_type temp_res(operator()(G, B));
            result.splice(result.end(), temp_res);
            return result;
          }
        }
        
      };


      /*!
        \class TransversalPredicate
        \brief Unary predicate for checking whether a set of vertices is a transversal of a given hypergraph
      */

      template <class SetSystem>
      struct TransversalPredicate : std::unary_function<const typename SetSystem::value_type&, bool> {
        typedef SetSystem set_system_type;
        typedef typename set_system_type::value_type hyperedge_type;
        typedef typename set_system_type::const_iterator iterator;
        const set_system_type& G;
        const iterator begin;
        const iterator end;
        TransversalPredicate(const set_system_type& G)
          : G(G), begin(G.begin()), end(G.end()) {}
        bool operator() (const hyperedge_type& T) const {
          return std::find_if(begin, end, OKlib::SetAlgorithms::Disjoint<hyperedge_type>(T)) == end;
        }
      };


      /*!
        \class Minimum_transversals_mongen
        \brief Computing all minimum transversals for hypergraphs gen(N0+1), ..., gen(Nmax).
        
        As minimum_transversals_mongen in
        ComputerAlgebra/Hypergraphs/Lisp/Transversals/Bounded/MaintainingBound.mac.
        <ul>
         <li> gen(n) is the list of new hyperedges for vertex n. </li>
         <li> So actually vertex names are assumed to be standardised
         (different from the more general Maxima function). </li>
        </ul>
      */
      
      template <class SetSystem,
                template <class SetSystem> class Generator,
                template <class SetSystem> class Output>
      struct Minimum_transversals_mongen {
                  
        typedef SetSystem set_system_type;
        
        typedef Bounded_transversals_bv<set_system_type> transversals_bv_type;
        typedef typename transversals_bv_type::const_iterator const_iterator;
        typedef typename transversals_bv_type::iterator iterator;
        typedef typename transversals_bv_type::hyperedge_type hyperedge_type;
        typedef typename transversals_bv_type::vertex_type vertex_type;
        typedef typename transversals_bv_type::size_type size_type;
        typedef typename transversals_bv_type::transversal_list_type transversal_list_type;
        typedef typename transversals_bv_type::const_result_iterator const_result_iterator;
        typedef typename transversals_bv_type::result_iterator result_iterator;
        
        typedef Generator<set_system_type> generator_type;
        typedef Output<set_system_type> output_type;

        typedef typename generator_type::hyperedge_list_type hyperedge_list_type;

        void operator() (
                         const size_type N0,
                         const size_type Nmax,
                         set_system_type G,
                         transversal_list_type MT0,
                         generator_type gen,
                         output_type out) const {
          if (N0 > Nmax) return;
          assert(not MT0.empty());
          size_type t = MT0.begin() -> size();
          for (size_type n = N0+1; n <= Nmax; ++n) {
            const hyperedge_list_type E(gen(n));
            const vertex_type a = n;
            hyperedge_list_type Er(E);
            { // removing vertex a from Er
              typedef typename hyperedge_list_type::iterator i_t;
              const i_t& end(Er.end());
              for (i_t i = Er.begin(); i != end; ++i)
                i -> erase(a);
            }
            transversal_list_type MT1;
            { // MT1 = elements of MT0 which are transversals of E (or Er):
              TransversalPredicate<hyperedge_list_type> t_p(Er);
              const const_result_iterator end(MT0.end());
              for (const_result_iterator i = MT0.begin(); i != end; ++i)
                if (t_p(*i)) MT1.push_back(*i);
            }
            if (MT1.empty()) {
              ++t;
              const result_iterator end(MT0.end());
              for (result_iterator i = MT0.begin(); i != end; ++i)
                i -> insert(a);
              MT1.splice(MT1.end(), MT0);
              set_system_type Gr(G);
              Gr.insert(Er.begin(), Er.end());
              transversal_list_type temp_res(transversals_bv_type(Gr,t)());
              MT1.splice(MT1.end(), temp_res);
            }
            out(n,t,MT1);
            G.insert(E.begin(), E.end());
            MT0.swap(MT1);
          }
        }

        void operator() (
                         const size_type Nmax,
                         generator_type gen,
                         output_type out) const {
          operator()(0, Nmax, set_system_type(), transversal_list_type(1), gen, out);
        }

      };

      template <class SetSystem> struct TrivialOutput {
        typedef SetSystem set_system_type;
        typedef Bounded_transversals_bv<set_system_type> transversals_bv_type;
        typedef typename transversals_bv_type::size_type size_type;
        typedef typename transversals_bv_type::transversal_list_type transversal_list_type;
        std::ostream& out;
        TrivialOutput(std::ostream& out) : out(out) {}
        void operator() (const size_type n, const size_type t, const transversal_list_type& MT) {
          out << n << " " << t << " " << MT.size() << std::endl;
        }
      };

    }
   }
  }
 }
}

#endif
