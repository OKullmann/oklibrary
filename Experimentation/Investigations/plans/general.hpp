// Oliver Kullmann, 14.2.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/plans/general.hpp
  \brief Plans for the super-module for collecting data (not tools, but
  actual data)
  
  
  \todo Structure
  <ul>
   <li> Is the name "Investigations" right? </li>
   <li> Is the part Experimentation the right part? </li>
   <li> How shall we present the data? </li>
  </ul>


  \todo Create milestones.


  \todo Pigeonhole formulas
  <ul>
   <li> Collect the known data.
    <ol>
     <li> ComputerAlgebra/Satisfiability/Lisp/Backtracking/plans/OKsolver2002.hpp
     </li>
    </ol>
   </li>
   <li> Likely we should have a dedicated sub-module. </li>
   <li> Conflict-independence number:
    <ol>
     <li> weak_php(1,0) -> 1 </li>
     <li> weak_php(2,1) -> 2 </li>
     <li> weak_php(3,2) -> 6 </li>
     <li> weak_php(4,3) -> 18 </li>
     <li> weak_php(5,4) -> 40 </li>
     <li> weak_php(6,5) -> 75 </li>
     <li> weak_php(7,6) -> 126 </li>
    </ol>
    We should be able to figure this out; see below.
   </li>
   <li> The conflict-partition_number as upper-bounded by the length of
   hitting_decomposition_m_cs(weak_php(n+1,n)[2]) seems to be the
   same as the conflict-independence number. Perhaps the conflict-graph
   is even perfect?! </li>
   <li> In general: Is the hermitean deficiency actually an upper bound
   on the conflict-partition-number ?!? </li>
   <li> Hermitean rank and deficiency:
    <ol>
     <li> weak_php(1,0) -> [0,1] </li>
     <li> weak_php(2,1) -> [1,2] </li>
     <li> weak_php(3,2) -> [3,6] </li>
     <li> weak_php(4,3) -> [4,18] </li>
     <li> weak_php(5,4) -> [5,40] </li>
     <li> weak_php(6,5) -> [6,75] </li>
    </ol>
    We should be able to figure this out:
     <ol>
      <li> hermitean_rank(weak_php(m+1,m)) = m+1 </li>
      <li> hermitean_deficiency and conflict_independence_number follow
      suit. </li>
     </ol>
     Analogously to "eigensharp", we can create a notion for clause-sets where
    the conflict-indepence-number equals the hermitean defect, and show that
    weak_php is an instance (while not being eigensharp).
   </li>
   <li> The number of prime implicates for satisfiable php would be
   interesting. </li>
   <li> Minimally unsatisfiable sub-clause-sets of phi * weak_php(m,n) for
   m > n:
    <ol>
     <li> Are these all isomorphic to some weak_php(k+1,k) ? </li>
     <li> No; for example
     \verbatim
L3 : random_splitting_mus(weak_php(7,5)[2],3,dll_simplest_trivial2)$
map(deficiency_cs,L3);
  [51, 47, 43, 39, 36, 32, 29, 26, 24, 20, 8, 7, 5, 3, 1, 1, 1, 1]
     \endverbatim
     but
     \verbatim
for n : 1 thru 5 do print(deficiency_weak_php(n+1,n));
1 3 10 25 51
     \endverbatim
     so there are new deficiencies! </li>
     <li> Which deficiencies can be realised? </li>
     <li> What can be said about the clause-sets obtained? </li>
     <li> Perhaps deficiency 2 cannot be realised? </li>
     <li> What are the types obtained for deficiency 3 ? Experiment:
     \verbatim
def3cls : [];
experiment(m,n) := block(
 [count : 0, F : weak_php(m,n)[2]],
  for seed : 0 do block(
   [L : random_splitting_mus(F,seed,dll_simplest_trivial2), S],
    print("seed:", seed, map(deficiency_cs,L)),
    S : sublist(L,lambda([F],is(deficiency_cs(F) = 3))),
    if not emptyp(S) then (
      S : map(singular_dp_reduction, S),
      def3cls : append(def3cls,S),
      for i : 1 thru length(S) do print(count + 1, ":", S[i]),
      count : count + length(S)
    )))$
experiment(7,5);
     \endverbatim
     </li>
    </ol>
   </li>
  </ul>


  \todo Sat-probability-approximations
  <ul>
   <li> Likely we should have a dedicated sub-module. </li>
   <li> Study the sequence of sat-probability-approximations (as computed by
   satprob_seqap in
   ComputerAlgebra/Satisfiability/Lisp/Counting/InclusionExclusion.mac). </li>
   <li> Clearly strict upper and lower bounds alternate, until the final
   entry is correct. </li>
   <li> One could conjecture that the distance to the correct value first
   strictly increases, and then strictly decreases.
    <ol>
     <li> Verified with weak_php(m+1,m) for 0 <= m <= 3. </li>
    </ol>
   </li>
   <li> Perhaps even stronger statements hold?!? </li>
  </ul>


  \todo Low variable-degrees and high clause-lengths
  <ul>
   <li> This should become its own module. </li>
   <li> The general question considers clause-sets F (boolean and also
   non-boolean) with minimal clause-length k and maximal variable-degree
   r. How low can r be so that still unsatisfiable instances are possible?
   </li>
   <li> One can restrict the clause-sets to uniform clause-sets. </li>
   <li> Since elements in MUSAT(1) have relatively the largest number
   of variables possible, one can only consider F in MUSAT(1).
   See
   ComputerAlgebra/Satisfiability/Lisp/MinimalUnsatisfiability/plans/general.hpp.
   </li>
   <li> Or one considers only hitting clause-sets; see
   ComputerAlgebra/Satisfiability/Lisp/ConflictCombinatorics/plans/general.hpp.
   </li>
   <li> One can also demand variable- or literal-regularity (see also the
   considerations for hitting clause-sets). </li>
   <li> Instead variable- or literal-degrees one can consider the maximal
   degrees in the common-variable graph or on the conflict-graph (both
   now are clause-degrees). See again hitting clause-sets. </li>
  </ul>


  \todo Experimental investigations on heuristics
  <ul>
   <li> Above we have precise data on families of problem instances. </li>
   <li> Is now this supermodule also the right place for collecting
   data about computational experiments? </li>
   <li> Finally, a database is needed (like SatEx), and this should go
   somewhere else. </li>
   <li> Taking [Hooker, Vinay, 1995, Branching Rules for Satisfiability]
   as a starting point, we should completely update the data, using
   new heuristics and benchmarks (and also various evaluation techniques).
   But we should stay within the lookahead-paradigm, but perhaps generalised
   on the strength of reduction and look-ahead. </li>
  </ul>

*/

