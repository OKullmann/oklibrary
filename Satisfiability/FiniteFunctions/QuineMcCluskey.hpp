// Oliver Kullmann, 22.5.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/FiniteFunctions/QuineMcCluskey.hpp
  \brief Definitions for computing all minimal resolvents from a given full clause-set2
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
      const int nVars = NUMBER_VARIABLES;
#else
      const int nVars = 4;
#endif

      /*!
	\typedef typedef int Variables
	\brief Type defined for the variables of the clause sets.
      */
      typedef int Variables;
      /*!
	\typedef typedef int Literals
	\brief Type defined for the literals of the clause sets.
      */
      typedef int Literals;
      /*!
	\typedef typedef std::vector<Literals> Clauses
	\brief Type defined for the Clauses of the clause sets.
      */
      typedef std::vector<Literals> Clauses;
      /*!
	\typedef typedef std::vector<Clauses> ClauseSets
	\brief Type defined for the ClauseSets used in the problem.
      */
      typedef std::vector<Clauses> ClauseSets;
      /*!
	\typedef typedef std::vector<bool> HashTable
	\brief Type defined for the HashTable structure used to store and lookup
	clauses in a clause set.

	Such a structure provides constant time elementship tests and inserts for
	clauses in a clause set, although has exponential space requirements
	in the number of variables.
      */
      typedef std::vector<bool> HashTable;
      /*!
	\typedef typedef HashTable::size_type hash_index
	\brief Type defined for hashes used as index for HashTables used within
	this module.
      */
      typedef HashTable::size_type hash_index;


      /* XXX : Asserts that size types are sufficient are needed here */

      /*! 
	\fn void print_clause(const Clauses& clause)
        \brief Taking a Clause and printing the clause in Dimacs format to stdout.
       */
      void print_clause(const Clauses& clause) {
        for (Clauses::const_iterator iter = clause.begin();
             iter != clause.end(); ++iter) {
          std::cout << (int) *iter;
          std::cout << " ";
        }
        std::cout << "0" << std::endl;
      }
      
      /*!
	\fn void print_clauseset(const ClauseSets& clauseSet)
	\brief Taking a ClauseSet and printing the clause set in Dimacs format to stdout.
      */
      void print_clauseset(const ClauseSets& clauseSet) {
        for (ClauseSets::const_iterator iter = clauseSet.begin();
             iter != clauseSet.end(); ++iter) {
          print_clause(*iter);
        }
      }
      
      /*!
	\fn hash_index ipow(const int b, int e)
	\brief Given the base b and power e, computes b raised to the power of e.
	
	The key feature of this function is it provides integer computation whereas
	the standard library works with doubles.

      */
      hash_index ipow(const int b, int e) {
        long result = 1;
        while (e-- > 0) {
          result *= b;
        }
        return result;
      }
      
      /*!
	\fn hash_clause(const Clauses& clause)
	\brief Computes the hash value for a given Clause.

	The clause hash is simply the sum of c * 3^i for all variables
	i (where variables are integers in the range 1-nVars), where c
	is:
	<ul>
	 <li> 0 if variable i does not occur in the given clause </li>
	 <li> 1 if variable i occurs negated in the given clause </li>
	 <li> 2 if variable i occurs positively in the given clause. </li>
	</ul>

       */
      hash_clause(const Clauses& clause) {
        long returnValue = 0;
        for (Clauses::const_iterator iter = clause.begin();
             iter != clause.end(); ++iter) {
          if (*iter < 0) {
            returnValue += ipow(3, abs(*iter) - 1);
          } else if (*iter > 0) {
            returnValue += 2 * ipow(3, abs(*iter) - 1);
          }
        }
        return returnValue;
      }
      
      /*!
	\fn hash_index flip_literal_sign_in_hash(hash_index hash, const Literals literal)
	\brief Given a hash value for a Clause, computes the new hash value 
	where the given literal is negated.
	
	The key point here is that the given literal occurs in the Clause 
	represented by the input hash.
      */
      hash_index 
      flip_literal_sign_in_hash(hash_index hash, const Literals literal) {
        if (literal < 0) {
          hash += ipow(3, abs(literal) - 1);
        } else if (literal > 0) {
          hash -= ipow(3, abs(literal) - 1);
        }
        return hash;
      }
      
      /*! 
	\fn hash_index remove_literal_in_hash(hash_index  hash, const Literals literal)
	\brief Given a hash value for a Clause, computes a new hash for the 
	clause where the given literal has been removed.

	The key point here is that the given literal is assumed to occur within
	the clause associated with the input hash.
      */
      hash_index 
      remove_literal_in_hash(hash_index  hash, const Literals literal) {
        if (literal < 0) {
          hash -= ipow(3, abs(literal) - 1);
        } else if (literal > 0) {
          hash -= 2 * ipow(3, abs(literal) - 1);
        }
        return hash;
      }
      
      /*!
	\fn unsigned int hash_to_clause(hash_index hash, int clause[], const int nVars)
	\brief Computes the clause represented by a given hash.
       */
      unsigned int 
      hash_to_clause(hash_index hash, int clause[], const int nVars) {
        hash_index iValue = 1;
        Literals numLit = 0;
        for (int lit = nVars; lit > 0; --lit) {
          iValue = ipow(3, abs(lit) - 1);
          // Work out whether the literal is in the hash
          if (hash >= (2 * iValue)) {
            clause[numLit++] = lit;
            hash -= (2 * iValue);
          } else if (hash >= iValue) {
            clause[numLit++] = -lit;
            hash -= iValue;
          }
        }
        return numLit;
      }
      
      /*!
	\fn ClauseSets quine_mccluskey(const ClauseSets& inputCS)
	\brief Given a clause set with only variables of size m where m < nVars,
	all prime implicates of the given clause set are returned.

	Running time and space requirements are exponential (powers of 3) in 
	the number of variables.
       */
      ClauseSets
      quine_mccluskey(const ClauseSets& inputCS) {
        int clause[nVars];
        hash_index nPartialAssignments = ipow(3, nVars);
        std::cerr << "Number of Partial Assignments " << nPartialAssignments << std::endl;
        // Marked is used to keep track of all found clauses 
        HashTable marked(nPartialAssignments, 0);
        // Marked in is used to keep track of all clauses that are still in the 
        //  result set 
        HashTable markedIn(nPartialAssignments, 0);
        Variables clauseSize = 0;
        hash_index hash = 0;
        hash_index partnerHash = 0;
        // First Mark Clauses 
        for (ClauseSets::const_iterator cIter = inputCS.begin();
             cIter != inputCS.end(); ++cIter) {
          hash = hash_clause(*cIter);
          marked[hash] = true;
          markedIn[hash] = true;
        }
        // Perform Algorithm
        for (Variables level = nVars; level > 0; --level) {
          // Output 
          std::cerr << "Level " << (int) level << std::endl;
          // Run through all clauses 
          for (hash_index cIter = 0; cIter < nPartialAssignments; ++cIter) {
            // Go through literals in clause
            if (marked[cIter]) {
              clauseSize = hash_to_clause(cIter, clause, nVars);
              if (clauseSize == level) {
                for (Variables lIter = 0; lIter < clauseSize; ++lIter) {
                  // If it's partner clause exists 
                  partnerHash =
                    flip_literal_sign_in_hash(cIter, clause[lIter]);
                  if (marked[partnerHash]) {
                    long newHash = remove_literal_in_hash(cIter, clause[lIter]);
                    // Work out it's partner exists and add the clause to the next 
                    // level if we don't already have it 
                    marked[newHash] = true;
                    markedIn[newHash] = true;
                    markedIn[cIter] = false;
                    markedIn[partnerHash] = false;
                  }
                }
              }
            }
          }
          // At the end of each level, we only need those clauses that are in 
          // markedIn 
          for (hash_index cIter = 0; cIter < nPartialAssignments; ++cIter) {
            marked[cIter] = markedIn[cIter];
          }
        }
        // Add clauses to CS 
        ClauseSets resultCS;
        for (hash_index cIter = 0; cIter < nPartialAssignments; ++cIter) {
          if (markedIn[cIter]) {
            clauseSize = hash_to_clause(cIter, clause, nVars);
            Clauses sClause(clause, clause + clauseSize);
            
            resultCS.push_back(sClause);
          }
        }
        return resultCS;
      }

    }
  }
}

#endif
