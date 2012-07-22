# Matthew Gwynne, 20.10.2010 (Swansea)
# Copyright 2010, 2011, 2012 Oliver Kullmann
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
#       The path to the file to evaluate the ubcsat algorithms on
#       (a DIMACS CNF file).
#     include_algs
#       A list of ubcsat-okl algorithms to evaluate on the given input file,
#       where the names of the algorithms are abbreviations for the names
#       given in ubcsat. For a list of such abbreviations, see
#       "names(run_ubcsat_cnf_algs)" in R. (Optional).
#     exclude_algs
#       A list of ubcsat-okl algorithms not to evaluate on the given input
#       file. The list of algorithms evaluated are exactly those in
#       include_algs which are not in exclude_algs (Optional).
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
#     that ubcsat-okl outputs for each invocation. For a specification
#     of such columns as used in run_ubcsat, see run_ubcsat_column_names.
#
#     All columns as given by ubcsat-okl are integer types (except
#     in the case of an "inf" value, in which case they are double).
#     The "alg" (algorithm) column is an R factor (see "? factor" in R),
#     as strings are by default converted to factors in R dataframes.
#     For full details of each field, please see the "ubcsat-okl" and the
#     ubcsat documentation.
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
#
# Files:
#     Additional to this output, various log files are created in a temporary
#     directory, with files for each invocation of ubcsat (with each
#     algorithm), namely the ubcsat result file (see the ubcsat documentation
#     for "-r out"), ubcsat statistics file (see the ubcsat documentation for
#     "-r stats") and any errors and output from ubcsat-okl stored in this
#     directory (in the same file). The directory and various files have the
#     following paths relative to the current directory (resp.) :
#
#     ubcsat_tmp_${FILENAME}_${DATE}
#     ubcsat_tmp_${FILENAME}_${DATE}/${ALG}.run_ubcsat_result
#     ubcsat_tmp_${FILENAME}_${DATE}/${ALG}.run_ubcsat_stats
#     ubcsat_tmp_${FILENAME}_${DATE}/${ALG}.run_ubcsat_log
#
#     where ${FILENAME} is the basename of the given input filename, and
#     ${ALG} is the algorithm run with the corresponding invocation of
#     ubcsat-okl on the file. ${DATE} is simply the current date in
#     the format specified by "%Y-%m-%d-%H%M%S".
#
#     Any errors during the computation may result in corrupt files (for
#     example if any ubcsat-okl runs crash/segfault. In this case, any
#     results already created before the crash for that invocation of
#     ubcsat-okl are moved to ubcsat_tmp_${FILENAME}/corrupt/ to ensure
#     ubcsat_tmp_${FILENAME} is readable by read_ubcsat_dir.
run_ubcsat = function(
 input,
 include_algs = names(run_ubcsat_cnf_algs),
 exclude_algs = list(),
 tmp_directory=run_ubcsat_temp_dir(basename(input)),
 monitor=TRUE,
 ubcsat_wrapper = "ubcsat-okl",
 ...) {

  filename = basename(input)
  error_directory = paste(tmp_directory,"/corrupt/",sep="")

  # Get only those algorithms in the included list which
  # are not excluded.
  algs = include_algs[!(include_algs %in% exclude_algs)]
  num_algs = length(algs)

  # Create the temporary directory (error if it doesn't exist)
  if ( ! file.exists(tmp_directory)) 
    if (!dir.create(tmp_directory, showWarnings=FALSE)) {
      print(paste("ERROR[run_ubcsat]: Unable to create directory '",
                  tmp_directory, "'."))
      return(FALSE)
    }
  
  # Create directory for potentially corrupt data
  if ( ! file.exists(error_directory))
    if (!dir.create(error_directory,showWarnings=FALSE)) {
      print(paste("ERROR[run_ubcsat]: Unable to create directory '",
                  error_directory, "'."))
      return(FALSE)
    }
  
  # Run ubcsat-okl with each algorithm
  run_ubcsat_df = NULL
  errors_l = list()
  counter_algs = 0
  for (alg in algs) {
    error = TRUE
    try({
      output_file =
        run_ubcsat_result_path(tmp_directory,alg)
      stats_output_file =
        run_ubcsat_stats_path(tmp_directory,alg)
      command =
        run_ubcsat_command(input, alg,run_ubcsat_cnf_algs[alg],
                           tmp_directory, wrapper=ubcsat_wrapper, ...)

      counter_algs=counter_algs+1;
      if (monitor) {
        cat("\n", "[",counter_algs, "/", num_algs, "]: Running ", alg, " on ",
            filename, ".\n", sep="")
        cat(command)
      }
      
      error_code = system(command, intern=FALSE, ignore.stderr=TRUE)
      if (error_code == 0) error = FALSE

      if (monitor && !error) {
        result_df = read.table(output_file,
                               col.names = as.vector(run_ubcsat_column_names))
        print(table(result_df$min))
        cat(system(paste("grep Flips", stats_output_file), intern=TRUE), "\n")
      }
    })

    # Check if there has been an error and inform the user
    if (error) {
      errors_l = c(errors_l, list(alg))
      # Clean up files (move them to a special errors directory)
      try({
        system(paste("mv", output_file,
                     paste(error_directory, output_file),
                     "2>&1 > /dev/null"), ignore.stderr=TRUE)
      })
      try({
        system(paste("mv", output_file,
                     paste(error_directory, stats_output_file),
                     "2>&1 > /dev/null"), ignore.stderr=TRUE)
      })
      # Tell the user
      print(paste("WARNING[run_ubcsat]: There has been an error generating",
                  "data for the", alg,"algorithm."))
    }
  }
  
  result = read_ubcsat_dir(directory=tmp_directory, include_algs=algs)

  # If there are errors, inform the user.
  if (length(errors_l) > 0)
    cat("\nWARNING[run_ubcsat]: There have been", length(errors_l),
                "errors with the following algorithms:\n",
                do.call(paste,c(errors_l,list(sep=", "))),".\n",
                "See the corresponding log file and (potentially corrupt)",
                "result files in\n  ", error_directory, "\n for details.\n")

  result
}

