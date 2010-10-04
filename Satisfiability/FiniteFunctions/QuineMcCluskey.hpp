// Oliver Kullmann, 22.5.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
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

namespace OKlib {
  namespace Satisfiability {
    namespace FiniteFunctions {
      
#ifdef NUMBER_VARIABLES
      const int num_vars = NUMBER_VARIABLES;
#else
      const int num_vars = 4;
#endif
      
      //! Boolean variables as integers
      typedef int Variables;
      //! Boolean literals as integers
      typedef int Literals;
      //! Boolean clauses as vectors of literals
      typedef std::vector<Literals> Clauses;
      //! Boolean clause-sets as vectors of clauses
      typedef std::vector<Clauses> ClauseSets;
      /*!
        \brief Hash-table structure used to store and lookup clauses in a
        clause-set.
        
        Such a structure provides constant time elementship tests and inserts
        for clauses in a clause-set, although it has exponential space
        requirements in the number of variables.
      */
      typedef std::vector<bool> HashTable;
      //! Hashes used as index for HashTables
      typedef HashTable::size_type hash_index;
      
      
      /* XXX : Asserts that size types are sufficient are needed here */
      

      /*!
        \brief Given the base b and power e, computes b raised to the power of
        e. The key feature of this function is it provides integer computation
        whereas the standard library works with doubles.
        
      */
      hash_index ipow(const int b, int e) {
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
      hash_index hash_clause(const Clauses& clause) {
        long return_value = 0;
        for (Clauses::const_iterator iter = clause.begin(); iter != clause.end(); ++iter)
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
      hash_index flip_literal_sign_in_hash(hash_index hash, const Literals literal) {
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
      hash_index remove_literal_in_hash(hash_index  hash, const Literals literal) {
        if (literal < 0)
          hash -= ipow(3, abs(literal) - 1);
        else if (literal > 0)
          hash -= 2 * ipow(3, abs(literal) - 1);
        return hash;
      }
      
      /*!
        \brief Computes the clause represented by a given hash.
      */
      unsigned int hash2clause(hash_index hash, int clause[], const int num_vars) {
        hash_index var_value = 1;
        Literals num_lit = 0;
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
      ClauseSets quine_mccluskey(const ClauseSets& input_cs) {
        int clause[num_vars];
        hash_index num_partial_assignments = ipow(3, num_vars);
        // marked is used to keep track of all found clauses:
        HashTable marked(num_partial_assignments, 0);
        // marked_in is used to keep track of all clauses that are still in the
        // result set:
        HashTable marked_in(num_partial_assignments, 0);
        Variables clause_size = 0;
        hash_index hash = 0;
        hash_index partner_hash = 0;
        // first mark clauses:
        for (ClauseSets::const_iterator citer = input_cs.begin(); citer != input_cs.end(); ++citer) {
          hash = hash_clause(*citer);
          marked[hash] = true;
          marked_in[hash] = true;
        }
        // perform algorithm:
        for (Variables level = num_vars; level > 0; --level) {
          // run through all clauses:
          for (hash_index citer = 0; citer < num_partial_assignments; ++citer) {
            // go through literals in clause:
            if (marked[citer]) {
              clause_size = hash2clause(citer, clause, num_vars);
              if (clause_size == level) {
                for (Variables liter = 0; liter < clause_size; ++liter) {
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
          for (hash_index citer = 0; citer < num_partial_assignments; ++citer)
            marked[citer] = marked_in[citer];
        }
        ClauseSets result_cs;
        for (hash_index citer = 0; citer < num_partial_assignments; ++citer)
          if (marked_in[citer]) {
            clause_size = hash2clause(citer, clause, num_vars);
            Clauses s_clause(clause, clause + clause_size);
            result_cs.push_back(s_clause);
          }
        return result_cs;
      }
      
    }
  }
}

#endif
