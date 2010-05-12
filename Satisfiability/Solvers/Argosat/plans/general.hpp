// Oliver Kullmann, 11.5.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Solvers/Argosat/plans/general.hpp
  \brief Plans regarding the Argosat solver and library


  \todo Connections
  <ul>
   <li> Regarding installation see "Argo" in
   Buildsystem/ExternalSources/SpecialBuilds/plans/SAT.hpp. </li>
  </ul>


  \todo Document options
  <ul>
   <li> In src/Solver.cpp (not available in the installed files) we find some
   options, which we should document. </li>
   <li> First we have "--literal_selection_strategy" ("-l"), followed by some
   "LiteralSelectionStrategy specification". </li>
   <li> Then there is "--forget_strategy" ("-f"), followed by some
   "FrogetStrategy specification" (typo should be corrected). </li>
   <li> And "--forget_selection_strategy" ("-fs"), followed by some
   "ForgetSelectionStrategy specification". </li>
   <li> And "--restart_strategy" ("-r"), followed by some
   "RestartStrategy specification". </li>
   <li> And "--variable_selection_strategy" ("-v"), followed by some
   "VariableSelectionStrategy". </li>
   <li> And "--literal_polarity_selection_strategy", followed by some
   "LiteralPolaritySelectionStrategy specification". </li>
   <li> Error in code: Error message in case that specification does not work
   says "Error setting variable selection strategy". </li>
   <li> Finally "--use_pure_literal" ("-pl"), without further arguments. </li>
   <li> Peculiar, that the option need to be specified *after* the filename.
   </li>
   <li> How to set the timeout? (correction needed: the file
   "src/listeners/TimeoutSolverListener" uses microsoft-line-endings.) </li>
  </ul>


  \todo Control verbosity of output
  <ul>
   <li> We should introduce an option to eliminate output of "..." etc. </li>
   <li> It makes the output unreadable, especially for harder problems. </li>
   <li> What's the meaning of this output?)  It seems to be the "trail",
   documenting certain events. </li>
  </ul>


  \todo Output of learned clauses
  <ul>
   <li> Compare "Output learned clauses" in
   Satisfiability/Solvers/Minisat/Minisat2/plans/general.hpp. </li>
  </ul>


  \todo Output all information when aborted by SIGINT
  <ul>
   <li> So signals need to be caught. </li>
   <li> See minisat2 or the OKsolver. </li>
  </ul>


  \todo Correcting include-directives
  <ul>
   <li> The wrong inclusion-form
   \code
#include "Literal.hpp"
#include "Valuation.hpp"
   \endcode
   is used. See "Improved include-directives in the source code" in
   Satisfiability/Algorithms/LocalSearch/Ubcsat/plans/general.hpp for some
   further explanations. </li>
   <li> A directory "Argosat" (or "argosat") needs to be introduced,
   all paths need to be prefixed with this directory, and the corrected
   library-inclusion-form "<>" needs to be used. </li>
  </ul>


  \todo Usage as library
  <ul>
   <li> The library-files use asserts, but they do not include "cassert". </li>
   <li> See above "Correcting include-directives". </li>
   <li> Include-guards like "__CLAUSE_H__" seem to be dangerous (quite possible
   that some other library uses also this). Adding a random suffix, as in the
   OKlibrary, would solve this problem. </li>
  </ul>

*/

