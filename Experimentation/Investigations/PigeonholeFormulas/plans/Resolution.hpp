// Oliver Kullmann, 2.3.2012 (Swansea)
/* Copyright 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/PigeonholeFormulas/plans/Resolution.hpp
  \brief Investigations regarding (full) resolution for pigeonhole clause-sets


  \todo Resolution complexity
  <ul>
   <li> Now we consider the minimum complexity rc(n) for PHP^{n+1}_n (counting
   the number of different clauses). </li>
   <li> It is known rc(n) >= 2^(Omega(n)). </li>
   <li> In [Iwama, Miyazaki, 1999] it is shown rc(n) <= O(n^2 * 2^n). </li>
   <li> In [Cook, 1976] one finds the claim
   rc(n) <= 1/2 n^3 + 1/2 n^2 + n + 1 + (n^2 + 3 n) * 2^(n-2).
   (In the paper one has (n^2 + 3 n) * 2^(n-2), which stands for the number of
   steps, and thus the number of clauses of PHP has to be added.) </li>
   <li> Alaisdair Urquhart conjectures this to be exact. </li>
   <li> Numerically:
   \verbatim
f(n) := 1/2*n^3 + 1/2*n^2 + n + 1 + (n^2 + 3*n) * 2^(n-2);
for n : 0 thru 20 do print(n,f(n),(n^2 + 3*n) * 2^(n-2));

0 1 0
1 5 2
2 19 10
3 58 36
4 157 112
5 401 320
6 997 864
7 2444 2240
8 5929 5632
9 14239 13824
10 33841 33280
11 79586 78848
12 185269 184320
13 427181 425984
14 976333 974848
15 2213656 2211840
16 4982929 4980736
17 11143739 11141120
18 24775705 24772608
19 54791726 54788096
20 120590461 120586240
   \endverbatim
   </li>
   <li> Experimentally:
   \verbatim
output_res_php(n,k) := outputext_fcl(sconcat("PHP with n=",n," and k=",k,"."), shortresref_aloamo_fcl(fcs2fcl(weak_php_fcs(n+1,n)),k),sconcat("PHP_RES_",n,"_",k,".cnf"));

output_res_php(1,1);
> n=1 k=1; cat PHP_RES_${n}_${k}.cnf | ExtendedToStrictDimacs-O3-DNDEBUG > Php_res_${n}_${k}.cnf
UNSATISFIABLE

output_res_php(1,2);
> n=1 k=2; cat PHP_RES_${n}_${k}.cnf | ExtendedToStrictDimacs-O3-DNDEBUG > Php_res_${n}_${k}.cnf
SATISFIABLE

output_res_php(2,9);
> n=2 k=9; cat PHP_RES_${n}_${k}.cnf | ExtendedToStrictDimacs-O3-DNDEBUG > Php_res_${n}_${k}.cnf
UNSATISFIABLE (minisat-2.2.0: 22714981 conflicts, 1303.62 s)
output_res_php(2,10);
n=2 k=10; cat PHP_RES_${n}_${k}.cnf | ExtendedToStrictDimacs-O3-DNDEBUG > Php_res_${n}_${k}.cnf
SATISFIABLE

output_res_php(3,35);
# (likely) needs too much memory for a 4GB machine
   \endverbatim
   </li>
   <li> Established values for rc(n):
    <ol>
     <li> rc(0) = 1 (= 1+0) </li>
     <li> rc(1) = 5 (= 3+2) </li>
     <li> rc(2) = 19 (= 9+10) </li>
    </ol>
   </li>
   <li> For upper bounds extracted from tree-refutations see
   "Tree-resolution refutations found by simple algorithms" in
   Investigations/PigeonholeFormulas/plans/TreeResolution.hpp. </li>
  </ul>

*/
