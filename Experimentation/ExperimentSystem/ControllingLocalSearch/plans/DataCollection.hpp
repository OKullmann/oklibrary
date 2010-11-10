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


  \bug Bad parameter of run_ubcsat
  <ul>
   <li> It needs to be specified what are the defaults. </li>
   <li> A *concept* for the input handling is needed, not just a hack. </li>
   <li> DONE Collecting the parameters in a list is inappropriate, but just 
   the standard R-handling should be used, so that for example one can just
   specify "cutoff=1000000" as additional parameter, without, of course,
   changing anything else --- currently parameter "runs" is overwritten in
   this case. </li>
  </ul>


  \todo Handling of temporary files
  <ul>
   <li> Nowhere are the created files mentioned?? One needs the 
   specification of their names and their contents. And it should be possible 
   to disable them (likely this should be the default); that is, if these 
   files are needed, then by default they should be "temporary" files. </li>
   <li> The temporary files are current placed in a directory created by 
   run_ubcsat, specified by the tmp_directory parameter, which defaults
   to run_ubcsat_temp_dir, taking the filename (not path) as an argument. 
   </li>
   <li> The directory is not currently deleted after run_ubcsat is complete,
   allowing the data to be reread again using read_ubcsat_dir, or for
   the user to evaluate the statistics and log files. </li>
  </ul>


  \bug Bad columns produced by run_ubcsat
  <ul>
   <li> "Clauses", "Variables" and other constant measures should not show up
   in such dataframes. (MG: This could be done by just adding a field to the 
   dataframe.) </li>
   <li> For the data which is constant per algorithm, a second dataframe
   should be returned. </li>
   <li> There is no need to have more or less of these parameters --- we need
   them all, in suitable packaging! </li>
   <li> The point of ubcsat_eval and such tools must be to give convenient
   access to *all* the data. </li>
   <li> And what is the type of the algorithm-column? Shouldn't it be a
   *factor*, with values given by strings? In any case, its use must be
   documented. </li>
   <li> Access to the factor levels should be possible through the variable
   run_ubcsat_cnf_algs, however this is not possible. </li>
   <li> It seems that MG didn't understand the nature of dataframes. </li>
   <li> DONE The column-names should be identical to the names used by ubcsat (in
   the output!). </li>
   <li> So "found -> sat", "best -> min", "beststep -> osteps", "steps ->
   msteps". And references to these columns must be replaced in all files
   (typically these references use e.g. E$best). </li>
  </ul>


  \bug Missing evaluation tools
  <ul>
   <li> When producing a function like run_ubcsat, then it is a must to
   produce at the same time basic evaluation tools for the dataframe. </li>
   <li> Given the dataframe in E, the best algorithms seems best graphed
   by
   \verbatim
plot(E$alg,E$best)
   \endverbatim
   which plots per algorithm the boxplots of the min-distribution. </li>
   <li> One only needs to make sure that on the x-axis all algorithms are
   listed. </li>
   <li> Sorting the algorithms
    <ol>
     <li> Considering single algorithms by e.g.
     \verbatim
> table(E$best[E$alg=="adaptnoveltyp"])
     \endverbatim
     (note that currently algorithm names are inappropriately handled). </li>
     <li> These tables can be put into a linear order by sorting first
     according to min-value reached (the lower the better), and second by
     count obtained (the higher the better). </li>
     <li> A function should be written which prints out the
     sorted tables in a nice way. </li>
     <li> As a first attempt we have eval_ubcsat_dataframe, which just shows
     all results in table form. </li>
     <li> Perhaps then the (first) evaluation tool just uses
     plot(E$alg,E$best), followed by printing those sorted tables. </li>
    </ol>
   </li>
  </ul>


  \todo Collecting data
  <ul>
   <li> Steps to be taken:
    <ol>
     <li> DONE A function "run_ubcsat" is to be written which runs Ubcsat for
     a list of algorithms on a specific instance, and computes a data frame.
     </li>
     <li> DONE This has now been basically achieved, and the functions written
     have to be made available. </li>
     <li> The function run_ubcsat is now available in 
     Experimentation/ExperimentSystem/ControllingLocalSearch/Evaluation.R. 
     </li>
     <li> After that, the whole todo needs to be completely updated, so that
     only future plans remains. </li>
    </ol>
   </li>
   <li> For an application-example see
   Experimentation/Investigations/plans/RamseyProblems.hpp. </li>
   <li> And see Experimentation/Investigations/plans/VanderWaerdenProblems.hpp
   for further examples. </li>
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
   <li> DONE We have already "ubcsat-okl".
    <ol>
     <li> See "ubcsat-okl" in
     Buildsystem/ExternalSources/SpecialBuilds/plans/Ubcsat.hpp. </li>
     <li> A single execution of Ubcsat shall return the data for the
     single runs, in a form which can directly be read by R (supplying
     directly all data). </li>
     <li> So for example
     \verbatim
