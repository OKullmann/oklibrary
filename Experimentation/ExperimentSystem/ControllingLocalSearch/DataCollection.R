# Matthew Gwynne, 20.10.2010 (Swansea)
# Copyright 2010 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

# ##################
# # Running Ubcsat #
# ##################

# Runs a selection of ubcsat algorithms on the given input file, via
# ubcsat-okl, and returns the results of these runs as a data-frame.
#
# Parameters:
#     input
#       The path to the file to evaluate the ubcsat algorithms on.
#     include_algs
#       A list of ubcsat-okl algorithms to evaluate on the given input file,
#       where the name of each item is a reference for the
#       algorithm, and the value of each item is the algorithm parameter
#       as given to ubcsat (Optional).
#     exclude_algs
#       A list of ubcsat-okl algorithms not to evaluate on the given input
#       file. The list of algorithms evaluated are exactly those in
#       include_algs which are not in exclude_algs, ignoring names (Optional).
#     tmp_directory
#       The path (string) to the temporary directory where run_ubcsat
#       stores it's temporary files (log files etc) (Optional).
#     monitor
#       Boolean variable, indicating whether or not to print the
#       system commands issued (for ubcsat); default is TRUE (Optional).
#     ...
#       Additional parameters to ubcsat-okl can be specified as additional
#       parameters to this function. So adding "runs=1", would
#       result in each call to ubcsat-okl including "-runs=1"
#       in it's command string (Optional).
#
# Result:
#     The result data.frame has as columns the output parameters,
#     output by ubcsat-okl (see run_ubcsat_column_names below for
#     more information), from each run of ubcsat, with
#     additional columns added specifying the algorithm, the run,
#     as well as columns for each of the statistics variables
#     that ubcsat-okl outputs for each invocation.
#
#     Every algorithm is evaluated using a single call to ubcsat-okl, where
#     the number of runs of the algorithm is either the default for
#     ubcsat-okl or the value of the optional "runs" parameter for
#     run_ubcsat. Likewise, all parameters for ubcsat-okl other than
#     -alg, -i, -r stats and -r out, if not specified as arguments to
#     run_ubcsat, take on the default value assigned by ubcsat-okl.
#
#     Each row in the result dataframe then represents a run in ubcsat-okl
#     (note that a run is not the same as one invocation of ubcsat-okl).
run_ubcsat = function(
 input,
 include_algs = names(run_ubcsat_cnf_algs),
 exclude_algs = list(),
 tmp_directory=run_ubcsat_temp_dir(basename(input)),
 monitor=TRUE,...) {

  filename = basename(input)

  # Get only those algorithms in the included list which
  # are not excluded.
  algs = include_algs[!(include_algs %in% exclude_algs)]

  # Create the temporary directory (error if it doesn't exist)
  if ( ! file.exists(tmp_directory)) 
    if (!dir.create(tmp_directory, showWarnings=FALSE)) {
      print(paste("ERROR[run_ubcsat]: Unable to create directory '",
                  tmp_directory, "'."))
      return(FALSE)
    }

  # Run ubcsat-okl with each algorithm
  run_ubcsat_df = NULL
  for (alg in algs) {
    try({
      output_file =
        run_ubcsat_result_path(filename,alg,tmp_directory)
      stats_output_file =
        run_ubcsat_stats_path(filename,alg,tmp_directory)
      command =
        run_ubcsat_command(input, alg,run_ubcsat_cnf_algs[alg],
                            tmp_directory,...)

      # Run the ubcsat-okl command
      if (monitor) print(command)
      system(command, intern=FALSE)    
    
    })
  }
  
  read_ubcsat_dir(filename, include_algs=algs, tmp_directory=tmp_directory)
}

