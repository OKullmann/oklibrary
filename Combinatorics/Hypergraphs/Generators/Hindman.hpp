// Oliver Kullmann, 30.7.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKlib/Combinatorics/Hypergraphs/Generators/Hindman.hpp
  \brief %Tools for generating Hindman hypergraphs
*/

#ifndef HINDMANHYPERGRAPH_jjdhsTfr435R
#define HINDMANHYPERGRAPH_jjdhsTfr435R

#include <vector>
#include <cassert>
#include <cmath>

namespace OKlib {
  namespace Combinatorics {
    namespace Hypergraphs {
      namespace Generators {

        /*!
          \class Hindman_k2
          \brief Functor for creating Hindman hypergraphs with k=2

          Specification: hindman_k2_ohg and hindmani_k2_ohg in
          ComputerAlgebra/Hypergraphs/Lisp/Generators/Hindman.mac, together
          with their statistics functions.

          The hypergrap is represented as a vector of vectors.

          \todo To be implemented.
        */

        template <typename UInt = unsigned int>
        struct Hindman_k2 {

          typedef UInt vertex_type;
          typedef std::vector<vertex_type> hyperedge_type;
          typedef std::vector<hyperedge_type> set_system_type;
          typedef typename set_system_type::size_type size_type;

          const vertex_type a; // start index
          const vertex_type n; // end index
          const bool inj; // whether only different vertices are considered

          Hindman_k2(const vertex_type a, const vertex_type n, const bool inj = false)
            : a(a), n(n), inj(inj) {
            assert(a >= 1);
          }

          size_type nver() const {
            if (n < a) return 0; else return n-a+1;
          }
          size_type nhyp() const { // ATTENTION: overflow possible
            if (n < a+a+inj or n < a*(a+inj)) return 0;
            size_type s = std::sqrt((double) n);
            if (a == 1) {
              size_type sum = 0;
              for (vertex_type x = 2; x <= s; ++x) sum += n/x;
              return n-1 + sum - (s+2*inj-1)*s/2;
            }
            else {
              size_type sum = 0;
              for (vertex_type x = a; x <= s; ++x) sum += n/x;
              return sum - ((s+1-a)*(s+a+2*inj-2))/2;
            }
          }

          set_system_type operator()() const {

          }
        };

      }
    }
  }
}

#endif
