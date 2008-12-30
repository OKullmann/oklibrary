// Oliver Kullmann, 16.1.2006 (Swansea)
/* Copyright 2006 - 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Applications/LatinSquares/plans/general.hpp
  \brief Plans for the module on searching latin squares and generalisations


  \todo Update namespaces.


  \todo Update milestones.


  \todo Update the following todos.


  \todo Review: Software and literature review.


  \todo Connections to other modules
  <ul>
   <li> See
   ComputerAlgebra/CombinatorialMatrices/Lisp/LatinSquares/plans/general.hpp.
   </li>
   <li> See
   ComputerAlgebra/Satisfiability/Lisp/Generators/plans/LatinSquares.hpp
   for generators for SAT-problems at Maxima/Lisp level. </li>
   <li> Compare topic "Generators for finding incidence structures and
   designs" in Transformers/Generators/plans/general.hpp. </li>
  </ul>


  \todo Basic direct algorithms
  <ul>
   <li> Write tests, whether one square is a latin square, and whether
   a sequence of squares is mutually orthogonal (these tests are only for
   result-checking). </li>
   <li> See predicates ls_p, ols_p and mols_p in
   ComputerAlgebra/Satisfiability/Lisp/Generators/tests/LatinSquares.mac.
   </li>
  </ul>


  \todo Active clause-sets
  <ul>
   <li> By combinining injectivity (bijectivity) constraints,
   the extension problem for latin squares and the problem of finding
   mutually orthogonal sets of latin squares (MOLS) shall be provided as
   an active clause-set. </li>
   <li> Perhaps the matching techniques
   underlying the injectivity constraints can be generalised?! </li>
   <li> Perhaps is makes sense for an array v_ij of variables
   to have an active clause-set LS((v_ij)), expressing that the variables
   form a latin square. </li>
   <li> The direct implementation of LS((v_ij))
   uses INJ((v_1j, ..., v_nj) and INJ(v_i1, ..., v_in). </li>
   <li> Such active clause-sets do not offer poly-time performance for (e.g.)
   satisfiability checking, but they might be useful if it has
   nevertheless "special knowledge". LS(V) could offer
   only incomplete services (then it should be poly-time), or it
   might offer complete services (then it won't be poly-time).
  </ul>


  \todo Completing partial latin squares
  <ul>
   <li> Every partial latin squares of order n with n-1 entries can be
   completed (and there exists an efficient algorithm for it): How do the
   various (generalised) SAT algorithms perform here? </li>
   <li> Consider a partial latin square of order n with n+k entries. One could
   conjecture that it can be completed iff the alliance of active clause-sets
   given by the bijectivity-constraints is not found inconsistent by r_k
   reduction? (The case k = 0 follows by [Andersen, Hilton, 1983].) </li>
  </ul>


  \todo Strong hypergraph colouring
  <ul>
   <li> A latin square of dimension n is nothing else than a
   hypergraph G_n with 2n hyperedges of size n for the
   rows and columns together with the strong n-colouring
   problem for hypergraphs
   (see Hypergraphs/Colourings/plans/StrongColouring.hpp). </li>
   <li> What can be done specially for latin squares, which cannot
   be done in general for the strong hypergraph colouring problem?
   (For example regarding symmetries?) </li>
  </ul>


  \todo MOLS
  <ul>
   <li> MOLS((v_ijk)) can be expressed by
    <ol>
     <li> LS((v_ij1)), ..., LS((v_ijn))
     <li> for 1 <= k1 < k2 <= k introduce variables w_{ij(k1,k2)} with
     domains {1,...,n}^2 and
     w_{ij(k1,k2)} = (v1, v2) <-> v_{ijk1} = v1 and v_{ijk2} = v2; </li>
     <li> now require INJ(w_{ij(k1,k2)}) for every pair (k1,k2). </li>
    </ol>
   </li>
   <li> The second type of active clause-sets can be captured by active
   clause-sets MOS((v_ij), (v'_ij)) ("mutually orthogonal squares"; note that
   this notion makes sense for arbitrary squares). Thus MOLS((v_ijk)) becomes
   the conjunction of LS for all single squares and MOS for all
   pairs of squares. </li>
   <li> The new variables w are only necessary if we want to use INJ directly
   on variables; expressing INJ "by hand", we can express MOS(v, v') as a
   conjunction of of (n^2 over 2) * n^2 = n^4 * (n^2 - 1) / 2 4-clauses using
   only the variables v, v'. </li>
   <li> If n is the uniform domain size (the order of the latin square), then
   for all 2-sets {(p,q), (p',q')}, where p,q,p',q' in {1, ..., n}, and for all
   eps, eps' {1, ..., n} we get the 4-clause
   (v_pq <> eps or v_p'q' <> eps or v'_pq <> eps' or v'_p'q' <> eps'). </li>
  </ul>


  \todo Sizes
  <ul>
   <li> Thus the sizes for the direct clause representation are (using LS(n)
   for the clause-set expressing the condition on the existence of a latin
   square of order n, and MOS(n,k) for the condition on the existence of k
   squares of order n which are mutually orthogonal):
    <ol>
     <li> LS(n) has 2*n*binom(n,2)*n = n^3 * (n-1) = O(n^4) 2-clauses
     (using n^2 variables, each with domain size n). </li>
     <li> MOS(n, k) has binom(k,2)*n^4*(n^2-1)/2 = k*(k-1)*n^4*(n^2-1)/4 =
     O(k^2 * n^6) 4-clauses (using k*n^2 variables, each with domain size n).
     </li>
     <li> MOLS(n,k) has k * n^3 * (n-1) = O(k * n^4) 2-clauses and 
     k*(k-1)*n^4*(n^2-1)/4 = O(k^2 * n^6) 4-clauses
     (using k*n^2 variables, each with domain size n). </li>
    </ol>
   </li>
   <li> For n=10 we get that MOLS(n,k) has k * 9000 2-clauses (9000 for each
   LS), and k*(k-1) * 247500 4-clauses, and that with k * 100 variables of
   domain size 10. </li>
   <li> For k=3 we get 27,000 2-clauses and 1,485,000 4-clauses with 300
   variables of domain size 10, which is pretty big. In principle one can
   run a SAT solver on it, but likely without active clause-sets we won't have
   a chance. </li>
  </ul>


  \todo N(n)
  <ul>
   <li> For n >= 1 let N(n) be the maximal number of MOLS. Thus
    <ol>
     <li> N(1) = 1 </li>
     <li> N(n) <= n - 1 for n >= 2 </li>
     <li> N(n) = n - 1 if n = p^k for a prime number p and a natural number
     k >= 1 </li>
     <li> N(6) = 1 </li>
    </ol>
   </li>
   <li> This determines N(n) for 1 <= n <= 9. The first open case is N(10).
   </li>
   <li> It is known
    <ol> 
     <li> n notin {1, 2, 6} -> N(n) >= 2 </li>
     <li> n >= 2: either N(n) = n - 1 or N(n) <= n - 4 </li>
     <li> N(10) <> 9 (computer experiments showed that there is no
     projective plane of order 10). </li>
    </ol> 
    Thus 2 <= N(10) <= 6 (more recent results?). Can we improve this?! </li>
   <li> The next case is N(12). It is known that 5 <= N(12) <= 11 (more
   recent results?); can we improve this?! </li>
   <li> Apparently for all n >= 11 it is known N(n) >= 3 ?! </li>
   <li> Two approaches possible: starting from above,
   showing that the problem is unsatisfiable, or starting from below,
   showing that the problem is satisfiable.) </li>
   <li> In general we have:
    <ol>
     <li> N(n) = n - 1 iff an affine plane of order n exists iff a projective
     plane of order n exists. </li>
     <li> It is stated in [Beth, Jungnickel, Lenz] that the exact value of N(n)
     is only known for n = p^k, p prime number, k >= 0, and for n = 6. </li>
    </ol>
   </li>
  </ul>


  \todo Experiments
  <ul>
   <li> So we could try decide MOLS(10,3) or MOLS(10,6) --- in both cases any
   result SAT/UNSAT would yield a new bound (most interesting:
   MOLS(10,3) in UNSAT => N(10) = 3, MOLS(10,6) in SAT => N(10) = 6). </li>
   <li> Not a new result, but still interesting (and publishable) would be,
   if a generalised  SAT solver could show the unsatisfiability of some
   MOLS(10,k) for 7 <= k <= 9. </li>
  </ul>


  \todo Pairs of orthogonal latin squares
  <ul>
   <li> MOLS(n,2) in SAT for all n notin {1,2,6} yields interesting
   satisfiable problems. </li>
  </ul>


  \todo The upper bounds for N(k)
  <ul>
   <li> MOLS(n,n) in UNSAT for all n yields unsatisfiable problems, which
   could also be interesting (at least for active clause-sets; real clause-sets
   get very soon very big). </li>
  </ul>

*/

/*!
  \namespace OKlib::LatinSquares
  \brief Tools for latin squares and generalisations and refinements.
*/

namespace OKlib {
  namespace LatinSquares {
  }
}

