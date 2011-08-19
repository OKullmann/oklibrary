// Oliver Kullmann, 12.5.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Interfaces/DistributedSolving/plans/general.hpp
  \brief Plans regarding distributed solving of SAT problems

  See ExperimentSystem/plans/DistributedSolving.hpp for tools/plans which
  are mostly related to managing different machines and/or different
  groups.


  \todo Create milestones


  \todo Connections
  <ul>
   <li> See Solvers/OKsolver/SAT2002/plans/SimpleParallelisation.hpp for the
   currently only splitting tool (SplittingViaOKsolver). </li>
   <li> See ExperimentSystem/plans/DistributedSolving.hpp for general plans
   on distributed solving. </li>
   <li> See "Launching and monitoring" in ExperimentSystem/plans/general.hpp
   for general plans on experimentation on remote machines. </li>
   <li> See
   Investigations/RamseyTheory/VanderWaerdenProblems/plans/Parallelisation/general.hpp
   for solving hard van-der-Waerden problems, as an application. </li>
  </ul>


  \todo Sampling tool
  <ul>
   <li> A tool "SamplingSplitting" is to be written, for sampling N
   splitting-instances from each batch of equally-sized partial assignments
   (which yield the splitting-instances). </li>
   <li> Arguments "SamplingSplitting data-file instance-dir N CNF-file solver":
    <ol>
     <li> instance-dir contains the partial assignments in files whose
     names are just natural numbers, as given in data-file. </li>
     <li> N is the sample size. </li>
     <li> N samples for each occurring size of the partial assignment. </li>
     <li> If not N sub-instances (i.e., partial assignments) are available,
     then all are considered. </li>
    </ol>
   </li>
   <li> Creates, as usual, an experiment-directory:
    <ol>
     <li> Containing files i_OUT in sub-directory "Runs" for the solver output
     on sub-instance (partial assignment) i. </li>
     <li> And a file "Statistics" with the extracted data in R-format. </li>
     <li> Plus a file "Parameter" specifying all the parameters of the run.
     </li>
     <li> There is the problem that there is no information on the original
     splitting. This could be solver if parameters data-file and instance-dir
     would be replaced by the splitting-directory --- the name of this
     directory shows then the original splitting-situation. </li>
     <li> The instance CNF-file might then also be inferred automatically.
     </li>
     <li> DONE
     For that the splitting directory should contain a file "F", which
     contains the name of the (original) CNF-file (as provided in the
     splitting-directory, but without a fixed name). </li>
     <li> DONE
     Or perhaps "F" should be a symbolic link? Actually, we also need
     the name, and then it should be provided. </li>
    </ol>
   </li>
  </ul>

*/

