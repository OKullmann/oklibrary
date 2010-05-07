// Matthew Gwynne 20.4.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/LogicalDataAnalysis/plans/SnowCrest2000Analysis.hpp
  \brief Plans regarding investigations a case-study for the logical analysis of data

  The underlying article is [The Outcomes of Homeless Mobilization; Cress,
  Snow, The American Journal of Sociology, 2000].
  

  \todo Create milestones.


  \todo Links
  <ul>
   <li> See "Matrices" in
   ComputerAlgebra/LogicalDataAnalysis/Lisp/plans/general.hpp for
   the data. </li>
  </ul>


  \todo Code for analysing such instances
  <ul>
   <li> When considering any kind of analysis 
   (in particular when partially-automatically computed)
   we have the following questions
    <ol>
     <li> What is the input? </li>
     <li> What types of analysis/computation are needed, that is, what is the 
     output? </li>
     <li> What tools do we use? (%e.g., Maxima, R, shell-scripts etc). </li>
    </ol>
   </li>
   <li> In the first case,
    <ul>
     <li> we consider several partial boolean functions
     given by a partial truth table, where we have a list of input variables 
     VI, a list of output variables VO and a list of total assignments
     over append(VI,VO) (which will likely not cover all possible
     total assignments). </li>
     <li> we consider how to represent this data. We have two options
      <ul>
       <li> as a combinatorial matrix </li> 
       <li> as a conflict-variable matrix </li>
      </ul>
      and there are advantages to both, however, due to the better readability
      of 0,1 for false,true, as well as the fact this relates directly to the
      original presentation as a truth table, it is best to use the combinatorial
      matrix for the basic representation of such truth tables, and then provide
      a simple conversion function such as 
      \verbatim
