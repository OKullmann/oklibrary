# Oliver Kullmann, 26.7.2009 (Swansea)
# Copyright 2009 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

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

# Plotting levels -> average-nodes and levels -> nodes.
# Use "cut" to remove the rows from 1 to cut.
# Use "step" to draw vertical lines at levels which are multiples of 2^step.
# If 2^step is larger than the maximum level, then just the default x-axis
# annotation is used (for the lower plot).
plot_oksolver_mon_nodes = function(E, cut=100, step=8, ...) {
  step = 2^step
  old_mfrow = par("mfrow")
  old_mar = par("mar")
  par(mfrow=c(2,1), mar=c(0,3,3,2))
  m = max(E$level)
  if (step <= m) {
    S = seq(step,m,step)
    plot(E$level[-(1:cut)], E$ave_nodes[-(1:cut)], xaxt="n", ann=F, ...)
    axis(3, at=S, labels=seq(1,m/step))
    abline(v=S)
    par(mar=c(3,3,0,2))
    plot(E$level[-(1:cut)], E$nodes[-(1:cut)], xaxt="n", ann=F, ...)
    axis(1, at=S, labels=seq(1,m/step))
    abline(v=S)
  }
  else {
    plot(E$level[-(1:cut)], E$ave_nodes[-(1:cut)], xaxt="n", ann=F, ...)
    par(mar=c(3,3,0,2))
    plot(E$level[-(1:cut)], E$nodes[-(1:cut)], ...)
  }
  par(mfrow=old_mfrow, mar=old_mar)
}