# As an example, generating the SAT problem relating to testing whether
# the (diagonal) van der Waerden number vdw_2(5,5) > 200 using
#
# > output_vanderwaerden2_stdname(5,200);
#
# in Maxima (oklib --maxima), and then running
#
# > E = run_ubcsat("VanDerWaerden_2-5-5_200.cnf", seed=1255465118)
#
# in R (oklib --R), results in something similar to ??? (ubcsat-okl makes
# random choices, see XXX):
#
#    run sat min osteps msteps       seed                 alg Clauses Variables
# 1    1   0 115     58 100000 1255465118                gsat    9800       200
# 2    1   0 114    114 100000 1255716183         gsat_simple    9800       200
# 3    1   0  83  66954 100000 1255875199               gwsat    9800       200
# 4    1   0  83  18027 100000 1256245045           gsat_tabu    9800       200
# 5    1   0 117    139 100000 1256522994                hsat    9800       200
# 6    1   0 108   9514 100000 1256682510               hwsat    9800       200
# 7    1   0 174  42839 100000 1257104571             walksat    9800       200
# 8    1   0 149  21426 100000 1257321352        walksat_tabu    9800       200
# 9    1   0  22  63217 100000 1257520853             novelty    9800       200
# 10   1   0  21  10863 100000 1257801927            noveltyp    9800       200
# 11   1   0  22  10886 100000 1258063146       adaptnoveltyp    9800       200
# 12   1   0  67  12799 100000 1258315948            rnovelty    9800       200
# 13   1   0  68  24649 100000 1258595755           rnoveltyp    9800       200
# 14   1   0  22   3905 100000 1258904684                saps    9800       200
# 15   1   0  21  12835 100000 1259352347               rsaps    9800       200
# 16   1   0  21  46556 100000 1259801333              sapsnr    9800       200
# 17   1   0  64  65556 100000 1260280969                rots    9800       200
# 18   1   0  21   8102 100000 1260583021               irots    9800       200
# 19   1   0  96  60290 100000 1260918141                samd    9800       200
# 20   1   0  24  88250 100000 1261205059 walksat_tabu_nonull    9800       200
#    TotalLiterals FlipsPerSecond BestStep_Mean Steps_Mean Steps_Max
# 1          49000         434783            58      1e+05     1e+05
# 2          49000         769231           114      1e+05     1e+05
# 3          49000         294118         66954      1e+05     1e+05
# 4          49000         384615         18027      1e+05     1e+05
# 5          49000         714286           139      1e+05     1e+05
# 6          49000         243902          9514      1e+05     1e+05
# 7          49000         526316         42839      1e+05     1e+05
# 8          49000         555556         21426      1e+05     1e+05
# 9          49000         370370         63217      1e+05     1e+05
# 10         49000         400000         10863      1e+05     1e+05
# 11         49000         416667         10886      1e+05     1e+05
# 12         49000         384615         12799      1e+05     1e+05
# 13         49000         344828         24649      1e+05     1e+05
# 14         49000         232558          3905      1e+05     1e+05
# 15         49000         232558         12835      1e+05     1e+05
# 16         49000         217391         46556      1e+05     1e+05
# 17         49000         344828         65556      1e+05     1e+05
# 18         49000         312500          8102      1e+05     1e+05
# 19         49000         370370         60290      1e+05     1e+05
# 20         49000         625000         88250      1e+05     1e+05
#    PercentSuccess BestSolution_Mean BestSolution_Median BestSolution_Min
# 1               0               115                 115              115
# 2               0               114                 114              114
# 3               0                83                  83               83
# 4               0                83                  83               83
# 5               0               117                 117              117
# 6               0               108                 108              108
# 7               0               174                 174              174
# 8               0               149                 149              149
# 9               0                22                  22               22
# 10              0                21                  21               21
# 11              0                22                  22               22
# 12              0                67                  67               67
# 13              0                68                  68               68
# 14              0                22                  22               22
# 15              0                21                  21               21
# 16              0                21                  21               21
# 17              0                64                  64               64
# 18              0                21                  21               21
# 19              0                96                  96               96
# 20              0                24                  24               24
#    BestSolution_Max
# 1               115
# 2               114
# 3                83
# 4                83
# 5               117
# 6               108
# 7               174
# 8               149
# 9                22
# 10               21
# 11               22
# 12               67
# 13               68
# 14               22
# 15               21
# 16               21
# 17               64
# 18               21
# 19               96
# 20               24
#
# One can then, for instance, restrict oneself to only using the
# algorithms which performed best (for a particular metric). So, for example,
# one might restrict oneself to novelty+, rsaps and sapsnr above, and allow
# more runs and a higher cutoff, like so
#
# > E2 = run_ubcsat("VanDerWaerden_2-5-5_200.cnf", cutoff=1000000, runs=200, include_algs=list(noveltyp="novelty+",rsaps="rsaps",sapsnr="sapsnr")))
#
#    run sat min osteps  msteps       seed      alg Clauses Variables
# 1    1   0  22  43382 1000000 1694350974 noveltyp    9800       200
# 2    2   0  21  25031 1000000 2212095318 noveltyp    9800       200
# 3    3   0  21  30920 1000000  718683817 noveltyp    9800       200
# 4    4   0  22 165095 1000000  919974690 noveltyp    9800       200
# 5    5   0  21  39981 1000000 3421966001 noveltyp    9800       200
# 6    1   0  21   1347 1000000 1722378942    rsaps    9800       200
# 7    2   0  21   1168 1000000  816551823    rsaps    9800       200
# 8    3   0  21  35542 1000000 2428062255    rsaps    9800       200
# 9    4   0  21  44524 1000000 3429666798    rsaps    9800       200
# 10   5   0  21  29640 1000000 2001159883    rsaps    9800       200
# 11   1   0  21  14472 1000000 1769745035   sapsnr    9800       200
# 12   2   0  21 180056 1000000 3960436902   sapsnr    9800       200
# 13   3   0  21   4050 1000000 4068325161   sapsnr    9800       200
# 14   4   0  21  35297 1000000 2486223150   sapsnr    9800       200
# 15   5   0  21  33255 1000000 3128418692   sapsnr    9800       200
#    TotalLiterals FlipsPerSecond BestStep_Mean Steps_Mean Steps_Max
# 1          49000         393082       60881.8      1e+06     1e+06
# 2          49000         393082       60881.8      1e+06     1e+06
# 3          49000         393082       60881.8      1e+06     1e+06
# 4          49000         393082       60881.8      1e+06     1e+06
# 5          49000         393082       60881.8      1e+06     1e+06
# 6          49000         229253       22444.2      1e+06     1e+06
# 7          49000         229253       22444.2      1e+06     1e+06
# 8          49000         229253       22444.2      1e+06     1e+06
# 9          49000         229253       22444.2      1e+06     1e+06
# 10         49000         229253       22444.2      1e+06     1e+06
# 11         49000         245821       53426.0      1e+06     1e+06
# 12         49000         245821       53426.0      1e+06     1e+06
# 13         49000         245821       53426.0      1e+06     1e+06
# 14         49000         245821       53426.0      1e+06     1e+06
# 15         49000         245821       53426.0      1e+06     1e+06
#    PercentSuccess BestSolution_Mean BestSolution_Median BestSolution_Min
# 1               0              21.4                  21               21
# 2               0              21.4                  21               21
# 3               0              21.4                  21               21
# 4               0              21.4                  21               21
# 5               0              21.4                  21               21
# 6               0              21.0                  21               21
# 7               0              21.0                  21               21
# 8               0              21.0                  21               21
# 9               0              21.0                  21               21
# 10              0              21.0                  21               21
# 11              0              21.0                  21               21
# 12              0              21.0                  21               21
# 13              0              21.0                  21               21
# 14              0              21.0                  21               21
# 15              0              21.0                  21               21
#    BestSolution_Max
# 1                22
# 2                22
# 3                22
# 4                22
# 5                22
# 6                21
# 7                21
# 8                21
# 9                21
# 10               21
# 11               21
# 12               21
# 13               21
# 14               21
# 15               21
#
# One can see that only 3 algorithms have been run, and each has had 5 runs.
#
#
# One can then store the resulting data.frame using write.table
#
# > write.table(E2, file="VDW-ubcsat-result.dat")
#
# where the above command writes the previous run_ubcsat results
# to "VDW-ubcsat-result.dat". For specification of the file
# format see the R documentation for write.table ("help(write.table)" in R).
#

