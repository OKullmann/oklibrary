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

# First attempt, just showing all results in table form:
eval_ubcsat_dataframe = function(E) {
  for (A in names(run_ubcsat_cnf_algs)) {
    cat(A,": ")
    print(table(E$best[E$alg==A]))
  }
}
