// Oliver Kullmann, 1.12.2012 (Swansea)
/* Copyright 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/PigeonholeFormulas/plans/Hardness.hpp
  \brief Investigations regarding hardness of pigeonhole clause-sets


  \todo Influence of totally blocked clauses
  <ul>
   <li> For unsatisfiable clause-sets addition of totally blocked clauses does
   not influence the hardness:
   \verbatim
for n : 0 thru 4 do print(n,hardness_u_cs(weak_php_cs(n+1,n)));
0 0
1 1
2 2
3 3
4 4

for n : 0 thru 4 do print(n,hardness_u_cs(special_php_cs(n+1,n)));
0 0
1 1
2 2
3 3
4 4
   \endverbatim
   The special PHP-clause-sets are not obtained by addition of blocked clauses
   from the weak PHP-clause-sets, since the AMO-clauses can be resolved with
   the long hole-clauses, but as the above experiment shows, even for that
   form the hardness is not influenced. </li>
   <li> For satisfiable clause-sets:
   \verbatim
for n : 0 thru 4 do print(n,hardness_cs(weak_php_cs(n,n)));
0 0
1 0
2 1
3 2
# aborted

for n : 0 thru 4 do print(n,hardness_cs(strong_php_cs(n,n)));
0 0
1 0
2 1
3 2
4 3

for n : 0 thru 4 do print(n,hardness_cs(dual_strong_php_cs(n,n)));
0 0
1 0
2 1
3 1
4 2

for n : 0 thru 5 do print(n,hardness_cs(special_php_cs(n,n)));
0 0
1 0
2 1
3 1
4 1
# n=5: reached memory limit with heap_size_ecl_okl=10^9 (after several days)
   \endverbatim
   </li>
  </ul>

*/
