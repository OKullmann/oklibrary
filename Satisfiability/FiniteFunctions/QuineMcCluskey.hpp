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
      const int nVars = NUMBER_VARIABLES;
#else
      const int nVars = 4;
#endif

      typedef int Variables;
      typedef int Literals;
      typedef std::vector<Literals> Clauses;
      typedef std::vector<Clauses> ClauseSets;
      typedef std::vector<bool> HashTable;
      typedef HashTable::size_type hash_index;


      /* XXX : Asserts that size types are sufficient are needed here */

      
      void print_clause(const Clauses& clause) {
        for (Clauses::const_iterator iter = clause.begin();
             iter != clause.end(); ++iter) {
          std::cout << (int) *iter;
          std::cout << " ";
        }
        std::cout << "0" << std::endl;
      }
      
      void print_clauseset(const ClauseSets& clauseSet) {
        for (ClauseSets::const_iterator iter = clauseSet.begin();
             iter != clauseSet.end(); ++iter) {
          print_clause(*iter);
        }
      }
      
      hash_index ipow(const int b, int e) {
        long result = 1;
        while (e-- > 0) {
          result *= b;
        }
        return result;
      }
      
      // Hash considers 0 = variable not in clause, 1 = variable occurs
      // negated in clause, 2 = variable occurs positively in clause 
      hash_index hash_clause(const Clauses& clause) {
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
      
      // Given a hash for a clause and a literal (within the clause
      // represented by the hash), return a new hash representing a clause
      // where the literal has the opposite sign 
      hash_index 
      flip_literal_sign_in_hash(hash_index hash, const Literals literal) {
        if (literal < 0) {
          hash += ipow(3, abs(literal) - 1);
        } else if (literal > 0) {
          hash -= ipow(3, abs(literal) - 1);
        }
        return hash;
      }
      
      hash_index 
      remove_literal_in_hash(hash_index  hash, const Literals literal) {
        if (literal < 0) {
          hash -= ipow(3, abs(literal) - 1);
        } else if (literal > 0) {
          hash -= 2 * ipow(3, abs(literal) - 1);
        }
        return hash;
      }
      
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
