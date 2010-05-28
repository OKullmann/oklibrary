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
   are false, for all i in {0, ..., p+q} and all p, q. </li>
   <li> Let's call these partial assignments the "standard falsifying
   partial assignments". </li>
   <li> Examples for m=3, n=4, considering 2+1=3, are the corresponding
   CNF-clauses
     {-x_1,-x_2,x_3, -y_1,y_2,y_3,y_4, z_i}
   for 1 <= i <= 3, or using -z_i for 4 <= z_i <= 7. </li>
   <li> In the above example, also {-x_2, -y_1, z_3} or {x_3, y_2, -z_4}
   *could* be valid, however in general from the standard falsifying partial
   assignments and the corresponding standard CNF-clauses no literals can be
   removed. </li>
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
  </ul>


  \todo Smallest prime CNF-representation
  <ul>
   <li> For given m, n, a boolean function f representing unary addition up to
   m, n is sought which has the smallest number of CNF prime-clauses. </li>
   <li> Or, in other words, which has a maximally prime CNF-representation
   which is minimal w.r.t. the number of clauses. </li>
   <li> Is the above F_0 a maximal prime clause-set (is identical with the set
   of its prime clauses)? </li>
   <li> F_0 has (m+1)*(n+1)*(m+p+1) many clauses. </li>
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
  </ul>


  \todo Smallest r_1-based CNF-representation without new variables


  \todo Smallest r_2-based CNF-representation without new variables

*/