# #####################
# Auxiliary functions #
# #####################

# Path to directory to store run_ubcsat intermediate files.
#
# Parameters :
#     filename
#       The filename (basename, not path) for the Dimacs CNF
#       file being investigated.
#
# Result :
#     The default path (string) to store "run_ubcsat"
#     temporary files in for the given input file name.
#
run_ubcsat_temp_dir = function(filename) {
  return ( paste("ubcsat_tmp_",filename, sep="") )
}

# List of output column names (as we would like them to be called
# in the output dataframe) which ubcsat-okl outputs (CONSTANT).
#
# The default list is:
#     run (positive integer)
#       The index (1-number of runs) of the ubcsat-okl run that
#       the particular output row corresponds with.
#     sat (boolean (0 or 1))
#       Whether or not the given run of ubcsat-okl found a satisfying
#       assignment for the input formula (1 indicates it found
#       a satisfying assignment, 0 indicates it didn't).
#     min (positive integer)
#       The minimum number of falsified clauses which occurred during
#       all of the assignments that ubcsat-okl tried in the current
#       run.
#     osteps (positive integer)
#       The number of steps (i.e. total assignments considered by
#       ubcsat-okl) in the current run before the minimum was reached.
#     msteps (positive integer)
#       Number of steps (i.e. total assignments considered by ubcsat-okl)
#       in the current run in total.
#     seed (positive integer)
#       Seed value for the random number generator. This can then be used
#       later to reproduce the same run and retrieve the solution using
#       ubcsat-okl.       
# 
# NOTE: This should match up with the columns output by ubcsat-okl.
run_ubcsat_column_names = list("run","sat","min","osteps","msteps","seed")

