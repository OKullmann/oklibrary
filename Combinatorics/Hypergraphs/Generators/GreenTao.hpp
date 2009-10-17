// Oliver Kullmann, 17.10.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKlib/Combinatorics/Hypergraphs/Generators/GreenTao.hpp
  \brief %Tools for generating GreenTao hypergraphs
*/

#ifndef GREENTAOHYPERGRAPH_mmnVre09h
#define GREENTAOHYPERGRAPH_mmnVre09h

#include <vector>
#include <cassert>
#include <cmath>
#include <stdexcept>

namespace OKlib {
  namespace Combinatorics {
    namespace Hypergraphs {
      namespace Generators {

        /*!
          \class First_prime_numbers
          \brief Functor for computing the first n prime numbers as a std::vector
        */

        template <typename UInt = unsigned int>
        struct First_prime_numbers {
          typedef UInt uint_type;
          typedef std::vector<uint_type> range_type;
          range_type operator()(const uint_type n) const {

          }
        };
        template <typename UInt>
        typename First_prime_numbers<UInt>::range_type first_prime_numbers(const UInt n) {
          return First_prime_numbers<UInt>()(n);
        }

        /*!
          \class GreenTao
          \brief Class for creating GreenTao hypergraphs

          Specification: arithprog_primes_ohg(k,n) in
          ComputerAlgebra/Hypergraphs/Lisp/Generators/GreenTao.mac, together
          with nver_arithprog_primes_ohg and nhyp_arithprog_primes_ohg.

          The hypergraph is represented as a vector of vectors.

          \todo To be implemented
          

          \todo To be tested
          <ul>
           <li> Test the statistics-functions. </li>
          </ul>

        */

        template <typename UInt = unsigned int>
        struct GreenTao {

          typedef UInt vertex_type;
          typedef std::vector<vertex_type> hyperedge_type;
          typedef std::vector<hyperedge_type> set_system_type;
          typedef typename set_system_type::size_type size_type;

          const vertex_type n; // number of vertices

          GreenTao(const vertex_type n) : n(n), vertex_set_(first_prime_numbers(n)) {}
          GreenTao() : n(0) {}

          size_type nver() const { return n; }
          size_type nhyp() const;

          hyperedge_type vertex_set() { return vertex_set_; }

          set_system_type hyperedge_set() const {

          }

        private :

          const hyperedge_type vertex_set_;

        };

      }
    }
  }
}

#endif
