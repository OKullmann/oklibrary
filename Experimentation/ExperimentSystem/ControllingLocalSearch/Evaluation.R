# Oliver Kullmann, 4.6.2009 (Swansea)
# Copyright 2009, 2010, 2011 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

oklib_load("OKlib/Statistics/R/Utilities.R")


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


# #######################################
# # Evaluation functions for run_ubcsat #
# #######################################

# See DataCollection.R for run_ubcsat.

# Sorting the table form of the results in decreasing
# lexicographical order, that is, sorting first according to min-value
# reached (the lower the better), and second by count obtained (the higher the
# better).
# Showing also a plot of table-data, and the flips-per-second.
eval_ubcsat_dataframe = function(E, plotting=TRUE) {
  if (plotting) plot(E$alg,E$min)
  i=1
  for (A in dimnames(lexico_col_dec(table(E$min,E$alg)))[[2]]) {
    cat(i,". ", A,": ", sep="")
    print(table(E$min[E$alg==A]))
    cat("fps:", max(E$FlipsPerSecond[E$alg==A]), "\n")
    i = i + 1
  }
}
