// Oliver Kullmann, 27.5.2009 (Swansea)
/* Copyright 2009, 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ExperimentSystem/ControllingLocalSearch/plans/DataCollection.hpp
  \brief %Tools for getting data from local search algorithms

  Especially we consider running Ubcsat, while tools are often written in R.


  \todo run_ubcsat as shell script
  <ul>
   <li> run_ubcsat should not be an R-function, but a bash script. </li>
   <li> R is for statistical evaluation, bash for running programs. </li>
   <li> It might nevertheless be sensible, since we have run_ubcsat, to
   finish it first, and only then to replace it, in order to have a completed
   tool which can act as reference. </li>
   <li> On the other hand, writing the bash script should be straightforward,
   using our experience with writing such scripts. So run_ubcsat should not
   be further developed, but replaced by the bash script. </li>
   <li> There is somewhere a todo for this; we need to connect/update this.
   </li>
   <li> Of course, all todos below must be updated and applied to this new
   situation. </li>
   <li> The bash-script must have all (good) features currently run_ubcsat
   has; this includes the form of monitoring. </li>
   <li> The name could be "ExpRunUbcsat"; compare "Running experiments" in
   ExperimentSystem/SolverMonitoring/plans/general.hpp. </li>
   <li> Different from there however here we run many solvers on one instance;
   perhaps this should be reflected in the name. </li>
  </ul>


  \todo Input checking
  <ul>
   <li> Before running, run_ubcsat needs to check all parameter. </li>
   <li> For example the algorithm list needs to be checked, whether all
   names are correct. </li>
  </ul>


  \todo Bad output should not be incorporated into the dataframe
  <ul>
   <li> If some algorithm could not be evaluated, then no data should be
   collected into the dataframe. </li>
   <li> So that eval_ubcsat_dataframe runs correctly, just on the algorithms
   where it worked. </li>
   <li> This is currently not the case, when false algorithm-names were used.
   </li>
  </ul>


  \bug Incomplete data collection
  <ul>
   <li> run_ubcsat, as every such tool, must collect *all* the data available
   from ubcsat. </li>
   <li> This shall be achieved by using "fubcsat_okl"; see "Better output" in
   ExperimentSystem/ControllingLocalSearch/plans/Ubcsat-okl.hpp. </li>
   <li> Also the time needs to be recorded.
    <ol>
     <li> However, adding "time" to the list of parameters to be reported
     yields always "0.000000" ? </li>
     <li> Apparently this is for "rtd" only. </li>
     <li> So it seems not available? Ask on the Ubcsat mailing list. </li>
     <li> It seems, some form of runtime-info is only available through the
     statistics, namely "fps" (flips per second), and "totaltime". </li>
     <li> We wait for version 2.0. </li>
    </ol>
   </li>
  </ul>


  \todo Bad documentation of run_ubcsat
  <ul>
   <li> Every documentation must be concise and to the point. </li>
   <li> Instead here we find pages of irrelevant data. </li>
  </ul>


  \todo Better output of run_ubcsat
  <ul>
   <li> A paramater-file is needed, so that the experiment can be reproduced.
   </li>
   <li> DONE
   So a summary for each (single) run should be printed out.
    <ol>
     <li> DONE We need the tabulation of the min-values. </li>
     <li> DONE
     Perhaps it's better not to have an empty line between the playback
     of the command and the min-tabulation. </li>
     <li> DONE
     Since there are big differences in running times, we also need
     timing information. </li>
    </ol>
   </li>
   <li> The result-files should be directly readable by read_ubcsat, and thus
   they should include the leading row with the column names. </li>
   <li> DONE (a warning is shown for any errors for each algorithm now
   and a summary of all algorithms with warnings is given at the end)
   Segmentation faults should be very visible (currently they aren't).
   And they should be summarised at the end of all runs. </li>
   <li> DONE The directory should have a timestamp; compare RunVdW3k. </li>
   <li> DONE (now prints algorithm name and summary of min column)
   An obvious problem with run_ubcsat is that it doesn't give intermediate
   results: runs in general take quite a time, and one needs to wait until
   the end. </li>
  </ul>


  \todo Add new-ubcsat-okl as an option for run_ubcsat
  <ul>
   <li> DONE (the new version is now the default)
   We will likely need to run experiments using new-ubcsat-okl
   before ubcsat-1-2-0 is released, but ubcsat-okl segfaults in 
   various ways using weighted algorithms. </li>
   <li> DONE (new version is now the default)
   As we need weighted algorithms, for example in the case of
   minimising CNF representations (see 
   Investigations/Cryptography/AdvancedEncryptionStandard/plans/Representations/BoxMinimisation.hpp)
   we need an option to specify that we wish to use "new-ubcsat-okl"
   instead of "ubcsat-okl". </li>
   <li> We also need to ensure that all the weighted
   versions of each algorithm are listed in the algorithms list, as well as
   new versions. </li>
   <li> DONE (use ubcsat_wrapper = "old-ubcsat-okl")
   For some time we still want to be able to use (conveniently) version
   1-0-0. </li>
  </ul>


  \todo Make run_ubcsat interruptible
  <ul>
   <li> Since it takes a long time to finish a computation, it should be
   possible to stop the current computation and just use the results obtained
   so far. </li>
   <li> No documentation exists on this issue: is this already possible? Or are
   certain clean-up steps required? </li>
   <li> It would also be needed to be able to complete a computation later:
    <ol>
     <li> First the currently processed algorithm needs to be represented in
     a file, so that a continuation just can pick up where the computation was
     aborted. </li>
     <li> Perhaps manual deletion of the files related to the currently
     processed algorithm is needed: For that it must be clear which files
     are these. </li>
    </ol>
   </li>
   <li> Perhaps for these things we wait for the new version 2.0 of ubcsat,
   since then our ubcsat-tools need to be rewritten anyway, and at this time
   then perhaps the pure running-experiments-functionality of run_ubcsat is
   handled by a shell-script. </li>
  </ul>


  \bug Bad columns produced by run_ubcsat
  <ul>
   <li> "Clauses", "Variables" and other constant measures should not show up
   in such dataframes. </li>
   <li> For the data which is independent of the algorithm, a second dataframe
   should be returned. </li>
   <li> These names are also inappropriate (see the general standards in the
   library). </li>
   <li> Actually, three dataframes are needed:
    <ol>
     <li> One with data regarding only the instance. </li>
     <li> One with statistics regarding the algorithms (not the runs), e.g.,
     fps (flips per second). </li>
     <li> One containing all run-information. </li>
    </ol>
   </li>
   <li> DONE (see "Incomplete data collection")
   There is no need to have more or less of these parameters --- we need
   them all, in suitable packaging! </li>
   <li> DONE
   The point of ubcsat_eval and such tools must be to give convenient
   access to *all* the data. </li>
   <li> How to represent algorithms:
    <ol>
     <li> And what is the type of the algorithm-column? Shouldn't it be a
     *factor*, with values given by strings? In any case, its use must be
     documented. </li>
     <li> Access to the factor levels should be possible through the variable
     run_ubcsat_cnf_algs, however this is not possible. </li>
     <li> The "nature" of dataframes needs to be investigated. </li>
     <li> At present the "algs" column is a factor, even those the algorithm
     is given to the dataframe as a string. This is due to the 
     "stringsAsFactors" of the data.frame constructor. </li>
     <li> By default any string columns in a data.frame are converted to 
     factors. To keep these strings as strings, we should set 
     "stringsAsFactors" to FALSE. </li>
    </ol>
   </li>
   <li> DONE
   The column-names should be identical to the names used by ubcsat (in
   the output!). </li>
   <li> So "found -> sat", "best -> min", "beststep -> osteps", "steps ->
   msteps". And references to these columns must be replaced in all files
   (typically these references use e.g. E$best). </li>
  </ul>


  \bug Missing evaluation tools
  <ul>
   <li> When producing a function like run_ubcsat, then it is a must to
   produce at the same time basic evaluation tools for the dataframe. </li>
   <li> See "Evaluation tools for run-ubcsat" in
   ExperimentSystem/ControllingLocalSearch/plans/Evaluation.hpp. </li>
  </ul>


  \todo Parallelisation
  <ul>
   <li> There should be an additional parameter "threads", with default-value
   1, which specifies the number of threads used. </li>
   <li> The runs of the algorithms are then simply distributed over the
   spreads. </li>
   <li> This should be a fairly easy task. </li>
   <li> And it would be useful, since the running time can be substantial,
   while one wants to the results as quick as possible, to start the real
   experiments. </li>
  </ul>

*/

