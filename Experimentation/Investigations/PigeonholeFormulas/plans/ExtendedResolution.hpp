// Oliver Kullmann, 2.3.2012 (Swansea)
/* Copyright 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/PigeonholeFormulas/plans/ExtendedResolution.hpp
  \brief Investigations regarding extended resolution for pigeonhole clause-sets


 \todo Extended PHP clause-sets
  <ul>
   <li> Computed by output_weak_php_unsat_ext_stdname(n). </li>
   <li> Performance of SAT solvers (n is the number of holes):
    <ol>
     <li> First impression with cryptominisat (csltok with 2.0 GHz):
     \verbatim
n=10
18.65s
n=11
38.64s
n=12
249.72s
n=13
886.28s
n=14
13187.05s
# with n=14 worse than PHP itself
     \endverbatim
     </li>
     <li> The other solvers seem to perform worse (and also not better than
     their PHP-performance). Though one needs to see complete data. </li>
    </ol>
   </li>
   <li> So apparently no solver can make use of the extension. This seems
   understandable, given the complicated nature of the resolution proof for
   the extension --- all clauses have to be used, in a special order. </li>
  </ul>

*/