ubcsat -r out stdout run,found,best,beststep,steps -rclean -r stats null -runs 3 -cutoff 1000 -alg rsaps -i Ramsey_5_2_30.cnf
     \endverbatim
     is the right form (we then know how to interprete the five columns). </li>
     <li> See the current ubcsat-okl (which needs to be extended). </li>
     <li> And see "Evaluating the data frames" for some functionality which
     might be integrated into ubcsat-okl. </li>
    </ol>
   </li>
   <li> DONE One parameter of run_ubcsat is the list of algorithms, which are
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
   <li> DONE Another parameter is "runs"; default 100. </li>
   <li> DONE And default value for parameter "cutoff" is 10000. </li>
   <li> DONE Parameter "filename" must be given. </li>
   <li> DONE Optionally the results of ubcsat are appended to a file. </li>
   <li> DONE There are some other parameters (like "-tabu 10"), which one
   can optionally append. </li>
   <li> DONE Also the time needs to be recorded.
    <ol>
     <li> However, adding "time" to the list of parameters to be reported
     yields always "0.000000" ? </li>
     <li> Apparently this is for "rtd" only. </li>
     <li> So it seems not available? Ask on the Ubcsat mailing list. </li>
     <li> It seems, some form of runtime-info is only available through the
     statistics, namely "fps" (flips per second), and "totaltime". </li>
    </ol>
   </li>
   <li> DONE Other parameter to report:
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
   <li> (DONE Moved into R-subsystem) Something like the following seems 
   reasonable: (as discussed, one needs to specify what this should achieve,
   and furthermore, all text-formatting etc. should be handled by the wrapper
   ubcsat-okl):
   \verbatim
ubcsat_command <- "ubcsat -rclean \\
 -r out '$OUTPUT' $OUTPUT_PARAMS \\
  -r stats '$STATS_OUTPUT'  numclauses,numvars,numlits,fps,totaltime,time \\
   $STD_PARAMS -alg $ALG -i $TARGET"

ubcsat_output_params = list("run","found","best","beststep","steps","seed",
  "nullflips","percentnull","rand","candidates","agemean")
ubcsat_cnf_algs = list("gsat", "gsat -v simple", "gwsat", "gsat-tabu", "hsat", 
  "hwsat","walksat", "walksat-tabu", "novelty", "novelty+", "adaptnovelty+",
  "rnovelty", "rnovelty+", "saps", "rsaps", "sapsnr", "rots",
  "irots", "samd", "walksat-tabu -v nonull")
ubcsat_std_params = list(runs=100,cutoff=10000)

add_constant_column <- function(df,const_var, name) {
  temp_df <- data.frame(do.call(c,lapply(df[[1]],function(a){const_var})))
  colnames(temp_df) <- c(name)
  data.frame(df, temp_df)
}

