# Oliver Kullmann, 4.6.2009 (Swansea)
# Copyright 2009 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

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


# Reading monitoring-data produced by OKsolver_2002:
read_oksolver_mon = function(filename, ...) {
  E = read.table(file = filename, header=T,
        colClasses = c("integer", "integer", "numeric", "numeric", "numeric"),
        ...)
  cat(length(E$level),"\n")
  E
}
# A first line
#    level  nodes  ave_nodes      time  ave_time
# is assumed.
# Remarks:
# plot(E) yields a nice graphical overview (all combinations of combined
# data plots); but often, say, plot(E[-(1:100),]) is more appropriate,
# which eliminates the first 100 rows from the data frame.

