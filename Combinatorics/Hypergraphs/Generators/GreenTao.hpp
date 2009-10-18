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
#include <stdexcept>
#include <limits>

#include <gmpxx.h>

namespace OKlib {
  namespace Combinatorics {
    namespace Hypergraphs {
      namespace Generators {

        /*!
          \class First_prime_numbers
          \brief Functor for computing the first n prime numbers as a std::vector

          \todo Move to module Structures/NumberTheory

        */

        template <typename UInt = unsigned long>
        struct First_prime_numbers {
          typedef UInt uint_type;
          typedef std::vector<uint_type> range_type;
          range_type operator()(uint_type n) const {
            range_type primes;
            primes.reserve(n);
            const uint_type max = std::numeric_limits<uint_type>::max();
            for (mpz_class p = 2; n > 0; --n) {
              if (p > max)
                throw std::overflow_error("ERROR[First_prime_numbers]: n too big.");
              primes.push_back(p.get_ui());
              mpz_nextprime(p.get_mpz_t(), p.get_mpz_t());
            }
            return primes;
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

        template <typename UInt = unsigned long>
        struct GreenTao {

          typedef UInt vertex_type;
          typedef std::vector<vertex_type> hyperedge_type;
          typedef std::vector<hyperedge_type> set_system_type;
          typedef typename set_system_type::size_type size_type;

          const vertex_type k; // length of arithmetic progressions
          const vertex_type n; // number of vertices
          const bool prime_k;

          static const unsigned int max_k = 19; // for k=20 the first
          // arithmetic progressions apears for n=22,009,064,470.
          static const unsigned int rounds_compositeness_test = 10;

          GreenTao(const vertex_type k, const vertex_type n)
            : k(k), n(n), prime_k(prime(k)), vertex_set_(first_prime_numbers(n)) {
            // C++0X: the following assert should become compile-time
            assert(std::numeric_limits<vertex_type>::max() >= 4294967295UL);
            assert(k <= max_k);
          }

          size_type nver() const { return n; }
          size_type nhyp() const;

          hyperedge_type vertex_set() const { return vertex_set_; }

          set_system_type hyperedge_set() const {
            set_system_type result;
            if (k==0) { result.push_back(hyperedge_type()); return result; }
            if (n==0) return result;
            typedef typename hyperedge_type::const_iterator v_it_type;
            const v_it_type vbegin(vertex_set_.begin());
            const v_it_type vend(vertex_set_.end());
            if (k==1) {
              for (v_it_type i = vbegin; i != vend; ++i)
                result.push_back(hyperedge_type(1,*i));
              return result;
            }
            if (k==2) {
              for (v_it_type i = vbegin; i != vend; ++i)
                for (v_it_type j = i+1; j != vend; ++j) {
                  // C++0X: result.push_back(hyperedge_type {*i,*j})
                  hyperedge_type H;
                  H.push_back(*i); H.push_back(*j);
                  result.push_back(H);
                }
              return result;
            }
            if (n <= k) return result;
            assert(n >= 4);
            assert(k >= 3);
            assert(n > k);
            std::vector<bool> primes_table(vertex_set_.back()+1,false);
              for (v_it_type i = vbegin; i != vend; ++i)
                primes_table[*i] = true;
            vertex_type prd_primes = 1;
            for (v_it_type i = vbegin; *i <= k; ++i)
              prd_primes *= *i;
            // Now the main loop:
            for (v_it_type pi = vbegin; pi != vend; ++pi) {
              const vertex_type p = *pi;
              if (p <= prd_primes) continue;
              set_system_type F;
              const vertex_type max_slope = (p-k)/(k-1);
              for (vertex_type d = prd_primes; d <= max_slope; d+=prd_primes) {
                bool all_primes = true;
                for (vertex_type i = 1, e = p-d; i < k; ++i, e -= d)
                  if (not primes_table[e]) {all_primes = false; break;}
                if (all_primes) {
                  // TODO: use progression-iterator here
                  hyperedge_type H;
                  H.reserve(k);
                  for (vertex_type i = 0, e = p - (k-1)*d; i < k; ++i, e += d)
                    H.push_back(e);
                  F.push_back(H);
                }
              }
              if (prime_k) {
                const vertex_type q = (p-k)/(k-1);
                if ((k-1)*q == p-k) {
                  bool all_primes = true;
                  for (vertex_type e = k+q; e <= p; e+= q)
                    if (not primes_table[e]) {all_primes = false; break;}
                  if (all_primes) {
                    // TODO: use progression-iterator here
                    hyperedge_type H;
                    H.reserve(k);
                    for (vertex_type e = k; e <= p; e += q)
                      H.push_back(e);
                    F.push_back(H);
                  }
                }
              }
              result.insert(result.end(), F.rbegin(), F.rend());
            }
            return result;
          }

          bool prime(const vertex_type e) const {
            const unsigned int res = mpz_probab_prime_p(mpz_class(e).get_mpz_t(), rounds_compositeness_test);
            if (res == 2) return true;
            if (res == 0) return false;
            throw std::runtime_error("ERROR[GreenTao]: Uncertain about primality.");
          }

        private :

          const hyperedge_type vertex_set_;

        };

      }
    }
  }
}

#endif
