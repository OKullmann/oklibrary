// Oliver Kullmann, 29.6.2007 (Swansea)
/* Copyright 2007, 2008, 2009, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/plans/general.hpp
  \brief Plans for SAT at Maxima/Lisp level in general


  \todo Relations to other modules
  <ul>
   <li> What is the relation to
   LogicalReflection/FormalisedProofs/plans/FundamentalADTs.hpp ? </li>
  </ul>


  \todo Update plans:
  <ul>
   <li> Satisfiability/Lisp/plans/Clauses.hpp </li>
   <li> Satisfiability/Lisp/plans/Literals.hpp </li>
   <li> Satisfiability/Lisp/plans/MinimalSatisfyingAssignments.hpp </li>
   <li> Satisfiability/Lisp/plans/SatisfactionProblems.hpp </li>
  </ul>


  \todo Redesign
  <ul>
   <li> See "Clause-lists instead of clause-sets" below. </li>
   <li> See "Better general naming conventions" below. </li>
  </ul>


  \todo MaxSAT, weighted MaxSAT, and partial MaxSAT
  <ul>
   <li> We have the following definitions:
    <ul>
     <li> A MaxSAT problem, given a clause-set, is the problem of
     finding a total assignment that maximises the number of satisfied
     clauses. </li>
     <li> A weighted MaxSAT problem, given a clause-set and a mapping from
     the clauses to positive-integer weights, is the problem of finding a
     total assignment which maximises the sum of the weights of the
     satisfied clauses. </li>
     <li> A partial MaxSAT problem, given a clause-set and a partitioning of
     the clause-set into "hard" and "soft" clauses, is the problem of finding
     a total assignment which satisfies all of the "hard" clauses, and
     maximises the number of satisfied "soft" clauses. </li>
     <li> A weighted partial MaxSAT problem, given a clause-set, a
     partitioning of the clause-set into "hard" and "soft" clauses, and map
     from "soft" clauses to positive-integer weights, is the problem of
     finding a total assignment which satisfies all "hard" clauses, and
     maximises the sum of the weights of the satisfied "soft" clauses. </li>
     <li> Such definitions are also provided in [The First and Second Max-SAT
     Evaluations; Josep Argerlich, Chu-Min Li, Felip Manyá and Jordi Planes].
     </li>
     <li> MaxSAT competitions are available at http://maxsat.ia.udl.cat/ . </li>
    </ul>
   </li>
   <li> Note that in all cases where we "maximise the X of the
   satisfied clauses", we could also consider that we
   "minimise the X of the falsified clauses". </li>
   <li> The following tools currently utilise translations
   to (Weighted, Partial or standard) MaxSAT:
    <ul>
     <li> MinOnes2WeightedMaxSAT in
     Satisfiability/Interfaces/InputOutput/MinOnes2WeightedMaxSAT.cpp. </li>
     <li> The AES MaxSAT benchmarks in
     Experimentation/Benchmarks/docus/SAT2011.hpp. </li>
    </ul>
   </li>
   <li> For MaxSAT solvers, see
    <ul>
     <li> maxsatz under "Satz", </li>
     <li> "MiniMaxSAT", and </li>
     <li> "Maxsat" </li>
    </ul>
    in Buildsystem/ExternalSources/SpecialBuilds/plans/SAT.hpp.
   </li>
   <li> We need data-structures at the lisp level to handle
   these concepts:
    <ul>
     <li> For MaxSAT, the standard clause-lists and so on
     are sufficient, as the input is the same as for
     standard satisfiability. </li>
     <li> For weighted MaxSAT, we could represent this as
     a pair [F,w] where F is the (formal) clause-set and w is
     the map from clauses to weights. </li>
     <li> For partial MaxSAT:
      <ul>
       <li> We could represent the input as a pair [F_H, F_S] of the
       "hard" clause-set and "soft" clause-set. </li>
       <li> For a "formal" partial MaxSAT instance, it seems best
       to have a triple [V,F_H,F_S], rather than a pair of formal
       clause-sets; we (likely) never consider the total assignments over
       just the variables of the "soft" clauses. </li>
      </ul>
     </li>
     <li> For weighted partial MaxSAT, it then seems natural to have a
     triple [F_H,F_S,w_s] where F_H and F_S are the "hard" and "soft"
     clauses, and w_s is the map from the soft clauses to their weights.
     </li>
    </ul>
   </li>
   <li> We should also a basic (weighted) (partial) MaxSAT solver
   at the Maxima level. </li>
  </ul>


  \todo Clause-lists instead of clause-sets
  <ul>
   <li> See "General design: Lists *here* are more fundamental than sets"
   in ComputerAlgebra/plans/Maxima.hpp for the general discussion. </li>
  </ul>


  \todo Better general naming conventions
  <ul>
   <li> See "Notions for clauses, clause-sets etc." in
   ComputerAlgebra/Satisfiability/Lisp/ClauseSets/plans/general.hpp
   for special issues regarding clause-sets etc. </li>
   <li> See "Naming" in
   ComputerAlgebra/Satisfiability/Lisp/ClauseSets/plans/Statistics.hpp
   for naming of statistics. </li>
   <li> Systematise the use of suffices like "cs" and "cs_f".
    <ol>
     <li> For variables: "v". </li>
     <li> For literals: "l". </li>
     <li> For clauses: "c". </li>
     <li> For clause-sets: "cs". </li>
     <li> For clause-lists: "cl". </li>
     <li> For ordered clause-sets: "ocs". </li>
     <li> The "formal" versions:
      <ul>
       <li> "fcs", "ofcs" </li>
       <li> "lcs", "olcs". </li>
      </ul>
     </li>
    </ol>
   </li>
   <li> For "standardised clause-sets we use prefix "std", e.g., "stdcs",
   or "stdnbfcs". </li>
   <li> What about literals as sets of pairs [v,e], where e is
   a value (forbidden for CNF, allowed for DNF)?
    <ol>
     <li> We could use the prefix "ms" for monosigned? </li>
     <li> So "msl", "msc", "mscs", "mscl", "msfcs", "msfocs",
     "msfmcs", "msfomcs", "msflcs", "msfolcs". </li>
     <li> However then we could not express that in a CNF (or DNF)
     we wanted to have both allowed and forbidden values? </li>
     <li> Then we needed triples [v,e,s], where [v,e] as before, while
     s in {-1,+1} ? (This would allow negation.) </li>
     <li> We could also have "signed literals" (see below) additionally
     with negation? </li>
     <li> We could call pairs [n,e] "non-boolean literals" ("nbl"), while
     triples [n,e,s] are "monosigned literals" ("msl") ? </li>
     <li> Clause-sets would then be "nbcs" resp. "mscs" etc. </li>
    </ol>
   </li>
   <li> And then we have signed literals as sets of pairs [v,E],
   where E is a set of values (or a list?)?
    <ol>
     <li> We could use the prefix "s" for signed? </li>
     <li> So "sl", "sc", "scs", "scl", "sfcs", "sofcs",
     "smucs", "somucs", "slcs", "solcs". </li>
     <li> It seems sensible to allow also triple [v,E,s], where s in {-1,+1}
     (for efficient negation). </li>
     <li> Such triples would sensibly be called "signed literals" ("sl");
     now how to call pairs [v,E]? "Power literals" ("pl")! </li>
    </ol>
   </li>
   <li> For the various forms of generalised clause-sets D we need also forms
   which are pairs [F,D], where D yields the domain of variables:
    <ol>
     <li> Using the suffix "ud" for "uniform domain" (D is just a set resp.
     a list for the ordered cases). </li>
     <li> While the suffix "fd" for "function domain" mean a function which
     assigns to every variable a set resp. a list. </li>
    </ol>
   </li>
   <li> Then we have the different kind of partial assignments:
    <ol>
     <li> Likely, partial assignments should be syntactically identical
     with clauses. </li>
     <li> Notions: "pa", "nbpa", "mspa", "ppa", "spa". </li>
     <li> And then, like for literals, clauses, clause-sets etc.
     one has the CNF- and the DNF-*interpretation*. </li>
     <li> For clauses we wanted to stay with sets; perhaps for partial
     assignments we could have them ordered?
      <ul>
       <li> So speaking of "opa", "omspa" and "ospa". </li>
       <li> Perhaps one should actually also allow to have ordered clauses?
       Better not! Then we have too many combinations. </li>
      </ul>
     </li>
     <li> But we also need the representation of partial assignments via
     total assignments! How to call them --- "total partial assignments"? </li>
     <li> Or perhaps, since we are already using suffixes like "_l" for
     stating representational differernces, "pa_mp" (for map). Seems
     reasonable. </li>
     <li> Then we could also have "pa_l" (as lists). </li>
     <li> And then there are partial assignments which are actually "total
     assignments" (w.r.t. a set of variables)?! Perhaps using "tpa" ? </li>
    </ol>
   </li>
   <li> It seems that in general we do not mention the output-type in the
   name:
    <ol>
     <li> However, if we do, for example "clause-sets maps to hash-map",
     then we should use "...hm_cs(args)". </li>
     <li> The special syntax "cs2hm(F)" should be reserved for conversions.
     </li>
    </ol>
   </li>
   <li> There is the issue of using underscores ("min_var_deg_cs") or
   not ("varregcsp").
    <ol>
     <li> Perhaps leaving it out only for those predicates. </li>
    </ol>
   </li>
   <li> See discussion of in-place modifications in "Singular DP-reduction" in
   Satisfiability/Lisp/Reductions/plans/DP-Reductions.hpp. </li>
  </ul>


  \todo Develop combinations of look-ahead with conflict-driven solvers
  <ul>
   <li> See [OK, Present and future of practical SAT solving;
   CSR 8-2008] for first schemes. </li>
   <li> This should go to Backtracking/CombinedSchemes.mac. </li>
   <li> Perhaps first we do not consider autarkies (and we add them
   later). </li>
  </ul>
   

  \todo Proof systems
  <ul>
   <li> We need a dedicated module for proof systems. </li>
   <li> See Satisfiability/ProofSystems/plans/general.hpp. </li>
  </ul>


  \todo Finding out "all properties" of an instance
  <ul>
   <li> We need some scheme which allows to let the system run all existing
   tests. </li>
   <li> One application would be to located poly-time classes which
   include the instance. </li>
   <li> Perhaps all relevant functions have an "exploration" version,
   these versions are put into a global list, and then one just runs
   through this list. </li>
   <li> Then there will be however duplications in the output; but perhaps
   this is the lesser evil.
    <ol>
     <li> Though for example once we know that the instance is satisfiable,
     then there is no point in running tests for minimal unsatisfiability.
     </li>
     <li> Perhaps for each module we write a basic exploration function,
     and this function then organises the exploration at module level,
     while if needed asks for input established by other modules. </li>
     <li> Such a hierarchical scheme looks alright; fits into our general,
     "tree-based" organisation. </li>
    </ol>
   </li>
   <li> Different from normal functions, this also needed text-output,
   to explain the findings:
    <ol>
     <li> Perhaps this output all goes to a file. </li>
    </ol>
   </li>
  </ul>


  \todo Survey and belief propagation
  <ul>
   <li> We should create a new module on belief propagation and survey
   propagation. </li>
  </ul>


  \todo Converting CNFs to DNFs
  <ul>
   <li> This should be the special domain of look-ahead solvers. </li>
   <li> See
   ComputerAlgebra/Satisfiability/Lisp/Backtracking/plans/SplittingTrees.hpp.
   </li>
   <li> See Satisfiability/Algorithms/AllSolutions/plans/general.hpp for
   plans at the C++-level. </li>
   <li> An important application is discussed in
   ComputerAlgebra/Satisfiability/Lisp/Preprocessing/plans/general.hpp. </li>
   <li> In a certain sense the task is a special case of the problems discussed
   in ComputerAlgebra/Satisfiability/Lisp/FiniteFunctions/plans/general.hpp,
   however it seems to have a different focus. </li>
   <li> The task of computing the transversal hypergraph is related, but
   different, namely this is computing (for a CNF) the set of *all*
   prime implicantes (minimal satisfying assignments), while here we typically
   want to minimise the number of clauses. Of course, knowing all prime
   implicants can help, but this restricts the problem to only small cases
   (i.e., "finite functions"). </li>
   <li> MG put at some time some heuristics into
   ComputerAlgebra/Satisfiability/Lisp/ClauseSets/Statistics.mac, but this
   is clearly not an appropriate place. </li>
   <li> The whole tau-methodology should be applicable here (especially
   since we are computing the whole tree; however now we are looking
   at satisfiable cases). </li>
   <li> ComputerAlgebra/Satisfiability/Lisp/Counting/plans/general.hpp is
   related, but likely we should have an independent module. </li>
   <li> There are plans in Solvers/OKsolver/SAT2002/plans/general.hpp
   (see "Enable finding all solutions" there), but due to the use of
   autarkies it is not clear whether the OKsolver is the right tool here.
   </li>
   <li> What is the appropriate name for the new module? "Cnf2Dnf"? Or perhaps
   better "Dualisation"? </li>
  </ul>

*/

