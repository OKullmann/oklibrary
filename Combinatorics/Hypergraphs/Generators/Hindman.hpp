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
#include <stdexcept>

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

          \todo To be implemented
          <ul>
           <li> Only missing is the function-application member-function (which
           creates the hypergraph). </li>
          </ul>

          \todo To be tested
          <ul>
           <li> Test the statistics-functions. </li>
          </ul>

          \todo Design of the concept
          <ul>
           <li> See "The simplest form of hypergraphs" in
           Hypergraphs/concepts/plans/general.hpp. </li>
           <li> Currently the class (functor) is a mixture, on the one hand
           providing elements (like hyperedge_type etc.) as if the objects
           would be hypergraphs themselves, on the other hand actually just
           computing a range of ranges (in this way delegating all
           access-functionality). </li>
          </ul>

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
          size_type nhyp() const {
            if (n < a+a+inj or n < a*(a+inj)) return 0;
            const size_type s = std::sqrt((double) n);
            if (a == 1)
              return sum(2,s) - (s+2*inj-1)*s/2 + (n-1);
            else
              return sum(a,s) - ((s+a+2*inj-2)*(s+1-a))/2;
          }

          set_system_type operator()() const {

          }

        private :

          size_type sum(const vertex_type start, const size_type end) const {
            size_type sum = 0;
            for (vertex_type x = start; x <= end; ++x) {
              const vertex_type q =  n/x;
              sum += q;
              if (sum < q)
                throw std::overflow_error("ERROR[Hindman]: n too big.");
              return sum;
            }
          }
        };

      }
    }
  }
}

#endif
