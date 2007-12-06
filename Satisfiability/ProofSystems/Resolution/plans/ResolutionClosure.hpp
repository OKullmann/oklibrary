// Oliver Kullmann, 21.6.2007 (Swansea)
/* Copyright 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ProofSystems/Resolution/plans/ResolutionClosure.hpp
  \brief Plans for computing all resolvents (resp. all minimal resolvents)


  \todo Primimplicates via resolution
  <ul>
   <li> The basic algorithm is to create all resolvents round
   for round, removing all subsumptions found as early as possible. </li>
   <li> Compare "Prime implicants and implicates" in
   Transitional/Satisfiability/FiniteFunctions/plans/general.hpp. </li>
  </ul>

*/
