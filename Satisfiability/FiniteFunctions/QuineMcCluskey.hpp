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
      
      int numVars(const std::vector<std::vector<int> >& cs) {
        std::set<unsigned int> variables;
        for (std::vector<std::vector<int> >::const_iterator cIter = cs.begin();
             cIter != cs.end(); ++cIter) {
          for (std::vector<int>::const_iterator lIter = cIter -> begin();
               lIter != cIter -> end(); ++lIter) {
            variables.insert(abs(*lIter));
          }
        }
        return variables.size();
      }
      
      void printClause(const std::vector<int>& clause) {
        for (std::vector<int>::const_iterator iter = clause.begin();
             iter != clause.end(); ++iter) {
          std::cout << (int) *iter;
          std::cout << " ";
        }
        std::cout << "0" << std::endl;
      }
      
      void printClauseSet(const std::vector<std::vector<int> >& clauseSet) {
        for (std::vector<std::vector<int> >::const_iterator iter = clauseSet.begin();
             iter != clauseSet.end(); ++iter) {
          printClause(*iter);
        }
      }
      
      long ipow(const int b, int e) {
        long result = 1;
        while (e-- > 0) {
          result *= b;
        }
        return result;
      }
      
      // Hash considers 0 = variable not in clause, 1 = variable occurs
      // negated in clause, 2 = variable occurs positively in clause 
      long hashClause(const std::vector<int>& clause) {
        long returnValue = 0;
        for (std::vector<int>::const_iterator iter = clause.begin();
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
      long flipLiteralSignInHash(long hash, const int literal) {
        if (literal < 0) {
          hash += ipow(3, abs(literal) - 1);
        } else if (literal > 0) {
          hash -= ipow(3, abs(literal) - 1);
        }
        return hash;
      }
      
      long removeLiteralInHash(long hash, const int literal) {
        if (literal < 0) {
          hash -= ipow(3, abs(literal) - 1);
        } else if (literal > 0) {
          hash -= 2 * ipow(3, abs(literal) - 1);
        }
        return hash;
      }
      
      unsigned int hashToClause(long hash, int clause[], const int nVars) {
        long iValue = 1;
        int numLit = 0;
        for (int lit = nVars; lit > 0; --lit) {
          iValue = ipow(3, abs(lit) - 1);
          // Work out whether the literal is in the hash
          if ((hash - (2 * iValue)) >= 0) {
            clause[numLit++] = lit;
            hash -= (2 * iValue);
          } else if ((hash - iValue) >= 0) {
            clause[numLit++] = -lit;
            hash -= iValue;
          }
        }
        return numLit;
      }
      
      std::vector<std::vector<int> >
      quineMcCluskey(const std::vector<std::vector<int> >& inputCS) {
        int clause[nVars];
        long nPartialAssignments = ipow(3, nVars);
        std::cerr << "Number of Partial Assignments " << nPartialAssignments << std::endl;
        // Marked is used to keep track of all found clauses 
        std::vector<bool> marked(nPartialAssignments, 0);
        // Marked in is used to keep track of all clauses that are still in the 
        //  result set 
        std::vector<bool> markedIn(nPartialAssignments, 0);
        unsigned int clauseSize = 0;
        int numClausesIn = 0;
        unsigned long hash = 0;
        unsigned long partnerHash = 0;
        unsigned long newHash = 0;
        // First Mark Clauses 
        for (std::vector<std::vector<int> >::const_iterator cIter = inputCS.begin();
             cIter != inputCS.end(); ++cIter) {
          hash = hashClause(*cIter);
          marked[hash] = true;
          markedIn[hash] = true;
        }
        // Perform Algorithm
        for (int level = nVars; level > 0; --level) {
          // Output 
          std::cerr << "Level " << (int) level << std::endl;
          // Run through all clauses 
          for (int cIter = 0; cIter < nPartialAssignments; ++cIter) {
            // Go through literals in clause
            if (marked[cIter]) {
              clauseSize = hashToClause(cIter, clause, nVars);
              if (clauseSize == level) {
                for (int lIter = 0; lIter < clauseSize; ++lIter) {
                  // If it's partner clause exists 
                  partnerHash =
                    flipLiteralSignInHash(cIter, clause[lIter]);
                  if (marked[partnerHash]) {
                    long newHash = removeLiteralInHash(cIter, clause[lIter]);
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
          for (int cIter = 0; cIter < nPartialAssignments; ++cIter) {
            marked[cIter] = markedIn[cIter];
          }
        }
        // Add clauses to CS 
        std::vector<std::vector<int> > resultCS;
        for (int cIter = 0; cIter < nPartialAssignments; ++cIter) {
          if (markedIn[cIter]) {
            clauseSize = hashToClause(cIter, clause, nVars);
            std::vector<int> sClause(clause, clause + clauseSize);
            
            resultCS.push_back(sClause);
          }
        }
        return resultCS;
      }

    }
  }
}

#endif
