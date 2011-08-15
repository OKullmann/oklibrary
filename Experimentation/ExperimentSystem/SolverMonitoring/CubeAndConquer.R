# Oliver Kullmann, 15.8.2011 (Swansea)
# Copyright 2011 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

# Tools for evaluating the results of ProcessSplitViaOKsolver, and other
# tools following the "Cube and Conquer" approach.



# Reads the statistics-file when minisat (in some version) was used, prints
# some basic statistics, and returns the data-frame.
# Usages:
# - E=read_processsplit_minisat() reads SubinstanceStatistics in current dir
# - E=read_processspliy_minisat(dir) reads dir/SubinstanceStatistics
# - E=read_processspliy_minisat(dir,file) reads dir/file
# dirname and file can also be specified explicitly.
# Via summary(E) one obtains a reasonable summary, and via plot(E) one
# can view it.
read_processsplit_minisat = function(dirname, file, ...)  {
  if (missing(file)) {
    if (missing(dirname)) filename = "SubinstanceStatistics"
    else filename = paste(dirname, "/SubinstanceStatistics", sep="")
  }
  else {
    if (missing(dirname)) filename = file
    else filename = paste(dirname, file, sep="")
  }
  E = read.table(file = filename,
        header = T,
        colClasses = c(rep("integer",5),"numeric","integer",rep("numeric",8)),
        ...)
  cat(sprintf("%d: %.2fh, sum-cfs=%e, mean-t=%.3fs, mean-cfs=%.0f",length(E$t),sum(E$t)/60/60,sum(E$cfs),mean(E$t),mean(E$cfs)),"\n")
  print(summary(E$t))
  print(summary(E$cfs))
  E
}
