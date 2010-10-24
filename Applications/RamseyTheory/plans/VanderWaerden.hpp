// Oliver Kullmann, 31.10.2006 (Swansea)
/* Copyright 2006 - 2007, 2009, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Applications/RamseyTheory/plans/VanderWaerden.hpp
  \brief Plans on the module on computing van der Waerden numbers


  \todo Connections
  <ul>
   <li> See
   ComputerAlgebra/Satisfiability/Lisp/Generators/RamseyTheory/plans/VanderWaerdenProblems.hpp
   and
   ComputerAlgebra/Satisfiability/Lisp/Generators/RamseyTheory/VanderWaerdenProblems.mac.
   </li>
   <li> See Combinatorics/Hypergraphs/Generators/plans/VanderWaerden.hpp. </li>
   <li> See Satisfiability/Transformers/Generators/plans/VanderWaerden.hpp.
   </li>
   <li> See
   Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/general.hpp
   and
   Experimentation/Investigations/RamseyTheory/GreenTaoProblems/plans/general.hpp.
   </li>
   <li> See the various generators in Satisfiability/Transformers/Generators/.
   </li>
  </ul>


  \todo Create tests
  <ul>
   <li> Application tests are needed for
   Applications/RamseyTheory/plans/MinimumTransversals_VanderWaerden.cpp. </li>
   <li> The problem is that currently we cannot compile this application
   automatically, due to the build dependencies not expressible, and thus
   yet we cannot create application tests. </li>
  </ul>


  \todo Checking certificates
  <ul>
   <li> An interesting algorithmic problem is how to check with highest
   efficiency a vdW-certificate. </li>
   <li> See "Certificates" in
   ComputerAlgebra/RamseyTheory/Lisp/VanderWaerden/plans/general.hpp. </li>
   <li> Checking seems also require quite a bit of time, and thus it cannot
   be done fully properly at Maxima-level. </li>
  </ul>

*/

