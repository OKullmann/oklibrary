// Oliver Kullmann, 19.7.2008 (Swansea)
/* Copyright 2008, 2009, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Hypergraphs/Lisp/Generators/plans/general.hpp
  \brief Plans regarding generators for hypergraphs


  \bug ramseyrv_ohg handles r=0 incorrectly
  <ul>
   <li> ramseyrv_ohg(0,0,0) should return [[rv()],[{rv()}]], not 
   [[rv()],[{{}}]]. </li>
  </ul>


  \todo Connections
  <ul>
   <li> Finally all the generators provided here are also to be provided at
   C++ level in module Combinatorics/Hypergraphs/Generators; see
   Combinatorics/Hypergraphs/Generators/plans/general.hpp. </li>
  </ul>


  \todo Organisation
  <ul>
   <li> The current Generators/Generators.mac needs to be split into several
   files:
    <ol>
     <li> Basics.mac </li>
     <li> VanderWaerden.mac </li>
     <li> GreenTao.mac : DONE </li>
     <li> Schur.mac </li>
     <li> Rado.mac DONE </li>
     <li> Folkman.mac </li>
     <li> Hindman.mac DONE </li>
     <li> Ramsey.mac </li>
    </ol>
   </li>
  </ul>
  
  
  \todo Statistics
  <ul>
   <li> For all generated hypergraphs we need accompanying
   statistic-functions. </li>
   <li> As for clause-set statistics. </li>
  </ul>


  \todo Create complete r-graphs : DONE (using colexicographical order)
  <ul>
   <li> Implement "complete_hg(V,r), complete_stdhg(n,r), complete_ohg(V,r),
   complete_stdohg(n,r)", where the order is given by lexicographical order
   (see "Enumerating all k-subsets lexicographically" in
   ComputerAlgebra/Combinatorics/Lisp/Enumeration/plans/Subsets.hpp). </li>
   <li> However, see "Ramsey hypergraphs" below --- to be used as the
   vertex set of a %Ramsey hypergraph, we needed to use colexicographical
   ordering. </li>
   <li> This would also yield that the hyperedge list of complete_stdohg(n,r)
   extends that of complete_stdohg(n',r) for n' <= n. </li>
  </ul>


  \todo Hypergraphs of arithmetic progressions
  <ul>
   <li> The current implementation of arithprog_ohg/arithprog_hg can only
   handle small values of n --- otherwise we get a stack-size error!
   \verbatim
C-STACK overflow at size 139456. Stack can probably be resized.
   \endverbatim
   (this on a 32-bit machine with 2GB memory). </li>
   <li> Perhaps this is due to Ecl --- can we grow the stack size?!
    <ol>
     <li> ulimit reports that there are no restrictions from the bash-side.
     </li>
     <li> There are various Ecl memory limits which are discussed in the
     manual -
     http://ecls.sourceforge.net/new-manual/re34.html#table.memory.limits . 
     </li>
     <li> In this case the C-STACK is overflowing with a current limit of 
     128KB. This can be adjusted to say 1MB in the following way:
     \verbatim
:lisp (ext:set-limit 'ext:c-stack 1048576)
     \endverbatim
     </li>
     <li> MG: Please make the documentation of Ecl
     available (locally), document all the memory option (apparently
     there are five: "frame-stack, binding-stack, c-stack, heap-size,
     lisp-stack"; they can also be set on the command line), together
     with appropriate values, and move this important documentation into its 
     right (general) place. Likely the OKlibrary should provide meta-commands,
     which work for all Lisps supported; perhaps just asking for
     one argument, the amount of memory available for Maxima,
     and then calculating the approriate values.
     One also needs to find out how to pass command-line arguments to
     ecl (for example "--frame-stack 4096"). </li>
     <li> Compare "Reasonable memory and stack limits" in
     Buildsystem/MasterScript/SpecialProcessing/plans/Maxima_Init.hpp. </li>
    </ol>
   </li>
   <li> On the other hand, a non-recursive solution is also very easy to
   produce. </li>
   <li> However, such little problems shouldn't pose a problem! </li>
  </ul>


  \todo Sudoku
  <ul>
   <li> Create Sudoku-hypergraphs (such that the Sudoku-problems can be
   expressed as strong colouring problems). </li>
   <li> For box dimension p the vertex set should be {1,...,p}^2. </li>
   <li> The 3N hyperedges (N=p^2) just correspond to the rows, columns
   and boxes (the hypergraph is thus N-uniform). </li>
  </ul>


  \todo %Ramsey hypergraphs
  <ul>
   <li> We should always have that standardisation should not change the
   order of vertices or hyperedges, however this seems to be the case here?
   </li>
   <li> Perhaps we should use complete_hg and variations. </li>
   <li> Compare with ComputerAlgebra/Hypergraphs/Lisp/Generators/GreenTao.mac
   (so that we arrive at a certain standardisation of generator-facilities).
   </li>
   <li> One needs to revise the extreme cases; compare "Ramsey problems"
   in ComputerAlgebra/Satisfiability/Lisp/Generators/RamseyTheory/plans/RamseyProblems.hpp.
   </li>
   <li> Providing a standardised vertex set:
    <ol>
     <li> DONE
     To handle large hypergraphs, we need to provide a version with
     standardised vertex names, using a standard enumeration of r-subsets
     (compare "Create complete r-graphs" above). </li>
     <li> DONE
     We only need to consider lexicographical ordering of the
     vertex names. Actually, in order to preserve monotonicity, we better
     use colexicographical ordering. </li>
     <li> This standardised hypergraph has then a canonical order-preserving
     isomorphism to ramsey_ohg(q,r,n).
      <ol>
       <li> For this to hold (for the vertices) we needed ramsey_ohg
       to use colexicographical ordering. </li>
       <li> Perhaps this is a sensible thing to do; however it seems
       we don't need to do something on the hyperedges (i.e., we continue
       to use lexicographical order there). </li>
       <li> Currently we use the lexicographical order of ramsey_ohg on
       vertex set and hyperedge set, but using colexicographical ranking. </li>
       <li> This has the advantage that ramsey_ohg is easy to compute and
       isomorphic to ramsey_stdohg. </li>
       <li> But for n' >= n we don't have that ramsey_stdohg(q,r,n') is
       an extension of ramsey_stdohg(q,r,n) --- we only have that
       ramsey_stdhg(q,r,n) is a subhypergraph of ramsey_stdhg(q,r,n'). </li>
      </ol>
     </li>
     <li> The same numbering should also be used in the C++ generator
     (see Ramsey.cpp). </li>
     <li> So that we can easily create additional clauses with Maxima,
     added then to the C++-generated files. </li>
     <li> DONE
     The new generators should be named "ramsey_stdhg" and
     "std_ramsey_stdohg". </li>
    </ol>
   </li>
   <li> DONE (we provide two versions, with vertices like {1,3} and with
   vertices like rv(1,3))
   The hypergraphs ramsey_hg use sets directly as vertex names
   (like "{1,3}"), not naming schemes like "rv(1,3)".
    <ol>
     <li> On the one hand, this is natural in this situation. </li>
     <li> But it makes the approach less flexible: Using unevaluated
     functions like "rv", renaming should be possible by just stipulating
     an interpretation of "rv"! </li>
     <li> The task is to figure out, how (locally, in a block) we can
     evaluate expressions containing terms like "rv(i,j)" using
     some (locally) specified function f(i,j).
      <ol>
       <li> This works as follows:
       \verbatim
ev([rv(1,3),rv(2,3,5)],rv([L]):=rank_colex_subsets(setify(L)),nouns);
 [2,7]
       \endverbatim
       </li>
       <li> "nouns" is needed to evaluate the "nouns" rv. </li>
       <li> Shown is also how to handle n-ary functions. </li>
       <li> Strange Maxima error:
       \verbatim
kill(rv)$
declare(rv,noun)$
rcs(S) := block([L : listify(S)],
  apply("+",create_list(binomial(L[i]-1,i), i,1,length(L))) + 1)$
ev(rv(1,4),rv([L]):=rcs(setify(L)),nouns);
 4
ev(rv(1,5),rv([L]):=rcs(setify(L)),nouns);
 7.0
rcs({1,5});
 7
       \endverbatim
       </li>
       <li> According to Stavros (Maxima mailing list) this is due to
       an old (buggy?) interpretation of verbification as computing
       floating point values. </li>
       <li> The solution is not to use the global flag "nouns", but
       just to list the function which shall be treated as "verbs":
       \verbatim
ev(rv(1,5),rv([L]):=rcs(setify(L)),rv);
 7
       \endverbatim
       </li>
       <li> This seems to be of reasonable speed (and faster than
       standardise_fcl). </ol>
     </li>
     <li> DONE (we use rv(1,2,3), and if the user uses e.g. rv(2,1,3)
     then this is his responsibility; the standardisation of rv(1,2,3)
     and rv(2,1,3) is the same)
     It remains whether we should use, %e.g., rv(1,2,3) or rv({1,2,3}).
      <ol>
       <li> The form rv(1,2,3) is a bit shorter. </li>
       <li> But the vertices rv(1,2,3) and rv(2,1,3) are different, though
       they represent the same set {1,2,3}. </li>
       <li> One could enforce associativity and commutativity, but perhaps
       this would introduce unnecessary complexities here. </li>
       <li> Or one disallows rv(2,1,3). </li>
      </ol>
     </li>
     <li> Several possibilities for creating ramsey-hypergraphs:
      <ol>
       <li> DONE (we now have ramseyrv_ohg, which directly computes
       trans_rv(ramsey_ohg(q,r,n)))
       As it is now, with sets as vertices, and then apply a
       translation function:
       \verbatim
kill(rv)$
declare(rv,noun)$
rv_var(v) := nounify(rv)(v)$

trans_rv(G) :=
 [map(lambda([s],uaapply(rv_var,listify(s))),G[1]), 
  map(lambda([S], map(lambda([s],uaapply(rv_var,listify(s))),S)), G[2])]$
       \endverbatim
       </li>
       <li> DONE (ramsey_stdohg does this)
       Then using
       \verbatim
ev_rv(t) :=
 ev(t, rv([L]):=rank_colex_subsets(setify(L)), rv)$
       \endverbatim
       one can get rid off the rv-terms by using ev_rv(G). </li>
       <li> We can also directly translate the subsets via
       \verbatim
trans_colex(G) :=
 [map(lambda([s],rank_colex_subsets(s)),G[1]), 
  map(lambda([S], map(lambda([s],rank_colex_subsets(s)),S)), G[2])]$
       \endverbatim
       </li>
       <li> And we can directly generate the hypergraphs using the
       colexicographical ranks of subsets; or we directly create them
       with the rv-terms, and evaluate them at this point. </li>
      </ol>
     </li>
     <li> DONE (it is not possible(?))
     Regarding "late" translation, a question is also whether a
     term like "{1,2,3}", which should stand for "set(1,2,3)", can
     also be locally evaluated by evaluating set(1,2,3) as f(1,2,3). </li>
    </ol>
   </li>
   <li> Accompanying statistics are needed.
    <ol>
     <li> See okltest_ramsey_hg for the formula for the number of
     hyperedges. </li>
     <li> Likely only basic statistics should be computed, while for example
     the size of the automorphism group etc. is handled in module.
     RamseyTheory/Lisp/Ramsey. </li>
    </ol>
   </li>
  </ul>


  \todo Generalised %Ramsey hypergraphs
  <ul>
   <li> We can define %Ramsey hypergraphs for arbitrary hypergraphs G
   (ramsey_hg uses the complete r-graph).
    <ol>
     <li> The vertices are the hyperedges of G. </li>
     <li> Hyperedges are sets of hyperedges of G which are the hyperedge-set
     of a complete q-graph. </li>
     <li> If G is a graph, then we are considering the hypergraph of
     "cliques", restricted to cliques of size q, and where the clique
     is viewed as a collection of edges. </li>
     <li> We need to check whether this is the common point of view. </li>
    </ol>
   </li>
  </ul>

*/

