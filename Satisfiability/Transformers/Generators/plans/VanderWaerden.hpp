// Oliver Kullmann, 11.6.2009 (Swansea)
/* Copyright 2009, 2010, 2011 Oliver Kullmann
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


  \bug DONE
  App tests use obsolete tail syntax
  <ul>
   <li> The VanderWaerdenCNF application tests yield the following error on some
   systems:
   \verbatim
TEST PROBLEMS FOUND:
test_cases/basic/VanderWaerdenCNF/VanDerWaerden_2-2-2_2.cnf
test_cases/basic/VanderWaerdenCNF/VanDerWaerden_2-3-3_3.cnf
test_cases/basic/VanderWaerdenCNF/VanDerWaerden_2-3-3_6.cnf
test_cases/basic/VanderWaerdenCNF/VanDerWaerden_2-2-2_5.cnf
test_cases/basic/VanderWaerdenCNF/VanDerWaerden_2-3-4_7.cnf
PROCESSING:
/home/csmg/Work/OKlibrary/OKplatform/system_directories/bin/VanderWaerdenCNF
Filename: test_cases/basic/VanderWaerdenCNF/VanDerWaerden_2-2-2_2.cnf
tail: cannot open `+5' for reading: No such file or directory
0a1,3
> p cnf 2 2
> 1 2 0
> -1 -2 0
make: *** [app_tests] Error 1
make: Leaving directory `/home/csmg/Work/OKlibrary/OKplatform/OKsystem/OKlib/Satisfiability/Transformers/Generators'
   \endverbatim
   </li>
   <li> This is because the "tail +5" syntax is obsolete, and should now be
   "tail -n +5". See
   http://www.gnu.org/software/coreutils/manual/html_node/tail-invocation.html .
   </li>
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
> time PdVanderWaerden-O3-DNDEBUG 3 1000
user    3m3.253s
sys     0m0.152s
user    2m51.189s
sys     0m0.183s

> time PdVanderWaerdenCNF-O3-DNDEBUG 3 30 1000
user    2m54.118s
sys     0m0.092s
user    2m51.469s
sys     0m0.076s

   \endverbatim
   </li>
   <li> Could this be caused by the Boost upgrade?
    <ol>
     <li> We need to rebuild the application at commit
     5bb52740ad8a3394d2713d61a2c1c74df4d6a22e (before update to new Boost):
     \verbatim
Generators> git checkout 5bb52740ad8a3394d2713d61a2c1c74df4d6a22e
Generators> oklib cleanall
Generators> oklib all

kullmann-0:OKplatform> time PdVanderWaerden-O3-DNDEBUG 3 1000 > Test02
user    2m47.409s
sys     0m0.107s
user    2m47.883s
sys     0m0.099s

Generators> git checkout master
     \endverbatim
     </li>
     <li> So it seems the hypergraph-generation was a bit faster (but not
     much). </li>
     <li> We need to find out whether this difference is real or not. </li>
    </ol>
   </li>
  </ul>

*/

