// Oliver Kullmann, 22.5.2009 (Swansea)
/* Copyright 2009, 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/FiniteFunctions/QuineMcCluskey.hpp
  \brief Components for computing all minimal resolvents of a full clause-set

  A "full" clause-sets contains only full clauses, which in turn are
  characterised as containing all variables. The minimal resolvents are the
  prime implicates / implicants of the underlying boolean function when
  interpreting the clause-set as CNF resp. DNF. Considering clause-sets F as
  combinatorial structures, we speak of the "prime clauses" of F.

  The function template FiniteFunctions::quine_mccluskey(F) returns the
  prime-clauses of F, using the same clause-set-type as F.
*/

#ifndef QUINEMCCLUSKEY_jdbVce4
#define QUINEMCCLUSKEY_jdbVce4

#include <vector>
#include <algorithm>
#include <cstdlib>
#include <cassert>

#include <boost/range.hpp>
#include <boost/static_assert.hpp>

namespace OKlib {
  namespace Satisfiability {
    namespace FiniteFunctions {

      /*!
        \class QuineMcCluskey
        \brief Functor, computing all prime implicates of a full clause-set

        The Maxima input/output specification is min_2resolution_closure_cs in 
        ComputerAlgebra/Satisfiability/Lisp/Resolution/PrimeImplicatesImplicants.mac.

        For ease of use the helper-function FiniteFunctions::quine_mccluskey
        is provided below.
      */
      template<int n = 4,
               class ClauseContainer = std::vector<std::vector<int> > >
      struct QuineMcCluskey {

        //! the number of variables
        static const int num_vars = n;
      
        //! boolean literals as integers
        typedef typename boost::range_value<typename boost::range_value<ClauseContainer>::type >::type literal_type;
        //! boolean variables as integers
        typedef literal_type variable_type;
        //! clauses as given by the clause-container
        typedef typename boost::range_value<ClauseContainer>::type clause_type;
        //! clause-sets as given by the template parameter
        typedef ClauseContainer clause_set_type;
        //! iterator for clauses
        typedef typename boost::range_iterator<clause_type>::type clause_iterator_type;
        //! iterator for clauses
        typedef typename boost::range_const_iterator<const clause_type>::type const_clause_iterator_type;
        //! iterator for clause-sets
        typedef typename boost::range_iterator<clause_set_type>::type clause_set_iterator_type;
        //! iterator for const clause-sets
        typedef typename boost::range_const_iterator<const clause_set_type>::type const_clause_set_iterator_type;
        /*!
          \brief Hash-table structure used to store and lookup clauses in a
          clause-set
          
          Such a structure provides constant-time elementship tests and inserts
          for clauses in a clause-set, using space 3^n for n variables.
        */
        typedef std::vector<bool> HashTable;
        //! Hashes used as index for HashTables
        typedef HashTable::size_type hash_index_type;

        /* Asserts that size types are sufficient are needed here: */
        /* 2^size >?= 3^num_vars */
        /* log(2^size) >?= log(3^num_vars) */
        /* size * log(2) >?= num_vars * log(3) */
        /* ~=~ size * 70 >?= num_vars * 101 */
        BOOST_STATIC_ASSERT((sizeof(hash_index_type)*8-1) * 70 >= num_vars * 101);

        //! pow3[i] = 3^i for 0 <= i <= num_vars
        const hash_index_type* const pow3;
        const hash_index_type* fill_pow3() {
          hash_index_type* const pow3 = new hash_index_type[num_vars+1];
          hash_index_type pow = 1;
          for (int i = 0; i < num_vars; ++i, pow *= 3) pow3[i] = pow;
          pow3[num_vars] = pow;
          return pow3;
        }

        QuineMcCluskey() : pow3(fill_pow3()) {}
        ~QuineMcCluskey() { delete[] pow3; }
      
        /*!
          \brief Computes the hash-value of a clause
          
          The clause-hash is the sum of c * 3^(i-1) over all variables i
          in the clause (recall that variables are natural numbers > 0), where
          c is 0, 1 or 2 respectively if variable i does not occur or occurs
          negatively resp. positively in the clause.
        */
        hash_index_type hash_clause(const clause_type& clause) {
          hash_index_type return_value = 0;
          const const_clause_iterator_type cend(boost::const_end(clause));
          for (const_clause_iterator_type iter = boost::const_begin(clause); iter != cend; ++iter)
            if (*iter < 0) return_value += pow3[std::abs(*iter) - 1];
            else if (*iter > 0) return_value += 2 * pow3[std::abs(*iter) - 1];
          return return_value;
        }
      
