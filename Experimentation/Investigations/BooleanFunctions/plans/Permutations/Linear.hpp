// Oliver Kullmann, 11.2.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/BooleanFunctions/plans/Permutations/Linear.hpp
  \brief Investigating boolean functions representing linear permutations of {0,1}^n


  \todo Connections
  <ul>
   <li> See
   Cryptography/AdvancedEncryptionStandard/plans/Representations
   for several interesting examples in the context of AES. </li>
   <li> See
   ComputerAlgebra/Satisfiability/Lisp/LinearConditions/plans/general.hpp for
   the handling of linear constraints in general. </li>
  </ul>


  \todo Affine bijections over ZZ_2
  <ul>
   <li> We need to find out all about the representations of boolean functions
   x -> x -> a + M*x, i.e., affine maps, which are bijective (equivalently,
   M is invertible). </li>
   <li> Simplest are translations x -> a+x by vector a:
    <ol>
     <li> Here we have independent parallel composition, and so we just have
     the appropriate combinations of the representations of identity
     and negation (bitwise) as in "Prime implicates of simple permutations" in
     Experimentation/Investigations/BooleanFunctions/plans/general.hpp. </li>
    </ol>
   </li>
   <li> So the set of prime implicants of affine bijections over ZZ_2 is
   isomorphic to the set of prime implicants of the underlying linear
   bijection, and we only need to be concerned about them. </li>
   <li> That is, we consider linear bijections x -> M*x for invertible matrices
   M. </li>
  </ul>


  \todo The complement of the diagonal matrix
  <ul>
   <li> Just a natural example, to start the investigations.
   \verbatim
diagz(n) := zeromatrix(n,n)+1 - ident(n);
for n : 1 thru 20 do print(n,newdet(diagz(n)));
1 0 
2 -1 
3 2 
4 -3 
5 4 
6 -5 
7 6 
8 -7 
9 8 
10 -9 
11 10 
12 -11 
13 12 
14 -13 
15 14 
16 -15 
17 16 
18 -17 
19 18 
20 -19 
   \endverbatim
   </li>
   <li> So (over RR) det(diagz(n)) = (-1)^(n-1) * (n-1), and thus over ZZ_2
   we have that diagz(n) is invertible iff n is even. </li>
   <li> Experiment:
   \verbatim
> mkdir ExpLTrans
> cd ExpLTrans

for n : 1 thru 8 do output_m_fullcnf(diagz(n),sconcat("LTrans_", n, ".cnf"));

ALG=~/OKplatform/system_directories/bin/QuineMcCluskeySubsumptionHypergraphFullStatistics-n16-O3-DNDEBUG; for (( n=1; n<=8; ++n )); do ${ALG} LTrans_${n}.cnf n; done

for (( n=1; n<=8; ++n )); do F=LTrans_${n}.cnf; echo ${F}; cat ${F}_primes_stats; cat ${F}_shg_stats; done

LTrans_1.cnf
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
2 1 1 0 1 0 1
 length count
1 1
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
1 1 1 0 1 0 1
 length count
1 1

LTrans_2.cnf
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
4 4 8 0 8 0 1
 length count
2 4
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
4 8 12 0 12 0 1
 length count
1 4
2 4

LTrans_3.cnf
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
6 40 144 0 144 0 1
 length count
3 16
4 24
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
40 56 224 0 224 0 1
 length count
4 56

LTrans_4.cnf
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
8 112 448 0 448 0 1
 length count
4 112
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
112 240 1792 0 1792 0 1
 length count
7 128
8 112

LTrans_5.cnf
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
10 336 1600 0 1600 0 1
 length count
4 80
5 256
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
336 992 13312 0 13312 0 1
 length count
12 320
14 640
16 32

LTrans_6.cnf
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
12 1144 6624 0 6624 0 1
 length count
4 120
6 1024
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
1144 4032 96256 0 96256 0 1
 length count
21 768
24 1920
25 1280
32 64

LTrans_7.cnf
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
14 4264 29344 0 29344 0 1
 length count
4 168
7 4096
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
4264 16256 696320 0 696320 0 1
 length count
38 1792
42 5376
44 8960
64 128

LTrans_8.cnf
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
16 16608 131968 0 131968 0 1
 length count
4 224
8 16384
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
16608 65280 5111808 0 5111808 0 1
 length count
