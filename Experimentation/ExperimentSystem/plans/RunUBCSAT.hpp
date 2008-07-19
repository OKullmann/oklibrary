// Oliver Kullmann, 19.7.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ExperimentSystem/plans/RunUBCSAT.hpp
  \brief %Tools for running and evaluating algorithms from Ubcsat

  %Tools written in R.


  \todo Collecting data
  <ul>
   <li> A function "eval_ubcsat" is to be written which runs Ubcsat for a list
   of algorithms on a specific instance, and computes a data frame. </li>
   <li> A single execution of Ubcsat only returns the data for the
   single runs. </li>
   <li> So for example
   \verbatim
ubcsat -r out stdout run,found,best,beststep,steps -rclean -r stats /dev/null -runs 3 -cutoff 1000 -alg rsaps -i Ramsey_5_2_30.cnf
   \endverbatim
   is the right form (we then know how to interprete the five columns). </li>
   <li> One parameter of eval_ubcsat is the list of algorithms, which are
   strings using the Ubcsat abbreviations.
    <ol>
     <li> Default is all cnf-algorithms. </li>
     <li> I.e.,
     \verbatim
"gsat", "gsat -v simple", "gwsat", "gsat-tabu", "hsat", "hwsat",
"walksat", "walksat-tabu", "novelty", "novelty+", "adaptnovelty+",
"rnovelty", "rnovelty+", "saps", "rsaps", "sapsnr", "rots",
"irots", "samd", "walksat-tabu -v nonull"
     \endverbatim
     for SAT algorithm. </li>
     <li> And
     \verbatim
"gsat -w", "gwsat -w", "gsat-tabu -w", "hsat -w", "hwsat -w",
"walksat -w", "walksat-tabu -w", "novelty -w", "novelty+ -w",
"adaptnovelty+ -w", "rots -w", "irots -w", "samd -w"
     \endverbatim
     for weighted MAXSAT. </li>
    </ol>
   </li>
   <li> Another parameter is "runs"; default 100. </li>
   <li> And default value for parameter "cutoff" is 1000. </li>
   <li> Parameter "filename" must be given. </li>
   <li> Optionally the results of ubcsat are appended to a file. </li>
   <li> There are some other parameters (like "-tabu 10"), which one
   can optionally append. </li>
  </ul>


  \todo Evaluating the data frames
  <ul>
   <li> Functions are needed for standard evaluations. </li>
   <li> Sort the data frame by
    <ol>
     <li> first smallest average number of falsified clauses, </li>
     <li> then smallest average number of (total) steps. </li>
    </ol>
   </li>
   <li> Also the average number of steps to the optimum is relevant:
   When it is closer to the cutoff, then increasing the cutoff likely
   might yield an improvement, while if not then the algorithm wanders
   randomly around without achieving something (so increasing the cutoff
   seems of little value). </li>
  </ul>

*/

