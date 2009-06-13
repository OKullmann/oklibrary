// Oliver Kullmann, 9.6.2007 (Swansea)
/* Copyright 2007, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Hypergraphs/Transversals/plans/ExactTransversals.hpp
  \brief Plans regarding exact transversals

  A transversal of a hypergraph is called <strong>exact</strong> if it
  intersects every hyperedge exactly once. Denote it by ETr(G).


  \todo The basic algorithm
  <ul>
   <li> For every hypergraph G the set ETr(G) can be enumerated in the
   following obvious way:
   \code
template <class Hypergraph>
void enumerate_exact_transversals(const Hypergraph G, std::ostream& out, const Hypergraph::vertex_set U) {
// Assumption: V(G) intersect U is empty.
// Returns the minimal exact transversals of G, with U added, on out (without repetition).
  if (G contains empty hyperedge) return;
  if (G has no hyperedge) {
    out << U;
    return;
  }
  choose v in V(G); // (*)
  { // the minimal exact transversals containing v
    let S be the star of v in G;
    let H be the set of vertices in S;
    let G' be G with the hyperedges of S removed and the vertices from H crossed out;
    enumerate_exact_transversals(G', out, U union {v});
  }
  { // the minimal exact transversals not containing v
    let G' be G with the vertex v crossed out;
    enumerate_exact_transversals(G', out, U);
  }
  return;
}
   \endcode
   <li> Remarks:
   <ol>
    <li> If a linear order on V(G) is given and at (*) the smallest element of
    V(G) is taken, then the transversals are output in lexicographical order.
    </li>
    <li> If the output would be given as a complete list, then parameter U
    wouldn't be needed, but it could be added to the outputs obtained from
    recursive calls. However here we need U, since we output the transversals
    one at a time. </li>
   </ol>
   </li>
   <li> Has this algorithm only polynomial delay for inputs G which are exact
   transversal hypergraphs ? </li>
   <li> Is deciding whether a hypergraph has an exact transversal NP-complete?
   </li>
   <li> By dualisation this decision problem is equivalent to decide, whether a
   hypergraph has a matching (a set of disjoint hyperedges) which is also an
   edge cover (a set of hyperedges covering all edges). </li>
   <li> By complementation it is equivalent to the problem whether an
   independent set I in a hypergraph (a set of vertices not containing any
   hyperedge) exists, such that for every hyperedge H all but one vertices of
   H are in I. </li>
   <li> Apparently the proof of NP-completeness
   is in [Karp 1972]; one should investigate the proof (also implementing the
   reduction). </li>
  </ul>


  \todo Exact transversal hypergraphs (that is, Tr(G) = ETr(G))
  <ul>
   <li> Implement the decision algorithm from [Eiter 1994]; exists
   already as mupad-function "ist_exakt_transversal" in Mupad/Orthogonal.mup.
   </li>
   <li> Implement the decision algorithm for the special class (? open)
   given in [Galesi, Kullmann 2004]; this exist already as mupad-function
   "ist_speziell_exakt_transversal" in Mupad/Orthogonal.mup. </li>
   <li> One would guess that the above general algorithm is actually
   more efficient than the algorithm from [Eiter 1994]? (Exists as
   mupad-function "Transversals_exakt" in Mupad/Orthogonal.mup.) </li>
  </ul>


  \todo Generalisation to boolean CNF
  <ul>
   <li> As Tr(G) was generalised to Tr(F) (see
   HypergraphTransversals/plans/DirectTransversalEnumeration.hpp), we can also
   generalise ETr(G) to ETr(F), that is the set of all minimal partial
   assignments (w.r.t. the standard partial ordering) which satisfy in every
   clause of F exactly one literal. </li>
   <li> Is Tr(F) = ETr(F) (F is exact transversal) also decidable in
   polynomial time? </li>
   <li> It seems the above algorithm for computing ETr(G) (for arbitrary G)
   cannot be (easily) generalised to (boolean) clause-sets F? (The problem is
   the minimality condition.) </li>
   <li> For F which are exact transversal, can we compute Tr(F) in polynomial
   time? </li>
   <li> As remarked above, to decide whether for a positive clause-set an
   exactly satisfying partial assignment exists is NP-complete; what about a
   1-regular hitting clause-set --- is it still NP-complete?!? </li>
   <li> The point here is, that finding an exactly satisfying partial
   assignment for a clause-set F just means to find a clause which clashes
   with every clause from F in exactly one literal (and so, if we can find
   exactly satisfying partial assignment for 1-regular hitting clause-sets
   efficiently, then we can "grow" 1-regular hitting clause-sets; see
   HittingClauseSets/plans/ExtendingHittingClauseSets.hpp). </li>
   <li> It must be emphasised, that an "exactly satisfying assignment" is a
   partial assignment; the corresponding satisfiability problem, finding an
   exactly satisfying (partial) assignment, should be called PXSAT (where
   XSAT is the standard abbreviation for "exact SAT", which uses total
   assignments). </li>
   <li> For example ((a or b) and (a or not b)) is PXSAT-satisfiable, but not
   XSAT-satisfiable. </li>
   <li> Of course, from XSAT follows PXSAT, but the reverse direction does
   not hold in general, as the example shows:
    <ol>
     <li> It holds for positive clause-sets (here we can extend a partial
     assignment to a total one by setting all other variables to 0). </li>
     <li> PXSAT seems to be a harder than SAT (it is not a "SAT-problem" in
     the sense of "systems with partial instantiation" (see [Kullmann,
     Annals of Mathematics and Artificial Intelligence 40: 303-352, 2004]),
     since an extension of an exactly satisfying assignment need not to be
     exactly satisfying again). </li>
    </ol>
   </li>
   <li> Reduction to standard SAT
    <ol>
     <li> For positive clause-sets we can just use the XSAT -> SAT
     translation, which adds all binary clauses excluding the possibility of
     two satisfied literals in a clause. </li>
     <li> This, of course, is the same as translating exact transversal for
     hypergraphs into SAT (the satisfying assignments of the translation,
     after removal of variables set to 0, correspond 1-1 to the exact
     transversals). </li>
     <li> For general (boolean) CNF F a natural translation into a monosigned
     CNF SAT problem over variables with domain {0,1,*}, where * means
     "unassigned", translates every clause C to the corresponding clause C',
     where literal x becomes x' stating that the underlying variable v
     must be equal to 0 (if x is negative) or 1 (if x is positive), and adds
     clauses (v <> e1) of (w <> e2) for clauses C' (as just constructed)
     containing literals (x = e1) and (w = e2) (for different variables v, w).
     </li>
     <li> The translated F' has only total satisfying assignments,
     corresponding exactly to the exactly satisfying (partial) assignments of
     F. </li>
    </ol>
   </li>
  </ul>


  \todo Exact covers
  <ul>
   <li> The dual problem to exact transversals is the "exact cover"-problem;
   see "Exact coverings" in Hypergraphs/Matchings/plans/general.hpp. </li>
  </ul>

*/

