// Oliver Kullmann, 5.12.2009 (Swansea)
/* Copyright 2009, 2010, 2011, 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/BooleanFunctions/plans/Permutations/general.hpp
  \brief Investigating boolean functions representing permutations of {0,1}^n


  \todo Generalities
  <ul>
   <li> For a natural number n >= 0 one considers the set V_n = {0,1}^n of all
   bit-vectors. </li>
   <li> The functions to be studied here are permutations f of V_n. </li>
   <li> Permutations are represented as relations by boolean functions
   f^*: {0,1}^(2n), that is, f^*(x,y) = 1 iff f(x) = y. </li>
   <li> These boolean function have exactly one DNF-representation (using 2^n
   clauses of length 2*n). </li>
   <li> The CNF-representations are of special interest here. </li>
   <li> In ComputerAlgebra/Satisfiability/Lisp/FiniteFunctions/Permutations.mac
   we have perm2cnffcs(P), which for a permutation P in list-form creates
   the full cnf-fcs. </li>
   <li> In
   ComputerAlgebra/Satisfiability/Lisp/Primality/PrimeImplicatesImplicants.mac
   we have all_minequiv_bvs_fcs to compute then all minimum CNF's (that is,
   by all_minequiv_bvs_fcs(perm2cnffcs(P)). </li>
   <li> A simpler function there is rsubsumption_hg_full_fcs: By
   rsubsumption_hg_full_fcs(FF)[2] we obtain the list of necessary prime
   implicates (that is, by rsubsumption_hg_full_fcs(perm2cnffcs(P))[2]). </li>
   <li> The permutations are (according to our standard representation)
   not permutations of bit-vectors, but of the numbers 1,..,2^n; which
   are considered as bit-vectors using binary representation of numbers. </li>
   <li> So all permutations are created by permutations(setn(2^n)). </li>
   <li> There are 2^(2^(2n)) boolean functions altogether of 2n arguments,
   while there are (2^n)! such permutations f. </li>
   <li> V_n can be considered as an n-dimensional algebra over F_2 (the
   two-element field), consisting of the n-dimensional vectorspace over F_2
   plus the field-structure of V_n as a field of order 2^n. </li>
   <li> Special permutations of interest are as follows (based on the
   algebraic structures).
    <ol>
     <li> Linear automorphisms, of which there are order_gl(n,2). </li>
     <li> Special linear automorphisms are given by multiplication with
     non-zero field elements, of which there are 2^n-1. </li>
     <li> Translations, of which there are 2^n. </li>
     <li> Affine automorphisms, of which there are order_gl(n,2) * 2^n. </li>
     <li> The multiplicative inverse x -> x^-1, extended by 0 -> 0. </li>
     <li> The compositions of inversions with affine automorphisms. </li>
     <li> For such compositions, is there a fundamental difference between
     first applying the inversion or first applying the affine automorphism?
     </li>
    </ol>
    We need general tools (including representations and conversions) to handle
    these objects.
   </li>
   <li> Of course, besides these "algebraic permutations" we need to study
   random permutations. </li>
   <li> Isomorphism of boolean functions:
    <ol>
     <li> Most powerful is to admit permutations of variables and individual
     flips of variables. </li>
     <li> A basic question is how this compares with conjugatedness of
     permutations (equivalent to having the same cycle type)? </li>
    </ol>
   </li>
  </ul>


  \todo What to investigate for these boolean functions
  <ul>
   <li> The number of prime implicates is of importance, the number of
   necessary clauses amongst them, the size of minimum CNF representations,
   and their number. </li>
   <li> At Maxima-level this is computed as available by
   investigate_permutations(n) (in
   Experimentation/Investigations/BooleanFunctions/Permutations.mac). </li>
   <li> Shortest r_k-compressions of the set of prime implicates are of
   high interest (since we expect them to be most useful for their use
   in SAT-translations). See rand_rbase_cs(F,r) in
   ComputerAlgebra/Satisfiability/Lisp/Reductions/RBases.mac. </li>
   <li> Also OBDD-representations are to be studied. </li>
   <li> And shortest circuit-representations. </li>
  </ul>


  \todo Isomorphism types
  <ul>
   <li> See "Considering special boolean functions" in
   Satisfiability/Lisp/FiniteFunctions/plans/IsomorphismTypes.hpp. </li>
  </ul>


  \todo Trivial cases
  <ul>
   <li> The case n=0: investigate_permutations(0) yields "1 1 1 1". </li>
   <li> The case n=1: investigate_permutations(1) yields two times
   "2 2 2 1". </li>
  </ul>


  \todo The case n=2
  <ul>
   <li> Here we have just (2^2)! = 24 permutations altogether, so we can
   conveniently list them all (by permutations({1,2,3,4})). </li>
   <li> We get
   \verbatim
h2 : investigate_permutations(2)$
ev_ip(h2);

  [4,4,4,1] 8
  [10,0,5,2] 16
   \endverbatim
   </li>
   <li> So we have two cases: One with 4 prime implicates, which all are
   necessary, and one with 10 prime implicates, none of which are
   necessary, and having 2 minimum representations, each with 5 clauses. </li>
   <li> The first case is given by the identity, the second case by
   the permutation [1,2,4,3]:
   \verbatim
last(ev_hm(h2,[4,4,4,1]));
  [1,2,3,4]
last(ev_hm(h2,[10,0,5,2]));
[1,2,4,3]

all_minequiv_bvs_fcs(perm2cnffcs([1,2,3,4]));
  [{{-4,2},{-3,1},{-2,4},{-1,3}}]
all_minequiv_bvs_fcs(perm2cnffcs([1,2,4,3]));
  [{{-4,-3,2},{-4,-2,1},{-3,-2,4},{-1,3},{1,2,4}},
   {{-4,-2,3},{-4,-1,2},{-3,1},{-2,-1,4},{2,3,4}}]
   \endverbatim
   </li>
   <li> The identity is treated in general below. For the second case
   (a transposition) actually the two minimum clause-sets are disjoined
   (a partitioning of the set of all prime-clauses). </li>
   <li> The number of linear automorphisms is order_gl(2,2) = 6, while there
   are 2^2=4 translationen, which makes 24 affine automorphisms altogether.
   </li>
   <li> So here every permutation is an affine automorphism. </li>
  </ul>


  \todo The case n=3
  <ul>
   <li> Here we have just (2^3)! = 40320 permutations altogether, so we
   can still consider them all (algorithmically). </li>
   <li> Experiment:
   \verbatim
oklib_monitor:true;
h3 : investigate_permutations(3)$
[1,2,3,4,5,6,7,8] [6,6,6,1]
[1,2,3,4,5,6,8,7] [20,4,10,256]
[1,2,3,4,5,7,6,8] [26,2,10,288]
[1,2,3,4,5,7,8,6] [30,2,11,512]
[1,2,3,4,6,5,8,7] [12,2,7,2]
[1,2,3,4,6,7,8,5] [28,0,10,48]
[1,2,3,4,6,8,5,7] [26,0,9,2]
[1,2,3,4,8,7,6,5] [26,0,8,2]
[1,2,3,5,4,6,7,8] [36,0,14,870]
[1,2,3,5,4,6,8,7] [36,0,12,2]
[1,2,3,5,4,7,6,8] [44,0,12,485]
[1,2,3,5,4,7,8,6] [38,0,12,106]
[1,2,3,5,6,4,7,8] [36,0,12,1]
[1,2,3,5,6,4,8,7] [34,0,10,6]
[1,2,3,5,6,7,4,8] [41,0,12,592]
[1,2,3,5,6,7,8,4] [38,0,11,82]
[1,2,3,5,6,8,4,7] [38,0,11,1]
[1,2,3,5,8,4,6,7] [38,0,11,67]
[1,2,3,5,8,4,7,6] [37,0,12,4]
[1,2,3,5,8,7,6,4] [40,0,10,8]
[1,2,3,6,5,4,8,7] [40,0,11,64]
[1,2,3,6,5,7,4,8] [39,0,12,2088]
[1,2,3,6,5,7,8,4] [41,0,11,60]
[1,2,3,6,7,8,5,4] [28,2,10,192]
[1,2,3,6,8,7,4,5] [40,0,10,64]
[1,2,3,6,8,7,5,4] [44,0,11,100]
[1,2,3,8,6,5,7,4] [35,2,12,7680]
[1,2,3,8,6,7,5,4] [45,0,12,576]
[1,2,4,3,6,5,7,8] [36,0,10,4]
[1,2,4,3,6,7,5,8] [40,0,11,70]
[1,2,4,3,7,8,6,5] [30,0,9,6]
[1,2,4,5,6,3,7,8] [48,0,12,722]
[1,2,4,5,6,7,3,8] [47,0,12,694]
[1,2,4,5,7,8,6,3] [42,0,10,3]
[1,2,4,5,8,7,3,6] [44,0,11,84]
[1,2,4,7,6,8,3,5] [48,0,11,2]
[1,2,4,7,8,5,6,3] [51,0,12,1152]
[1,4,6,7,8,5,3,2] [40,0,10,20]
XXX
   \endverbatim
   Computation aborted (last output for permutation 1897, and then until
   permutation 3277 no new cases; unclear whether there might be more
   cases --- faster computation, at C++ level is needed). </li>
   <li> Some permutations take more than a minute to process, while some take
   only a few seconds? It can't be the small hash-map? It seems it is just
   that some permutations are more difficult than others. </li>
   <li> See "Write analyse_all_permutations" in
   Investigations/BooleanFunctions/plans/general.hpp for a tool at C++ level.
   </li>
   <li> Some special cases of permutations p:
    <ol>
     <li> Some transpositions (2-cycles):
     \verbatim
P : [2,1,3,4,5,6,7,8];
evalpermasbf(P);
  [20,4,10,256]
P : [1,3,2,4,5,6,7,8];
evalpermasbf(P);
  [26,2,10,288]
P : [8,2,3,4,5,6,7,1];
evalpermasbf(P);
  [36,0,14,870]
     \endverbatim
     </li>
     <li> There are 8*7/2=28 transpositions (in general 2^n*(2^n-1)/2):
     \verbatim
t3 : investigate_transpositions(3);
ev_ip(t3);
 [20,4,10,256] 12
 [26,2,10,288] 12
 [36,0,14,870] 4
     \endverbatim
     </li>
     <li> The isomorphism-type of the boolean function should be just
     determined by the number of bits flipped in the transposition? </li>
     <li> Then in general there would be exactly n isomorphism types,
     corresponding to 1 <= f <= n flipped bits. </li>
     <li> And for f flipped bits there would be 2^n*binom(n,i)/2 permutations
     belonging to it. </li>
     <li> So in this case, for n=3: i=1 -> 8*3/2=12, i=2 -> 8*3/2=12, i=3 ->
     8*1/2=4. </li>
     <li> It remains the task to find for these 3 cases (in general n cases)
     all prime implicates, and to determine their structure (irredundant
     clauses, minimum representations). </li>
     <li> Special transitive permutations:
     \verbatim
P : cyclepres2perl({[1,2,3,4,5,6,7,8]});
  [2,3,4,5,6,7,8,1]
evalpermasbf(P);
  [28,0,10,48]

P : cyclepres2perl({[1,4,7,2,5,8,3,6]});
  [4,5,6,7,8,1,2,3]
evalpermasbf(P);
  [28,0,10,48]

P : cyclepres2perl({[1,2,6,5,4,3,8,7]});
  [2,6,8,3,4,5,1,7]
evalpermasbf(P);
  [41,0,12,592]
     \verbatim
     </li>
     <li> There are 7! = 5040 transitive permutations; perhaps this class is
     too large? Or can we say something, e.g., what are the most complicated
     boolean functions amongst them? </li>
     <li> The maximal order of a permutation here is 3*5=15:
     \verbatim
P : cyclepres2perl({[1,2,3],[4,5,6,7,8]});
  [2,3,1,5,6,7,8,4]
evalpermasbf(P);
  [38,0,11,67]

P : cyclepres2perl({[1,8,3],[2,6,4,7,5]});
  [8,6,1,7,2,4,5,3]
evalpermasbf(P);
  [35,2,12,7680]
     \endverbatim
     </li>
    </ol>
   </li>
   <li> The number of linear automorphisms is order_gl(3,2) = 168, while
   there are 2^3=8 translations, which makes 1344 affine automorphisms
   altogether. </li>
  </ul>


  \todo The case n=8
  <ul>
   <li> This case is especially interesting because of AES; see
   Cryptography/AdvancedEncryptionStandard/plans/FieldMulInvestigations.hpp
   and
   Cryptography/AdvancedEncryptionStandard/plans/SboxInvestigations.hpp.
   </li>
   <li> Here we have (2^8)! ~ 8.578*10^506 permutations altogether (while
   there are ~ 2.003*10^19728 boolean functions (in 16 variables). </li>
   <li> The number of linear automorphisms is order_gl(8,2) ~ 5.348*10^18.
   </li>
   <li> Special linear (affine) automorphisms to consider are the ones
   involved in the S-box and its inverse. </li>
  </ul>


  \todo Prime implicates of simple permutations
  <ul>
   <li> The identity function: 
   <ul>
    <li> For the identity function id over {0,1}^n the prime implicates of
    id^* are exactly the clauses {v_{1,i},-v_{2,i}} and
    {-v_{1,i},v_{2,i}} for all 1 <= i <= n. </li>
    <li> These clauses encode the equality of the variables in the input set
    with those of the output set using binary constraints. </li>
    <li> We have here the case of a parallel and independent composition of
    n boolean functions, namely v_{1,i} = v_{2,i}. </li>
    <li> In general, for such independent parallel compositions the prime
    clauses are just taken alltogether. </li>
   </ul>
   </li>
   <li> The negation:
    <ol>
     <li> x -> neg x componentwise. </li>
     <li> Here we have parallel independent composition of
     v_{i,1} = not v_{i,2}, and so the prime implicates are exactly the
     clauses {v_{1,i},v_{2,i}} and {-v_{1,i},-v_{2,i}} for all 1 <= i <= n.
     </li>
    </ol>
   </li>
  </ul>


  \todo Sampling random permutations
  <ul>
   <li> The data should be at least the total number of clauses, the number
   of clauses of each size, and the number of irredundant clauses (easy to
   compute in this case, since the truth table is given). </li>
   <li> See "Preparations for computing optimum representations" in
   OKlib/Satisfiability/FiniteFunctions/plans/QuineMcCluskeySubsumptionHypergraph.hpp 
   for discussion of adding output of necessary clauses to the subsumption
   hypergraph generator. </li>
   <li> DONE (see analyse_random_permutations)
   A script is to be written, which generates random permutations
   (simplest via Maxima), computes then (by our C++ program) the set of all
   prime implicates, computes basic measurements and puts it into a file,
   for evaluation by R. </li>
  </ul>


  \todo First considerations of random permutation
  <ul>
   <li> n=8:
   \verbatim
set_random(1)$
P : random_permutation(create_list(i,i,1,256))$
output_perm_fullcnf_stdname(P)$

> QuineMcCluskeySubsumptionHypergraph-n16-O3-DNDEBUG Permutation_full.cnf > SP.cnf

> cat Permutation_full.cnf_primes | ExtendedDimacsFullStatistics-O3-DNDEBUG
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
16 140925 1035782 0 1035782 1 1
 length count
5 2
6 3556
7 85110
8 51647
9 610

> cat SP.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n > SP_stat

> head -2 SP_stat
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
140925 65280 60521472 0 60521472 1 1

> E=read.table("SP_stat",skip=2,header=TRUE)
> summary(E)
     length           count       
 Min.   : 177.0   Min.   :  1.00  
 1st Qu.: 575.8   1st Qu.:  9.00  
 Median : 876.5   Median : 34.00  
 Mean   : 876.9   Mean   : 54.22  
 3rd Qu.:1177.2   3rd Qu.:100.00  
 Max.   :1593.0   Max.   :175.00  
> plot(E)
   \endverbatim
   </li>
   <li> Looks similar to the AES Sbox (see "Basic data" in
   Investigations/Cryptography/AdvancedEncryptionStandard/plans/Representations/Sbox_8.hpp).
   </li>
   <li> 1557 experiments for n=8:
   \verbatim
shell> ${OKlib}/Experimentation/Investigations/BooleanFunctions/analyse_random_permutations 8 1
^C
   \endverbatim
   and then:
   \verbatim
R> E = read_experiment_dirs("random_perm", list("e","seed"), "Permutation_full.cnf_primes_stats", header=TRUE, skip=2)
R> ET = rows2columns_df(E, "length", "count", list("e","seed"))
R> summary(ET)
       5         
 Min.   : 0.000  
 1st Qu.: 3.000  
 Median : 5.000  
 Mean   : 5.314  
 3rd Qu.: 7.000  
 Max.   :20.000  
       6              7               8               9        
 Min.   :2824   Min.   :76750   Min.   :39658   Min.   :502.0  
 1st Qu.:3999   1st Qu.:81310   1st Qu.:45265   1st Qu.:657.0  
 Median :4272   Median :82419   Median :47247   Median :709.0  
 Mean   :4289   Mean   :82355   Mean   :47256   Mean   :713.6  
 3rd Qu.:4563   3rd Qu.:83483   3rd Qu.:49195   3rd Qu.:765.0  
 Max.   :5737   Max.   :86368   Max.   :58320   Max.   :997.0  
       10
 Min.   :0.0000
 1st Qu.:0.0000
 Median :0.0000 
 Mean   :0.1933 
 3rd Qu.:0.0000 
 Max.   :4.0000 
       16    e       
 Min.   :0   16:1557  
 1st Qu.:0        
 Median :0      
 Mean   :0       
 3rd Qu.:0        
 Max.   :0
   \endverbatim
   Note the experiment was stopped and restarted with a different seed at one 
   point.
   </li>
   <li> Note here that not all permutations have prime implicates of length 5
   or length 10, but all have length 6,7,8, and 9. </li>
   <li> We can have permutations without any length 5 or length 10 
   prime implicates:
   \verbatim
R> E2[E2[6] == 0,]
      0 1 2 3 4 5    6     7     8   9 10 11 12 13 14 15 16       seed e
392   0 0 0 0 0 0 3522 85462 51359 746  0  0  0  0  0  0  0         12 16
2415  0 0 0 0 0 0 3471 84533 52137 746  0  0  0  0  0  0  0 2147483733 16
4098  0 0 0 0 0 0 3662 83895 52057 603  1  0  0  0  0  0  0 2147483832 16
6036  0 0 0 0 0 0 4143 82732 49082 739  0  0  0  0  0  0  0 2147483946 16
6376  0 0 0 0 0 0 3503 84905 52757 753  0  0  0  0  0  0  0 2147483966 16
6580  0 0 0 0 0 0 3472 85034 51732 649  0  0  0  0  0  0  0 2147483978 16
7141  0 0 0 0 0 0 3839 82370 51682 722  0  0  0  0  0  0  0 2147484011 16
8892  0 0 0 0 0 0 3878 82136 53194 837  0  0  0  0  0  0  0 2147484114 16
12921 0 0 0 0 0 0 3310 83318 57665 749  0  0  0  0  0  0  0 2147484351 16
15624 0 0 0 0 0 0 3842 83251 50269 713  0  0  0  0  0  0  0 2147484510 16
17188 0 0 0 0 0 0 3616 85329 49868 595  0  0  0  0  0  0  0 2147484602 16
17460 0 0 0 0 0 0 3427 83065 55923 770  0  0  0  0  0  0  0 2147484618 16
20962 0 0 0 0 0 0 3611 85249 50021 654  0  0  0  0  0  0  0 2147484824 16
21285 0 0 0 0 0 0 3727 84473 51727 761  0  0  0  0  0  0  0 2147484843 16
23835 0 0 0 0 0 0 4444 83594 45167 634  0  0  0  0  0  0  0 2147484993 16
24804 0 0 0 0 0 0 3702 84340 50569 613  0  0  0  0  0  0  0 2147485050 16
24821 0 0 0 0 0 0 3689 84816 49657 674  1  0  0  0  0  0  0 2147485051 16
25263 0 0 0 0 0 0 4063 83608 47128 604  0  0  0  0  0  0  0         35 16
25297 0 0 0 0 0 0 4327 82446 48222 725  1  0  0  0  0  0  0         37 16
25739 0 0 0 0 0 0 3862 83246 51154 581  0  0  0  0  0  0  0         60 16
   \endverbatim
   In fact most of the permutations we tested didn't have a clause of length
   10 when it had a clause of length 5 (as it would be expected if the events
   would be independent). </li>
   <li> For the total number of prime implicates we have:
   \verbatim
R>  min(as.data.frame(addmargins(as.matrix(E2)))$Sum)
[1] 124400
R>  max(as.data.frame(addmargins(as.matrix(E2)))$Sum)
[1] 148212
   \endverbatim
   </li>
  </ul>
  
*/

