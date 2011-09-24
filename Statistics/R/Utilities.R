# Oliver Kullmann, 20.9.2011 (Swansea)
# Copyright 2011 Oliver Kullmann
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


# #################
# # File handling #
# #################

# The number of strings in the first line in the file with name f:
lengthfirstline = function(f) {
  length(unlist(strsplit(trim(readLines(f,1)), " ")))
}
