// Oliver Kullmann, 4.9.2009 (Swansea)
/* Copyright 2009, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/UHittingCatalogue/plans/general.hpp
  \brief Plans regarding building the catalogue of unsatisfiable non-singular hitting clause-sets of low deficiency


  \todo Connections
  <ul>
   <li> W.r.t. general investigations into minimal unsatisfiability see
   Experimentation/Investigations/MinimalUnsatisfiability/plans/general.hpp.
   </li>
  </ul>
  
  
  \todo Deficiency = 3, n = 5
  <ul>
   <li> It should be possible to completely determine these instances. </li>
   <li>
   \verbatim
oklib_monitor:true;
oklib_monitor_level:1;
all_unsinghitting_def(3,5,'Result_3_5);
   \endverbatim
   </li>
   <li> What does the monitor-output mean? Several times the same clause-sets
   are output? It seems that the isomorphism check takes only place
   afterwards. But we don't need to see these outputs. </li>
   <li> We need the output into a file. </li>
   <li> We need to incorporate the already known instances (from the
   catalogue). </li>
   <li> And we need the possibility to stop and re-start the whole
   computation, so that we have a chance of finally finishing this case.
   </li>
   <li> See "derived_hitting_cs_pred_isoelim" in
   ComputerAlgebra/Satisfiability/Lisp/ConflictCombinatorics/plans/HittingClauseSets.hpp.
   </li>
  </ul>

*/
