// Oliver Kullmann, 27.5.2009 (Swansea)
/* Copyright 2009, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ExperimentSystem/ControllingLocalSearch/plans/DataCollection.hpp
  \brief %Tools for getting data from local search algorithms

  Especially we consider running Ubcsat, while tools are often written in R.


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


  \bug Specification of run_ubcsat is badly written
  <ul>
   <li> The specification doesn't specify much. </li>
   <li> DONE Commas are needed (and semicolons, and full-stops). </li>
   <li> (DONE See Evaluation.R comments) 
   At its most basic the run_ubcsat function can be run on
   a given DIMACS file ("test.cnf" in this case), by running:
   \verbatim
df = run_ubcsat("test.cnf")
   \endverbatim
   </li>
   <li> DONE Many nonsensical uses of quotation marks. </li>
   <li> DONE Also "should" is mis-used (since this likely is not an option for
   the function!). </li>
   <li> DONE What is "and returns a new adds data frame" ?? </li>
   <li> (DONE These are defaults only, and are taken
   as optional arguments for run_ubcsat) Are environment variables to be used
   by the user?? (Obviously this shouldn't be the case.) </li>
   <li> DONE Often a (non-sensical) "will be" is used (does this refer to 
   later extensions?) ?? </li>
   <li> DONE How to use this function? </li>
  </ul>


  \todo Better output of run_ubcsat
  <ul>
   <li> So a summary for each (single) run should be printed out. </li>
   <li> Segmentation faults should be very visible (currently they aren't).
   And they should be summarised at the end of all runs. </li>
   <li> The directory should have a timestamp; compare RunVdW3k. </li>
   <li> The result-files should be directly readable by read_ubcsat, and thus
   they should include the leading row with the column names. </li>
   <li> DONE (now prints algorithm name and summary of min column)
   An obvious problem with run_ubcsat is that it doesn't give intermediate
   results: runs in general take quite a time, and one needs to wait until
   the end. </li>
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
   <li> Actually, three dataframes are needed:
    <ol>
     <li> One with data regarding only the instance. </li>
     <li> One with statistics regarding the algorithms (not the runs), e.g.,
     flips-per-second. </li>
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


  \bug DONE Bad documentation for run_ubcsat
  <ul>
   <li> DONE
   Nowhere are the created files mentioned? One needs the 
   specification of their names and their contents. </li>
   <li> DONE (in context "standard output files" was said; changed to
   "default output files" and added additional explanation to avoid
   confusion)
   Reference to "standard output" is wrong here, since the output appears
   in the R-terminal. It is also not explained what that output is. </li>
   <li> DONE (specified data types and referenced ubcsat documentation)
   The attributes of the resulting dataframe need to be specified 
   more precisely; what are their data types? </li>
   <li> DONE (added the seed)
   When examples are given, then they need to be reproducible (as 
   much as possible). </li>
   <li> DONE (run_ubcsat function is now at top)
   The organisation of DataCollection.R is user-unfriendly:
    <ol>
     <li> The main function needs to stand out. </li>
     <li> The helper functions should be in the "background". </li>
    </ol>
   </li>
   <li> DONE A cutoff=1 is nonsense. </li>
   <li> DONE These files must also not pollute the user-directory; so they 
   should all be placed in some created directory. </li>
   <li> DONE And obviously, it needs to be possible just to evaluate the files,
   without generating them --- so *two* functions are needed. </li>
   <li> DONE The examples don't show the important step that the computed 
   dataframe MUST BE STORED. </li>
  </ul>


  \bug DONE run_ubcsat can't handle segmentation faults
  <ul>
   <li> DONE (wait for ubcsat 1.2.0 for fix for the segfault issues)
   The given example in DataCollection.R produces segmentation faults
   on csltok for saps and rsaps. </li>
   <li> DONE (now the dataframe is created; partially created files will
   be used, but if the summary-statistics-file can't be read, then the data
   for this algorithm will be ignored)
   The other algorithms are still evaluated, but then an error occurs,
   and no dataframe is created. </li>
   <li> DONE (data will be ignored except in cases a readable
   summary-statistics-file was created)
   The dataframe should likely just not contain anything on these two
   algorithms (while otherwise being usable). </li>
   <li> DONE (now outputs a warning for each algorithm and a list
   at the end)   
   And warnings should be issued on the faulty algorithms.
    <ul>
     <li> Algorithms with error are stored in a list. </li>
     <li> For the running output we should get clear messages. </li>
     <li> And also a final statement, that there have been errors, and which
     algorithms were concerned. </li>
    </ul>
   </li> 
  </ul>


  \bug DONE (Issue with eval_ubcsat_dataframe went away; only new abbreviated 
  algorithm names are now used) 
  False algorithm names
  <ul>
   <li> DONE (the names appeared in the alg-column again)
   The algorithm-names were always defect, but now they become completely
   unusable: the table contains now not the strings, but something else,
   where one doesn't have access to! </li>
   <li> DONE
   Accordingly eval_ubcsat_dataframe fails! </li>
   <li> DONE (the tags in run_ubcsat_cnf_algs can be treated as "real thing")
   As discussed many times, the nonsensic doubling of algorithm-names
   must be eliminated. </li>
   <li> DONE As discussed, just strings are to be used (throughout) to 
   identify algorithms, and a rational system of either only 3-letter or only 
   4-letter words needs to be developed.
    <ol>
     <li> "gsat" : gsat </li>
     <li> "gsat -v simple" : gsats </li>
     <li> "gwsat" : gwsat </li>
     <li> "gsat-tabu" : gsatt </li>
     <li> "hsat" : hsat </li>
     <li> "hwsat" : hwsat </li>
     <li> "walksat" : wsat </li>
     <li> "walksat-tabu" : wsatt </li>
     <li> "novelty" : nov </li>
     <li> "novelty+" : novp </li>
     <li> "adaptnovelty+" : anovp </li>
     <li> "rnovelty" : rnov </li>
     <li> "rnovelty+" : rnovp </li>
     <li> "saps" : saps </li>
     <li> "rsaps" : rsaps </li>
     <li> "sapsnr" : sapsnr </li>
     <li> "rots" : rots </li>
     <li> "irots" : irots </li>
     <li> "samd" : samd </li>
     <li> "walksat-tabu -v nonull" : wsattn </li>
    </ol>
   </li>
  </ul>

*/

