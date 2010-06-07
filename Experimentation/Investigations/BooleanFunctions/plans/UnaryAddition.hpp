// Oliver Kullmann, 20.5.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/BooleanFunctions/plans/UnaryAddition.hpp
  \brief Investigations regarding the presentation of unary addition


  \todo Boolean functions representing unary addition
  <ul>
   <li> Consider n in NN_0. </li>
   <li> "Boolean literals x_1, ..., x_n represent 0 <= k <= n in unary" is
   defined by the condition (as constraint) that the first k values are true,
   while the remaining values are false. </li>
   <li> For m, n in NN, that a boolean function f in m+n+(m+n) variables
   x_1, ..., x_m, y_1, ..., y_n, z_1, ..., z_{m+n} represents unary
   addition of numbers up to m, n is defined by the following conditions:
    <ol>
     <li> Let x, y, z be the tuples of variables. </li>
     <li> Consider x representing 0 <= p <= m in unary and y representing
     0 <= q <= n in unary. </li>
     <li> Then for z representing p+q in unary we have f(x,y,z)=1, while
     for all other z we have f(x,y,z)=0. </li>
     <li> For all other x,y,z the value f(x,y,z) is arbitrary. </li>
    </ol>
   </li>
  </ul>


  \todo CNF and DNF representations of unary addition
  <ul>
   <li> For such f representing unary addition w.r.t. m, n, the extensions of
   the partial assignments given by
     x representing p, y representing q, z_i different from z representing p+q
   are false, for all i in {1, ..., p+q} and all p, q. </li>
   <li> Let's call these partial assignments the "standard falsifying
   partial assignments". </li>
   <li> Examples for m=3, n=4, considering 2+1=3, are the corresponding
   CNF-clauses
     {-x_1,-x_2,x_3, -y_1,y_2,y_3,y_4, z_i}
   for 1 <= i <= 3, or using -z_i for 4 <= z_i <= 7. </li>
   <li> In the above example, also {-x_2, -y_1, z_3} or {x_3, y_2, -z_4}
   *could* be valid, but we use only those that must be included for the
   "standard". </li>
   <li> Such clauses are reducible in some cases. For instance, for p=1,
   q=1, we have {-x_1,z_2} and {-y_1,z_2} as CNF clauses, noting the fact that
   as both input values are at most 1, then if one of them is 0, then the
   result can not be 2. If we handle these trivial cases (p=m,q=n), do we
   get prime clauses? </li>
   <li> And the total assignments
     x representing p, y representing q, z representing p+q
   are all true. </li>
   <li>  Let's call these partial assignments the "standard satisfying total
   assignments". </li>
   <li> Again, in general, if nothing more is known about f, then these total
   assignments can not be made smaller. </li>
   <li> Considering the CNF clause-set F_0 corresponding to all standard
   falsifying partial assignments, we obtain f_0, which is the smallest w.r.t.
   the falsified points (and thus largest w.r.t. satisfied points). </li>
   <li> Considering the DNF clause-set F_1 corresponding to all standard
   satisfying total assignments, we obtain f_1, which is the smallest w.r.t.
   satisfied points (and thus largest w.r.t. falsified points). </li>
   <li> To generate the standard satisfying and falsifying total assignments 
   respectively, for p >= 0 and q >= 0 we have the following
   \verbatim
declare(una,noun)$
declare(una,posfun)$
una_var(v,i) := apply(nounify(una),[v,i])$
una_var_l(v,a,b) := create_list(una_var(v,i),i,a,b)$

