// Oliver Kullmann, 1.8.2012 (Swansea)
/* Copyright 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/SchurProblems/plans/Symmetry.hpp
  \brief Symmetry for Schur problems (and generalisations)


  \todo Automorphisms
  <ul>
   <li> The non-boolean clause-set schur_nbfclud(r,n) as well as
   wschur_nbfclud(r,n) has at least the following automorphisms:
    <ol>
     <li> r! many permutations of the r values. </li>
     <li> The automorphisms of the hypergraph schurtriples_hg(n):
      <ol>
       <li> Multiplication by such x in {1,...,n} modulo n+1 which are
       relatively prime to n+1 would be automorphisms if the triples would
       "wrap around", i.e., would be given by the condition x+y=z mod n+1.
       </li>
       <li> However for ordinary Schur-hypergraphs these seem to be never
       automorphisms (except in the trivial case x=1). For the test-function
       \verbatim
test_auto_schur(n) := block([G : schurtriples_hg(n), p_ : mod_mul(n+1)],
 subset(inv_residues(n+1), lambda([x], automorphism_bydef_hg(lambda([v], p_(x,v)),G))))$
for n : 1 thru 10 do print(n, test_auto_schur(n));
       \endverbatim
       we get {1} except for n = 2 --- and this should be trivial to prove.
       </li>
       <li> It seems the Schur-hypergraph don't have non-trivial automorphisms
       at all?!
       </li>
      </ol>
     </li>
    </ol>
   </li>
   <li> Automorphisms of schurtriples_pd_hg(n):
    <ol>
     <li> Here now multiplication by invertible elements of ZZ_{n+1} yield
     automorphisms:
     \verbatim
test_auto_pdschur(n) := block(
 [G : schurtriples_pd_hg(n), p_ : mod_mul(n+1), m_ : mirrorfold_schur(n)],
 every(lambda([x], automorphism_bydef_hg(lambda([v], m_(p_(x,v))),G)), inv_residues(n+1)))$
for n : 1 thru 50 do print(n, test_auto_pdschur(n));
  all true
     \endverbatim
     </li>
     <li> In [Fredricksen, Sweet, 2000] we find the assertion that such
     bijections preserve solutions; we now know stronger that they are
     automorphisms of the underlying hypergraph. </li>
     <li> What is the kernel of the homomorphism ZZ_{n+1}^* -> automorphisms ?
     \verbatim
auto_pdschur(n,x) := buildq([n,x], lambda([v], mirrorfold_schur(n)(mod(x*v,n+1))));
kernel_pdschur(n) := block([V : ver_schurtriples_pd_ohg(n)],
 subset(inv_residues(n+1), lambda([x], block([phi : auto_pdschur(n,x)], every_s(lambda([v], is(phi(v)=v)), V)))))$
     \endverbatim
     shows that exactly for n+1 divisible by 3 the operation is faithful, while
     otherwise the kernel consists exactly of 1 and -1=n. </li>
     <li> How to exploit these symmetries?!
      <ol>
       <li> Via the colour-symmetries one can assign to the vertices
       v_1 := schur(0), ..., v_r := schur(r-1) the colours 1, ..., r. Now
       we can apply a hypergraph-automorphism phi to (v_1,...,v_r), obtaining
       (phi(v_1), ..., phi(v_r)), which might be more powerful for symmetry-
       breaking. </li>
       <li> Considering n=161, with totient(162)=54 automorphisms of the
       hypergraph:
       \verbatim
test_auto_pdschur(162);
  true
       \endverbatim
       </li>
       <li> Now the mapping:
       \verbatim
r : 5;
n : 161;
V : setify(create_list(schur(i),i,0,r-1));
R : inv_residues(n+1);
for x in R do block([m : auto_pdschur(n,x)], print(x, map(m,V)));
# none looks better than V itself
       \endverbatim
       </li>
      </ol>
     </li>
    </ol>
   </li>
  </ul>


  \todo Full symmetry-breaking
  <ul>
   <li> This should better be called "rigid colour symmetry-breaking" (or even
   "stiff"). </li>
   <li>
   \verbatim
> CRunPdSchurFsb 1 2 "minisat-2.2.0"
1:1 2:0
> CRunPdSchurFsb 2 5 "minisat-2.2.0"
1:1 ... 4:1 5:0
> CRunPdSchurFsb 3 14 "minisat-2.2.0"
1:1 ... 13:1 14:0
> CRunPdSchurFsb 4 45 "minisat-2.2.0"
1:1 ... 44:1 45:0

create_list(schurfsb(r),r,0,4);
  [1,2,5,14,45]
create_list(pdschurfsb(r),r,0,4);
  [lambda([x],is(x < 1)),
   lambda([x],is(x < 2)),
   lambda([x],is(x < 5)),
   lambda([x],is(x < 14)),
   lambda([x],is(x < 45))]
   \endverbatim
   </li>
   <li> So even for palindromic problems there is no evidence yet that actually
   full symmetry-breaking is not satisfiability-equivalent (note that at least
   for the above parameters regarding satisfiability there is no difference to
   the ordinary problems). </li>
   <li>
   \verbatim
> CRunPdWSchurFsb 1 3 "minisat-2.2.0"
1:1 2:1 3:0
> CRunPdWSchurFsb 2 6 "minisat-2.2.0"
1:1 ... 5:1 6:0
> CRunPdWSchurFsb 3 15 "minisat-2.2.0"
1:1 ... 14:1 15:0
> CRunPdWSchurFsb 4 48 "minisat-2.2.0"
1:1 ... 44:1 45:0 46:0 47:1 48:0

create_list(wschurfsb(r),r,0,4);
  [1,3,9,24,67]
create_list(pdwschurfsb(r),r,0,4);
  [lambda([x],is(x < 1)),
   lambda([x],is(x < 3)),
   lambda([x],is(x < 6)),
   lambda([x],is(x < 15)),
   lambda([n],if n >= 48 or n = 45 or n = 46 then false else true)]
   \endverbatim
   Again no deviation from the cases without full symmetry-breaking. </li>
   <li> To summarise, for the four standard cases (pd)(w)schur(r), we do not
   have yet any evidence that full symmetry-breaking is not satisfiability-
   equivalent. </li>
   <li> See "Palindromic problem with full symmetry-breaking (direct
   encoding)" and "Weak palindromic problem with full symmetry-breaking (direct
   encoding)" in Schur5/SplittingViaOKsolver.hpp for investigations into the
   ordinary and weak palindromic problems with full symmetry-breaking. </li>
  </ul>


  \todo Embedding colour-symmetry-breaking into the search
  <ul>
   <li> The idea is simple: as soon as a new colour is used, the other
   yet unused colours do not need to be considered. </li>
   <li> The underlying fundamental lemma: Let F represent a Schur-problem in
   one of its four main forms (weak or not, palindromic or not). Consider a
   partial assignment phi. Let C be the set of colours not involved in phi.
   Then any permutation of C (leaving everything else fixed) is an automorphism
   of phi * F. </li>
   <li> This can be relatively easily integrated into a look-ahead solver.
    <ol>
     <li> Consider OKsolver_2002. </li>
     <li> Maintaining for the variables nbv(i,j) assigned on the current path
     which j are involved (the rest constitute the above C), and if a new j is
     considered for a decision variable nbv(i,j), setting for all other yet
     untouched j' the variables nbv(i,j') to true. </li>
     <li> This is an assignment like an autarky, with the difference that after
     that the reduction-process has to fully restart. </li>
    </ol>
   </li>
   <li> A simpler version is just to postprocess the splitting assignments of
   the C&C approach (here SplittingViaOKsolver). This is easy, especially if
   it is known which are the decision-variables. </li>
  </ul>

*/
