// Oliver Kullmann, 14.11.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Interfaces/InputOutput/Colouring.hpp
  \brief Components to transfer colouring problems to CLS-adaptors

  \todo Write tests.
*/

#ifndef COLOURING_HgFrR439J
#define COLOURING_HgFrR439J

#include <string>
#include <vector>
#include <algorithm>
#include <cassert>

namespace OKlib {
  namespace Satisfiability {
    namespace Interfaces {
      namespace InputOutput {

        /*!
          \class HypergraphTwoColouring
          \brief Translating two hypergraphs into a boolean clause-set
          (as a 2-colouring problem).

          Requirements:
          <ul>
           <li> Int is promotable to CLSAdaptor::int_type. </li>
           <li> SetSystem is a sequence of sequences. </li>
           <li> The hyperedges contain no repetitions. </li>
          </ul>
          The vertices of the hypergraphs (more precisely, set-systems)
          can be arbitrary, while a NameMap must be provided to standardise
          these names.

          The functionality is similar to tcol2sat_stdohg2stdfcl in
          ComputerAlgebra/Hypergraphs/Lisp/Colouring.mac, but here we
          can use two hypergraphs (for the positive and the negative part),
          and the vertices don't need to be already standardised.

          \todo Handle the range-requirements correctly.

          \todo Provide the mapping-information in the comments.

        */

        template <class SetSystem, class CLSAdaptor, typename Int=int>
        struct HypergraphTwoColouring {

          typedef Int int_type;
          typedef SetSystem hypergraph_type;
          typedef typename hypergraph_type::value_type hyperedge_type;
          typedef typename hyperedge_type::value_type vertex_type;
          typedef CLSAdaptor cls_adaptor_type;

          // Prerequisite: NameMap is a functor from vertex_type to Int.
          // This function must be injective, with range {1..,n}.
          template <class NameMap>
          HypergraphTwoColouring(const hypergraph_type& G1, const hypergraph_type& G2, const NameMap& M, const int_type n, const std::string& comment, cls_adaptor_type& out) {
            out.comment(comment);
            out.n(n);
            out.c(G1.size() + G2.size());
            // TODO: use iterator-adaptors in the following
            typedef typename hypergraph_type::const_iterator h_iterator;
            typedef typename hyperedge_type::const_iterator v_iterator;
            typedef std::vector<int_type> aux_clause_type;
            typedef typename aux_clause_type::size_type size_type;
            {
              const h_iterator end = G1.end();
              for (h_iterator i = G1.begin(); i != end; ++i) {
                const size_type s = i -> size();
                aux_clause_type C;
                C.reserve(s);
                for (v_iterator j = i -> begin(); j != i -> end(); ++j)
                  C.push_back(M(*j));
                out.clause(C, s);
              }
            }
            {
              const h_iterator end = G2.end();
              for (h_iterator i = G2.begin(); i != end; ++i) {
                const size_type s = i -> size();
                aux_clause_type C;
                C.reserve(s);
                for (v_iterator j = i -> begin(); j != i -> end(); ++j)
                  C.push_back(-M(*j));
                out.clause(C, s);
              }
            }
            out.finish();
          }

        };


        /*!
          \class GeneralisedHypergraphColouring
          \brief Transforming generalised hypergraph colouring to (boolean) CNF

          Using the regular encoding with AMO-clauses. The specifications at
          Maxima-level are
           - gcol2sat_ohg2nbfclud in
             Hypergraphs/Lisp/Colouring.mac, together with
           - nbfclud2fcl_aloamo in
             Satisfiability/Lisp/Generators/NonBooleanTranslations.mac.

          \todo Write tests.

          \todo Would the use of smart pointers be appropriate?

        */
        template <class SetSystem, typename Int=int>
        struct GeneralisedHypergraphColouring {

          typedef SetSystem hypergraph_type;
          typedef typename hypergraph_type::value_type hyperedge_type;
          typedef typename hyperedge_type::value_type vertex_type;

          typedef Int int_type;

          typedef std::vector<const hypergraph_type*> list_hypergraphs_type;
          typedef typename list_hypergraphs_type::const_iterator list_hypergraphs_iterator;
          typedef std::vector<vertex_type> list_vertices_type;
          typedef typename list_vertices_type::const_iterator list_vertices_iterator;

          typedef typename list_hypergraphs_type::size_type size_type;

          const list_hypergraphs_type& list_hypergraphs;
          const list_vertices_type& list_vertices;
          const size_type num_colours;
          const size_type num_vertices;
          const size_type n;
          const list_vertices_iterator begin_vertices, end_vertices;

          GeneralisedHypergraphColouring(const list_hypergraphs_type& list_h, const list_vertices_type& list_v) :
            list_hypergraphs(list_h),
            list_vertices(list_v),
            num_colours(list_h.size()),
            num_vertices(list_v.size()),
            n(num_colours * num_vertices),
            begin_vertices(list_vertices.begin()),
            end_vertices(list_vertices.end()) {}

          int_type standardise(const vertex_type v, const size_type colour) const {
            assert(colour >= 1);
            assert(colour <= num_colours);
            const int_type sv = (std::lower_bound(begin_vertices, end_vertices, v) - begin_vertices);
            return sv * num_colours + colour;
          }

          template <class CLSAdaptor>
          void transfer(CLSAdaptor& out, const std::string& comment) const {
            out.comment(comment);
            out.n(n);
            const list_hypergraphs_iterator end_list = list_hypergraphs.end();
            {
              size_type c = 0;
              for (list_hypergraphs_iterator i = list_hypergraphs.begin(); i != end_list; ++i)
                c += (*i) -> size();
              c += num_vertices;
              c += num_vertices * (num_colours * (num_colours-1))/2;
              out.c(c);
            }
            typedef std::vector<int_type> aux_clause_type;
            for (list_hypergraphs_iterator Gi = list_hypergraphs.begin(); Gi != end_list; ++Gi) {
              const hypergraph_type& G = **Gi;
              const int_type colour = (Gi - list_hypergraphs.begin()) + 1;
              // TODO: use iterator-adaptors in the following
              typedef typename hypergraph_type::const_iterator h_iterator;
              typedef typename hyperedge_type::const_iterator v_iterator;
              const h_iterator end = G.end();
              for (h_iterator i = G.begin(); i != end; ++i) {
                const int_type s = i -> size();
                aux_clause_type C;
                C.reserve(s);
                for (v_iterator j = i -> begin(); j != i -> end(); ++j)
                  C.push_back(- standardise(*j,colour));
                out.clause(C, s);
              }
            }
            for (list_vertices_iterator i = begin_vertices; i != end_vertices; ++i) {
              const vertex_type v = *i;
              aux_clause_type C;
              C.reserve(num_colours);
              for (size_type colour = 1; colour <= num_colours; ++colour)
                C.push_back(standardise(v,colour));
              out.clause(C, num_colours);
            }
            for (list_vertices_iterator i = begin_vertices; i != end_vertices; ++i) {
              const vertex_type v = *i;
              for (size_type c1 = 1; c1 < num_colours; ++c1)
                for (size_type c2 = c1+1; c2 <= num_colours; ++c2) {
                  aux_clause_type C;
                  C.reserve(2);
                  C.push_back(-standardise(v,c1));
                  C.push_back(-standardise(v,c2));
                  out.clause(C, 2);
                }
            }
            out.finish();
          }

        };


      }
    }
  }
}

#endif
