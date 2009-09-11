# Oliver Kullmann, 4.6.2009 (Swansea)
# Copyright 2009 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

# ################
# # Reading data #
# ################

# Reading files created by ubcsat-okl, printing most basic statistics, and
# returning the dataframe:
read_ubcsat = function(filename, ...) {
  E = read.table(file = filename, 
        colClasses = c("character", "factor", "integer", "numeric", "numeric", "character"), 
        ...)
 print(table(E$min))
 cat(length(E$min),"\n")
 E
}
# The data format is
# line-counter 0/1 min-falsified optimal-steps maxima-steps seed
# where 0/1 indicates unsat/sat.
# A first line
#     sat  min     osteps     msteps       seed
# is assumed.
# If the (given) line-counters are to be ignored, use the additional argument
# "row.names=NULL". And if m leading lines are to be ignored, use
# "skip=m".


# ##################
# # Running Ubcsat #
# ##################

# Default shell command used to call ubcsat when calling eval_ubcsat.
eval_ubcsat_command = "ubcsat -rclean \\
 -r out '$OUTPUT' $OUTPUT_PARAMS \\
  -r stats '$STATS_OUTPUT'  numclauses,numvars,numlits,fps,totaltime,time \\
   $STD_PARAMS -alg $ALG -i $TARGET"
# There are several variables which can be placed in the command string
# which will be directly substituted with the relevant values:
#
# $OUTPUT_PARAMS 
#     This variable will be substituted with a list of output
#     parameters, specified in "eval_ubcsat_output_params".
# $STD_PARAMS
#     This variable will be substituted with a string with
#     "-arg1 val1 -arg2 val2 ..." etc, where "arg1" etc. are
#     arguments to ubcsat. The default list of argument-value pairs
#     is stored in "eval_ubcsat_std_params" as a list with labelled
#     values. For example "eval_ubcsat_std_params=list(runs=500,cutoff=10)"
#     results in $STD_PARAMS being replaced with "-runs 500 -cutoff 10".
# $TARGET
#     This variable will be substituted with the input file which
#     "eval_ubcsat" is evaluating.
# $ALG
#     This variable will be substituted with the current ubcsat algorithm
#     that eval_ubcsat is evaluating with the command.
# $OUTPUT
#     This variable will be substituted with the file to which
#     the standard ubcsat output should be placed. By default this
#     is "$TARGET-$ALG.result".
# $STATS_OUTPUT
#     This variable will be substituted with the file to which
#     the ubcsat statistics should be output.

# List of output parameters which ubcsat should output.
eval_ubcsat_output_params = list("run","found","best","beststep","steps","seed",
  "nullflips","percentnull","rand","candidates","agemean")

# List of ubcsat algorithms eval_ubcsat should evaluate.
eval_ubcsat_cnf_algs = list("gsat", "gsat -v simple", "gwsat", "gsat-tabu",
  "hsat", "hwsat","walksat", "walksat-tabu", "novelty", "novelty+",
  "adaptnovelty+","rnovelty", "rnovelty+", "saps", "rsaps", "sapsnr", "rots",
  "irots", "samd", "walksat-tabu -v nonull")

# Labelled list "arg=val" of argument-value pairs of arguments to the
# ubcsat command. 
eval_ubcsat_std_params = list(runs=100,cutoff=10000)

# Takes a dataframe, a constant value C and returns a new adds data frame with
# an additional column where that column has value C for every row in the
# data.frame.
#
# MG: This helper function should be moved to a more appropriate location.
add_constant_column = function(df,const_var, name) {
  temp_df = data.frame(do.call(c,lapply(df[[1]],function(a){const_var})))
  colnames(temp_df) = c(name)
  data.frame(df, temp_df)
}

