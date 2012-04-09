// Oliver Kullmann, 27.3.2008 (Swansea)
/* Copyright 2008, 2009, 2010, 2011, 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/Primality/plans/PrimeImplicatesImplicants.hpp
  \brief Plans for Maxima-components regarding computation of prime implicates/implicants of boolean functions and clause-sets


  \todo Tests
  <ul>
   <li> Add tests for all functions, including
    <ul>
     <li> all_minequiv_bvs_rsubhg </li>
     <li> all_minequiv_bvsr_cs </li>
     <li> all_minequiv_bvsr_sub_cs </li>
    </ul>
   </li>
  </ul>


  \todo Organisation
  <ul>
   <li> DONE (to be moved to
   ComputerAlgebra/Satisfiability/Lisp/Primality/plans/general.hpp)
   The resolution-module doesn't seem to be the right place. </li>
   <li> DONE Perhaps we create a whole new module. </li>
   <li> The basic tasks for a given *clause-set* to compute "prime clauses"
   and "dual prime clauses". </li>
   <li> For a boolean function this means that the prime implicates and prime
   implicants are to be computed. </li>
   <li> DONE
   We have this basic terminology problem:
    <ol>
     <li> Coming from boolean functions, an absolute point of view is taken,
     where implicates belong to the "false" side, and "implicants" to the
     "true" side. </li>
     <li> The other point of view (as taken above with "prime clauses" etc.)
     is relative: Within the given representation (CNF or DNF based) either
     stay with the representation and minimise the clauses (given a CNF,
     this would mean the prime implicates), or "go to the other side"
     (for a CNF this means prime implicants). </li>
     <li> So we use "prime clauses" and "dual prime clauses" for the relative
     point of view, and "prime implicates" resp. "prime implicants" for the
     absolute point of view. </li>
    </ol>
   </li>
   <li> See "Dualisation" below. </li>
   <li> See "Connections" below. </li>
   <li> We need generic testfunctions "okltest_prime_clauses_cs",
   "okltest_dual_prime_clauses_cs" and the special cases
   "okltest_prime_clauses_full_cs", "okltest_dual_prime_clauses_full_cs"
   (since there are many algorithms for computing these fundamental
   functions). </li>
  </ul>
  
  
  \todo min_2resolution_closure_cs
  <ul>
   <li> Currently the name likely is not corrected, since if the input is not
   a full clause-set, then likely we do not compute the min-closure under
   2-subsumption resolution (example?). </li>
   <li> What is the complexity of min_2resolution_closure_cs(F) ?
    <ol>
     <li> If we take the input size as 2^n, then the procedure is trivially
     polynomial (altogether there are at most 3^n clauses). But the question
     is whether the running time is polynomial in the real input, that is,
     in F ?! </li>
     <li> We need to count the total number of clauses created. </li>
     <li> This is exactly the number of clauses which follow from F
     (over the same variables; no need to distinguish between the old
     clauses in F and the new created ones, but just altogether is counted).
     </li>
     <li> If F has 2^n clauses, then altogether 3^n clauses are created,
     where (2^n)^(log_2(3)) = 3^n. </li>
     <li> So one could conjecture that the total number of clauses which
     follow from a full clause-set F (over the same variables, of course)
     is at most c(F)^(log_2(3)). </li>
     <li> In "Computations of all prime clauses for full clause-sets"
     in Satisfiability/Lisp/Primality/plans/general.hpp it is shown that
     that number is at most 1/2 c(F)^2 + 1/2 c(F). </li>
    </ol>
   </li>
   <li> Hashing:
    <ol>
     <li> Efficient implementations of min_2resolution_closure_cs rely on
     efficient hashing schemes for clauses. </li>
     <li> In Satisfiability/FiniteFunctions/QuineMcCluskey.hpp a "total"
     hashing scheme is used, that is, simply a boolean array of length 3^n
     is used, with the bijection from clauses with n variables to {0,...,3^n-1}
     as indexing (hashing) scheme. </li>
     <li> This should be efficient for dense clause-sets, but it is very
     inefficient for sparse clause-sets (and somewhat larger n). </li>
     <li> Alternative hashing schemes need to be developed. </li>
    </ol>
   </li>
  </ul>


  \todo Dualisation
  <ul>
   <li> Implement the algorithm given by "dual_cs" in
   ComputerAlgebra/Satisfiability/Lisp/ClauseSets/Constructions.mac
    <ol>
     <li> That is, just subsumption elimination needs to be added. </li>
     <li> How to do this in a better way? For example doing subsumption
     elimination inbetween? </li>
    </ol>
   </li>
   <li> DONE Or should this go somewhere else?? </li>
   <li> DONE
   Should we perhaps have modules "PrimeClauses" and "DualPrimeClauses" ?
   Or one module "Primality" (which contains those two)? </li>
  </ul>


  \todo Connections
  <ul>
   <li> See OKlib/Satisfiability/FiniteFunctions/plans/QuineMcCluskey.hpp
   for the C++ level. </li>
   <li> See
   Experimentation/Investigations/BooleanFunctions/plans/QuineMcCluskey.hpp
   for investigations. </li>
   <li> The above dualisation yields a representation of all
   satisfying assignments:
    <ol>
     <li> See
     ComputerAlgebra/Satisfiability/Lisp/Counting/plans/general.hpp
     </li>
     <li> See AllSolutions/plans/general.hpp, and especially
     AllSolutions/plans/MinimalAssignments.hpp. </li>
    </ol>
   </li>
   <li> One application is discussed in
   ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/plans/SboxAnalysis.hpp.
   </li>
  </ul>


  \todo Hitting clause-sets
  <ul>
   <li> The input of Quine/McCluskey is a (special) hitting clause-set
   (namely a full clause-set) --- can we somehow generalise the algorithm
   to work with arbitrary hitting clause-sets? </li>
   <li> Given a hitting clause-set F, we can easily compute for each
   clause C in F a prime implicate C' <= C of F, by greedily removing
   literals from C and checking whether still F implies C' holds.
    <ol>
     <li> Achieved by "replace_by_prime_implicates_hitting". </li>
     <li> Can all prime implicates of F be obtained in this way (for
     fixed F)? Likely not (examples?). </li>
     <li> Likely there can be exponentially many prime implicates
     C' <= C ? </li>
     <li> And likely it is hard to find some C' of minimal length? </li>
     <li> Once we know phi_C' * F is unsatisfiable, it should be possible
     to read off directly from this computation which literals from C'
     can be further dropped. </li>
    </ol>
   </li>
   <li> In this way we obtain F' <= F consisting of some prime implicates
   of F such that F' is equivalent to F. By removing redundant clauses
   from F' we can further go into the direction of a shortest CNF
   representation of the underlying boolean function.
    <ol>
     <li> Since SAT solvers can be used to obtain F, in this way we can
     harness the power of SAT solvers (w.r.t. computing small splitting
     trees) for computing small CNF/DNF representations. </li>
    </ol>
   </li>
   <li> It should be possible to compute with polynomial delay
   all prime implicates which are subsuming some clause from F.
   See "contained_prime_implicate" below. </li>
  </ul>


  \todo contained_prime_implicate
  <ul>
   <li> Write the more general function "all_contained_prime_implicates",
   which returns the set of all prime implicates contained in S (so now
   we also allow that C doesn't follow from F, which holds iff the returned
   clause-set is empty). </li>
   <li> For a recursive solution the problem is to avoid overlaps. </li>
  </ul>

*/

