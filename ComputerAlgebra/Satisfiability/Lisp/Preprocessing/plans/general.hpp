// Oliver Kullmann, 3.5.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/Preprocessing/plans/general.hpp
  \brief Plans for preprocessing of SAT problems in general


  \todo A general perspective
  <ul>
   <li> A "reduction" eliminates "trivial cases", while "preprocessing"
   reorganises the problem. </li>
   <li> Given a CNF F, a natural preprocessing scheme is to partition
   (or, more generally, to cover) F by parts F_i, where each F_i is then
   analysed and compiled into some "active clause" C_i, a condition
   which is equivalent to F_i. </li>
   <li> A "good condition" C_i determines for every partial assignment whether
   instantiation yields an always-true or an always-false problem; in the
   former case we can drop the condition (an "autarky" was found), while
   in the latter case the whole (current) problem is unsatisfiable. </li>
   <li> The active clauses C_i may be translated back into CNFs F_i', possibly
   using new variables (this seems to be crucial). Then the detection of
   C_i being a unsatisfiable or a tautology under a partial assignment (only
   to the original variables in F_i) should be achieved by means of certain
   reductions. </li>
   <li> Considering only the unsatisfiability case, this notion of a good
   representation F_i' w.r.t. reduction r has been called a "r-based
   representation" in  "Investigating conditions and their representations" in
   Experimentation/Investigations/Cryptography/AES/plans/general.hpp. </li>
   <li> The tautology case doesn't seem to fit into the standard SAT
   methods (forced assignments and autarkies), but seems to need a
   specific test, which however can be easily achieved by just checking
   whether the partial assignment satisfies the original CNF F_i (using
   that every non-empty CNF is falsifiable). </li>
   <li> The active clause C_i could just be some representation, for example
   by circuits or branching programs, which are then translated into
   CNFs F_i'. See "Preprocess a CNF via computing DNFs for parts" below
   for the perhaps most natural starting point, namely using a DNF for C_i.
   </li>
   <li> Since a non-empty DNF C_i is always satisfiable, and partial assignment
   phi yielding an unsatisfiable problem is easily detected by phi * F_i = {}.
   However, using for example circuits and partial evaluation, this is not
   necessarily the case. </li>
   <li> How is it with OBDDs?
    <ol>
     <li> At the OBDD-level, applying a partial assignment should be possible
     in polynomial time, and then we can just check whether we have a constant
     function. </li>
     <li> But is the preserved by the translation into a circuit? </li>
     <li> Likely, if the circuit can be easily checked for unsatisfiability,
     then the Tseitin-translation should preserve that. </li>
     <li> However, intuitively it seems unlikely to me (OK), that the
     translation from the OBDD to a circuit preserves unsatisfiability
     detection, since this likely needs some form of not completely trivial
     computation on the OBDD-level (namely the instantiation). </li>
    </ol>
   </li>
  </ul>


  \todo Preprocess a CNF via computing DNFs for parts
  <ul>
   <li> Partition (cover) the CNF F into F_i, compute good (small)
   DNF representations G_i for the parts F_i, translate the G_i into CNFs
   F_i', obtaining the preprocessed F' (with parts F_i'). </li>
   <li> Compare "Investigating conditions and their representations" in
   Experimentation/Investigations/Cryptography/AES/plans/general.hpp;
   there the (unique) DNFs are already given. </li>
   <li> A "good" DNF representation means here
    <ol>
     <li> small number of clauses </li>
     <li> using only variables on which the problem depends. </li>
    </ol>
    The number of clauses equals the number of new variables.
   </li>
   <li> Computing the DNF representation by DLL:
    <ol>
     <li> One way of obtaining a DNF representation is by computing a
     splitting tree for F_i, where r_k can (should) be used (perhaps
     k=3), and where the tree should be condensed. </li>
     <li> See
     ComputerAlgebra/Satisfiability/Lisp/Backtracking/plans/SplittingTrees.hpp.
     For really hard problems one could seek for an optimal r_k-splitting tree.
     </li>
     <li> One obtains a hitting DNF from that, and then, as discussed in
     ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/plans/SboxAnalysis.hpp
     and in
     ComputerAlgebra/Satisfiability/Lisp/Resolution/plans/PrimeImplicatesImplicants.hpp,
     a shortened DNF (consisting only of prime implicants) is computed.
     (Note that in these two case studies the roles of CNF/DNF are reversed;
     but it's always the problem of "dualisation" of a clause-set.)
     </li>
    </ol>
   </li>
   <li> A natural candidate for the translation G_i -> F_i' is dualts_fcl:
    <ol>
     <li> For partial assignments phi on the primary variables via
     r_1(phi * F_i') an inconsistent problem F_i is detected. </li>
     <li> The always-true case (i.e., whether phi * F is a tautology) would be
     detected by checking whether phi * F_i = {} holds. This could be called
     "tautology monitoring". The point is that F_i' can not yield any
     restriction on the primary variables, while the auxiliary variables
     of F_i' are local, and thus restrictions on them don't matter. Thus
     once phi * F_i = {} holds, one can safely remove F_i' (i.e., its
     current residuum). </li>
     <li> For permutations like the Sbox, dualts_fcl should be optimal
     (and no tautology monitoring is needed, since instantiation can not
     yield tautologies), but in general "sharing" should be important. </li>
    </ol>
   </li>
  </ul>


  \todo Finding and exploiting functional dependencies
  <ul>
   <li> Given a boolean function B in variables x_1, ..., x_n, variable
   x_i is dependent on S <= {x_1,...,x_n} - {x_i} if for every partial
   assignment phi with var(phi) = S the value of x_i in phi*B is forced
   (has at most one possibility to make phi*B true). </li>
   <li> If we find x_i dependent on {}, then we can just add the corresponding
   unit-clause to the representation of F. </li>
   <li> If x_i depends on {x_j}, and this is minimal, then either x_i = x_j
   or x_i = -x_j, and this substitution should be performed in F, while
   by adding the corresponding equivalence we maintain equivalence. </li>
   <li> These simplifications are likely "always" beneficial. </li>
   <li> What about a dependency on two variables?
    <ol>
     <li> One could always add a representation of the dependency, making
     it explicit. </li>
     <li> This is beneficial if some implications are now "easily" available;
     one could check this. </li>
     <li> But in general it seems not appropriate to perform some kind
     of substitution. </li>
     <li> But if we are producing a DNF-representation of B, then we could
     project that to {x_1,...,x_n} - {x_i}, and add then the representation
     of x_i = f(S). </li>
     <li> This could also be done for BDD-representations. </li>
    </ol>
   </li>
   <li> How to find functional dependencies efficiently?
    <ol>
     <li> Which representation of B is best here? </li>
     <li> We can always systematically run through all the corresponding
     partial assignments, and solve a SAT problem. </li>
     <li> A possibility here is to use the r_k-reductions, which may yield
     several forced assignments at once. </li>
     <li> If B can be represented by a Horn-CNF (likely we should check
     this?!), then at least the determining a specific dependency is
     easy; see [Ibaraki, Kogan, Makino; 1999 + 2001; Artificial Intelligence]
     for further considerations. </li>
    </ol>
   </ul>
  </ul>

*/

