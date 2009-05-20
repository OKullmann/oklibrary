// Oliver Kullmann, 6.12.2007 (Swansea)
/* Copyright 2007, 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKlib/Satisfiability/FiniteFunctions/plans/general.hpp
  \brief Super-module for finite functions (especially boolean functions)

  Considered are functions f: D_1 x ... X D_n -> {0,1}, where D_i
  are finite sets (in the boolean case we have F_i = {0,1}).


  \todo Links and plans
  <ul>
   <li> Establish links to other modules (especially computer algebra).
    <ol>
     <li> See
     ComputerAlgebra/Satisfiability/Lisp/FiniteFunctions/plans/general.hpp.
     </li>
     <li> See
     ComputerAlgebra/Satisfiability/Lisp/PropositionalLogic/plans/general.hpp.
     </li>
    </ol>
   </li>
   <li> See
   Buildsystem/ExternalSources/SpecialBuilds/plans/BooleanFunctions.hpp. </li>
   <li> Create a development plan and further milestones. </li>
  </ul>


  \todo Prime implicants and implicates
  <ul>
   <li> This whole topic seems to deserve its own module (and plans-file).
   </li>
   <li> What terminology to use for *clause-sets*, where we could have
   CNF's or DNF (thus prime implicates or prime implicants) ? </li>
   <li> We consider the whole range of generalised clause-sets, from boolean
   clause-sets to signed clause-sets. </li>
   <li> Implicates for F are generalised clauses C which follows from F. </li>
   <li> Between generalised clauses we have the partial order "implies",
   which is quickly decidable. </li>
   <li> Prime implicates are implicates which are minimal %w.r.t. to the
   partial order of implication amongst the type of generalised clauses
   considered. </li>
   <li> The algorithms which works always is to create all resolvents and
   keep the minimal ones (the "resolution algorithm").
    <ol>
     <li> See ProofSystems/Resolution/plans/ResolutionClosure.hpp. </li>
     <li> So in this module we only connect the resolution algorithm
     (which likely shouldn't care about "DNF" or "CNF") to the
     concept of finite functions. </li>
    </ol>
   </li>
   <li> The other main alternative is to start with a CNF or DNF (generalised)
   given, and then compute the minimal satisfying partial assignments (using
   the given literal type), which yields the prime implicates resp.
   the prime implicants (call it the "dualisation algorithm").
    <ol>
     <li> See AllSolutions/plans/MinimalAssignments.hpp. </li>
     <li> So again, likely the general algorithm is not implemented in
     this module. </li>
    </ol>
   </li>
  </ul>


  \todo Quine-McCluskey
  <ul>
   <li> A special situation for the computation of prime implicates/implicants
   is given when we start with the full table, that is, with all full
   implicates and implicants given. </li>
   <li> Likely this module is the right place for this specialisation. </li>
   <li> For the boolean case the algorithm derived fromt the general
   resolution algorithm together with some obvious simplications regarding
   subsumption-elimination is called the "Quine-McClusky algorithm". </li>
   <li> Actually, the algorithm only needs either all full implicates
   (for the prime implicates) or all full implicants (for the prime
   implicants), and both together are only used to make the algorithm
   polynomial time (since then, %e.g. in the boolean case, the input
   is of size 2^n, while a trivial essential uppper bound is 3^n). </li>
   <li> The algorithm in more detail:
    <ol>
     <li> Input is a clause-set F (either CNF or DNF). </li>
     <li> In general a "subsumption-resolution" is one resolution step where
     the resolvent subsumes at least one parent clause. A
     "2-subsumption-resolution" is where both parent clauses are subsumed
     (that is, iff both parent clauses are identical except of the clashing
     literal). </li>
     <li> Let k := n(F), F_k := F. </li>
     <li> While k > 0 do
      <ol>
       <li> F_{k-1} := all 2-subsumption-resolvents from F_k. </li>
       <li> Remove from F_k all clauses which were involved in a
       resolution; and furthermore check whether "by chance" a clause
       in F_k is otherwise subsumed by some clause in F_{k-1} (then that
       clause is also removed). </li>
      </ol>
     </li>
     <li> The union of F_n, ..., F_0 then is the set of prime implicates
     (for CNF) resp. prime implicants (for DNF), where F_k contains the
     clauses of length k. </li>
    </ol>
   </li>
   <li> Correctness follows by the simple fact that when F_k is first
   computated, then it contains exactly all implicates (implicants) of
   size k. and furthermore if a clause of size k was not involved in a
   resolution step when going to level k-1, then it cannot become useful
   later, since we already knew all implicates of size k-1. </li>
   <li> The main point for the efficient implementation is finding the
   2-subsumption-resolutions fast.
    <ul>
     <li> One could think of representing the clauses in the levels F_k
     not directly, but indirectly, how they were created. This should
     save space, and could also be more time efficient. </li>
     <li> The most promising approach seems to me (OK):
      <ol>
       <li> The underlying observation is, that a clause of length k
       has exactly k possible partners for 2-subsumption resolution. </li>
       <li> So at level k (where all clauses have length k) each clause has
       an associated vector of boolean of length k, showing whether on
       the associated variable of the clause a 2-subsumption resolution
       has been done. </li>
       <li> Now processing of level k happens by running through the list
       of clauses, for each clauses finding the available 2-subsumption
       resolution partners, performing the resolutions and entering this
       information into the vectors. </li>
       <li> The main (potential) saving lies in actively searching for
       the resolution partners, which must be supported by some appropriate
       search data structure. Either a search tree or a hash table.
        <ul>
         <li> For small n (<= 16) one can treat clauses as vectors of
         length n with entries 0,1,2, which can be hashed to a bit-vector
         of length 3^n by interpretation as a positional representation
         of a ternary number. </li>
         <li> This vector for n = 16 has 3^16 / 8 = 5380840 bytes, and
         initialising this data structure once shouldn't be a problem
         (setting the bits for existing clauses to 1, others to 0).
         (The hash map could be implemented via template
         metaprogramming.) </li>
         <li> For n=20 we need 436 MB; best to use a macro, with
         default value 18 (48 MB), for the maximal n where such a hash
         table is to be used. </li>
         <li> This hash map can also be used for computing the prime
         implicates via the resolution method in general, however
         since it doesn't solve the subsumption problem it is perhaps
         less useful. </li>
        </ul>
       </li>
       <li> Perhaps one could also use the 2-subsumption resolution graph,
       where the edges are labelled with the resolution variables. </li>
       <li> The vectors saves a factor of 2 (possibly more, since the
       resolvent needs not to be considered for the next level) due to
       not attempting the second time a resolution. When using the
       above hash table, then perhaps it is not worth the effort. </li>
       <li> Further savings could be achieved by more generally not
       attempting resolution steps where the resolvent has already
       been obtained. </li>
      </ol>
     </li>
    </ul>
   </li>
   <li> One also has to think about the additional subsumption
   checks. (We also need an example to see that these additional checks
   are indeed needed.) </li>
   <li> Many implementations of the Quine-McCluskey algorithm seem to 
   include an additional minimisation step to calculate all essential prime 
   implicates/implicants, removing certain clauses after the steps listed 
   above.
    <ol>
     <li> For example, the above algorithm given 
     {{- 3, - 1, 2}, {- 3, 1, 2}, {- 2, - 1, 3}, {- 1, 2, 3}} returns
     {{- 3, 2}, {- 1, 2}, {- 1, 3}}, however the input clause set is
     equivalent to {{-3,2},{-1,3}} and the R implementation mentioned in 
     Buildsystem/ExternalSources/SpecialBuilds/plans gives {{-3,2},{-1,3}
     as well. </li>
     <li> These are likely computed by the "Carnaugh maps", which we also
     need to do. </li>
    </ol>
   </li>
   <li> Is there a way of using both the satisfying and the falsifying
   total assignments? Likely the way to do this is to combine the resolution
   algorithm with the dualisation algorithm. </li>
   <li> Can the Quine/McClusky-algorithm be generalised from boolean
   clause-sets to more general forms of clause-sets?
    <ol>
     <li> For clause-sets with non-boolean variables it should be similar
     (only that for a variable of domain size m then m parent
     clauses are needed). </li>
     <li> Questionable for general signed clause-sets. </li>
    </ol>
   </li>
  </ul>


  \todo C++ implementation of Quine-McCluskey
  <ul>
   <li> A C++ implementation is needed for finding prime implicates as the 
   current maxima implementation simply takes far too long. </li>
   <li> What parts of Satisfiability/ in the library would be useful (classes 
   for clausesets, clauses etc)? </li>
   <li> What other considerations are there?
    <ul>
     <li> Naming conventions? </li>
     <li> Efficiency Considerations? </li>
     <li> Testing? </li>
    </ul>
   </li>
   <li> An initial (poor) implementation by MG :
   <ul> 
    <li> QuineMcCluskey.cpp : 
    \verbatim
#include "SATParser.hpp"
#include "QuineMcCluskey.hpp"
#include<fstream>
#include<iostream>
#include<set>

int main(int argc, const char* argv[]) {
    std::ifstream inputFile (argv[1]);
    std::vector< std::vector<int> > clauseSet = readDIMACSFormat(&inputFile); 
    std::vector< std::vector<int> > resultSet = quineMcCluskey(clauseSet);
    // List Clauses
    for (std::vector< std::vector<int> >::iterator iter = resultSet.begin(); iter != resultSet.end(); iter++) {
       printClause(*iter); 
    }
}
    \endverbatim
    </li>
    <li> QuineMcCluskey.hpp :
    \verbatim
#include<set>
#include<vector>
#include<set>
#include<algorithm>
#include<cmath>

signed int numVars (std::vector < std::vector < signed int > >&cs)
{
  std::set<unsigned int> variables;
  std::vector<std::vector<signed int> >::iterator cIter;
  for (cIter = cs.begin(); cIter != cs.end(); cIter++) {
    std::vector<signed int>::iterator lIter;
    for (lIter = (*cIter).begin(); lIter != (*cIter).end();lIter++) {
        variables.insert(abs(*lIter));
    }
  }
  return variables.size();
}

void printClause (std::vector < signed int >&clause)
{
  std::vector < signed int >::iterator iter;

  for (iter = clause.begin (); iter != clause.end (); iter++) {
    std::cout << (int) *iter;
    std::cout << " ";
  }
  std::cout << "0" << std::endl;
}

void printClauseSet (std::vector < std::vector < signed int > >&clauseSet)
{
  std::vector < std::vector < signed int > >::iterator iter;

  for (iter = clauseSet.begin (); iter != clauseSet.end (); iter++) {
      printClause (*iter);
  }
}

long ipow (signed int b, signed int e)
{
  long result = 1;

  while (e-- > 0) {
      result *= b;
  }
  return result;
}

// Hash considers 0 = variable not in clause, 1 = variable occurs negated in 
// clause, 2 = variable occurs positively in clause 
long hashClause (std::vector < signed int >&clause)
{
  long returnValue = 0;

  std::vector < signed int >::iterator iter;

  for (iter = clause.begin (); iter != clause.end (); iter++) {
    if (*iter < 0) {
      returnValue += ipow (3, abs (*iter) - 1);
    } else if (*iter > 0) {
      returnValue += 2 * ipow (3, abs (*iter) - 1);
    }
  }
  return returnValue;
}

// Given a hash for a clause and a literal (within the clause represented by the 
// hash), return a new hash representing a clause where the literal has the 
// opposite sign 
long flipLiteralSignInHash (long hash, signed int literal)
{
  if (literal < 0) {
      hash += ipow (3, abs (literal) - 1);
  } else if (literal > 0) {
      hash -= ipow (3, abs (literal) - 1);
  }
  return hash;
}

long removeLiteralInHash (long hash, signed int literal)
{
  if (literal < 0) {
      hash -= ipow (3, abs (literal) - 1);
  } else if (literal > 0) {
      hash -= 2 * ipow (3, abs (literal) - 1);
  }
  return hash;
}

unsigned int hashToClause (long hash, signed int clause[], signed int nVars)
{
  long iValue = 1;

  signed int numLit = 0;

  for (signed int lit = nVars; lit > 0; lit--) {
    iValue = ipow (3, abs (lit) - 1);
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

std::vector < std::vector < signed int > >quineMcCluskey (std::vector < std::vector <
                           signed int > >inputCS)
{
  signed int nVars = numVars (inputCS);

  signed int clause[nVars];

  long nPartialAssignments = ipow (3, nVars);

  std::cerr << "Number of Partial Assignments " << nPartialAssignments << std::endl;
  // Marked is used to keep track of all found clauses 
  std::vector < bool > marked (nPartialAssignments, 0);
  // Marked in is used to keep track of all clauses that are still in the 
  //  result set 
  std::vector < bool > markedIn (nPartialAssignments, 0);
  unsigned int clauseSize = 0;

  int numClausesIn = 0;

  unsigned long hash = 0;

  unsigned long partnerHash = 0;

  unsigned long newHash = 0;

  // First Mark Clauses 
  std::vector < std::vector < signed int > >::iterator cIter;
  for (cIter = inputCS.begin (); cIter != inputCS.end (); cIter++) {
    hash = hashClause (*cIter);
    marked[hash] = true;
    markedIn[hash] = true;
  }
  
  // Perform Algorithm
  for (signed int level = nVars; level > 0; level--) {
    // Output 
    std::cerr << "Level " << (int) level << std::endl;
    // Run through all clauses 
    for (int cIter = 0; cIter < nPartialAssignments; cIter++) {
      // Go through literals in clause
      if (marked[cIter]) {
        clauseSize = hashToClause (cIter, clause, nVars);
        if (clauseSize == level) {
          for (int lIter = 0; lIter < clauseSize; lIter++) {
            // If it's partner clause exists 
            partnerHash =
              flipLiteralSignInHash (cIter, clause[lIter]);
            if (marked[partnerHash]) {
              long newHash = removeLiteralInHash (cIter, clause[lIter]);
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
    for (int cIter = 0; cIter < nPartialAssignments; cIter++) {
      marked[cIter] = markedIn[cIter];
    }
  }


  // Add clauses to CS 
  std::vector < std::vector < int > >resultCS;
  for (int cIter = 0; cIter < nPartialAssignments; cIter++) {
    if (markedIn[cIter]) {
      clauseSize = hashToClause (cIter, clause, nVars);
      std::vector < int >sClause (clause, clause + clauseSize);

      resultCS.push_back (sClause);
    }
  }
  return resultCS;
};
    \endverbatim
    </li>
    <li> SATParser.hpp :
    \verbatim
#include<iostream>
#include<fstream>
#include<vector>

std::vector< std::vector<int> > readDIMACSFormat(std::ifstream * inputFile) {
    int c = 0; // Character reading
    int rawLiteral = -1;
    std::vector< std::vector<int> > clauseSet;
    while ( (c = inputFile->peek()) != EOF ) {
        switch(c) {
            case 'c' :
                inputFile->get();
                // Skip
                while((c = inputFile->get()) != '\n');
                break;
            case 'p' :
                inputFile->get();
                // Skip
                while((c = inputFile->get()) != '\n');
                break;
            default :
                std::vector<int> clause;
                *inputFile >> rawLiteral;
                while (rawLiteral) {
                    clause.push_back(rawLiteral);
                    *inputFile >> rawLiteral;
                }
                clauseSet.push_back(clause);
                // Jump to newline
                c = inputFile->get();
                while((c != '\n') && (c != EOF)) {
                    c = inputFile->get();
                }
                break;
        }
    }
    return clauseSet;
}
    \endverbatim
    </li>
   </ul>
   </li>
  </ul> 


  \todo Minimisation


  \todo BDDs
  <ul>
   <li> In general, the decision diagrams are no longer "binary".
   Perhaps it's called "FDD" ("finite decision diagram") ? </li>
   <li> But at the beginning, especially at the Maxima/Lisp level, we only
   consider the binary case. </li>
  </ul>

*/

/*!
  \namespace OKlib::Satisfiability::FiniteFunctions
  \brief %Algorithms and representations for finite (especially boolean) functions

  Alias "FiF".
*/

namespace OKlib {
  namespace Satisfiability {
    namespace FiniteFunctions {
    }
    namespace FiF = FiniteFunctions;
  }
}

