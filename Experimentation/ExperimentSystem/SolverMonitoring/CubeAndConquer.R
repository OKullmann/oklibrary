# Oliver Kullmann, 15.8.2011 (Swansea)
# Copyright 2011 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

# Tools for evaluating the results of ProcessSplitViaOKsolver, and other
# tools following the "Cube and Conquer" approach.

# Helper functions for statistics

# Prints basic statistics of vector x:
basic_stats = function(x, qpoints = seq(0.95, 1, 0.01)) {
  print(summary(x))
  cat("sd=", sd(x), "\n")
  print(quantile(x,probs = qpoints))
  cat("sum=", sum(x), "\n")
}

# Converts x (in seconds) into a string using seconds, minutes, hours, days
# or years appropriately:
display_seconds = function(x) {
  if (x <= 60) return(paste(round(x,2),"s",sep=""))
  x = x / 60
  if (x <= 60) return(paste(round(x,2),"m",sep=""))
  x = x / 60
  if (x <= 24) return(paste(round(x,3),"h",sep=""))
  x = x / 24
  if (x <= 365) return(paste(round(x,3),"d",sep=""))
  x = x / 365
  paste(round(x,4),"y")
}

# For the object obtained by lm, print a shorter summary:
short_summary_lm = function(L) {
  S = summary(L)
  printCoefmat(S$coefficients, signif.legend=FALSE)
  digits = max(3,getOption("digits")-3)
  cat("R-squared:", formatC(S$r.squared, digits = digits), "\n")
}
# Remark: Obtained by inspecting the code of summary.lm by
# "getAnywhere(print.summary.lm)".



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
    else filename = paste(dirname, "SubinstanceStatistics", sep="/")
  }
  else {
    if (missing(dirname)) filename = file
    else filename = paste(dirname, file, sep="/")
  }
  E = read.table(file = filename,
        header = T,
        colClasses = c(rep("integer",5),"numeric","integer",rep("numeric",8)),
        ...)
  cat(sprintf("%d: %s, sum-cfs=%e, mean-t=%.3fs, mean-cfs=%.0f",length(E$t),display_seconds(sum(E$t)),sum(E$cfs),mean(E$t),mean(E$cfs)),"\n")
  cat("$t:\n")
  basic_stats(E$t)
  cat("$cfs:\n")
  basic_stats(E$cfs)
  L = lm(E$t ~ E$cfs)
  cat("$t ~ $cfs:\n")
  short_summary_lm(L)
  E
}
