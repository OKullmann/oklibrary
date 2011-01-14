# Oliver Kullmann, 4.6.2009 (Swansea)
# Copyright 2009, 2010 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

# Required for run_ubcsat_cnf_algs in eval_ubcsat_dataframe
oklib_load("OKlib/Experimentation/ExperimentSystem/ControllingLocalSearch/DataCollection.R")

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
# "row.names=NULL". If m leading lines are to be ignored, use
# "skip=m". If trailing lines are to be ignored, use "nrows=n" for the
# number of rows to be selected.

# #######################################################
# # Evaluation functions for series of experiment files #
# #######################################################

# Takes a directory prefix, as well as a list of parameter names, and
# a filename exp_filename and returns a data.frame containing the data from
# exp_filename in every directory in the current working directory with the
# name prefix followed by an arbitrary number of underscore separated
# parameters as the name.
#
# The parameters in the filename are added to the data.frame as a column
# with the name given by the associated element of the parameter name list.
read_experiment_dirs = function(prefix, param_names, exp_filename, ...) {
  prefix_dirs = dir(pattern=paste(prefix,".*",sep=""))
  prefix_dirs = Filter(function(d) file.info(d)["isdir"][[1]], prefix_dirs)
  result_df = NULL
  num_processed_dirs = 0
  for (dir in prefix_dirs) {
    if (file.info(dir)["isdir"][[1]]) {
      param_string = substring(dir, nchar(prefix)+2)
      parameters = unlist(strsplit(param_string, "_"))
      
      dir_df = tryCatch(
        read.table(paste(dir,"/",exp_filename,sep=""),...),
        error = function(e) NULL, warning=function(e) NULL)
      if (!is.null(dir_df)) {
        print(
              paste("[",num_processed_dirs,"/", length(prefix_dirs),
                    "]: Reading ", paste(dir,"/",exp_filename,".",sep="")))
        for (i in 1:length(parameters)) {
          dir_df = add_constant_column(dir_df, parameters[i], param_names[i])
        }
        num_processed_dirs = num_processed_dirs + 1
      } else {
        print(paste(
                    "WARNING[read_experiment_dirs]: Skipping '",
                    dir,"/",exp_filename,"' due to error.", sep=""))
      }
      result_df = rbind(result_df,dir_df)
    }
  }
  print(paste("Processed [",
              num_processed_dirs, "/", length(prefix_dirs),
              "] experiment directories.",sep=""))
  return(result_df)
}

# #######################################
# # Evaluation functions for run_ubcsat #
# #######################################

# See DataCollection.R.

# First attempt, just showing all results in table form:
eval_ubcsat_dataframe = function(E) {
  for (A in names(run_ubcsat_cnf_algs)) {
    cat(A,": ")
    print(table(E$min[E$alg==A]))
  }
}
