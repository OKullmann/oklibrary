// Oliver Kullmann, 11.6.2009 (Swansea)
/* Copyright 2009, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Transformers/Generators/plans/VanderWaerden.hpp
  \brief Plans for generators for van der Waerden problems


  \todo Connections
  <ul>
   <li> See
   ComputerAlgebra/Satisfiability/Lisp/Generators/RamseyTheory/plans/VanderWaerdenProblems.hpp
   for the fundamental (basic) specifications. </li>
   <li> See OKlib/Combinatorics/Hypergraphs/Generators/VanderWaerden.hpp
   for hypergraph generators. </li>
  </ul>


  \todo Update
  <ul>
   <li> See the todos in
   Satisfiability/Transformers/Generators/VanderWaerden.hpp. </li>
   <li> Then also Satisfiability/Transformers/Generators/VanderWaerden.hpp
   should be enhanced to produce the full ranges of instances. </li>
  </ul>


  \todo Speed of PdVanderWaerdenCNF.cpp
  <ul>
   <li> On csltok, for k=30 and n=1000 it took once a bit more than one minute.
   However on cs-wsok it now takes about five minutes? </li>
   <li> Timings on csltok:
   \verbatim
> time PdVanderWaerden-O3-DNDEBUG 3 1000 > Test01
user    3m3.253s
sys     0m0.152s
user    2m51.189s
sys     0m0.183s

> time PdVanderWaerdenCNF-O3-DNDEBUG 3 30 1000 > Test01
user    2m54.118s
sys     0m0.092s
user    2m51.469s
sys     0m0.076s

   \endverbatim
   </li>
   <li> Could this be caused by the Boost upgrade?
    <ol>
     <li> We need to rebuild the application at commit
     5bb52740ad8a3394d2713d61a2c1c74df4d6a22e (before update to new Boost).
     </li>
     <li> Then we check the speed of the hypergraph creation. </li>
    </ol>
   </li>
  </ul>

*/

