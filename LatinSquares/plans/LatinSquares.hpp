// Oliver Kullmann, 16.1.2006 (Swansea)

/*!
  \file LatinSquares/plans/LatinSquares.hpp
  \brief Plans for the module on latin squares and generalisations

  \todo Concepts: Create a (preliminary) concept, based on a matrix concept.

  \todo Review: Software and literature review.

  \todo Basic direct algorithms:
  Write tests, whether one square is a latin square, and whether
  a sequence of squares is mutually orthogonal.

  \todo By combinining injectivity (bijectivity) constraints,
  the extension problem for latin squares and the problem of finding
  mutually orthogonal sets of latin squares (MOLS) shall be provided as
  an alliance of active clause-sets. Perhaps the matching techniques
  underlying the injectivity constraints can be generalised?!)

  \todo Perhaps is makes sense for an array v_ij of variables
  to have an active clause-set LS((v_ij)), expressing that the variables
  form a latin square. The direct implementation of LS((v_ij))
  uses INJ((v_1j, ..., v_nj) and INJ(v_i1, ..., v_in).
  How to find all unit clauses for LS((v_ij)) ?!

  \todo MOLS((v_ijk)) can be expressed by
   - LS((v_ij1)), ..., LS((v_ijn))
   - for 1 <= k1 < k2 <= k introduce variables w_{ij(k1,k2)} with
     domains {1,...,n}^2 and
     w_{ij(k1,k2)} = (v1, v2) <-> v_{ijk1} = v1 and v_{ijk2} = v2;
     now require
     INJ(w_{ij(k1,k2)})
     for every pair (k1,k2).
   The second type of active clause-sets can be captured by active
   clause-sets MOS((v_ij), (v'_ij)) ("mutually orthogonal squares"; note that this
   notion makes sense for arbitrary squares). Thus MOLS((v_ijk)) becomes
   the conjunction of LS for all single squares and MOS for all
   pairs of squares.
   The new variables w are only necessary if we want to use INJ directly on variables;
   expressing INJ "by hand", we can express MOS(v, v') as a conjunction of of
   (n^2 over 2) * n^2 = n^4 * (n^2 - 1) / 2 4-clauses using only the variables v, v'.
   If n is the uniform domain size (the order of the latin square), then for all 2-sets {(p,q), (p',q')},
   where p,q,p',q' in {1, ..., n}, and for all eps, eps' {1, ..., n} we get the 4-clause
   (v_pq <> eps or v_p'q' <> eps or v'_pq <> eps' or v'_p'q' <> eps').

  \todo Thus the sizes for the direct clause representation are (using LS(n)
   for the clause-set expressing the condition on the existence of a latin square
   of order n, and MOS(n,k) for the condition on the existence of k squares of
   order n which are mutually orthogonal):
   - LS(n) has 2*n*binom(n,2)*n = n^3 * (n-1) = O(n^4) 2-clauses
     (using n^2 variables, each with domain size n)
   - MOS(n, k) has
     binom(k,2)*n^4*(n^2-1)/2 = k*(k-1)*n^4*(n^2-1)/4 = O(k^2 * n^6)
     4-clauses (using k*n^2 variables, each with domain size n).
   - MOLS(n,k) has k * n^3 * (n-1) = O(k * n^4) 2-clauses and
     k*(k-1)*n^4*(n^2-1)/4 = O(k^2 * n^6) 4-clauses
     (using k*n^2 variables, each with domain size n).
     For n=10 we get that MOLS(n,k) has
     k * 9000 2-clauses (9000 for each LS), and
     k*(k-1) * 247500 4-clauses,
     and that with k * 100 variables of domain size 10.
     For k=3 we get 27,000 2-clauses and 1,485,000 4-clauses with 300 variables
     of domain size 10, which is pretty big. In principle one can
     run a SAT solver on it, but likely without active clause-sets we won't have
     a chance.
  \todo For n >= 1 let N(n) be the maximal number of MOLS. Thus
   - N(1) = 1
   - N(n) <= n - 1 for n >= 2
   - N(n) = n - 1 if n = p^k for a prime number p and a natural number k >= 1
   - N(6) = 1
   This determines N(n) for 1 <= n <= 9. The first open case is N(10).
   It is known
    - n notin {1, 2, 6} -> N(n) >= 2
    - n >= 2: either N(n) = n - 1 or N(n) <= n - 4
    - N(10) <> 9 (computer experiments showed that there is no
      projective plane of order 10).
    Thus 2 <= N(10) <= 6 (more recent results?). Can we improve this?!
    The next case is N(12). It is known that 5 <= N(12) <= 11 (more
    recent results?); can we improve this?!
    (Two approaches possible: starting from above,
    showing that the problem is unsatisfiable, or starting from below,
    showing that the problem is satisfiable.) In general we have:
     - N(n) = n - 1 iff an affine plane of order n exists iff a projective plane
       of order n exists.
     It is stated in [Beth, Jungnickel, Lenz] that the exact value of N(n) is only known
     for n = p^k, p prime number, k >= 0, and for n = 6.

  \todo So we could try decide MOLS(10,3) or MOLS(10,6) --- in both cases any result
     SAT/UNSAT would yield a new bound (most interesting:
     MOLS(10,3) in UNSAT => N(10) = 3, MOLS(10,6) in SAT => N(10) = 6).
     Not a new result, but still interesting (and publishable) would be, if a generalised
     SAT solver could show the unsatisfiability of some MOLS(10,k) for 7 <= k <= 9.

  \todo MOLS(n,2) in SAT for all n notin {1,2,6} yields interesting satisfiable problems.

  \todo MOLS(n,n) in UNSAT for all n yields unsatisfiable problems, which could also
   be interesting (at least for active clause-sets; real clause-sets get very soon very big).

  \todo Sudoku:
  Given n in N, let I_1 := {1, ..., n} and I_2 := I_1^2, I_2' := {1,...,n^2}.
  Variables are v_{i,j} for i, j in I_2 with domains D(v) = I_2'.
  The clauses are
   1a) For all i in I_2: INJ({v_{i,j} : j in I_2})
   1b) For all j in I_2: INJ({v_{i,j} : i in I_2})
   2) For all i, j in I_1: INJ( { v_{(i,k), (k',j)} : k, k' in I_1 } ).
  Additionally a list of domain-restrictions can be specified.
  In terms of latin squares we can express 1a+1b as
   1) LS((v_ij)_ij).
*/

/*!
  \namespace OKlib::LatinSquares
  \brief Tools for latin squares and generalisations and refinements.
*/

namespace OKlib {
  namespace LatinSquares {
  }
}

