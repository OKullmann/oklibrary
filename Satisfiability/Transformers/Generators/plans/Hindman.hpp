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
   <li> And also general k might be problematic (w.r.t. efficiency), and so
   we only consider first the case k=2. </li>
   <li> See Combinatorics/Hypergraphs/Generators/plans/Hindman.hpp for plans
   for the hypergraph generator. </li>
   <li> Accordingly, we should create a clause-set generator as a range of
   ranges:
    <ol>
     <li> In this case we simply run twice through the hypergraph, where in
     the second phase all clauses are negated. </li>
     <li> We need also such a most rudimentary concept; see
     ProblemInstances/ClauseSets/plans/general.hpp. </li>
     <li> Such clause-sets must be possible as input to CLSAdaptor (see
     Interfaces/InputOutput/plans/ClauseSetAdaptors.hpp). </li>
     <li> We should also have this form of generator (first) at the Maxima/Lisp
     level. </li>
    </ol>
   </li>
   <li> The generators should be placed in Hindman_r2k2.hpp (for the boolean
   problems with k=2), and the application in Hindman_r2k2.cpp. </li>
  </ul>

*/

