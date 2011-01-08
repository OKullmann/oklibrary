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

#include <boost/range.hpp>
#include <boost/static_assert.hpp>

namespace OKlib {
  namespace Satisfiability {
    namespace FiniteFunctions {


      // ############################################

      /*!
        \class QuineMcCluskey
        \brief Functor, computing all prime implicates of a full clause-set

        The Maxima input/output specification is min_2resolution_closure_cs in 
        ComputerAlgebra/Satisfiability/Lisp/Resolution/PrimeImplicatesImplicants.mac.

        For ease of use the helper-function FiniteFunctions::quine_mccluskey
        is provided below.
      */
      template<int num_vars = 4,
               class ClauseContainer = std::vector<std::vector<int> > >
      struct QuineMcCluskey {
      
        //! Boolean literals as integers
        typedef typename boost::range_value<typename boost::range_value<ClauseContainer>::type >::type  literal_type;
        //! Boolean variables as integers
        typedef literal_type variable_type;
        //! Boolean clauses as vectors of literals
        typedef typename boost::range_value<ClauseContainer>::type clause_type;
        //! Boolean clause-sets as vectors of clauses
        typedef ClauseContainer clause_set_type;
        //! Iterator for clauses
        typedef typename boost::range_iterator<clause_type>::type clause_iterator_type;
        //! Iterator for clauses
        typedef typename boost::range_const_iterator<const clause_type>::type const_clause_iterator_type;
        //! Iterator for clause-sets
        typedef typename boost::range_iterator<clause_set_type>::type clause_set_iterator_type;
        //! Iterator for const clause-sets
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

        /*!
          \brief For integer e >= 0, computes 3^e

          Providing integer computation whereas the standard library works
          with doubles. Inefficient for larger exponents.
        */
        hash_index_type pow3(const int e) {
          hash_index_type result = 1;
          for (int i = 0; i < e; ++i) result *= 3;
          return result;
        }
      
        /*!
          \brief Computes the hash value of a clause
          
          The clause-hash is the sum of c * 3^(i-1) over all variables i
          in the clause (recall that variables are natural numbers > 0), where
          c is:
          <ul>
           <li> 0 if variable i does not occur in the given clause </li>
           <li> 1 if variable i occurs negated in the given clause </li>
           <li> 2 if variable i occurs positively in the given clause. </li>
          </ul>
        */
        hash_index_type hash_clause(const clause_type& clause) {
          hash_index_type return_value = 0;
          const const_clause_iterator_type cend(boost::const_end(clause));
          for (const_clause_iterator_type iter = boost::const_begin(clause); iter != cend; ++iter)
            if (*iter < 0)
              return_value += pow3(std::abs(*iter) - 1);
            else if (*iter > 0)
              return_value += 2 * pow3(std::abs(*iter) - 1);
          return return_value;
        }
      
        /*!
          \brief Given a hash value for a clause, computes the new hash value 
          where the given literal is negated.
          
          The key point here is that the given literal occurs in the Clause 
          represented by the input hash.
        */
        hash_index_type flip_literal_sign_in_hash(hash_index_type hash, const literal_type literal) {
          if (literal < 0)
            hash += pow3(std::abs(literal) - 1);
          else if (literal > 0)
            hash -= pow3(std::abs(literal) - 1);
          return hash;
        }
      
        /*!
          \brief Given a hash value for a clause, computes a new hash for the 
          clause where the given literal has been removed.
          
          The key point here is that the given literal is assumed to occur within
          the clause associated with the input hash.
        */
        hash_index_type remove_literal_in_hash(hash_index_type hash, const literal_type literal) {
          if (literal < 0)
            hash -= pow3(std::abs(literal) - 1);
          else if (literal > 0)
            hash -= 2 * pow3(std::abs(literal) - 1);
          return hash;
        }
      
        /*!
          \brief Computes the clause represented by a given hash-value

          The return-value is the number of literals in the clause, while
          the reference-parameter clause contains the clause itself.
        */
        unsigned int hash2clause(hash_index_type hash, int clause[]) {
          hash_index_type var_value = 1;
          literal_type num_lit = 0;
          for (int lit = num_vars; lit > 0; --lit) {
            var_value = pow3(std::abs(lit) - 1);
            // work out whether the literal is in the hash:
            if (hash >= (2 * var_value)) {
              clause[num_lit++] = lit;
              hash -= (2 * var_value);
            } else if (hash >= var_value) {
              clause[num_lit++] = -lit;
              hash -= var_value;
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
          hash_index_type num_partial_assignments = pow3(num_vars);
          // marked is used to keep track of all found clauses:
          HashTable marked(num_partial_assignments, 0);
          // marked_in is used to keep track of all clauses that are still in the
          // result set:
          HashTable marked_in(num_partial_assignments, 0);
          // first mark clauses:
          {const const_clause_set_iterator_type csend = boost::const_end(input_cs);
           for (const_clause_set_iterator_type iter = boost::const_begin(input_cs); iter != csend; ++iter) {
             const hash_index_type hash = hash_clause(*iter);
             marked[hash] = true;
             marked_in[hash] = true;
           }
          }
          // perform algorithm:
          int clause[num_vars];
          for (variable_type level = num_vars; level > 0; --level) {
            // run through all clauses:
            for (hash_index_type citer = 0; citer < num_partial_assignments; ++citer) {
              // go through literals in clause:
              if (marked[citer]) {
                const variable_type clause_size = hash2clause(citer, clause);
                if (clause_size == level) {
                  for (variable_type liter = 0; liter < clause_size; ++liter) {
                    // if it's partner clause exists:
                    const hash_index_type partner_hash =
                      flip_literal_sign_in_hash(citer, clause[liter]);
                    if (marked[partner_hash]) {
                      long new_hash = remove_literal_in_hash(citer, clause[liter]);
                      marked[new_hash] = true;
                      marked_in[new_hash] = true;
                      marked_in[citer] = false;
                      marked_in[partner_hash] = false;
                    }
                  }
                }
              }
            }
            // at the end of each level, we only need those clauses that are in 
            // marked_in:
            for (hash_index_type citer = 0; citer < num_partial_assignments; ++citer)
              marked[citer] = marked_in[citer];
          }
          clause_set_type result_cs;
          for (hash_index_type citer = 0; citer < num_partial_assignments; ++citer)
            if (marked_in[citer]) {
              const variable_type clause_size = hash2clause(citer, clause);
              std::sort(clause, clause + clause_size);
              clause_type s_clause(clause, clause + clause_size);
              result_cs.push_back(s_clause);
            }
          return result_cs;
        }

      };

      //! Helper function to ease usage of class QuineMcCluskey
      template<int num_vars, class ClauseContainer>
      ClauseContainer quine_mccluskey(const ClauseContainer& input_cs) {
        QuineMcCluskey<num_vars, ClauseContainer> qmc;
        return(qmc(input_cs));
      }

    }
  }
}

#endif