ttcom2cvm(M) := subst(-1,0,M)$
      \endverbatim
      to convert one to the other.
     </li>
    </ul>
   </li>
   <li> In the second case
    <ul>
     <li> we have discussions given in 
     "List statistics and types of analysis needed" and
     "Representations for data analysis". </li>
    </ul>
   </li>
   <li> In the third case
    <ul>
     <li> initially it is reasonable to simply implement each 
     set of functionality in whichever system "seems" appropriate. </li>
     <li> initially most functionality will be within the Maxima system
     or done by hand, as such functionality will be combinatorial 
     (handling clause-sets, assignments etc). </li>
     <li> when (later) considering any non-trivial statistics or
     presentation issues, R should be used, as it is much more suited to
     such things and offers a greater number of packages to aid in this. </li>
     <li> when (much later) generating any automated documents, shell
     scripts can be used to call Maxima, perform any formatting and then
     to call R and any other tools (LaTeX etc) to generate such documents.
     </li>
    </ul>
   </li>
  </ul>


  \todo Move discussions to general
  <ul>
   <li> A lot of these discussions are general and will apply to a variety
   of different data analyses. </li>
   <li> However, initially we have just a single instance for discussion
   and so all such discussion is centralised here, and can be moved to
   the relevant sections once it matures. </li>
  </ul>


  \todo List statistics and types of analysis needed
  <ul>
   <li> Initially we have a truth table specification of a
   partial boolean function, and we wish to understand the
   assumptions (in terms of unspecified total assignments)
   which one would need to make to derive different 
   representations. </li>
   <li> We usually consider size here, looking for shortest
   DNF or CNF representations, but we also look for 
   hitting clause-set representations, or representations 
   as DFAs etc. </li>
   <li> When considering shortest DNF and CNF representations,
   at a basic level we need the following statistics:
    <ul>
     <li> For each minimum-sized clause-set: 
      <ul>
       <li> the size of the clause-set. </li>
       <li> the prime implicates covered by this clause-set. </li>
      </ul>
      and for each of these, summary statistics such as:
      <ul>
       <li> the number of literal occurrences in the clause-set </li>
       <li> the number of clauses of each size in the clause-set. </li>
      </ul>
     </li>
     <li> A list of 
      <ul>
       <li> which total assignments are assumed satisfying (resp falsfying)
       by *all* minimum-sized clause-sets </li>
       <li> which total assignments are assumed satisfying (resp falsifying)
       by *none* of the minimum-sized clause-sets </li>
       <li> which total assignments are assumed satisfying (resp falsifying)
       by *some but not all* of the minimum-size clause-sets, with 
       additionally information on which minimum-sized clause-sets need which.
       </li>
      </ul>
     </li>
    </ul>
   </li>
  </ul>


  \todo Representations for data analysis
  <ul>
   <li> Given the types of statistics and analysis listed in
   "List statistics and types of analysis needed", how can
   we represent these statistics in a compact and yet informative
   way? </li>
   <li> As an example, truth tables such as those given in the Snow/Crest
   paper include a great deal of redundancy (for us), as they list 
   cases as individual entries in the truth table, as they consider
   each case individually. </li>
   <li> We on the other hand consider each configuration (the values in
   the truth table), and so some cases collapse as they have the same
   configuration. </li>   
   <li> Assignments assumptions
    <ul>
     <li> When considering the assumptions made for each minimum-sized
     clause-set (see "List statistics and types of analysis needed" above),
     we have the question of how to represent this data in a succinct
     and informative way. </li>
     <li> Simply listing the partitioning as 3 sets or lists does not
     seem a particularly good representation, as at a glance the reader
     learns very little and must delve immediately into the details
     and do a lot of additional computation to learn anything. </li>
     <li> A Venn diagram or Karnaugh map using 3 different colours would
     illustrate this partitioning in a succinct and reasonable way, and
     would also illustrate the connections and relations between individual
     total assignments (hamming distances etc.). </li>
     <li> However, this is restricted to only very small cases, and the only
     interesting thing to do in general is to represent again the boolean
     functions! </li>
     <li> In these cases however, we have total boolean functions (i.e.,
     we know exactly which total assignments are in each of the 3 sets. </li>
     <li> Therefore, we wish to find an expressive representation 
     (in the sense that the representation is succinct, easy to understand,
     offers a lot of information at a glance without a great deal of inference
     needed) of each of these boolean functions. </li>
     <li> For a start, we generate the minimal CNF and DNFs for these
     representations. </li>
    </ul>
   </li>
   <li> There are representations in machine learning etc, such as
   <ul>
    <li> DFAs
     <ul>
      <li> A Deterministic finite automata is a five tuple (Q,A,t,s,F) where
      <ol>
       <li> Q is the set of states. </li>
       <li> A is the alphabet. </li>
       <li> t : Q x A -> Q is a transition function determining the transitions
       from one state to another. </li>
       <li> s is the start state. </li>
       <li> F is the set of final states. </li>
      </ol>
      </li>
      <li> A DFA defines a (regular) language L a subset of A* where any word
      for which the path through the transition system given by the word,
      starting in the state s, ends in a final state. </li>
      <li> A boolean function can represented by a DFA by considering assignments
      as tuples of 1s and 0s with a given variable order and then any tuple/word
      which has an implicant as a prefix is in the language, and any tuple/word
      that has the negation of an implicate as a prefix is not in the language. We then 
      look for a DFA which recognises this language. </li>
     </ul>
    </li>
    <li> Decision trees
     <ul>
      <li> A decision tree is a rooted tree T with labelling functions
      l_e : e.E(T) -> D(T,e) and l_v : V(T) -> union(V,{0,1}) where 
      <ul>
       <li> A(T,e) is the domain of the variable v which e connects to it's child 
       in the tree (for instance in boolean problems we have D(T,e)={0,1} for all T 
       and e). </li>
       <li> V is a variable list. </li>
       <li> l_v labels internal nodes in the tree with 
       elements of V and labels leaves with elements of D where D
       is the domain of the decision tree itself. </li>
       <li> A decision tree representation of a boolean function f
       is a decision tree T where 
       <ul>
        <li> V=var(f). </li>
	<li> D=D(T,e)={0,1} for all e in E(T). </li>
	<li> l_v is defined such that for any path from the
	root to a leaf, the partial assignment phi given by taking the
	variables mapped to (by the nodes on the path), the value 
	given to the edge leaving that %node (one the path) by l_e is
	a satisfying partial assignment for f (i.e., all extensions
	of phi to full total assignment tuples psi for f have f(psi)=true). 
	</li>
       </ul>
       </li>
      </ul>
     </li>
    </ul>
    for which those in the machine
    learning community find minimum length (and other) representations.
    <ul>
     <li> Is there a simple relation between these different minimal size
     representations? </li>
     <li> What do different properties, both in terms of the ability of
     the reader to understand the object (DFA, clause-set etc) and in
     terms of succinctness etc, translate to when moving from one
     representation to another? </li>
    </ul>
    It is important here to consider all different representations, and
    get a good understanding for what the advantages of each different
    representations are. </li>
   </ul>
   </li>
   <li> See 
   Buildsystem/ExternalSources/SpecialBuilds/plans/BooleanFunctions.hpp
   for some packages for generating different minimum-sized representations.
   </li>
  </ul>


  \todo Terminology 
  <ul>
   <li> We need a comprehensive list of the terminology to be used
   here along with basic definitions, so that we can be consistent
   and ensure that as much as possible, any terminology that does not
   come from boolean algebra or satisfiability, or that refers specifically
   to concepts in QCA, should be called by a reasonable name or it's
   original term in the literature. </li>
  </ul>

*/