# Runs a selection of ubcsat algorithms on the given input file, and
# returns the results of these runs as a dataframe.
#
# Parameters:
#     input
#       The path to the file to evaluate the ubcsat algorithms on.
#     output
#       The path to the file to store temporary results from ubcsat
#       for each run in. Variables $TARGET and $ALG may be used as
#       placeholders for the input file path and algorithm being used
#       and will be substituted with the appropriate values.
#     command
#       The command to run when calling ubcsat, see
#       eval_ubcsat_command.
#     algs
#       A list of ubcsat-algorithms to evaluate on the given input file.
#     output_params
#       A list of ubcsat parameters/values that should be included as
#       columns in the output data.frame. For example "run" or "found".
#     params
#       A labelled list of the form "list(arg1=val1,arg2=val2,...)"
#       specifying the command-line arguments to provide to "ubcsat"
#       when it is called. That is, "-arg1 val1 -arg2 val2" etc. is
#       appended to the ubcsat command in the appropriate place.
#     stats_output
#       A temporary file to store the ubcsat statistics output for
#       each run. Variables $TARGET and $ALG may be used as
#       placeholders for the input file path and algorithm being used
#       and will be substituted with the appropriate values.
#     monitor
#       Boolean variable, indicating whether or not to print output
#       to stdout as eval_ubcsat is running. TRUE indicates output
#       should be output, whereas FALSE is the default.
#     
#
# Result:
#     The result data.frame has as its columns, the output parameters
#     given by "output_params" from each run of ubcsat, with
#     additional columns added, specifying the algorithm a particular
#     run used, as well as columns for each of the statistics variables
#     that ubcsat outputs for that run (of ubcsat, not a run within
#     "ubcsat").
#
#     Every algorithm is evaluated using a single run of ubcsat, where
#     the number of runs of the algorithm is determined by the output
#     parameters given in output_params (see "eval_ubcsat_output_params").
#
#     Each row in the result dataframe then represents a run in ubcsat.
eval_ubcsat = function(
 input,
 output="$TARGET-$ALG.result",
 command=eval_ubcsat_command,
 algs=eval_ubcsat_cnf_algs,
 output_params=eval_ubcsat_output_params, 
 params = eval_ubcsat_std_params,
 stats_output="$TARGET-$ALG.stats.result",
 monitor=FALSE) {

  eval_ubcsat_command = gsub("\\$TARGET", input, command)
  eval_ubcsat_command = gsub("\\$OUTPUT_PARAMS", 
    do.call(paste,c(output_params,list(sep=","))), eval_ubcsat_command)
  std_params = ""
  for (param_name in names(params)) {
    std_params = paste(std_params," -",param_name, " ",
      format(params[[param_name]],scientific=5000),sep="")
  }
  eval_ubcsat_command = gsub("\\$STD_PARAMS", std_params, eval_ubcsat_command)
  output_file = gsub("\\$TARGET", input, output)
  stats_output_file = gsub("\\$TARGET", input, stats_output)
  for (alg in algs) {
    output_file_t = gsub("\\$ALG", gsub(" ","",alg), output_file)
    stats_output_file_t = gsub("\\$ALG", gsub(" ","",alg), stats_output_file)
    eval_ubcsat_command_t = gsub("\\$ALG", alg, eval_ubcsat_command)
    eval_ubcsat_command_t =
      gsub("\\$OUTPUT", output_file_t, eval_ubcsat_command_t)
    eval_ubcsat_command_t = 
      gsub("\\$STATS_OUTPUT", stats_output_file_t, eval_ubcsat_command_t)
    if (monitor) print(eval_ubcsat_command_t)
    system(eval_ubcsat_command_t)
    result_df = read.table(output_file_t,col.names=as.vector(output_params))
    result_df = add_constant_column(result_df,alg, "alg")
    stats_df = read.table(stats_output_file_t,
      colClasses=c("character","character","real"))
    for (i in 1:length(stats_df[[1]])) {
      result_df = add_constant_column(result_df, 
        stats_df[[3]][[i]], stats_df[[1]][[i]])
    }
    if (exists("eval_ubcsat_df")) { 
      eval_ubcsat_df = rbind(eval_ubcsat_df, result_df) 
    } else {
      eval_ubcsat_df = result_df
    }
  }
  eval_ubcsat_df
}

