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
            typedef typename aux_clause_type::iterator aux_iterator;
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

      }
    }
  }
}

#endif