# Default list of ubcsat algorithms run_ubcsat evaluates, given
# as a named list, where
#
#   - the name of each item is a reference (with only alphanumeric
#     characters and "_") for the  algorithm, used by run_ubcsat wherever
#     "safe" names are needed (without spaces etc), and
#   - the value of each item is the algorithm parameter as given to
#     ubcsat-okl.
#
# NOTE: This list should match the full range of UBCSAT algorithms for the
#       default version of UBCSAT as provided in the OKlibrary.
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

# Construct a new dataframe which is the original dataframe with
# an additional constant column with value const_var.
#
# Parameters :
#     df
#       The dataframe to add the column to.
#     const_var
#       the constant value to use in the new column.
#     name
#       the name to assign the new column in the result data.frame.
#
# Result:
#     New data.frame with same data as df but with an additional column
#     with "name" as the name, and const_var as the data for every row.
#
# MG: This helper function should be moved to a more appropriate location.
add_constant_column = function(df,const_var, name) {
  temp_df = data.frame(do.call(c,lapply(df[[1]],function(a){const_var})))
  colnames(temp_df) = c(name)
  data.frame(df, temp_df)
}

# Returns the path to the log file run_ubcsat generates for the given
# parameters.
#
# Parameters :
#     filename
#       The name (not path) of the Dimacs CNF input file for run_ubcsat.
#     alg_safe_name
#       An alphanumeric string used as a reference for ubcsat-okl algorithm.
#       For example for "gsat -v simple" one might use "gsat_simple".
#     tmp_directory
#       The path (string) to the temporary directory where run_ubcsat
#       stores it's temporary files for the given input filename.
#
# Result :
#     The path (string) to the log file run_ubcsat generates for the
#     given parameters.
#
run_ubcsat_log_path = function(
  filename, alg_safe_name, tmp_directory=run_ubcsat_temp_dir(filename)) {
  return (paste(tmp_directory, "/",
                alg_safe_name,".run_ubcsat_log",
                sep = ""))
}

# Returns the path to the result file run_ubcsat generates for the given
# parameters.
#
# Parameters :
#     filename
#       The name (not path) of the Dimacs CNF input file for run_ubcsat.
#     alg_safe_name
#       An alphanumeric string used as a reference for ubcsat-okl algorithm.
#       For example for "gsat -v simple" one might use "gsat_simple".
#     tmp_directory
#       The path (string) to the temporary directory where run_ubcsat
#       stores it's temporary files for the given input filename.
#
# Result :
#     The path (string) to the result file run_ubcsat generates for the
#     given parameters. The word "result" here simply means the output
#     from ubcsat-okl generated when run_ubcsat ran the problem for the
#     given algorithm.
#
run_ubcsat_result_path = function(
  filename, alg_safe_name, tmp_directory=run_ubcsat_temp_dir(filename)) {
  return(paste(tmp_directory, "/",
               alg_safe_name, ".run_ubcsat_result",
               sep=""))
}

