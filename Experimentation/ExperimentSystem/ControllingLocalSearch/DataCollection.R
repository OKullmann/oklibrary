# Matthew Gwynne, 20.10.2010 (Swansea)
# Copyright 2010 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

# ##################
# # Running Ubcsat #
# ##################

# XXX WHAT IS THE MEANING OF THIS? XXX
# Default list of output parameters which ubcsat outputs.
# This default list is that used by ubcsat-okl.
# Use the output_params parameter of eval_ubcsat to specify
# a different list of output parameters (ubcsat output parameters).
eval_ubcsat_output_params = list(run="run",found="sat",best="min",
  beststep="osteps",steps="msteps",seed="seed")

# Default list of ubcsat algorithms eval_ubcsat evaluates, given
# as a named list, where the name of each item is a reference for the
# algorithm, and the value of each item is the algorithm parameter
# as given to ubcsat.
eval_ubcsat_cnf_algs = list(
   gsat="gsat", 
   gsat_simple="gsat -v simple",
   gwsat="gwsat", 
   gsat_tabu="gsat-tabu",
   hsat="hsat",
   hwsat="hwsat",
   walksat="walksat",
   walksat_tabu="walksat-tabu", 
   novelty="novelty",
   noveltyp="novelty+",
   adaptnoveltyp="adaptnovelty+",
   rnovelty="rnovelty",
   rnoveltyp="rnovelty+",
   saps="saps",
   rsaps="rsaps",
   sapsnr="sapsnr",
   rots="rots",
   irots="irots",
   samd="samd",
   walksat_tabu_nonull="walksat-tabu -v nonull"
)

# XXX WHAT IS THE MEANING OF THIS? XXX
# Default labelled list "arg=val" of argument-value pairs of arguments to the
# ubcsat command. 
eval_ubcsat_std_params = list(runs=100,cutoff=100000)

# Takes a dataframe, a constant value C and returns a new data frame with
# an additional column where that column has value C for every row in the
# data.frame.
#
# MG: This helper function should be moved to a more appropriate location.
add_constant_column = function(df,const_var, name) {
  temp_df = data.frame(do.call(c,lapply(df[[1]],function(a){const_var})))
  colnames(temp_df) = c(name)
  data.frame(df, temp_df)
}

# Runs a selection of ubcsat algorithms on the given input file, via
# ubcsat-okl, and returns the results of these runs as a data-frame.
#
# Parameters:
#     input
#       The path to the file to evaluate the ubcsat algorithms on.
#     algs
#       A list of ubcsat-algorithms to evaluate on the given input file,
#       where the name of each item is a reference for the
#       algorithm, and the value of each item is the algorithm parameter
#       as given to ubcsat.
#     output_params
#       A list of ubcsat parameters/values that is included as
#       columns in the output data.frame. For example "run" or "found".
#     params
#       A labelled list of the form "list(arg1=val1,arg2=val2,...)"
#       specifying the command-line arguments to provide to "ubcsat"
#       when it is called. That is, "-arg1 val1 -arg2 val2" etc. is
#       appended to the ubcsat command in the appropriate place.
#     monitor
#       Boolean variable, indicating whether or not to print the
#       system commands issued (for ubcsat); default is TRUE.
#
# Result:
#     The result data.frame has as columns the output parameters
#     given by output_params from each run of ubcsat, with
#     additional columns added specifying the algorithm a particular
#     run used, as well as columns for each of the statistics variables
#     that ubcsat outputs for that run.
#
#     Every algorithm is evaluated using a single run of ubcsat, where
#     the number of runs of the algorithm is determined by the output
#     parameters, given in output_params (see eval_ubcsat_output_params).
#
#     Each row in the result dataframe then represents a run in ubcsat.

# XXX WHAT IS THE STATUS OF THIS? XXX
# RENAME: run_ubcsat (also elsewhere (including plans-files))

eval_ubcsat = function(
 input,
 algs=eval_ubcsat_cnf_algs,
 output_params=eval_ubcsat_output_params, 
 monitor=TRUE,...) {

  eval_ubcsat_df = NULL
  # Setup parameter string
  std_params = ""
  params = list(...)
  for (param_name in names(params)) {
    std_params = paste(std_params," -",param_name, " ",
      format(params[[param_name]],scientific=5000),sep="")
  }
  # Run ubcsat-okl with each algorithm
  alg_names = names(algs)
  for (alg in 1:length(algs)) {
    output_file = paste(input,"-",alg_names[alg],".eval_ubcsat_result",sep="")
    stats_output_file =
      paste(input,"-",alg_names[alg],".eval_ubcsat_stats",sep="")
    eval_ubcsat_command = paste(
      "ubcsat-okl -r out '", output_file, "' ",
      do.call(paste,c(names(output_params),list(sep=","))),
      " -r stats '", stats_output_file, "' ",
      "numclauses,numvars,numlits,fps,totaltime,time,steps ",
      std_params," -alg ", algs[alg], " -i ",input, " > ",
      input,"-",alg_names[alg],".eval_ubcsat_log",sep="")
    if (monitor) print(eval_ubcsat_command)
    system(eval_ubcsat_command)
    # Read in output from respective files.
    result_df = read.table(output_file,col.names=as.vector(output_params))
    result_df = add_constant_column(result_df,alg_names[alg], "alg")
    # Add statistics data
    stats_df = read.table(stats_output_file,
      colClasses=c("character","character","real"))
    for (i in 1:length(stats_df[[1]])) {
      result_df = add_constant_column(result_df, 
        stats_df[[3]][[i]], stats_df[[1]][[i]])
    }
    # Add rows from this ubcsat result to the result data.frame
    eval_ubcsat_df = rbind(eval_ubcsat_df, result_df) 
  }
  eval_ubcsat_df
}

# For example, running:
#
# E = eval_ubcsat("Test.cnf",algs=list(gsat="gsat",walksat_tabu_nonull="walksat-tabu -v nonull"),runs=1,cutoff=1)
#
# for an example cnf, results in the data.frame E with the following values:
#
#   run found   best beststep steps      seed                 alg Clauses
# 1   1     0 510109        0     1 931021056                gsat  510108
# 2   1     0 510109        0     1 931350014 walksat_tabu_nonull  510108
#   Variables TotalLiterals FlipsPerSecond TotalCPUTimeElapsed CPUTime_Mean
# 1     32176       1499752            100                0.01         0.01
# 2     32176       1499752            100                0.01         0.01
#   CPUTime_Median
# 1           0.01
# 2           0.01
#
# Note that there are only 2 runs (one for each algorithm), and the cutoff is
# 1 for each of these runs.
#
# whereas running:
#
# E = eval_ubcsat("Test.cnf",algs=list(gsat="gsat",walksat_tabu_nonull="walksat-tabu -v nonull"),runs=1,cutoff=1,output_params=list("run","found","best","beststep","steps"))
#
# produces:
#
#   run found   best beststep steps                 alg Clauses Variables
# 1   1     0 510109        0     1                gsat  510108     32176
# 2   1     0 510109        0     1 walksat_tabu_nonull  510108     32176
#   TotalLiterals FlipsPerSecond TotalCPUTimeElapsed CPUTime_Mean CPUTime_Median
# 1       1499752            100                0.01         0.01           0.01
# 2       1499752            100                0.01         0.01           0.01
#
# where seed is no longer included in the results.

# XXX WHY IS THIS IN HERE? XXX
# First attempt, just showing all results in table form:
eval_ubcsat_dataframe = function(E) {
  for (A in names(eval_ubcsat_cnf_algs)) {
    cat(A,": ")
    print(table(E$best[E$alg==A]))
  }
}

