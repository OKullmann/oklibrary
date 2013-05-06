# Oliver Kullmann, 20.9.2011 (Swansea)
# Copyright 2011, 2013 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

# ###################################
# # Helper functions for statistics #
# ###################################

# Range difference for vector x:
ranged <- function(x) {
  r <- range(x);
  return(r[2]-r[1])
}

# Prints basic statistics of vector x:
basic_stats = function(x, qpoints = seq(0.95, 1, 0.01)) {
  print(summary(x))
  cat("sd=", sd(x), "\n")
  print(quantile(x,probs = qpoints))
  cat("sum=", sum(x), "\n")
}

# Aggregate a data.frame D by the fields in "by", using basic statistics
# functions (sd, mean, min, max) returning the aggregated result of each
# statistic over those fields for each column col, as a column col.stat_name.
#
# The parameters are the same as aggregate function, except that FUN should
# not be passed as an argument, and "by" should be a list of column names
# as strings of the fields to be aggregated.
#
# The result is a data.frame with columns col.mean, col.sd, col.min, col.max
# for each column col in D, excluding those in "by", which are included
# directly in the result from D.
# There is a row in the result for every unique setting of the columns in "by"
# in D. Each element in the result data.frame is the mean (sd, min, max etc)
# of all cells in the corresponding column in D, which have the same values
# for the columns in "by" as this row.
aggregate_statistics = function(D, by, ...) {
  by_ind_list = lapply(by,FUN=function(x) D[,x])
  names(by_ind_list) = unlist(by)
  D_mean = aggregate(D, by=by_ind_list, FUN=mean, ...)
  D_sd = aggregate(D, by=by_ind_list, FUN=sd, ...)
  D_min = aggregate(D, by=by_ind_list,  FUN=min, ...)
  D_max = aggregate(D, by=by_ind_list, FUN=max, ...)

  D_res = data.frame(D_mean[unlist(by)])
  for (col in colnames(D)) {
    if (!(col %in% by)) {
      D_res[paste(col, ".", "mean",sep="")] = D_mean[col]
      D_res[paste(col, ".", "sd",sep="")] = D_sd[col]
      D_res[paste(col, ".", "min",sep="")] = D_min[col]
      D_res[paste(col, ".", "max",sep="")] = D_max[col]
    }
  }

  D_res
}
# Example:
#
# > E = data.frame(aggr=c(1,2,1,2), a=c(1,2,3,4), b=c(5,100,200,1000))
# > E
#
#   aggr a    b
# 1    1 1    5
# 2    2 2  100
# 3    1 3  200
# 4    2 4 1000
#
# > aggregate_statistics(E, by=list("aggr"))
#
#   evens a.mean a.sd a.min a.max b.mean b.sd b.min b.max
# 1     1      2    3     1     3  102.5  200     5   200
# 2     2      3    4     2     4  550.0 1000   100  1000
#


# ####################################
# # Helper functions for regressions #
# ####################################

# For the object obtained by lm, print a shorter summary:
short_summary_lm = function(L) {
  S = summary(L)
  printCoefmat(S$coefficients, signif.legend=FALSE)
  digits = max(3,getOption("digits")-3)
  cat("R-squared:", formatC(S$r.squared, digits = digits), "\n")
}
# Remark: Obtained by inspecting the code of summary.lm by
# "getAnywhere(print.summary.lm)".


# #############################
# # Sorting and related tasks #
# #############################

# Sort the elements of vector x in ascending order, removing duplicates:
orderedset <- function(x) {
  l = length(x)
  if (l <= 1) return(x)
  x = sort(x, method="quick")
  sel = vector("logical", l)
  sel[1] = TRUE
  for (i in 2:l) sel[i] = x[i] > x[i-1]
  return(x[sel])
}

# Inefficient but easy implementention of lexicographical ordering
# of a matrix / 2-dimensional array according to its columns,
# in decreasing order:
lexico_col_dec = function(X) {
  for (i in nrow(X):1) X <- X[, order(X[i,],decreasing=TRUE)]
  X
}


# #################
# # Time handling #
# #################

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


# ###################
# # String handling #
# ###################

# Removes leading and trailing whitespaces from string x:
trim = function(x) gsub("^\\s+|\\s+$", "", x)

# Removes leading non-digit characters:
remleadnondigit = function(x) gsub("^\\D+", "", x)

# Returns the part of non-emtpy string x before the first "." (so that
# filename-extensions can be removed in this way):
remextension = function(x) strsplit(x,".",fixed=TRUE)[[1]][1]

# Splitting a string along underscore-characters; returns a vector of strings:
underscore_splitting = function(x) strsplit(x,"_")[[1]]

# Splitting a filename along the underscores, ignoring a filename-extension:
filename_splitting = function(x) underscore_splitting(remextension(x))


# #################
# # File handling #
# #################

# The number of strings in the first line in the file with name f:
lengthfirstline = function(f) {
  length(unlist(strsplit(trim(readLines(f,1)), " ")))
}
