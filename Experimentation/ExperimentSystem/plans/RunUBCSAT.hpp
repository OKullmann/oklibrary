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
   <li> For an application-example see
   Experimentation/Investigations/plans/RamseyProblems.hpp. </li>
   <li> And see Experimentation/Investigations/plans/VanderWaerdenProblems.hpp
   for further examples. </li>
   <li> We have already "ubcsat-okl". </li>
   <li> A single execution of Ubcsat shall only return the data for the
   single runs. </li>
   <li> So for example
   \verbatim
ubcsat -r out stdout run,found,best,beststep,steps -rclean -r stats null -runs 3 -cutoff 1000 -alg rsaps -i Ramsey_5_2_30.cnf
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
   <li> Also the time needs to be recorded.
    <ol>
     <li> However, adding "time" to the list of parameters to be reported
     yields always "0.000000" ? </li>
     <li> Apparently this is for "rtd" only. </li>
     <li> So it seems not available? Ask on the Ubcsat mailing list. </li>
     <li> It seems, some form of runtime-info is only available through the
     statistics, namely "fps" (flips per second), and "totaltime". </li>
    </ol>
   </li>
   <li> Other parameter to report:
    <ol>
     <li> As a kind of "hidden" parameter we should also record the seed. </li>
     <li> From the parameters reported by "ubcsat -hc" we can use
     \verbatim
run, found, steps, best, beststep, seed, nullflips, percentnull, rand,
candidates, agemean
     \endverbatim
     </li>
     <li> Of course, the data frames use these names. </li>
     <li> Perhaps we simply always report all data, and it is then up to
     the evaluation functions to choose. </li>
     <li> Some data is only available via the statistics (see "ubcsat -hs"),
     namely numclauses, numvars, numlits, and, as mentioned above, timing
     information. </li>
     <li> So perhaps
     \verbatim
ubcsat -rclean \
 -r out stdout run,found,best,beststep,steps,seed,nullflips,percentnull,rand,candidates,agemean \
 -r stats stdout numclauses,numvars,numlits,fps,totaltime \
 -runs 3 -cutoff 1000 -alg rsaps -i Ramsey_5_2_30.cnf
     \endverbatim
     is then the most informative output form. </li>
     <li> Should the statistics output go to a file (using "-r stats filename"
     instead of "-r stats stdout"), so that parsing is easier? </li>
    </ol>
   </li>
   <li> How to name data frames, and how to keep a collection of them,
   for different problem instances?
    <ol>
     <li> Main parameters for a data frame are the name of the instance,
     and cutoff. </li>
     <li> Perhaps one needs to envisage always a series, with external
     parameters determining the instance and a series name, according to
     which at least the data frames are consistently named. </li>
     <li> But we need to have "collection of data frames"? </li>
     <li> So perhaps actually the instance name is just an entry
     in the data frame, and perhaps also the main parameters. </li>
     <li> So for one series we just have one data frame; for example
     for %Ramsey formulas the parameters are s (the number of colours;
     for boolean problems s=2), [q_1,...,q_s] for the sizes of monochromatic
     subsets, r for the size of the hyperedges (for graph problems r=2),
     and n, the number of vertices. </li>
     <li> All in one data frame, the "database", and the evaluation
     functions need to sort out the right data. </li>
     <li> One needs to separate the "instance specifiers" (in this example
     s, [q_1,...,q_s], r), the "instance parameters" (number of variables,
     clauses etc.), and the "instance data". </li>
    </ol>
   </li>
  </ul>


  \todo Evaluating the data frames
  <ul>
   <li> Functions are needed for standard evaluations. </li>
   <li> Given a file Data produced by ubcsat-okl (with first line naming
   the column headers), one could create a nicer formatting by e.g.
   \verbatim
awk 'NR == 1 {printf("%8s %8s %8s %8s %11s\n", $1,$2,$3,$4,$5)} NR != 1 && NF > 0 {printf("%5u %2u %8u %8u %8u %11u\n", $1,$2,$3,$4,$5,$6)}' Data
   \endverbatim
   producing e.g.
   \verbatim
     sat      min   osteps   msteps        seed
    1  0       13     8525    20000  1352592974
    2  0       16     5984    20000  3521402437

 9999  0       15     4420    20000  4085996178
10000  0       12     5802    20000  3176480652
   \endverbatim
   However this is not needed. </li>
   <li> Reading into R by
   \verbatim
> E = read.table("Data", colClasses = c("character", "factor", "integer", "integer", "integer", "character"))
   \endverbatim
   and simple evaluation by "summary(E)" or, better,
   \verbatim
> summary(E[,c("sat","min","osteps")])
   \endverbatim
   </li>
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


  \todo Selecting the best algorithm
  <ul>
   <li> According to my experience (OK), for every (narrow) example
   class (like %Ramsey formulas) there is one best algorithm in the
   Ubcsat selection, which is also relatively easy recognisable. </li>
   <li> First we need to develop a method, and then we should try to
   automatise it. </li>
   <li> The starting point is to find out on a single instance which
   algorithm performs best. </li>
   <li> Then we consider three single instances, an "easy" one, a
   "middle easy" and a "harder" one, and see whether the predictions
   scale, where at least for the evaluation of our method we consider
   all algorithms. </li>
  </ul>


  \todo Selecting the best parameters
  <ul>
   <li> Given the a small set of algorithms, tuning of the UBCSAT parameters
   such as "cutoff", "runs" etc might yield better results for certain
   problem classes. </li>
   <li> As in "Selecting the best algorithm", testing parameters by hand first
   is best and then trying to automatise the process. </li>
   <li> Later on, with further automatisation, a slightly more dynamic 
   experiment system would be useful, where conditions may be used which
   dictate where more experiments are run (i.e certain algorithms and
   parameter changes are weeded out as they do not perform well, and 
   others are given more experiment time to provide finer grained 
   differentiation). </li>
  </ul>


  \todo Meta heuristics
  <ul>
   <li> See "Faster local search" in
   Experimentation/Investigations/plans/VanderWaerdenProblems.hpp for first
   plans. </li>
  </ul>

*/

