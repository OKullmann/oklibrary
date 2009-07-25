// Oliver Kullmann, 24.7.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Transformers/Generators/plans/Hindman.hpp
  \brief Plans for generators for Hindman problems


  \todo Connections
  <ul>
   <li> See ComputerAlgebra/Hypergraphs/Lisp/Generators/Hindman.mac for
   the hypergraph generators. </li>
   <li> See ComputerAlgebra/Satisfiability/Lisp/Generators/HindmanProblems.mac
   for the clause-set generators. </li>
  </ul>


  \todo Write simple generator for boolean problems (r=2)
  <ul>
   <li> We have currently "hindman2gen_fcs(a,n)" in
   ComputerAlgebra/Satisfiability/Lisp/Generators/HindmanProblems.mac. </li>
   <li> But it shall become hindmani_r2k2_fcs(a,n); see
   ComputerAlgebra/Satisfiability/Lisp/Generators/plans/HindmanProblems.hpp.
   </li>
   <li> And we also need hindman_r2k2_fcs(a,n). </li>
   <li> It would be good if we could implement the general versions
   hindmannd_r2_fcs(a,k1,k2,n) and hindmanind_r2_fcs(a,k1,k2,n). </li>
   <li> So the input would consist of four numbers a, k1, k2, n; likely
   no need to make a distinction in the names of the created files regarding
   "diagonal" (k1=k2) versus "non-diagonal", and regarding "standard" (a=1)
   versus "extended". </li>
   <li> But for the general versions (allowing arbitrary k) we should
   implement first the hypergraph generator, which might take a bit longer,
   and thus perhaps first we only consider the simple cases k1,k2 <= 2. </li>
  </ul>

*/