# As an example, generating the SAT problem relating to testing whether
# the (diagonal) van der Waerden number vdw_2(5,5) > 200 using
#
# > oklib_load_all();
# > output_vanderwaerden2_stdname(5,200);
#
# in Maxima (oklib --maxima), and then in R (oklib --R) running
#
# > E = run_ubcsat("VanDerWaerden_2-5-5_200.cnf", seed=1255465118)
#
# in R (oklib --R), results in:
#
#    run sat min osteps msteps       seed    alg Clauses Variables TotalLiterals
# 1    1   0 115     58 100000 1255465118   gsat    9800       200         49000
# 2    1   0 121     66 100000 1255465118  gsats    9800       200         49000
# 3    1   0  21  42829 100000 1255465118  gwsat    9800       200         49000
# 4    1   0  78  29454 100000 1255465118  gsatt    9800       200         49000
# 5    1   0 122     59 100000 1255465118   hsat    9800       200         49000
# 6    1   0 108  53654 100000 1255465118  hwsat    9800       200         49000
# 7    1   0 174  75721 100000 1255465118   wsat    9800       200         49000
# 8    1   0 146  54575 100000 1255465118  wsatt    9800       200         49000
# 9    1   0  22  35132 100000 1255465118    nov    9800       200         49000
# 10   1   0  22  53650 100000 1255465118   novp    9800       200         49000
# 11   1   0  22  10411 100000 1255465118  anovp    9800       200         49000
# 12   1   0  22   6684 100000 1255465118   rnov    9800       200         49000
# 13   1   0  21  25320 100000 1255465118  rnovp    9800       200         49000
# 14   1   0  21  80548 100000 1255465118   saps    9800       200         49000
# 15   1   0  21   1774 100000 1255465118  rsaps    9800       200         49000
# 16   1   0  21  46881 100000 1255465118 sapsnr    9800       200         49000
# 17   1   0  67  37811 100000 1255465118   rots    9800       200         49000
# 18   1   0  22  17325 100000 1255465118  irots    9800       200         49000
# 19   1   0  80  41166 100000 1255465118   samd    9800       200         49000
# 20   1   0 146  54575 100000 1255465118 wsattn    9800       200         49000
#    FlipsPerSecond BestStep_Mean Steps_Mean Steps_Max PercentSuccess
# 1          416667            58      1e+05     1e+05              0
# 2          263158            66      1e+05     1e+05              0
# 3          256410         42829      1e+05     1e+05              0
# 4          344828         29454      1e+05     1e+05              0
# 5          384615            59      1e+05     1e+05              0
# 6          263158         53654      1e+05     1e+05              0
# 7          357143         75721      1e+05     1e+05              0
# 8          370370         54575      1e+05     1e+05              0
# 9          277778         35132      1e+05     1e+05              0
# 10         263158         53650      1e+05     1e+05              0
# 11         294118         10411      1e+05     1e+05              0
# 12         294118          6684      1e+05     1e+05              0
# 13         277778         25320      1e+05     1e+05              0
# 14         208333         80548      1e+05     1e+05              0
# 15         200000          1774      1e+05     1e+05              0
# 16         200000         46881      1e+05     1e+05              0
# 17         312500         37811      1e+05     1e+05              0
# 18         285714         17325      1e+05     1e+05              0
# 19         370370         41166      1e+05     1e+05              0
# 20         357143         54575      1e+05     1e+05              0
#    BestSolution_Mean BestSolution_Median BestSolution_Min BestSolution_Max
# 1                115                 115              115              115
# 2                121                 121              121              121
# 3                 21                  21               21               21
# 4                 78                  78               78               78
# 5                122                 122              122              122
# 6                108                 108              108              108
# 7                174                 174              174              174
# 8                146                 146              146              146
# 9                 22                  22               22               22
# 10                22                  22               22               22
# 11                22                  22               22               22
# 12                22                  22               22               22
# 13                21                  21               21               21
# 14                21                  21               21               21
# 15                21                  21               21               21
# 16                21                  21               21               21
# 17                67                  67               67               67
# 18                22                  22               22               22
# 19                80                  80               80               80
# 20               146                 146              146              146
#
# One can then, for instance, restrict oneself to only using the
# algorithms which performed best (for a particular metric). So, for example,
# one might restrict oneself to novelty+, rsaps and sapsnr above, and allow
# more runs and a higher cutoff, like so
#
# > E2 = run_ubcsat("VanDerWaerden_2-5-5_200.cnf", cutoff=1000000, runs=200, include_algs=list("novp","rsaps","sapsnr"), seed=1255465118)
#
#    run sat min osteps msteps       seed    alg Clauses Variables TotalLiterals
# 1    1   0  22  53650 100000 1255465118   novp    9800       200         49000
# 2    2   0  22  25217 100000 3050844581   novp    9800       200         49000
# 3    3   0  22  35116 100000 1197107048   novp    9800       200         49000
# 4    4   0  22  46935 100000 1518106928   novp    9800       200         49000
# 5    5   0  22  57454 100000 2008792780   novp    9800       200         49000
# 6    6   0  22  48983 100000  390703691   novp    9800       200         49000
# 7    7   0  22  78871 100000  845786671   novp    9800       200         49000
# 8    8   0  22  19269 100000 4009731133   novp    9800       200         49000
# 9    9   0  22  15754 100000 1682572951   novp    9800       200         49000
# 10  10   0  22  32426 100000 1574280850   novp    9800       200         49000
# 11   1   0  21   1774 100000 1255465118  rsaps    9800       200         49000
# 12   2   0  21    458 100000 1524034911  rsaps    9800       200         49000
# 13   3   0  21   7313 100000 1884403664  rsaps    9800       200         49000
# 14   4   0  21  56454 100000 3676961508  rsaps    9800       200         49000
# 15   5   0  21  53036 100000 4290559716  rsaps    9800       200         49000
# 16   6   0  21  25687 100000 3943891847  rsaps    9800       200         49000
# 17   7   0  22  16190 100000 2407490954  rsaps    9800       200         49000
# 18   8   0  22    153 100000 3580297095  rsaps    9800       200         49000
# 19   9   0  21   1782 100000 1710561522  rsaps    9800       200         49000
# 20  10   0  21  16637 100000 1620589966  rsaps    9800       200         49000
# 21   1   0  21  46881 100000 1255465118 sapsnr    9800       200         49000
# 22   2   0  21    295 100000 1610127570 sapsnr    9800       200         49000
# 23   3   0  21  15008 100000 3747063770 sapsnr    9800       200         49000
# 24   4   0  21   1989 100000 2794788369 sapsnr    9800       200         49000
# 25   5   0  21  21829 100000  199888730 sapsnr    9800       200         49000
# 26   6   0  21  20411 100000 3864167715 sapsnr    9800       200         49000
# 27   7   0  21   7064 100000 2105493646 sapsnr    9800       200         49000
# 28   8   0  21   5761 100000 2653284662 sapsnr    9800       200         49000
# 29   9   0  22   8005 100000  350778943 sapsnr    9800       200         49000
# 30  10   0  22    520 100000 2865398216 sapsnr    9800       200         49000
#    FlipsPerSecond BestStep_Mean Steps_Mean Steps_Max PercentSuccess
# 1          276243       41367.5      1e+05     1e+05              0
# 2          276243       41367.5      1e+05     1e+05              0
# 3          276243       41367.5      1e+05     1e+05              0
# 4          276243       41367.5      1e+05     1e+05              0
# 5          276243       41367.5      1e+05     1e+05              0
# 6          276243       41367.5      1e+05     1e+05              0
# 7          276243       41367.5      1e+05     1e+05              0
# 8          276243       41367.5      1e+05     1e+05              0
# 9          276243       41367.5      1e+05     1e+05              0
# 10         276243       41367.5      1e+05     1e+05              0
# 11         207469       17948.4      1e+05     1e+05              0
# 12         207469       17948.4      1e+05     1e+05              0
# 13         207469       17948.4      1e+05     1e+05              0
# 14         207469       17948.4      1e+05     1e+05              0
# 15         207469       17948.4      1e+05     1e+05              0
# 16         207469       17948.4      1e+05     1e+05              0
# 17         207469       17948.4      1e+05     1e+05              0
# 18         207469       17948.4      1e+05     1e+05              0
# 19         207469       17948.4      1e+05     1e+05              0
# 20         207469       17948.4      1e+05     1e+05              0
# 21         198807       12776.3      1e+05     1e+05              0
# 22         198807       12776.3      1e+05     1e+05              0
# 23         198807       12776.3      1e+05     1e+05              0
# 24         198807       12776.3      1e+05     1e+05              0
# 25         198807       12776.3      1e+05     1e+05              0
# 26         198807       12776.3      1e+05     1e+05              0
# 27         198807       12776.3      1e+05     1e+05              0
# 28         198807       12776.3      1e+05     1e+05              0
# 29         198807       12776.3      1e+05     1e+05              0
# 30         198807       12776.3      1e+05     1e+05              0
#    BestSolution_Mean BestSolution_Median BestSolution_Min BestSolution_Max
# 1               22.0                  22               22               22
# 2               22.0                  22               22               22
# 3               22.0                  22               22               22
# 4               22.0                  22               22               22
# 5               22.0                  22               22               22
# 6               22.0                  22               22               22
# 7               22.0                  22               22               22
# 8               22.0                  22               22               22
# 9               22.0                  22               22               22
# 10              22.0                  22               22               22
# 11              21.2                  21               21               22
# 12              21.2                  21               21               22
# 13              21.2                  21               21               22
# 14              21.2                  21               21               22
# 15              21.2                  21               21               22
# 16              21.2                  21               21               22
# 17              21.2                  21               21               22
# 18              21.2                  21               21               22
# 19              21.2                  21               21               22
# 20              21.2                  21               21               22
# 21              21.2                  21               21               22
# 22              21.2                  21               21               22
# 23              21.2                  21               21               22
# 24              21.2                  21               21               22
# 25              21.2                  21               21               22
# 26              21.2                  21               21               22
# 27              21.2                  21               21               22
# 28              21.2                  21               21               22
# 29              21.2                  21               21               22
# 30              21.2                  21               21               22
#
# One can see that only 3 algorithms have been run, and each has had 10 runs.
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
#     By default this is
#       ubcsat_tmp_${FILENAME}
#     where ${FILENAME} is the given filename.
#
run_ubcsat_temp_dir = function(filename) {
  return ( paste("ubcsat_tmp_",filename,"_",
                 format(Sys.time(), "%Y-%m-%d-%H%M%S"), sep="") )
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
old_run_ubcsat_cnf_algs = list(
  gsat="gsat",
  gsats="gsat -v simple",
  gwsat="gwsat",
  gsatt="gsat-tabu",
  hsat="hsat",
  hwsat="hwsat",
  wsat="walksat",
  wsatt="walksat-tabu",
  nov="novelty",
  novp="novelty+",
  anovp="adaptnovelty+",
  rnov="rnovelty",
  rnovp="rnovelty+",
  saps="saps",
  rsaps="rsaps",
  sapsnr="sapsnr",
  rots="rots",
  irots="irots",
  samd="samd",
  wsattn="walksat-tabu -v nonull"
)
run_ubcsat_cnf_algs = append(old_run_ubcsat_cnf_algs, list(
  novpc="novelty+ -v cache",
  novpp="novelty++",
  novp_p="novelty+p",
  anovpp="adaptnovelty+ -v params",
  paws="paws",
  ddfw="ddfw",
  g2wsat="g2wsat",
  ag2wsat="adaptg2wsat",
  vw1="vw1",
  vw2="vw2",
  uwalk="urwalk",
  cwalk="crwalk",
  cwalks="crwalk -v schoening",
  dwalk="dcrwalk",
  dano="danov+",
  rgsat="rgsat",
  jack="jack",
  vw25="vw2 -v 2005",
  lllw="lllwalk"
))

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
#     tmp_directory
#       The path (string) to the temporary directory where run_ubcsat
#       stores it's temporary files for the given input filename.
#     alg_safe_name
#       An alphanumeric string used as a reference for ubcsat-okl algorithm.
#       For example for "gsat -v simple" one might use "gsat_simple".
#
# Result :
#     The path (string) to the log file run_ubcsat generates for the
#     given parameters.
#
#     By default this is
#
#     ${TMP}/${ALG}.run_ubcsat_log
#
#     where ${TMP} is the given temporary directory, and ${ALG} is the
#     algorithm run with the corresponding invocation of ubcsat-okl on the
#     file.
#
run_ubcsat_log_path = function(
  tmp_directory, alg_safe_name) {
  return (paste(tmp_directory, "/",
                alg_safe_name,".run_ubcsat_log",
                sep = ""))
}

# Returns the path to the result file run_ubcsat generates for the given
# parameters.
#
# Parameters :
#     tmp_directory
#       The path (string) to the temporary directory where run_ubcsat
#       stores it's temporary files for the given input filename.
#     alg_safe_name
#       An alphanumeric string used as a reference for ubcsat-okl algorithm.
#       For example for "gsat -v simple" one might use "gsat_simple".
#
# Result :
#     The path (string) to the result file run_ubcsat generates for the
#     given parameters. The word "result" here simply means the output
#     from ubcsat-okl generated when run_ubcsat ran the problem for the
#     given algorithm.
#
#     By default this is
#
#     ${TMP}/${ALG}.run_ubcsat_result
#
#     where ${TMP} is the given temporary directory, and ${ALG} is the
#     algorithm run with the corresponding invocation of ubcsat-okl on the
#     file.
#
run_ubcsat_result_path = function(tmp_directory, alg_safe_name) {
  return(paste(tmp_directory, "/",
               alg_safe_name, ".run_ubcsat_result",
               sep=""))
}

# Returns the path to the statistics file run_ubcsat generates for the given
# parameters.
#
# Parameters :
#     tmp_directory
#       The path (string) to the temporary directory where run_ubcsat
#       stores it's temporary files for the given input filename.
#     alg_safe_name
#       An alphanumeric string used as a reference for ubcsat-okl algorithm.
#       For example for "gsat -v simple" one might use "gsat_simple".
#
# Result :
#     The path (string) to the statistics file run_ubcsat generates for the
#     given parameters.
#
#     By default this is
#
#     ${TMP}/${ALG}.run_ubcsat_stats
#
#     where ${TMP} is the given temporary directory, and ${ALG} is the
#     algorithm run with the corresponding invocation of ubcsat-okl on the
#     file.
run_ubcsat_stats_path = function(tmp_directory, alg_safe_name) {
  return(paste(tmp_directory, "/",
               alg_safe_name,".run_ubcsat_stats",
               sep=""))
}

# ??? THE FOLLOWING IS APPARENTLY FALSE, AND IT IS MUCH TOO LONG AND MUCH
# TOO IMRPECISE ???
# Takes the following parameters for which "run_ubcsat" must already
# been run and then returns the result of the corresponding
# "run_ubcsat" command, reading the results from the previously
# generated files.
# 
# For example, if one has run run_ubcsat on a file X generating
# result files in the ubcsat_X/ directory then one can run
#
# > read_ubcsat_dir(X, ...)
#
# where "..." simply assumes you provide all the arguments (for
# include_algs, exclude_algs, and tmp_directory) as were provided
# to the run_ubcsat when ubcsat_X/ was created.
#
# The result is then simply the data.frame as original produced by
# run_ubcsat when ubcsat_X was created.
#
# This is useful for reading in experiment data which has been created
# before but the original R session is no longer running.
#
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
  directory,
  include_algs = names(run_ubcsat_cnf_algs),
  exclude_algs = list()) {
  
  # Create the temporary directory (error if it doesn't exist)
  if ( ! file.exists(directory)) {
      print(paste("ERROR[read_ubcsat_dir]: Unable to open directory '",
                  directory, "'."))
      return(FALSE)
  }

  # Get only those algorithms in the included list which
  # are not excluded.
  algs = include_algs[!(include_algs %in% exclude_algs)]

  run_ubcsat_df = NULL
  for (alg in algs ) {
    try({
      output_file =
        run_ubcsat_result_path(directory, alg)
      stats_output_file =
        run_ubcsat_stats_path(directory, alg)
    
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
#       stores it's temporary files (log files etc).
#     ...
#       Additional parameters to ubcsat-okl can be specified as additional
#       parameters to this function, so adding "runs=1", would
#       result in the result command including "-runs=1" (Optional).
#
# Result :
#     The ubcsat-okl command (string) running on the given input, using
#     the given algorithm, outputting to the default output files in
#     the given temporary directory (see run_ubcsat_result_path,
#     run_ubcsat_stats_path and run_ubcsat_temp_dir), with the given
#     additional parameters.
#
run_ubcsat_command = function(
  input, alg_safe_name, alg_name,
  tmp_directory, wrapper, ...) {

  filename = basename(input)
  output_file = run_ubcsat_result_path(tmp_directory, alg_safe_name)
  stats_output_file =
      run_ubcsat_stats_path(tmp_directory, alg_safe_name)
  
  # Setup parameter string
  std_params = ""
  params = list(...)
  for (param_name in names(params)) {
    std_params = paste(std_params," -",param_name, " ",
      format(params[[param_name]],scientific=5000),sep="")
  }

  return( paste(wrapper,
                " -r out '", output_file, "' ",
                " -r stats '", stats_output_file, "' ",
                std_params," -alg ", alg_name, " -i ",input, " 2>&1 > ",
                run_ubcsat_log_path(tmp_directory, alg_safe_name),
                sep="") )
}