71 4096
76 14336
79 28672
80 17920
128 256
   \endverbatim
   </li>
   <li> These counts look pretty regular, and one should be able to determine
   the laws. </li>
   <li> For n=8 some prime-clauses of length 4 are
   \verbatim
-10 -9 -2 1 0
-10 -9 -1 2 0
-10 -2 -1 9 0
-10 1 2 9 0
     \endverbatim
   This partially expresses that (M*x)_1 - (M*x)_2 = x_1 + x_2. </li>
   </li>
  </ul>


  \todo Random sampling
  <ul>
   <li> Random experiments for arbitrary invertible maps of size 8 for seed=1 
   to 2712:
   \verbatim
shell> ../OKlib/Experimentation/Investigations/BooleanFunctions/analyse_random_linear_maps 8 1
   \endverbatim
   and then:
   \verbatim
R> E = read_experiment_dirs("random_ss_linmap", list("e","seed"), "LinearTransformation_full.cnf_primes_stats", header=TRUE,skip=2)
R> ET = rows2columns_df(E, "length", "count", list("e","seed"))
R> summary(ET)

         2                3                4         
Min.   :0.0000   Min.   : 0.000   Min.   :  0.00  
1st Qu.:0.0000   1st Qu.: 4.000   1st Qu.: 40.00  
Median :0.0000   Median : 8.000   Median : 48.00  
Mean   :0.7097   Mean   : 7.965   Mean   : 53.62  
3rd Qu.:2.0000   3rd Qu.:12.000   3rd Qu.: 64.00  
Max.   :8.0000   Max.   :32.000   Max.   :176.00  
                                                                           
       5               6              7              8              9       
 Min.   :  0.0   Min.   : 256   Min.   :   0   Min.   :   0   Min.   :   0  
 1st Qu.:224.0   1st Qu.: 832   1st Qu.:2048   1st Qu.:3328   1st Qu.:2816  
 Median :272.0   Median : 928   Median :2432   Median :4224   Median :3840  
 Mean   :265.0   Mean   : 932   Mean   :2350   Mean   :4129   Mean   :3780  
 3rd Qu.:304.0   3rd Qu.:1024   3rd Qu.:2688   3rd Qu.:4864   3rd Qu.:4864  
 Max.   :464.0   Max.   :2432   Max.   :3968   Max.   :9472   Max.   :9216  
                                                                              
R> sizes = unlist(Map(function(i) sum(ET[i,1:17]),1:2712))
R> summary(sizes)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max. 
   1914    9733   11770   11520   13650   19030 
   \endverbatim
   </li>
   <li> In the above, "sizes" is the number of prime implicates for
   each random instance generated. This is calculated for each instance by 
   taking the sum, over the sizes of clause, of the number of prime implicates
   of each size. </li>
   <li> Overall, these are all much smaller sizes than those we see in
   "First considerations of random permutation" in
   Experimentation/Investigations/BooleanFunctions/plans/general.hpp. </li>
   <li> Why do we not see any clauses of size 10? </li>
  </ul>


  \todo Mathematical investigations
  <ul>
   <li> Can something be proven about the prime implicants of linear
   permutations? Is something in the literature? </li>
   <li> Are there better algorithms for computing r_k-bases (at least for
   special k)? </li>
   <li> In terms of linear algebra a prime implicant can be described as
   follows:
    <ol>
     <li> Consider an n-dimensional vectorspace V over ZZ_2, together with a
     basis B (a set of vector). </li>
     <li> Consider an automorphism f of V. </li>
     <li> The task is now to determine "minimal inconsistent subspaces", given
     by a pair (C,D) of subsets of B, such that the intersection of the affine
     subspaces f(sum(C)+hull(B-C)) and sum(D)+hull(B-D) is empty, and
     such that both C and D are minimal w.r.t. this property; here sum(X) is
     the sum over the elements of X, so that sum(X)+hull(B-X) is an n-|X|
     dimensional subspace (for a subset X of B). </li>
     <li> For more general fields instead of mere summation one had to consider
     linear combinations. </li>
    </ol>
   </li>
   <li> One route of attack is to consider in detail (very) low dimensions.
   </li>
   <li> Another is to consider examples like above the complement of the
   diagonal matrix. </li>
   <li> What can Groeber bases do in this context? </li>
  </ul>

*/
