// Oliver Kullmann, 24.3.2008 (Swansea)
/* Copyright 2008, 2010, 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/MinimalUnsatisfiability/plans/DeficiencyOne.hpp
  \brief Plans regarding minimally unsatisfiable clause-sets of deficiency 1


  \todo Decision algorithms
  <ul>
   <li> The two basic decision algorithms for MU(1), by singular DP-reduction
   or by finding accumulation variables. need to be implemented. </li>
   <li> Using DP-reduction:
    <ol>
     <li> Via sdp_reduction_cs(F) we can reduce the clause-set
     w.r.t. singular DP-reduction --- however also the degenerated
     cases are handled there. </li>
     <li> Perhaps it is actually enough to only test whether at the beginning
     and the end the deficiency is 1, since a case of degeneration will
     (permanently) lower the deficiency? </li>
     <li> But due to blocking more variables could vanish (in clauses which
     get "lost"), and then the deficiency can also increase. </li>
     <li> See "Singular DP-reduction" in
     Satisfiability/Lisp/Reductions/plans/DP-Reductions.hpp. </li>
    </ol>
   </li>
   <li> They also extract a tree representation (see "Tree representations"
   below). </li>
   <li> Also for the special cases "saturated" and "marginal" we need
   decision algorithms.
    <ol>
     <li> This can be done by checking each DP-step. </li>
     <li> Alternatively, the input can be checked whether it is hitting
     resp. whether every variable has 1-1-occurrence. </li>
    </ol>
   </li>
  </ul>


  \todo Tree representations
  <ul>
   <li> First task is, given F in MU(1), find some (the first, a random,
   or all) tree representation of F. </li>
   <li> The algorithm:
    <ol>
     <li> Find a variable v with degree 2. </li>
     <li> Find its two occurrences C,D. </li>
     <li> Replace C,D in F by their resolvent R, obtaining F'. </li>
     <li> Apply the algorithm recursively to F', obtain T'. </li>
     <li> In T', replace the leaf labelled with R by a new binary branching
     on v, with new leaves C,D. </li>
    </ol>
   </li>
   <li> Canonicity issues:
    <ol>
     <li> Most natural seems to be an ofcs input FF. </li>
     <li> The task then is to find the first variable with degree 2, "first"
     w.r.t. the order given by FF. </li>
     <li> For that, we need min_variable_degree_v_ofcs, which returns
     the first variable with minimal degree. </li>
     <li> This can be achieved by first setting the variables in the given
     order to 0 in the hash-map. </li>
    </ol>
   </li>
   <li> A basic question is whether tree representations are unique up to
   rooted-tree-isomorphism?
    <ol>
     <li> The most natural point of view seems not to distinguish between
     left and right children. </li>
     <li> If we do distinguish them (so the trees are what is called
     "left-right-trees"), then we only consider var-isomorphisms. </li>
    </ol>
   </li>
  </ul>


  \todo Pebbling contradictions
  <ul>
   <li> Consider a directed acyclic graph G, where the set of source vertices
   is S, and where G has exactly one sink z. Then we get a clause-set in MU(1)
   by considering the vertices of G as variables, taking all vertices as S
   as positive unit-clause, for all other vertices v Horn clauses of the form
   "conjunction of predecessors implies v", plus additionally {-z}. </li>
   <li> In this way we represent some Horn clause-sets in MU(1). Get we also
   all Horn claus-sets? It seems not, since apparently smusat_horn_cs(k) can't
   be obtained in this way. Definitely we always have at least one positive
   and one negative unit-clause. </li>
   <li> The general question arises here about the characterisation of all
   Horn clause-sets in MU(1). </li>
  </ul>


  \todo Small variable-degrees in MU(1)
  <ul>
   <li> Motivated by [Hoory, Szeider, TCS 2005] we consider the
   following decision problem:
    <ul>
     <li> Input a clause-set F and natural number k, r. </li>
     <li> It is also assumed that F does not contain pure literals, and
     and every clause has length at least k. </li>
     <li> Decide whether it is possible by removal of literal occurrences,
     one by one, without ever producing a pure literal and such that
     the minimal clause-length is at least k, to obtain a maximal
     variable-degree of at most r. </li>
     <li> The interesting case here is F in SMU(1), where it is
     guaranteed that the literal-removal-process will never contract
     clauses (since every clause contains a literal occurring only once).
     </li>
     <li> But this procedure is also relevant in general, since the resulting
     clause-set is always an unsatisfiable clause-set with parameters (k,r)
     in the typical setting here (i.e., minimal rank >= k, maximal
     variable-degree <= r). </li>
    </ul>
   </li>
   <li> Let's call it "REMLITOCC". </li>
   <li> The problem is in NP, one likely also NP-complete (also for
   inputs from SMU(1)). </li>
   <li> The general question is to find out for given uniform clause-length k
   what is the smallest possible variable-degree r = f(k)+1 which still allows
   unsatisfiable F, where in this todo we consider F in MU(1). </li>
   <li> See
   ComputerAlgebra/Satisfiability/Lisp/MinimalUnsatisfiability/SmallVariableDegrees.hpp
   for the general case F in MU (where f+1 is called "minvardeg_umu"). </li>
   <li> Back to MU(1), i.e., for f only F in MU(1) are considered, and
   we use f_1 to emphasise this:
    <ol>
     <li> Apparently f_1(3)+1 = 4 and f_1(4)+1 = 5 are the only known precise
     values. </li>
     <li> Interesting also the (minimal) clause-numbers for realising the
     minimal parameter values (since the deficiency is fixed, this is the
     same as minimising the number of variables used). </li>
     <li> [Dubois, DAM 1990] realises (k=3,r=4) with 22 clauses, (k=4,r=6) with
     209 clauses, and (k=5,r=11) with 3986 clauses. </li>
     <li> [Hoory, Szeider] realise (k=4,r=5) (optimal) and (k=5,r=7), and
     other values, but one has to investigate the underlying clause-sets
     (which are not discussed there explicitely). </li>
     <li> We should exhibit such examples. </li>
     <li> We should implement [Hoory, Szeider]; perhaps also [Dubois, DAM 1990]
     contains some constructions. </li>
     <li> As a name for "f_1+1" perhaps we use "minvardeg_umu_d1". </li>
    </ol>
   </li>
   <li> A translation into SAT of course is of interest, directly into
   CNF, or using active clauses (etc.). </li>
   <li> There is an obvious greedy algorithm: see below. </li>
  </ul>


  \todo The heuristical function remlitocc_greedy
  <ul>
   <li> The procedure needs to be randomised. </li>
   <li> Just to find an example for (k=3,r=5) from uniform_usat_hitting_min(m)
   doesn't work for m <= 14. </li>
   <li> Are these instances in MU(1) so large? </li>
   <li> Or should we start from different elements F in SMU(1) ? </li>
   <li> Or is the heuristic so weak?
    <ol>
     <li> For comparison, we need a precise decision algorithm. </li>
    </ol>
   </li>
  </ul>


  \todo Creating marginal elements of MU(1)
  <ul>
   <li> One can create such elements with
   si_inverse_singulardp_fcs(FF,p,0,a,b). </li>
   <li> What is the scope of marginal_musat1(k) ?
    <ol>
     <li> Perhaps, once we have realised the systematic generation of marginal
     elements of MU(1) below, we remove this function. </li>
     <li> Or, if we randomise it, and start with arbitrary saturated elements,
     then we obtain a random element of MMU(1) in some sense; perhaps
     this is worth to study. </li>
    </ol>
   </li>
   <li> In [Kullmann, 2008] it is shown that the marginal elements of
   MU(1) are exactly those whose conflict graph is a tree (and all
   trees are realisable in this way).
    <ol>
     <li> So a good way to create random elements of MMU(1) for a given
     number c of clauses is to create a random tree T with c vertices, and
     to construct the correspond F(T) in MMU(1). </li>
     <li> F(T) corresponds to the trivial biclique partition of T;
     compare "Translations to clause-sets" in
     ComputerAlgebra/Graphs/Lisp/BicliquePartitions/plans/general.hpp. </li>
     <li> Generators for trees are in
     ComputerAlgebra/Graphs/Lisp/Trees/Generators.mac. </li>
     <li> Via counting of isomorphism types of trees ("unlabelled trees") we
     thus obtain the number of isomorphism types of F in MMU(1) with a given
     number of variables. These counting functions should be implemented
     in module Graphs. </li>
     <li> If we only allow var-isomorphisms, what then is the number of
     isomorphism types? </li>
    </ol>
   </li>
   <li> For a given F in SMU(1), perhaps the marginal F' obtainable
   from F via literal-removal correspond exactly to the spanning trees
   of F? So if F has c clauses, then exactly c^(c-2) different marginal
   clause-sets are obtainable? Are these isomorphic iff the conflict-graphs
   are isomorphic? </li>
   <li> One should test how SAT-solvers (with the appropriate preprocessing)
   react to MU(1): SMU(1) definitely should be easy, also the marginal
   elements (since they can be solved by UCP), but close to the marginal
   there could be harder elements? </li>
  </ul>


  \todo Creating saturated elements of MU(1)
  <ul>
   <li> F in SMU(1) has a unique tree representation (see above). </li>
   <li> F, F' in SMU(1) are isomorphic iff their tree representations (as
   rooted trees) are isomorphic, while they are var-isomorphic iff their
   tree representations are isomorphic as left-right-trees (i.e., here we
   differentiate between "left" and "right"). </li>
   <li> So interesting things to do in Graphs/Trees are to enumerate/sample
   all binary trees with and without distinguishing left and right (for
   example there are 2 different such unlabelled structures with 5 vertices
   if distinguishing between left and right, and only one if not). </li>
  </ul>


  \todo Creating all F in MU(1) with var(F) = V
  <ul>
   <li> For a given set V of variables, all elements F in MU(1) with exactly
   this set of variables is to be listed. </li>
   <li> And canonical representatives of isomorphism classes. </li>
   <li> To get the elements of SMU(1), one just creates the binary trees
   where the inner nodes are labelled with unique elements from V.
    <ol>
     <li> We have all2i_rt and rt2lrt_il in
     ComputerAlgebra/Trees/Lisp/Basics.mac. </li>
    </ol>
   </li>
   <li> Given the elements of SMU(1), one can then consider all "thinning outs"
   (removing literal occurrences with creating pure literals):
    <ol>
     <li> In this way some clause-sets are created several times, but this
     seems hard to avoid. </li>
     <li> If there are m occurrences of a literal x, then from 0 to m-1 of
     these occurrences can be removed. </li>
    </ol>
   </li>
  </ul>

*/