unary_all_tass_std(p,q) := 
  all_tass(append(    
    una_var_l('x,1,max(p,1)),una_var_l('y,1,max(q,1)),
    una_var_l('z,1,max(p+q,2))))$

unary_add_full_dnf_fcl_std(p,q) := 
  unary_add_full_dnf_fcl(
    una_var_l('x,1,max(p,1)),una_var_l('y,1,max(q,1)),
    una_var_l('z,1,max(p+q,2)))$
unary_add_full_dnf_fcl(X,Y,Z) := 
  [append(X,Y,Z),create_list(
    setify(append(
      create_list(if i > p then -X[i] else X[i], i, 1, length(X)),
      create_list(if j > q then -Y[j] else Y[j], j, 1, length(Y)),
      create_list(if k > p+q then -Z[k] else Z[k], k, 1, length(Z)))),
    p,0,length(X), q, 0, length(Y))]$

unary_add_cnf_fcl_std(p,q) := 
  unary_add_cnf_fcl(
    una_var_l('x,1,max(p,1)),una_var_l('y,1,max(q,1)),
    una_var_l('z,1,max(p,1)+max(q,1)))$
unary_add_cnf_fcl(X,Y,Z) := 
  [append(X,Y,Z), create_list(
    comp_sl(setify(append(
      create_list(if i > p then -X[i] else X[i], i, 1, length(X)),
      create_list(if j > q then -Y[j] else Y[j], j, 1, length(Y)),
      [if r > p+q then Z[r] else -Z[r]]))),
    p,0,length(X), q, 0, length(Y), r, 1, length(X)+length(Y))]$
   \endverbatim
   Using "_std" isn't appropriate to denote that these functions only generate
   those assignments which must be true, and those which must be false. We have
   the following options
   <ul>
    <li> "unary_add_nec_full_dnf_fcl" where "nec" stands for necessary. 
    </li>
    <li> "unary_add_req_full_dnf_fcl" where "req" stands for required. </li>
   </ul>
   </li>
  </ul>


  \todo Smallest prime CNF-representation
  <ul>
   <li> For given m, n, a boolean function f representing unary addition up to
   m, n is sought which has the smallest number of CNF prime-clauses. </li>
   <li> Or, in other words, which has a maximally prime CNF-representation
   which is minimal w.r.t. the number of clauses. </li>
   <li> F_0 is not prime (as it stands), and would not be maximal if it was
   as the 3-clauses introduced by [BB 2003] are prime and are not
   (in general) included (see below). </li>
   <li> F_0 has (m+1)*(n+1)*(m+p) many clauses. </li>
   <li> The BB-paper example:
    <ol>
     <li> There are the 3-clauses (x_p & y_q -> z_{p+q}) for p in {0,...,m}
     and q in {0,...,n}, where x_0, y_0, z_0 := 1. </li>
     <li> And there are the 3-clauses (-x_p & -y_q -> -z_{p+q-1} for p in
     {1,...,m} and q in {1,...,n}, plus (-x_m -> -z_{m+n}) and
     (-y_n -> -z_{m+n}). </li>
     <li> This makes (m+1)*(q+1) + m*n + 2 clauses; let's call the clause-set
     F_BB ("F_BB(m,n)" fully written out, as for all these notations). </li>
     <li> The corresponding boolean function f_BB is neither f_0 nor f_1. </li>
     <li> For example f_BB(1,0,1; 0,0,0,0; 0,0,0,0,0,0,0) = 0, while f_0 yields
     1, and f_BB(1,0,1; 0,0,0,0; 0,0,1,0,0,0,0) = 1, while f_1 yields 0. </li>
     <li> F_BB is prime.
      <ol>
       <li> Consider any 3-clause of the form (x_p & y_q -> z_{p+q}) (as above),
       then if 0 < p, 0 < q then
        <ul> 
         <li> x_p -> z_{p+q} doesn't follow, as the total assignment
       	 that sets 
	  <ul>
	   <li> y_{q'} = 0 for all q' with 1 <= q' <= n </li>
	   <li> x_{p'} = 1 for all 1 <= p' <= p </li>
	   <li> x_{p''} = 0 for all p'' with p < p'' <= m and </li>
	   <li> z_{r} = 0 for all p < r <= m + n  </li>
	   <li> z_{r} = 1 for all 1 <= r <= p  </li>
	  </ul>
	 is falsifying for this clause but is a satisfying assignment for 
	 the original clause-set. </li>
         <li> y_q -> z_{p+q} doesn't follow, as the total assignment
	 that sets
	  <ul>
	   <li> x_{p'} = 0 for all p' with 1 <= p' <= m </li>
	   <li> y_{q'} = 1 for all 1 <= q' <= q </li>
	   <li> y_{q''} = 0 for all q'' with q < q'' <= n and </li>
	   <li> z_{r} = 0 for all q < r <= m + n  </li>
	   <li> z_{r} = 1 for all 1 <= r <= q  </li>
	  </ul>
	 is falsifying for this clause but is a satisfying assignment for 
	 the original clause-set. </li>
         <li> x_p & y_p doesn't follow, as the total assignment that
	 sets
	  <ul>
	   <li> x_{p'} = 0 for all p' with 1 <= p' <= m </li>
	   <li> y_{q'} = 0 for all q' with  1 <= q' <= n </li>
	   <li> z_{r} = 0 for all r with 1 <= r <= m + n  </li>
	  </ul>
	 is falsifying for this clause but is a satisfying assignment for 
	 the original clause-set. </li>
        </ul>
       otherwise we have p = 0 or q = 0, so the 3-clause is either satisfied
       (and so disappears from the clause-set) or reduces to a 2-clause, and no
       unary clause follows from the original clause-set, therefore any 
       3-clause of the form (x_p & y_q -> z_{p+q}) is prime.
       </li>
       <li> In the other case, we consider any 3-clause of the form 
       (-x_{p+1} & -y_{q+1} -> -z_{p+q+1}) (as above), then if p < m, q < n 
       then
        <ul> 
         <li> -x_{p+1} -> -z_{p+q+1} doesn't follow, as the total assignment
 	 that sets 
	  <ul>
	   <li> y_{q'} = 1 for all q' with 1 <= q' <= q+1 </li>
	   <li> x_{p'} = 1 for all 1 <= p' <= p </li>
	   <li> x_{p''} = 0 for all p < p'' <= m </li>
	   <li> z_{r} = 1 for all 1 <= r <= p+q+1  </li>
	   <li> z_{r} = 0 for all p+q+1 < r <= m+n  </li>
	  </ul>
	 is falsifying for this clause but is a satisfying assignment for 
	 the original clause-set. </li>
         <li> -y_{q+1} -> -z_{p+q+1} doesn't follow, as the total assignment
	 that sets 
	  <ul>
	   <li> x_{p'} = 1 for all p' with 1 <= p' <= p+1 </li>
	   <li> y_{q'} = 1 for all 1 <= q' <= q </li>
	   <li> y_{q''} = 0 for all q < q'' <= n </li>
	   <li> z_{r} = 1 for all 1 <= r <= p+q+1  </li>
	   <li> z_{r} = 0 for all p+q+1 < r <= m+n  </li>
	  </ul>
	 is falsifying for this clause but is a satisfying assignment for 
	 the original clause-set. </li>
         <li> -x_{p+1} & -y_{q+1} doesn't follow, as the total assignment
	 that sets
	  <ul>
	   <li> x_{p'} = 1 for all p' with 1 <= p' <= m </li>
	   <li> y_{q'} = 1 for all q' with  1 <= q' <= n </li>
	   <li> z_{r} = 1 for all r with 1 <= r <= m + n  </li>
	  </ul>
	 is falsifying for this clause but is a satisfying assignment for 
	 the original clause-set. </li>
        </ul>
       otherwise we have p = m or q = n, so the 3-clause is either satisfied
       (and so disappears from the clause-set) or reduces to a 2-clause, and no
       unary clause follows from the original clause-set, therefore any 
       3-clause of the form (-x_{p+1} & -y_{q+1} -> -z_{p+q+1}) is prime.
       </li>
      </ol>
     Therefore, FF_BB is prime.
     </li>
     <li> FF_BB is not maximally prime (in general) as, for instance
     (x_p+2 -> z_p) for any p <= m - 2 is an implicate of FF_BB but
     is not subsumed by any clause in FF_BB. </li>
    </ol>
   </li>
   <li> In some cases the BB example matches up to *the* minimum CNF
   representation, but only for trivial cases. </li>
   <li> Considering the number of minimum representations for different
   m and n, we have
   \verbatim
m : 2; n : 2;
print("p q #min_F_0 stat_min_F #min_F_1 stat_min_F_1 stat_BB_F Eq_0? Eq_1?");
for p : 0 thru m do 
  for q : 0 thru n do block([min_F_0_l, BB_F],
    min_F_0_l : all_minequiv_bvsr_cs(expand_fcs(map(setify,unary_add_cnf_fcl_std(p,q)))[2]),
    min_F_1_l : all_minequiv_bvsr_cs(
      map(comp_sl,
      setdifference(unary_all_tass_std(p,q),
        setify(unary_add_full_dnf_fcl_std(p,q)[2])))),
    BB_F : setify(unary_bb_add_fcl(
             una_var_l('x,1,max(1,p)),una_var_l('y,1,max(1,q)), 
             una_var_l('z,1,max(p,1)+max(q,1)))[2]),
    print(p,q,length(min_F_0_l),statistics_cs(min_F_0_l[1]), length(min_F_1_l),statistics_cs(min_F_1_l[1]),statistics_cs(BB_F),
      is(min_F_0_l[1] = BB_F), is(min_F_1_l[1] = BB_F)))$
   \endverbatim
   which produces
   \verbatim
p q #min_F_0 stat_min_F #min_F_1 stat_min_F_1 stat_BB_F Eq_0? Eq_1?
0 0 1 [4,6,14,3,2] 1 [4,6,14,3,2] [4,6,14,3,2] true true
0 1 1 [4,6,14,3,2] 1 [4,6,14,3,2] [4,6,14,3,2] true true
0 2 1 [6,10,30,4,2] 1 [5,7,17,3,2] [6,10,24,3,2] false false
1 0 1 [4,6,14,3,2] 1 [4,6,14,3,2] [4,6,14,3,2] true true
1 1 1 [4,6,14,3,2] 1 [4,6,14,3,2] [4,6,14,3,2] true true
1 2 1 [6,10,30,4,2] 1 [6,10,24,3,2] [6,10,24,3,2] false true
2 0 1 [6,10,30,4,2] 1 [5,7,17,3,2] [6,10,24,3,2] false false
2 1 1 [6,10,30,4,2] 1 [6,10,24,3,2] [6,10,24,3,2] false true
*timeout*
   \endverbatim
   So the clauses used in [BB 2003] are not simply one of these minimal
   clause-sets. This is possibly because such clause-sets do not have
   the required r_1 inference properties. This makes considering 
   "Smallest r_1-based CNF-representation without new variables" more
   important. 
   </li>
   <li> We should also check the properties of the minimum clause-set
   representations of F_0 and F_1. </li>
  </ul>


  \todo Smallest r_1-based CNF-representation without new variables


  \todo Smallest r_2-based CNF-representation without new variables

*/