eval_ubcsat <- 
function(input, output="$TARGET-$ALG.result", command=ubcsat_command,
  algs=ubcsat_cnf_algs, output_params=ubcsat_output_params, 
  params = ubcsat_std_params,stats_output="$TARGET-$ALG.stats.result",
  monitor=FALSE) {

  ubcsat_command <- gsub("\\$TARGET", input, command)
  ubcsat_command <- gsub("\\$OUTPUT_PARAMS", 
    do.call(paste,c(output_params,list(sep=","))), ubcsat_command)
  std_params <- ""
  for (param_name in names(params)) {
    std_params <- paste(std_params," -",param_name, " ",
      format(params[[param_name]],scientific=5000),sep="")
  }
  ubcsat_command <- gsub("\\$STD_PARAMS", std_params, ubcsat_command)
  output_file <- gsub("\\$TARGET", input, output)
  stats_output_file <- gsub("\\$TARGET", input, stats_output)
  for (alg in algs) {
    output_file_t <- gsub("\\$ALG", gsub(" ","",alg), output_file)
    stats_output_file_t <- gsub("\\$ALG", gsub(" ","",alg), stats_output_file)
    ubcsat_command_t <- gsub("\\$ALG", alg, ubcsat_command)
    ubcsat_command_t <- gsub("\\$OUTPUT", output_file_t, ubcsat_command_t)
    ubcsat_command_t <- 
      gsub("\\$STATS_OUTPUT", stats_output_file_t, ubcsat_command_t)
    if (monitor) print(ubcsat_command_t)
    system(ubcsat_command_t)
    result_df <- read.table(output_file_t,col.names=as.vector(output_params))
    result_df <- add_constant_column(result_df,alg, "alg")
    stats_df <- read.table(stats_output_file_t,
      colClasses=c("character","character","real"))
    for (i in 1:length(stats_df[[1]])) {
      result_df <- add_constant_column(result_df, 
        stats_df[[3]][[i]], stats_df[[1]][[i]])
    }
    if (exists("eval_ubcsat_df")) { 
      eval_ubcsat_df <- rbind(eval_ubcsat_df, result_df) 
    } else {
      eval_ubcsat_df <- result_df
    }
  }
  eval_ubcsat_df
}
   \endverbatim
   </li>
   <li> DONE The eval_ubcsat function runs each of the listed 
   algorithms, with the given parameters etc on a single cnf returning a 
   combined dataframe
   with each of the relevant fields. </li>
   <li> (DONE Fixed to 5000)
   The "format" function above with parameter "scientific" is used to 
   ensure any integer parameters are printed in decimal, not scientific 
   notation. For instance 100000 is usually printed 1e+05 as this is 5 
   characters which is shorter than 100000 (6 characters). The "scientific"
   parameters to "format" adds a bias so 5 + 5000 > 6. </li>
   <li> DONE For the "scientific" parameter, perhaps ".Machine$integer.max/2"
   is a better option? Just ".Machine$integer.max" obviously results in
   overflow and so doesn't work. </li>
  </ul>


  \bug Bad documentation for run_ubcsat
  <ul>
   <li> When examples are given, then they need to be reproducible (as 
   much as possible). </li>
   <li> DONE A cutoff=1 is nonsense. </li>
   <li> DONE The attributes of the resulting dataframe need to be specified 
   more precisely; what are their data types? </li>
   <li> DONE
   Reference to "standard output" is wrong here, since the output appears
   in the R-terminal. It is also not explained what that output is. </li>
   <li> DONE These files must also not pollute the user-directory; so they 
   should all be placed in some created directory. </li>
   <li> DONE And obviously, it needs to be possible just to evaluate the files,
   without generating them --- so *two* functions are needed. </li>
   <li> DONE The examples don't show the important step that the computed 
   dataframe MUST BE STORED. </li>
  </ul>


  \bug DONE Dangerious handling of filenames
  <ul>
   <li> (DONE Added temporary directory option)
   Obviously correct handling of the output-directory, which is 
   created, of course, from scratch and in the current directory, is needed. 
   </li>
   <li> DONE Currently the input-path is just copied, which is false and 
   dangerous, since the output-files as a result will be placed in the same 
   directory where this input-file is located. </li>
  </ul>


  \todo Evaluating the data frames
  <ul>
   <li> Functions are needed for standard evaluations.
    <ol>
     <li> At the R-level, we go for convenience, and overloading functions
     like "summary" or "plot" seems be the right way. </li>
     <li> One needs to find out how to do this, how to recognise our
     specific data frames. </li>
     <li> "summary" should show data, while "plot" might run through a
     serious of plots, showing the data from different angles. </li>
    </ol>
   </li>
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
   However this should be part of the extended ubcsat-okl (see above). </li>
   <li> DONE (function read_ubcsat does this)
   Reading into R by
   \verbatim
> E = read.table("Data", colClasses = c("character", "factor", "integer", "integer", "integer", "character"))
   \endverbatim
   and simple evaluation by "summary(E)" or, better,
   \verbatim
> summary(E[,c("sat","min","osteps")])
   \endverbatim
   Also the simple
   \verbatim
> table(E$min)
   \endverbatim
   is interesting (perhaps actually most interesting(!); may be plotted by
   "plot(table(E$min))"). </li>
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


  \bug (DONE Moved to DataCollection.R) Misplaced run_ubcsat
  <ul>
   <li> run_ubcsat is misplaced in Evaluation.R. </li>
   <li> See ExperimentSystem/ControllingLocalSearch/plans/Evaluation.hpp.
   </li>
   <li> After moving this function (and accompanying code), the todos below
   needs to be updated. </li>
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

*/

