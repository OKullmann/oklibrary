// Oliver Kullmann, 22.5.2009 (Swansea)
/* Copyright 2009, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/FiniteFunctions/QuineMcCluskey.hpp
  \brief Definitions for computing all minimal resolvents from a given full clause-set
*/

#ifndef QUINEMCCLUSKEY_jdbVce4
#define QUINEMCCLUSKEY_jdbVce4

#include <set>
#include <vector>
#include <set>
#include <algorithm>
#include <cmath>

#include <boost/range.hpp>
#include <boost/static_assert.hpp>

namespace OKlib {
  namespace Satisfiability {
    namespace FiniteFunctions {


      // ############################################

      /*!
        \class QuineMcCluskey
        \brief Functor: Computes the clause-set containing all prime implicates of the given full clause-set. ???

        Input/Output Specification: min_2resolution_closure_cs in 
        ComputerAlgebra/Satisfiability/Lisp/Resolution/PrimeImplicatesImplicants.mac . ???
      */
      template<int num_vars = 4,
               typename ClauseContainer = std::vector<std::vector<int> > >
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
          clause-set.
          
          Such a structure provides constant time elementship tests and inserts
          for clauses in a clause-set, although it has exponential space
          requirements in the number of variables.
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
          \brief Given the base b and power e, computes b raised to the power
          of e. The key feature of this function is it provides integer
          computation whereas the standard library works with doubles.
        
        */
        hash_index_type ipow(const int b, int e) {
          long result = 1;
          while (e-- > 0) result *= b;
          return result;
        }
      
        /*!
          \brief Computes the hash value for a given clause
          
          The clause hash is simply the sum of c * 3^i for all variables i
          (where variables are integers in the range 1,  ..., num_vars), where
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
              return_value += ipow(3, abs(*iter) - 1);
            else if (*iter > 0)
              return_value += 2 * ipow(3, abs(*iter) - 1);
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
            hash += ipow(3, abs(literal) - 1);
          else if (literal > 0)
            hash -= ipow(3, abs(literal) - 1);
          return hash;
        }
      
        /*!
          \brief Given a hash value for a clause, computes a new hash for the 
          clause where the given literal has been removed.
          
          The key point here is that the given literal is assumed to occur within
          the clause associated with the input hash.
        */
        hash_index_type remove_literal_in_hash(hash_index_type  hash, const literal_type literal) {
          if (literal < 0)
            hash -= ipow(3, abs(literal) - 1);
          else if (literal > 0)
            hash -= 2 * ipow(3, abs(literal) - 1);
          return hash;
        }
      
        /*!
          \brief Computes the clause represented by a given hash.
        */
        unsigned int hash2clause(hash_index_type hash, int clause[]) {
          hash_index_type var_value = 1;
          literal_type num_lit = 0;
          for (int lit = num_vars; lit > 0; --lit) {
            var_value = ipow(3, abs(lit) - 1);
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
          \brief Given a clause-set with only variables of size m where m < n,
          all prime implicates of the given clause-set are returned.
          
          Running time and space requirements are exponential (powers of 3) in 
          the number of variables.
        */
        clause_set_type quine_mccluskey(const clause_set_type& input_cs) {
          int clause[num_vars];
          hash_index_type num_partial_assignments = ipow(3, num_vars);
          // marked is used to keep track of all found clauses:
          HashTable marked(num_partial_assignments, 0);
          // marked_in is used to keep track of all clauses that are still in the
          // result set:
          HashTable marked_in(num_partial_assignments, 0);
          variable_type clause_size = 0;
          hash_index_type hash = 0;
          hash_index_type partner_hash = 0;
          // first mark clauses:
          const_clause_set_iterator_type iter = boost::const_begin(input_cs);
          for (; iter != boost::const_end(input_cs); ++iter) {
            hash = hash_clause(*iter);
            marked[hash] = true;
            marked_in[hash] = true;
          }
          // perform algorithm:
          for (variable_type level = num_vars; level > 0; --level) {
            // run through all clauses:
            for (hash_index_type citer = 0; citer < num_partial_assignments; ++citer) {
              // go through literals in clause:
              if (marked[citer]) {
                clause_size = hash2clause(citer, clause);
                if (clause_size == level) {
                  for (variable_type liter = 0; liter < clause_size; ++liter) {
                    // if it's partner clause exists:
                    partner_hash =
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
              clause_size = hash2clause(citer, clause);
              std::reverse(clause, clause + clause_size);
              clause_type s_clause(clause, clause + clause_size);
              result_cs.push_back(s_clause);
            }
          return result_cs;
        }

        clause_set_type operator() (const clause_set_type& input_cs) {
          return quine_mccluskey(input_cs);
        } 
      };

      //! Function ready for when we add template parameters.
      template<int num_vars, typename ClauseContainer>
      ClauseContainer quine_mccluskey(const ClauseContainer& input_cs) {
        QuineMcCluskey<num_vars, ClauseContainer> qmc;
        return(qmc(input_cs));
      }

    }
  }
}

#endif