        /*!
          \brief From a hash-value "hash" for a clause compute the new
          hash-value where the given literal is negated.
          
          Prerequisite is that the literal occurs in the clause represented by
          the input hash.
        */
        hash_index_type flip_literal(const hash_index_type hash, const literal_type literal) {
          assert(literal != 0);
          if (literal < 0) return hash + pow3[std::abs(literal) - 1];
          else return hash - pow3[std::abs(literal) - 1];
        }
      
        /*!
          \brief From a hash-value "hash" for a clause computes the new
          hash-value for the clause where the given literal has been removed.
          
          Prerequisite is that the literal occurs in the clause represented by
          the input hash.
        */
        hash_index_type remove_literal(const hash_index_type hash, const literal_type literal) {
          assert(literal != 0);
          if (literal < 0) return hash - pow3[std::abs(literal) - 1];
          else return hash - 2 * pow3[std::abs(literal) - 1];
        }
      
        /*!
          \brief Computes the clause represented by a given hash-value

          The return-value is the number of literals in the clause, while
          the reference-parameter clause contains the clause itself.

          Here a clause is an array of integers, where the length k of the
          clause must be known (so that only clause[0], ..., clause[k-1] are
          to be used).
        */
        unsigned int hash2clause(hash_index_type hash, int clause[]) {
          literal_type num_lit = 0;
          for (int lit = num_vars; lit > 0; --lit) {
            const hash_index_type var_val = pow3[lit-1];
            if (hash >= 2 * var_val) {
              clause[num_lit++] = lit;
              hash -= 2 * var_val;
            } else if (hash >= var_val) {
              clause[num_lit++] = -lit;
              hash -= var_val;
            }
          }
          return num_lit;
        }
      
        /*!
          \brief For a clause-set containing only variables v <= num_vars,
          all prime implicates are returned.
          
          Time and space complexity is polynomial in the full truth table,
          but exponential in num_vars.
        */
        clause_set_type  operator() (const clause_set_type& input_cs) {
          hash_index_type num_clauses = pow3[num_vars];
          // "curcls" contains (as hash-table) the current clauses, to be
          // processed:
          HashTable curcls(num_clauses);
          // "rescls" contains the clauses of the final result (completed
          // once a level is finished):
          HashTable rescls(num_clauses);
          // first mark input clauses:
          {const const_clause_set_iterator_type csend = boost::const_end(input_cs);
           for (const_clause_set_iterator_type iter = boost::const_begin(input_cs); iter != csend; ++iter) {
             const hash_index_type hash = hash_clause(*iter);
             curcls[hash] = true;
             rescls[hash] = true;
           }
          }
          // perform algorithm:
          int clause[num_vars];
          for (variable_type level = num_vars; level > 0; --level) {
            // run through all clauses:
            for (hash_index_type citer = 0; citer < num_clauses; ++citer)
              if (curcls[citer]) {
                const variable_type clause_size = hash2clause(citer, clause);
                if (clause_size == level) {
                  // go through literals in clause:
                  for (variable_type liter = 0; liter < clause_size; ++liter) {
                    // if it's partner clause exists:
                    const hash_index_type partner_hash =
                      flip_literal(citer, clause[liter]);
                    if (curcls[partner_hash]) {
                      long new_hash = remove_literal(citer, clause[liter]);
                      curcls[new_hash] = true;
                      rescls[new_hash] = true;
                      rescls[citer] = false;
                      rescls[partner_hash] = false;
                    }
                  }
                  curcls[citer] = false;
                }
              }
          }
          // extraction of the result:
          clause_set_type result_cs;
          for (hash_index_type citer = 0; citer < num_clauses; ++citer)
            if (rescls[citer]) {
              const variable_type clause_size = hash2clause(citer, clause);
              std::sort(clause, clause + clause_size);
              result_cs.push_back(clause_type(clause, clause + clause_size));
            }
          return result_cs;
        }

      };

      //! Helper function to ease usage of class QuineMcCluskey
      template<int num_vars, class ClauseContainer>
      inline ClauseContainer quine_mccluskey(const ClauseContainer& input_cs) {
        return QuineMcCluskey<num_vars, ClauseContainer>()(input_cs);
      }

    }
  }
}

#endif
