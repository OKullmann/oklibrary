// Oliver Kullmann, 10.11.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Transitional/OKsolver/plans/general.hpp
  \brief Plans for the module containing old OKsolver-etc-code


  \todo Submodule "Experimental"
  <ul>
   <li> "AllgKlassen" : All content should be transferred
    <ol>
     <li> AllgKlassen200102: DONE
      <ul>
       <li> DONE AllgKlassen200102/VarSet.hpp goes to
       Satisfiability/ProblemInstances/Variables. </li>
       <li> DONE AllgKlassen200102/Literal.hpp goes to
       Satisfiability/ProblemInstances/Literals. </li>
       <li> DONE AllgKlassen200102/Clause.hpp goes to
       Satisfiability/ProblemInstances/Clauses. </li>
       <li> DONE AllgKlassen200102/ClauseSet.hpp goes to
       Satisfiability/ProblemInstances/ClauseSets. </li>
       <li> DONE AllgKlassen200102/PartAssign.hpp goes to
       Satisfiability/Assignments. </li>
       <li> DONE AllgKlassen200102/DLL_Algorithms.hpp and
       AllgKlassen200102/DLL_Algorithms.cpp go to
       Satisfiability/Algorithms/Backtracking. </li>
       <li> DONE AllgKlassen200102/DLL-Implementations.cpp goes to
       Satisfiability/Solvers/DLL. It needs to be linked with
       DLL_Algorithms.o and Kommandozeile.o.
       </li>
      </ul>
     </li>
     <li> AllgKlassen200203:

     </li>
    </ol>
   </li>
   <li> After completed transfer, module should be removed. </li>
   <li> "Transformationen" : All content should be transferred
     (that is, what is still relevant). DONE (moved to
     Satisfiability/Transformers/Generators)
   </li>
   <li> "Graphen" : It seems there is nothing of real interest in here anymore.
     DONE (transferred to Combinatorics/Graphs/BoostSupport)
   </li>
  </ul>


  \todo Submodule "Generator" must be transferred to module RandomGenerator. DONE


  \todo Submodule "Hilfsprogramme" should be inspected, what the compilation-tool there
   is doing, so that interesting functions can be incorporated into the build system.
   DONE (transferred to module Satisfiability/Solvers/OKsolver)


  \todo Submodule "Quellen" should be transferred to module OKsolver_1_0. DONE
  (moved to module Satisfiability/Solvers/OKsolver)


  \todo Submodule "Datenbank" has been transferred to module OKdatabase,
  and should sooner or later be removed. DONE

*/
