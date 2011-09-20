# Oliver Kullmann, 290.9.2011 (Swansea)
# Copyright 2011 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

# Tools for evaluating the results of experiments in Ramsey theory
# (using complete solvers).


# Reads the statistics-file produced by CRunPdVdWk1k2 when minisat (in
# compatible versions) was used, prints some basic statistics, plots the
# relation between vertex number (in original vdW-problem) and
# conflict-numbers, and returns the data-frame.
# Usages:
# - E=read_crunpdvdw_minisat() reads Statistics in current dir
# - E=read_crunpdvdw_minisat(dir) reads dir/Statistics
# - E=read_crunpdvdw_minisat(dir,file) reads dir/file
# dirname and file can also be specified explicitly.
# Via summary(E) one obtains a reasonable summary, and via plot(E) one
# can view it.
# In the plot, (empty) circles are used for even vertex-numbers, (empty)
# triangles for odd vertex-numbers, and unsatisfiable problems are shown
# by added full points in red.
read_crunpdvdw_minisat = function(dirname, file, ...)  {
  if (missing(file)) {
    if (missing(dirname)) filename = "Statistics"
    else filename = paste(dirname, "Statistics", sep="/")
  }
  else {
    if (missing(dirname)) filename = file
    else filename = paste(dirname, file, sep="/")
  }
  E = read.table(file = filename,
        header = T,
        colClasses = c(rep("integer",4),"numeric","integer",rep("numeric",8)),
        ...)
  cat("$t:\n")
  basic_stats(E$t)
  cat("$cfs:\n")
  basic_stats(E$cfs)
  L = lm(E$t ~ E$cfs)
  cat("$t ~ $cfs:\n")
  short_summary_lm(L)
  plot(E$v, E$cfs,type="n")
  m = E$v %% 2
  points(E$v[m==0],E$cfs[m==0],pch=1)
  points(E$v[m==1],E$cfs[m==1],pch=2)
  lines(E$v, E$cfs)
  points(E$v[E$sat==0],E$cfs[E$sat==0],pch=20,col="red")
  E
}