# Returns the path to the statistics file run_ubcsat generates for the given
# parameters.
#
# Parameters :
#     filename
#       The name (not path) of the Dimacs CNF input file for run_ubcsat.
#     alg_safe_name
#       An alphanumeric string used as a reference for ubcsat-okl algorithm.
#       For example for "gsat -v simple" one might use "gsat_simple".
#     tmp_directory
#       The path (string) to the temporary directory where run_ubcsat
#       stores it's temporary files for the given input filename.
#
# Result :
#     The path (string) to the statistics file run_ubcsat generates for the
#     given parameters.
#
run_ubcsat_stats_path = function(
  filename, alg_safe_name, tmp_directory=run_ubcsat_temp_dir(filename)) {
  return(paste(tmp_directory, "/",
               alg_safe_name,".run_ubcsat_stats",
               sep=""))
}

# Takes the following parameters for which "run_ubcsat" must already
# been run and then returns the result of the corresponding
# "run_ubcsat" command, reading the results from the previously
# generated files. ??? what does this mean ??? for what is this useful ???
#
# Parameters :
#     input
#       The path to the Dimacs CNF file on which run_ubcsat has already
#       been run.
#     include_algs
#       A list of ubcsat-okl algorithms to evaluate on the given input file,
#       where the name of each item is a reference for the
#       algorithm, and the value of each item is the algorithm parameter
#       as given to ubcsat (Optional).
#     exclude_algs
#       A list of ubcsat-okl algorithms not to evaluate on the given input
#       file. The list of algorithms evaluated are exactly those in
#       include_algs which are not in exclude_algs, ignoring names (Optional).
#     tmp_directory
#       Path to the directory "run_ubcsat" stored it's temporary files in,
#       that is, the tmp_directory argument which was passed to "run_ubcsat"
#       when it was run before this command (Optional).
#
# Result:
#     The results returned by "run_ubcsat" in it's last invocation with
#     the input parameters (in the current directory / environment etc
#     if relative file paths are used).
#
read_ubcsat_dir = function(
  input,
  include_algs = names(run_ubcsat_cnf_algs),
  exclude_algs = list(),
  tmp_directory=run_ubcsat_temp_dir(basename(input))) {
  
  # Create the temporary directory (error if it doesn't exist)
  if ( ! file.exists(tmp_directory)) {
      print(paste("ERROR[read_ubcsat_dir]: Unable to open directory '",
                  tmp_directory, "'."))
      return(FALSE)
  }

  filename = basename(input)

  # Get only those algorithms in the included list which
  # are not excluded.
  algs = include_algs[!(include_algs %in% exclude_algs)]

  run_ubcsat_df = NULL
  for (alg in algs ) {
    try({
      output_file =
        run_ubcsat_result_path(filename,alg,tmp_directory)
      stats_output_file =
        run_ubcsat_stats_path(filename,alg,tmp_directory)
    
      # Read in output from respective temporary files.
      result_df = read.table(output_file,
                             col.names = as.vector(run_ubcsat_column_names))

      # Check we have rows before trying to reference them and add columns etc.
      # Otherwise segfaults will crash the program.
      if (length(row.names(result_df)) > 0) {
        result_df = add_constant_column(result_df,alg, "alg")
    
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
    })
  }
  run_ubcsat_df
}

# Returns the ubcsat-okl command run by run_ubcsat for the given
# parameters.
#
# Parameters :
#     input
#       The path to the Dimacs CNF input file.
#     alg_safe_name
#       An alphanumeric string (possibly including "_") used as a reference
#       for ubcsat-okl algorithm. For example for "gsat -v simple" one might
#       use "gsat_simple".
#     alg_name
#       The name of the ubcsat-okl algorithm for the result ubcsat-okl
#       to use.
#     tmp_directory
#       The path (string) to the temporary directory where run_ubcsat
#       stores it's temporary files (log files etc) (Optional).
#     ...
#       Additional parameters to ubcsat-okl can be specified as additional
#       parameters to this function, so adding "runs=1", would
#       result in the result command including "-runs=1" (Optional).
#
# Result :
#     The ubcsat-okl command (string) running on the given input, using
#     the given algorithm, outputting to the standard output files in
#     the given temporary directory, with the given additional parameters.
#
run_ubcsat_command = function(
  input, alg_safe_name, alg_name,
  tmp_directory=run_ubcsat_temp_dir(basename(input)),...) {

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

