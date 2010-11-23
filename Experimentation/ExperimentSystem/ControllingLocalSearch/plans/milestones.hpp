// Oliver Kullmann, 22.11.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ExperimentSystem/ControllingLocalSearch/plans/milestones.hpp

  \module_version ExperimentSystem/ControllingLocalSearch 0.3 (22.11.2010)


  \par Version 0.3.1 :

  \par
   In ExperimentSystem/ControllingLocalSearch/plans/DataCollection.hpp
   the following topics are handled:
    - run_ubcsat can't handle segmentation faults
    - Specification of run_ubcsat is badly written
    - Bad documentation for run_ubcsat
    - False algorithm names : DONE


  \par Version 0.3.2 :

  \par
   In ExperimentSystem/ControllingLocalSearch/plans/DataCollection.hpp
   the following topics are handled:
    - Incomplete data collection

  \par
   In ExperimentSystem/ControllingLocalSearch/plans/Ubcsat-okl.hpp
   the following topics are handled:
    - Better output


  \par Version 0.3.3 :

  \par
   In ExperimentSystem/ControllingLocalSearch/plans/DataCollection.hpp
   the following topics are handled:
    - Better output of run_ubcsat
    - Bad columns produced by run_ubcsat


  \par Version 0.4

  \par
   In ExperimentSystem/ControllingLocalSearch/plans/DataCollection.hpp
   the following topics are handled:
    - Missing evaluation tools

  \par
   In ExperimentSystem/ControllingLocalSearch/plans/Evaluation.hpp
   the following topics are handled:
    - Evaluation tools for run_ubcsat


  \par Version 0.4.1

  \par
   In ExperimentSystem/ControllingLocalSearch/plans/DataCollection.hpp
   the following topics are handled:
    - Make run_ubcsat interruptible


  \par
   In ExperimentSystem/ControllingLocalSearch/plans/Ubcsat-okl.hpp
   the following topics are handled:
    - More readable large numbers


  \par Version 0.5

  \par
   In ExperimentSystem/ControllingLocalSearch/plans/AlgorithmSelection.hpp
   the following topics are handled:
    - Selecting the best algorithm


  \par
   In ExperimentSystem/ControllingLocalSearch/plans/ParameterTuning.hpp
   the following topics are handled:
    - Selecting the best parameters

  \par
   In ExperimentSystem/ControllingLocalSearch/plans/PointOfUnsatisfiability.hpp
   the following topics are handled:
    - Finding the n where a problem series changes from SAT to UNSAT
    - Simpler strategies


  \par Version 0.6

  \par
   In ExperimentSystem/ControllingLocalSearch/plans/general.hpp
   the following topics are handled:
    - Meta heuristics



  -------------------------------------------------------------------------------------------------------------------------------------

  \par Version history

   - 0.3   : 22.11.2010; initial version number (established tools for processing ubcsat-created data, running ubcsat and evaluating ubcsat-algorithms; plans for meta-heuristics).

*/
