# Oliver Kullmann, 26.7.2009 (Swansea)
# Copyright 2009, 2011, 2012 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.


# #######
# Input #
# #######

# Reading monitoring-data produced by OKsolver_2002:
read_oksolver_mon = function(filename, ...) {
  E = read.table(file = filename, header=T,
        colClasses = c("integer", "integer", "numeric", "numeric", "numeric", "integer", "integer", "integer", "numeric"),
        ...)
  cat(length(E$level),"\n")
  E
}
# A first line
#    level  nodes ave_nodes      time  ave_time singles autarkies depth ave_reductions
# is assumed.
# Remarks:
# plot(E) yields a nice graphical overview (all combinations of combined
# data plots); but often, say, plot(E[-(1:100),]) is more appropriate,
# which eliminates the first 100 rows from the data frame.


# Reading the output of OKsolver_2002 solver from an R-like file, as produced
# by ExtractOKsolver (with header-line, and every line starting with the
# running number), and putting it into a dataframe.
# Can be also used when to the front (after the running number) various
# integer-fields are added (for example as produced by the CRun-scripts).
read_oksolver = function(filename, ...) {
  E = read.table(file = filename,
        header = T,
        colClasses = c(rep("integer",lengthfirstline(filename)-25),"numeric","integer",rep("numeric",5),"integer","character",rep("integer",5),rep("numeric",10),"character","numeric"),
        ...)
  E
}



# ##############
# # Evaluation #
# ##############

# Plotting levels -> average-nodes and levels -> nodes.
# Use "left" and "right" to restrict to the observation-nodes within this
# level-range (default is from observation 128 to the maximum).
# Use "ldstep" to draw vertical lines at levels which are multiples of 
# step = 2^ldstep (step can also be specified directly, thus ignoring
# ldstep).
# If step is larger than the maximum level, then just the default x-axis
# annotation is used (for the lower plot).
# All further parameters are passed to both plot commands; so e.g. with log="y"
# you can use a logarithmic scale for the values, etc.
plot_oksolver_mon_nodes = function(E, left=128, right=max(E$level), ldstep=round(log2(right-left))-3, step=2^ldstep, ...) {
  E = E[E$level >= left & E$level <= right,]
  old_mfrow = par("mfrow")
  old_mar = par("mar")
  par(mfrow=c(2,1), mar=c(0,3,3,2))
  if (step >= 1 && step <= right) {
    S = seq(step,right,step)
    plot(E$level, E$ave_nodes, xaxt="n", ann=F, ...)
    axis(3, at=S, labels=seq(1,right/step))
    abline(v=S)
    par(mar=c(3,3,0.2,2))
    plot(E$level, E$nodes, xaxt="n", ann=F, ...)
    axis(1, at=S, labels=seq(1,right/step))
    abline(v=S)
  }
  else {
    plot(E$level, E$ave_nodes, xaxt="n", ann=F, ...)
    par(mar=c(3,3,0.2,2))
    plot(E$level, E$nodes, ...)
  }
  par(mfrow=old_mfrow, mar=old_mar)
  cat("ldstep=",ldstep,"step=",step,"left=",left,"right=",right,"\n")
  cat("obs/count=", (right-left+1)/length(E$level), "nodes-range=", range(E$nodes), "ave-nodes-range=", range(E$ave_nodes), "\n")
}

# Printing some basic summary statistics, and linear regression information,
# and showing the resulting plots:
summary_oksolver = function(E, ...) {
  cat("Nodes:\n")
  print(summary(E$nodes))
  cat("2-reductions:\n")
  print(summary(E$ave_reductions[E$ave_reductions>0]))
  cat("Single nodes:\n")
  print(summary(E$singles))
  cat("Autarkies:\n")
  print(summary(E$autarkies))

  cat("Time ~ nodes:\n")
  TN = lm(E$time ~ E$nodes)
  print(summary(TN)$r.squared)
  print(coefficients(TN))
  cat("Single nodes ~ nodes:\n")
  SN = lm(E$singles ~ E$nodes)
  print(summary(SN)$r.squared)
  print(coefficients(SN))
  cat("Autarkies ~ nodes:\n")
  AN = lm(E$autarkies ~ E$nodes)
  print(summary(AN)$r.squared)
  print(coefficients(AN))

  old_mfrow = par("mfrow")
  old_mar = par("mar")
  par(mfrow=c(3,1))
  par(mar=c(0,4,1,1))
  plot(E$nodes,E$time, xaxt="n", ...)
  lines(E$nodes,predict(TN))
  par(mar=c(0,4,1,1))
  plot(E$nodes,E$singles, xaxt="n", ...)
  lines(E$nodes,predict(SN))
  par(mar=c(5,4,1,1))
  plot(E$nodes,E$autarkies, ...)
  lines(E$nodes,predict(AN))
  par(mfrow=old_mfrow, mar=old_mar)
}


# The histogram for the distribution of the binary logarithm of node counts,
# with the median as solid line, and the mean as dotted line:
hist_oksolver_mon_nodes = function(E, breaks=30, ...) {
  L = log2(E$nodes)
  hist(L,main="",breaks)
  Median = median(L)
  Mean = log2(mean(E$nodes))
  cat("Median=",Median,"\n")
  cat("Mean=",Mean,"\n")
  abline(v=Median)
  abline(v=Mean,lty=3)
}
# For the default number of break-points, use 
# breaks="Sturges"

