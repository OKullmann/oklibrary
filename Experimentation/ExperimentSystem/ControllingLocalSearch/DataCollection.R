# Matthew Gwynne, 20.10.2010 (Swansea)
# Copyright 2010 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

# ##################
# # Running Ubcsat #
# ##################

# Relative path to directory to store run_ubcsat intermediate files
# (CONSTANT)
run_ubcsat_temp_dir = "ubcsat_tmp"

# List of output columns which ubcsat-okl outputs (CONSTANT).
run_ubcsat_column_names = list("run","sat","min","osteps","msteps","seed")

# Default list of ubcsat algorithms run_ubcsat evaluates, given
# as a named list, where the name of each item is a reference for the
# algorithm, and the value of each item is the algorithm parameter
# as given to ubcsat.
run_ubcsat_cnf_algs = list(
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

# Takes the input file *name*, the algorithm name (as a filepath safe
# single word which is simply a "nickname" for the algorithm) and
# optionally the temporary directory used for run_ubcsat intermediate
# files and returns the file path for the log file for this ubcsat-okl run
# for that algorithm on that file.
run_ubcsat_log_path = function(
  filename, alg_safe_name, tmp_directory=run_ubcsat_temp_dir) {
  return (paste(tmp_directory, "/",
                filename,"-",alg_safe_name,".run_ubcsat_log",
                sep = ""))
}

# Takes the input file *name*, the algorithm name (as a filepath safe
# single word which is simply a "nickname" for the algorithm) and
# optionally the temporary directory used for run_ubcsat intermediate
# files and returns the file path for the result file for this ubcsat-okl run
# for that algorithm on that file.
run_ubcsat_result_path = function(
  filename, alg_safe_name, tmp_directory=run_ubcsat_temp_dir) {
  return(paste(tmp_directory, "/",
               filename,"-",alg_safe_name,".run_ubcsat_result",
               sep=""))
}

# Takes the input file *name*, the algorithm name (as a filepath safe
# single word which is simply a "nickname" for the algorithm) and
# optionally the temporary directory used for run_ubcsat intermediate
# files and returns the file path for the statistics file for this ubcsat-okl
# run for that algorithm on that file.
run_ubcsat_stats_path = function(
  filename, alg_safe_name, tmp_directory=run_ubcsat_temp_dir) {
  return(paste(tmp_directory, "/",
               filename,"-",alg_safe_name,".run_ubcsat_stats",
               sep=""))
}

# Takes the input file *name*, the algorithm name (as a filepath safe
# single word which is simply a "nickname" for the algorithm), the
# algorithm name (full name as given to ubcsat), the temporary directory
# used for run_ubcsat intermediate files, along with any parameters to be
# passed to ubcsat (for instance "runs=100" results in "-runs=100" being
# passed to the ubcsat command), and returns the ubcsat-okl command
# to run this algorithm with this set of parameters on this file.
run_ubcsat_command = function(
  input, alg_safe_name, alg_name, tmp_directory=run_ubcsat_temp_dir,...) {

  filename = basename(input)
  output_file = run_ubcsat_result_path(filename,alg_safe_name,tmp_directory)
  stats_output_file =
      run_ubcsat_stats_path(filename,alg_safe_name,tmp_directory)
  
  # Setup parameter string
  std_params = ""
  params = list(...)
  for (param_name in names(params)) {
    std_params = paste(std_params," -",param_name, " ",
      format(params[[param_name]],scientific=5000),sep="")
  }

  return( paste(
                "ubcsat-okl -r out '", output_file, "' ",
                " -r stats '", stats_output_file, "' ",
                std_params," -alg ", alg_name, " -i ",input, " > ",
                run_ubcsat_log_path(filename, alg_safe_name, tmp_directory),
                sep="") )
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
#     monitor
#       Boolean variable, indicating whether or not to print the
#       system commands issued (for ubcsat); default is TRUE.
#     ...
#       Additional to ubcsat-okl can be specified as additional
#       parameters to this function, so adding "runs=1", would
#       result in each call to ubcsat-okl including "-runs=1"
#       in it's command.
#
# Result:
#     The result data.frame has as columns the output parameters
#     output by ubcsat-okl from each run of ubcsat, with
#     additional columns added specifying the algorithm, the run,
#     as well as columns for each of the statistics variables
#     that ubcsat outputs for that invocation of ubcsat-okl.
#
#     Every algorithm is evaluated using a single run of ubcsat, where
#     the number of runs of the algorithm is either the default for
#     ubcsat-okl or the value of the optional "runs" parameter for
#     run_ubcsat.
#
#     Each row in the result dataframe then represents a run in ubcsat.

# XXX WHAT IS THE STATUS OF THIS? XXX
# RENAME: run_ubcsat (also elsewhere (including plans-files))

run_ubcsat = function(
 input,
 algs = run_ubcsat_cnf_algs,
 tmp_directory=run_ubcsat_temp_dir,
 monitor=TRUE,...) {

  filename = basename(input)

  # Create the temporary directory (error if it doesn't exist)
  if ( ! file.exists(tmp_directory)) 
    if (!dir.create(tmp_directory, showWarnings=FALSE)) {
      print(paste("ERROR[run_ubcsat]: Unable to create directory '",
                  tmp_directory, "'."))
      return(FALSE)
    }

  # Run ubcsat-okl with each algorithm
  run_ubcsat_df = NULL
  alg_names = names(algs)
  for (alg in 1:length(algs)) {
    output_file =
      run_ubcsat_result_path(filename,alg_names[alg],tmp_directory)
    stats_output_file =
      run_ubcsat_stats_path(filename,alg_names[alg],tmp_directory)
    command =
      run_ubcsat_command(input, alg_names[alg],algs[alg],
                          tmp_directory,...)

    # Run the ubcsat-okl command
    if (monitor) print(command)
    system(command)
    
    # Read in output from respective temporary files.
    result_df = read.table(output_file,
                           col.names = as.vector(run_ubcsat_column_names))
    result_df = add_constant_column(result_df,alg_names[alg], "alg")
    
    # Add statistics data
    stats_df = read.table(stats_output_file,
      colClasses=c("character","character","real"))
    for (i in 1:length(stats_df[[1]])) {
      result_df = add_constant_column(result_df, 
        stats_df[[3]][[i]], stats_df[[1]][[i]])
    }
    
    # Add rows from this ubcsat result to the result data.frame
    run_ubcsat_df = rbind(run_ubcsat_df, result_df) 
  }
  run_ubcsat_df
}

# For example, running:
#
# E = run_ubcsat("Test.cnf",algs=list(gsat="gsat",walksat_tabu_nonull="walksat-tabu -v nonull"),runs=1,cutoff=1)
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
# E = run_ubcsat("Test.cnf",algs=list(gsat="gsat",walksat_tabu_nonull="walksat-tabu -v nonull"),runs=1,cutoff=1,output_params=list("run","found","best","beststep","steps"))
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
