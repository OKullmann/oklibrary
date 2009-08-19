// Oliver Kullmann, 4.9.2008 (Swansea)
/* Copyright 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/Generators/plans/RamseyProblems.hpp
  \brief Plans for Maxima-generators for %Ramsey-problems


  \todo MG must completely update this file!
  <ul>
   <li> Also the milestones have been basically completely disregarded. </li>
  </ul>


  \todo Create a systematic naming scheme
  <ul>
   <li> Compare "Improve generator names" in
   ComputerAlgebra/Satisfiability/Lisp/Generators/plans/HindmanProblems.hpp.
   </li>
   <li> See the general module ComputerAlgebra/RamseyTheory. </li>
   <li> A systematic naming scheme across all Ramsey-type generators in this
   module is needed. </li>
  </ul>


  \todo Implement the general (i.e., non-diagonal) boolean case


  \todo Provide specification for all the functions


  \todo Extend Ramsey symmetry breaking generators to generate Extended Dimacs
  <ul>
   <li> Currently there are no output functions for the %Ramsey symmetry
   breaking generators, such as "ramsey2_symbr2_cs". </li>
   <li> Additionally, simply generating Dimacs files for the generated 
   clause-sets would mean losing information about which variables are which
   edges, and also presents problems, as the C++ Ramsey generator outputs
   extended Dimacs. </li>
   <li> Also, there is currently no way to translate the extended Dimacs file 
   output by the C++ generator to a Dimacs file using the correct 
   colexicographical variable ordering/naming. </li>
   <li> Therefore, output functions are needed for each of the symmetry
   breaking techniques which output in the same extended Dimacs format used
   in the C++ generator. </li>
   <li> This should be possible by extending the functionality of output_fcs,
   by redefining dimacs_l_string (see 
   ComputerAlgebra/Satisfiability/Lisp/ClauseSets/BasicOperations.mac). </li>
  </ul>


  \todo Relations to other modules
  <ul>
   <li> Compare "Ramsey graphs" in
   ComputerAlgebra/Hypergraphs/Lisp/plans/Generators.hpp. </li>
   <li> See Ramsey.cpp. </li>
   <li> See Experimentation/Investigations/RamseyTheory/RamseyProblems/plans/general.hpp. </li>
   <li> See ComputerAlgebra/RamseyTheory/Lisp/Ramsey/plans/general.hpp. </li>
  </ul>


  \todo Extreme cases
  <ul>
   <li> One needs to think about the definition of the extreme cases; it seems
   there are inconsistencies in our tests. </li>
   <li> See "Ramsey hypergraphs" in
   ComputerAlgebra/Hypergraphs/Lisp/plans/Generators.hpp. </li>
  </ul>


  \todo Improve docus
  <ul>
   <li> The discussion of how the variables are given needs improvement. </li>
   <li> The "reason" for using the variables "colv" is that they come from
   the hypergraph colouring problem! </li>
   <li> And the main translation should not use the "extended" Dimacs format,
   which is not used outside the OKlibrary, but should use the Dimacs format.
   </li>
   <li> The extension can be discussed, but as an extension. </li>
  </ul>


  \todo More efficient computation
  <ul>
   <li> For larger %Ramsey problems, we need the ability to compute directly
   the standardised clause-set (which just consists of the hypergraph edges
   in two versions, positive (as they are) and negative), without further
   renaming. </li>
   <li> This can be achieved by using the standardised order as discussed in
   "Ramsey hypergraphs" in
   ComputerAlgebra/Hypergraphs/Lisp/plans/Generators.hpp. </li>
   <li> Compare "Colouring problems" in
   ComputerAlgebra/Satisfiability/Lisp/Generators/plans/general.hpp. </li>
   <li> A fundamental design question is whether at the point of creation
   or later the renaming happens; compare "Ramsey hypergraphs" in
   ComputerAlgebra/Hypergraphs/Lisp/plans/Generators.hpp. </li>
  </ul>


  \todo Symmetry breaking by using symmetries of the solution space
  <ul>
   <li> Compare with "Symmetry breaking" in
   Experimentation/Investigations/plans/RamseyProblems.hpp. </li>
   <li> Consider the K_n, and parameters r=2 and q. </li>
   <li> Partial assignments phi assign the edges of K_n. </li>
   <li> The basic fact is that any 2-edge-labelling of K_q, which is
   not monochromatic, must contain two incident edges of different
   values (for q >= 3). </li>
   <li> So phi must contain for every q-clique a "multi-coloured" path of
   length 2. Without case distinctions it seems not possible to strengthen
   that. </li>
   <li> By symmetry (using the symmetries induced by S_n on the vertices)
   these two incident edges can be any pair of incident edges (for that
   we only need to be able to (arbitrarily) permute the vertices of
   the chosen clique). </li>
   <li> So, if there is a subset T <= V(K_n) of size q, such that no vertex
   of T has been considered yet, then %w.l.o.g. one can fix the
   values of 2 chosen incident edges to 1 and 2. </li>
   <li> This process can be repeated until no such T exists anymore. </li>
   <li> A question is whether there are better and worse choices for T
   and for the chosen edges? It shouldn't matter much. </li>
   <li> So, as done with "ramsey_symbr1_pass", we can use the partial
   assignment {colv({1,2}),-colv({1,3}), colv({q+1,q+2}), colv({q+1,q+3}),
   ...}. </li>
   <li> The number of assignments is 2 * floor(n/q). </li>
   <li> Since we are not using yet the symmetry between the values 0 and 1,
   we can additionally set one variable to a value we like. </li>
   <li> Choosing the partial assignment {colv({1,4})} seems reasonable ---
   at least it allows to show the unsatisfiability for [[3,3],2;6] via
   unit-clause propagation:
   \verbatim
generalised_ucp1(fcs2cs(apply_pa_fcs(ramsey_symbr1e_pass(3,6), ofcs2fcs(ramsey2_ofcs(3,2,6)))));
 {{}}
   \endverbatim
   </li>
   <li> So altogether 2*floor(n/q)+1 many variables can be eliminated. </li>
   <li> For the parameter tuple [[5,5],2] and 40 <= n <= 44 this makes 17
   eliminated variables; so well, better than nothing. </li>
   <li> One needs now to generalise this method to arbitrary r. </li>
  </ul>


  \todo Symmetry breaking by using %Ramsey-symmetries of the clause-set
  <ul>
   <li> The following considerations seem to be too optimistic:
    <ol>
     <li> The culprit is what "affected" means. </li>
     <li> If we for example assert that for given 18 vertices there must
     be a monochromatic 4-clique, then in order to just choose arbitrary
     4 vertices, one uses the full symmetry group for those 18 vertices
     --- thus they are then "used up". </li>
    </ol>
    Considerations which appear shady are marked by "BUG" in the following.
   </li>
   <li> The idea is, instead of looking at the q-subsets
   of V(K_n), to consider %Ramsey numbers m := NR([p,p],2) < n. </li>
   <li> Again, we are considering some arbitrary given phi. </li>
   <li> If we have an unaffected T <= V(K_n), then we can choose any
   p different a_1, ..., a_p in T, and set them equal (thus replace them
   by a single new variable a). </li>
   <li> It seems that for an affected T it is harder to do something:
   If one edge got colour c, then we had to make a case distinction,
   expressing that the monochromatic p-clique could have colour c or the
   other colour. </li>
   <li> For example for m=6, p=3, we actually know that there must
   exist two monochromatic triangles: However this again seems hard to
   exploit since we need to make case distinctions about the relative
   position of these two triangles. </li>
   <li> Use of the Pigeon Hole Principle to collapse more edges:
    <ol>
     <li> In general, when considering NR([p,p],2) with n vertices, 
     by the pigeonhole principle, given x monochromatic p-cliques, 
     ceil(x / 2) of the p-cliques must be the same colour. </li>
     <li> Therefore it should be possible to collapse half (rounded up) of
     the p-cliques (i.e all the variables representing the edges) to a
     single variable, for each p considered. </li>
     <li> Given the symmetry of the two colours (i.e the fact that
     flipping the colours in a given total assignment doesn't affect
     satisfiability), we can then set one of the above mentioned variables
     (collapsed using the php) to a particular colour. For instance, setting
     half of the p-cliques (for a given p) to colour 1. </li>
     <li> BUG (it seems we can only find 3 monochromatic triangles)
     For example, consider NR([4,4],2), n=18. We find 5
     monochromatic triangles in the graph. At least 3 of these must be
     coloured 1, or at least 3 of these must be coloured 2, therefore, we
     simply represent the equality of colour. This can be done using either
     additional clauses, or by replacing all occurrences of the variables
     (edges) in 3 of the triangles with a single variable (edge). </li>
     <li> BUG (it seems that we can only find 2 monochromatic 4-clique
     and and monochromatic 3-clique)
     Another example, considering NR([5,5],2), n=43. Using the
     calculation below, we choose 7 monochromatic 4-cliques and 4
     monochromatic 3-cliques, therefore we may colour 4 of the 4-cliques the
     same colour, and 2 of the 3-cliques. </li>
     <li> A proof is needed. (This should be similar to the proof for MG's
     idea, as intuitively the idea is that every p-clique, given they are
     vertex-disjoint, can be transformed to any other by some permutation in 
     S_n). </li>
    </ol>
   </li>
   <li> BUG
   It seems that this process yields more variable reductions than
   the above process (using symmetries of the solution space)?
    <ol> 
     <li> The method used below for calculation assumes that, starting with
     l unaffected vertices, if a process/idea needs x unaffected vertices
     for application at each step, and it affects y vertices, then the x - y
     unaffected vertices can be reused at each step, leaving l - y vertices
     to act on, until finally there are x - y <=  z  < x unaffected vertices
     left, at which point, z < x, therefore the process can no longer be
     applied, giving floor((l - (x - y)) / y) possible applications of the
     procedure.
      <ul>
       <li> For example, NR([4,4],2), n=l=18 </li>
       <li> First choose 6 = NR([3,3],2) vertices, and from them, arbitrarily
       choose 3 vertices to form the monochromatic triangle, leaving 3
       vertices affected, and 15 vertices unaffected. </li>
       <li> Choose 6 more, and another 3 for a triangle, leaving 6 vertices
       affected, 12 affected. </li>
       <li> Choose 6 more, and another 3 for a triangle, leaving 9 vertices
       affected, 9 unaffected. </li>
       <li> Choose 6 more, and another 3 for a triangle, leaving 12 vertices
       affected, 6 unaffected. </li>
       <li> Choose 6 more, and another 3 for a triangle, leaving 15 vertices
       affected, 3 unaffected. </li>
       <li> The procedure must stop here as we need 6 unaffected vertices,
       but only have 3. Giving 5 monochromatic triangles. </li>
       <li> Since there must always be (x-y) unaffected vertices more than
       the y we affect, to allow application (i.e x in total), we can simply
       take l - (x-y) and then divide by y (taking the floor as we can not
       apply the procedure for any number of unaffected vertices less than
       x). </li>
      </ul>
     </li>
     <li> (BUG: this refers to the first method, and there we seem only to
     be able to set 2 * 8 = 16 variables)
     Taking NR([5,5],2), n=43 as an example.
      <ul>
       <li> Each multicolour path of length 2 affects 3 vertices, and
       sets two edges/variables. 5 unaffected variables are needed to apply
       the reduction. </li>
       <li> Given the above formula, we have x = 5, y = 3, n = l = 43 for the
       number of multicolour paths of length 2 that we can choose, assuming
       the vertex sets of each path need to be disjoint. </li>
       <li> floor((l - (x - y)) / y) = floor((43 - (2))/3) = floor(41/3) = 13
       vertex-disjoint multicolour paths of length 2. </li>
       <li> Each of these paths comprises of 2 edges which we are setting
       with a colour, therefore there are 13 * 2 = 26 edges/variables being
       set. </li>
      </ul>
     </li>
     <li> BUG (it appears only (4-1) + (4-1) + 1 + (3-1) = 9 variables
     can be eliminated by this "Ramsey method")
     Using MG's suggestion for NR([5,5],2), n=43
      <ul>
       <li> NR([4,4],2) = 18, so x = 18, y = 4, n = l = 43. </li>
       <li> This gives, floor((l - (x - y)) / y) = floor((43 - (18 - 4))/4) =
       floor((43 - 14) / 4) = floor(29 / 4) = 7 vertex-disjoint monochromatic
       4-cliques. </li>
       <li> So therefore we can find 7 vertex-disjoint monochromatic 
       4-cliques. Each clique has 6 edges, representing 6 variables, which
       can be reduced to a single variable, so 7 * 6 = 42 edges, which can
       be represented using only 7 edges, so 42 - 7 = 35 variables removed.
       </li>
       <li> 7 4-cliques affect 7 * 4 = 28 vertices, leaving 43 - 28 = 15
       unaffected vertices. </li>
       <li> NR([3,3],2) = 6,  so x = 6, y = 4, l = 15. </li>
       <li> This gives, floor((l - (x - y)) / y) = floor((15 - (6 - 3)) / 3)
       = floor((15 -3)/3) = floor(12/3) = 4 vertex-disjoint monochromatic
       3-cliques. </li>
       <li> Each 3-clique has 3 edges, representing 3 variables, which can
       be reduced to a single variable, so 4 * 3 = 12 edges/variables before
       reduction, 12 - 4 = 8 removed. %I.e, since 4 variables are being used
       to instead of 12, we have removed/collapsed 8 variables. </li>
       <li> So without taking into account the PHP statement above, we see a
       reduction/loss of 35 + 8 = 43 variables. OK : Likely for the second
       step the above process can be used?! MG : Yes, this is possible,
       and should result in more variables removed/set for n=45. </li>
       <li> Including the PHP statement above, we can set 4 of the 7
       4-cliques to a colour c, and we can replace 2 of the variables
       representing the 4 3-cliques with 1 variable. Therefore we lose an
       additional 5 variables, 4 set to a specific colour and 2 reduced to
       1 (a loss of 1), so 43 + 5 = 48 variables lost. ??? </li>
      </ul>
     </li>
    </ol>
   </li>
   <li> The above process exploits a symmetry of the solution space,
   while this process just relies on a symmetry of the K_n (since
   every(!) total assignment of a K_m will have a monochromatic
   p-clique); on the other hand the above process is trivial, while here
   we rely on (non-trivial) knowledge about %Ramsey numbers. </li>
   <li> Again the question whether the different choices matter? </li>
   <li> In principle a combination with the above process is possible:
   However case distinctions for the relative positions of the path
   of length 2 and the monochromatic q-clique are needed. </li>
   <li> On the other hand, for unaffected subsets we can always choose
   which process we like. </li>
   <li> What is the best way (and place within the library) to implement these
   ideas? Obviously here. 
    <ul>
     <li> What is needed is a function which takes certain properties of
     the %Ramsey problem at hand (probably only "n" is needed), and produces
     a set of clauses which can be added to the problem to map those symmetries
     immediately obvious to the solver. </li>
     <li> There is now a function "ramsey2_sym_break_rec" which generates a set
     of clauses to break the symmetries described in MG's idea above. </li>
     <li> The need for this function to take mappings from variables to 
     their "meanings" should be alleviated once there is are "rank" and
     "unrank" functions for the enumeration of k-subsets 
     (see "Module Enumeration" in ComputerAlgebra/plans/general.hpp ). It
     should also only need the "n" for the %Ramsey problem then. </li>
     <li> Should this function also consider the PHP idea mentioned above?
     </li>
    </ul>
   </li>
   <li> How do the above ideas generalise to %Ramsey problems which aren't of
   the form R([q,q],r) ? </li>
  </ul>


  \todo Symmetry breaking by recursive application of pigeonhole principle
  <ul>
   <li> Another conjectured symmetry breaking technique, making use of the
    pigeonhole principle, is as follows : 
    <ul>
     <li> Fix a vertex v, and then consider edges from v to all other 
     vertices. </li>
     <li> Half (rounded up) of such edges will be of one colour, and so we
     add binary clauses to show the equivalence of the colouring of these
     edges, chosen arbitrarily. </li>
     <li> Apply the same technique recursively to the half of the vertices
     which had an edge to them "coloured" by the first step. </li>
     <li> Apply the same technique recursively to the half of the vertices
     which didn't have an edge to them "coloured" the first step. </li>
    </ul>
   </li>
   <li> A basic implementation seems to be : 
   \verbatim
ramsey_symbr3_cs(n) := ramsey_symbr3_cs_m(1,n)$
ramsey_symbr3_cs_m(m,n) := block([mid_p,rs : {},edge_equivs],
    mid_p : ceiling(((n-1) - (m+1)) / 2) + (m + 1),
    edge_equivs : lunion(map(
        lambda([x], {{-colv(first(x)), colv(second(x))},
          {colv(first(x)), -colv(second(x))}}),
        powerset(map(lambda([i],{m,i}),setmn(m+1,mid_p)),2))),
    if oklib_monitor then
      print("Length = ", mid_p - m, "Edges = ", m, " to ", setmn(m+1,mid_p)),
    rs : union(edge_equivs, 
      if (mid_p - (m+1)) > 2 then ramsey_symbr3_cs_m(m+1, mid_p) else {}, 
      if (n - (mid_p+1)) > 2 then ramsey_symbr3_cs_m(mid_p+1, n) else {}),
    return(rs))$
   \endverbatim
   </li>
   <li> Making the symmetry breaking conceptually clearer:
    <ol>
     <li> Those binary clauses just express a condition (namely that all
     variables in a given set have to have equal values). </li>
     <li> So we should employ a two-step process, where first a kind of
     "constraint" is created, expressing the general meaning, and then
     a second process expresses this via those binary clauses. </li>
     <li> Compare
     ComputerAlgebra/Satisfiability/Lisp/plans/SatisfactionProblems.hpp. </li>
    </ol>
   </li>
   <li> The basic idea here is that once you apply the php initially, you then
   have two sets of vertices (1) ones that have been touched (2) ones that 
   haven't been touched. Within each set ((1) or (2)) the vertices are 
   essentially the same, and so one can simply consider the induced subgraphs
   and apply the same php again. </li>
   <li> Some rough workings suggest that this method should result in 60 
   variable reductions for n=43 (using the same idea as in the other symmetry 
   breaking methods). </li>
   <li> Assuming the correctness of this method, it should result in both more
   conditions (or "variable reductions") and should be easily generalised to
   arbitrary numbers of colours (due to the generality of php). </li>
   <li> Add further rigour to above argument (the main point is the iterated
   application). </li>
   <li> Expand example with workings. </li>
  </ul>


  \todo Considering only labellings with bounded number of particular colours
  <ul>
   <li> An idea by MG is that intuitively, dominating an edge-labelling for a
   %Ramsey problem with more of one label than another only increases the
   probability of a monochromatic clique of that label occurring and so if a
   given %Ramsey problem is satisfiable, then it should be satisfiable with an
   equal amount (up to floor/ceil considerations) of each label in the
   labelling. </li>
   <li> So, for example for ramsey_2^2(4) <= 17, there should be a labelling
   with no monochromatic clique of size 4, where 136/2 = 68 of the edges are
   labelled with one colour, and 68 of the edges are labelled with the other
   colour. </li>
   <li> This is purely a conjecture and so experimental evidence should be
   compiled, that is, cardinality constraints can be added to generated %Ramsey
   problems to restrict the number of each colour to between floor(E(K_n)/2)
   and ceil(E(K_n)/2) for problems for which we have solvers which can
   determine satisfiability. </li>
   <li> See "Using cardinality constraints" in
   Experimentation/Investigations/RamseyTheory/RamseyProblems/plans/SymmetryBreaking.hpp
   . </li>
  </ul>


  \todo Reimplement "Symmetry breaking by using %Ramsey-symmetries of the 
  clause-set"
  <ul>
   <li> First the above todo needs to be completely updated, and merged with
   this todo. </li>
   <li> Function added called "ramsey_symbr2_cs" which takes only "n" and 
   has 
   \verbatim
load("obsubst");
is(opsubst(lambda([x], rank_lex_subsets(x,n)), colv, ramsey_symbr2_cs(n)) = 
   ramsey2_sym_break_rec(setn(n), lambda([x], rank_lex_subsets(x,n)), lambda([x], unrank_lex_subsets(x,n,2))));

true
   \endverbatim
   for a given "n". </li>
   <li> DONE Alter the second %Ramsey symmetry implementation so it makes
   use of the subset enumeration functions (see
   Combinatorics/Lisp/Enumeration/plans/Subsets.hpp). </li>
   <li> DONE This would mean "ramsey2_sym_break_rec" would only depend on n (and 
   optionally k) . </li>
  </ul>


  \todo %Generators for all standard %Ramsey problems
  <ul>
   <li> For every extended %Ramsey tuple [C,r;n] we need translations
   into (non-boolean) clause-sets. </li>
   <li> Compare "Generalised colouring problems" in
   ComputerAlgebra/Satisfiability/Lisp/Generators/plans/general.hpp. </li>
   <li> They shall use a well-defined enumeration of the variables. </li>
  </ul>

*/

