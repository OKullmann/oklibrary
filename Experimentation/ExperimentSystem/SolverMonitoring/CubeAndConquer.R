# Oliver Kullmann, 15.8.2011 (Swansea)
# Copyright 2011, 2012 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

# Tools for evaluating the results of ProcessSplitViaOKsolver, and other
# tools following the "Cube and Conquer" approach.


oklib_load("OKlib/Statistics/R/Utilities.R")


# Reads the statistics-file when minisat (in compatible versions) was used,
# prints some basic statistics, and returns the data-frame.
# Usages:
# - E=read_processsplit_minisat() reads SubinstanceStatistics in current dir
# - E=read_processsplit_minisat(dir) reads dir/SubinstanceStatistics
# - E=read_processsplit_minisat(dir,file) reads dir/file
# dirname and file can also be specified explicitly.
# Via summary(E) one obtains a reasonable summary, and via plot(E) one
# can view it.
# Can be used for the directory obtained when processing the complete
# partial assignments (ProcessSplitViaOKsolver, or ExtractiCNF followed by
# ProcessiCNF) as well as for the directory obtained when processing only the
# decisions (ExtractDecisionsiCNF followed by ProcessiCNF).

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
        colClasses = c(rep("integer",lengthfirstline(filename)-8-1),"numeric","integer",rep("numeric",8)),
        ...)
  cat(sprintf("%d: %s, sum-cfs=%e, mean-t=%.3fs, mean-cfs=%.0f, sat:",length(E$t),display_seconds(sum(E$t)),sum(E$cfs),mean(E$t),mean(E$cfs)),orderedset(E$sat),"\n")
  cat("$t:\n")
  basic_stats(E$t)
  cat("$cfs:\n")
  basic_stats(E$cfs)
  L = lm(E$t ~ E$cfs)
  cat("$t ~ $cfs:\n")
  short_summary_lm(L)
  E
}

read_processsplit_satz = function(dirname, file, ...)  {
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
        colClasses = c(rep("integer",6),"numeric","integer",rep("numeric",4),"character",rep("numeric",5)),
        ...)
  cat(sprintf("%d: %s, sum-nds=%e, mean-t=%.3fs, mean-nds=%.0f, sat:",length(E$t),display_seconds(sum(E$t)),sum(E$nds),mean(E$t),mean(E$nds)),orderedset(E$sat),"\n")
  cat("$t:\n")
  basic_stats(E$t)
  cat("$nds:\n")
  basic_stats(E$nds)
  L = lm(E$t ~ E$nds)
  cat("$t ~ $nds:\n")
  short_summary_lm(L)
  E
}

