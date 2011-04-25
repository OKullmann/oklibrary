# Oliver Kullmann, 26.7.2009 (Swansea)
# Copyright 2009, 2011 Oliver Kullmann
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


# Reading the output of an OKsolver_2002 computation from stats_filename and
# returning a data.frame containing the statistics on the computation.
# Inputs:
#   stats_filename
#     The filename containing the output of a run of the OKsolver_2002.
# Output:
#   A data.frame with a single row with the following fields in the
#   following order:
#
#     sat ({0,1,2}): SATISFIABLE (1), UNSATISFIABLE (0), UNKNOWN (2).
#     init_max_cl (pos int): Initial maximum clause-length.
#     n (pos int): Initial number of variables.
#     c (pos int): Initial number of clauses.
#     l (pos int): Initial number of literal occurrences.
#     init_unit (pos int): Number of unit-clause propagations.
#     diff_max_cl (pos int): Difference between maximum clause-length before
#       and after preprocessing.
#     diff_n (pos int): Diff. in variables before/after preprocessing.
#     diff_c (pos int): Diff. in clauses before/after preprocessing.
#     diff_l (pos int): Diff. in literal occurrences before/after preproc.
#     bin_c (pos int): Number of 2-clauses after preproc.
#     time (double): Total time in seconds to solve the problem.
#     nodes (pos int): Number of nodes in the search tree.
#     single_nodes (pos int): Number of single nodes in search tree.
#     quasi_single_nodes (pos int): Number of quasi-single nodes.
#     r2 (pos int): Number of r_2-reductions.
#     pure_lits (pos int): Pure literals found during search.
#     aut (pos int): Number of autarkies.
#     missed_1nodes (pos int): Nodes which would have been found as
#       single nodes if the the "other" branch had been chosen first.
#     depth (pos int): Maximum depth of the search tree.
#     tab_enlarge (pos int): Table enlargements during the search.
#     aut1 (pos int): Number of 1-autarkies. A 1-autarky satisfies all clauses
#       except one.
#     new_2c (pos int): New binary clauses learnt.
#     max_added_2c (pos int): Maximum number of new binary clauses added.
#     filename (string): Name of the DIMACS file input to OKsolver_2002.
#
read_oksolver_output = function(filename, ...) {
  S = system(paste("cat ", filename," | grep \"^c\\|s\""), intern=TRUE)
  result = list()
  for (line in S) {
    line = gsub("^[cs] *", "",line)
    if (length(grep("sat_status",line)) > 0) {
      result = c(result, list(sat = as.integer(gsub("([^0-9]|_[0-9]+)","",line))))
    } else if (length(grep("initial_maximal_clause_length",line)) > 0) {
      result = c(result,
        list(init_max_cl = as.integer(gsub("([^0-9]|_[0-9]+)","",line))))
    } else if (length(grep("initial_number_of_variables", line)) > 0) {
      result = c(result,
        list(n = as.integer(gsub("([^0-9]|_[0-9]+)","",line))))
    } else if (length(grep("initial_number_of_clauses",line)) > 0) {
      result = c(result,
        list(c = as.integer(gsub("([^0-9]|_[0-9]+)","",line))))
    } else if (length(grep("initial_number_of_literal_occurrences",line)) > 0) {
      result = c(result,
        list(l = as.integer(gsub("([^0-9]|_[0-9]+)","",line))))
    } else if (length(grep("number_of_initial_unit-eliminations",line)) > 0) {
      result = c(result,
        list(init_unit = as.integer(gsub("([^0-9]|_[0-9]+)","",line))))
    } else if (length(grep("reddiff_maximal_clause_length",line)) > 0) {
      result = c(result,
        list(diff_max_cl = as.integer(gsub("([^0-9]|_[0-9]+)","",line))))
    } else if (length(grep("reddiff_number_of_variables",line)) > 0) {
      result = c(result, list(diff_n = as.integer(gsub("[^0-9]","",line))))
    } else if (length(grep("reddiff_number_of_clauses",line)) > 0) {
      result = c(result, list(diff_c = as.integer(gsub("[^0-9]","",line))))
    } else if (length(grep("reddiff_number_of_literal",line)) > 0) {
      result = c(result, list(diff_l = as.integer(gsub("[^0-9]","",line))))
    } else if (length(grep("number_of_2-clauses_after",line)) > 0) {
      result = c(result,
        list(bin_c = as.integer(gsub("([^0-9]|_[0-9]+)","",line))))
    } else if (length(grep("running_time",line)) > 0) {
      result = c(result,
        list(time = as.double(gsub("[^0-9\\.]","",line))))
    } else if (length(grep("number_of_nodes",line)) > 0) {
      result = c(result,
        list(nodes = as.integer(gsub("([^0-9]|_[0-9]+)","",line))))
    } else if (length(grep("number_of_single_nodes",line)) > 0) {
      result = c(result,
        list(single_nodes = as.integer(gsub("([^0-9]|_[0-9]+)","",line))))
    } else if (length(grep("number_of_quasi_single_nodes",line)) > 0) {
      result = c(result,
        list(quasi_single_nodes = as.integer(gsub("([^0-9]|_[0-9]+)","",line))))
    }  else if (length(grep("number_of_2-reductions",line)) > 0) {
      result = c(result,
        list(r2 = as.integer(gsub("([^0-9]|_[0-9]+)","",line))))
    } else if (length(grep("number_of_pure_literals",line)) > 0) {
      result = c(result,
        list(pure_lits = as.integer(gsub("([^0-9]|_[0-9]+)","",line))))
    } else if (length(grep("number_of_autarkies",line)) > 0) {
      result = c(result,
        list(aut = as.integer(gsub("([^0-9]|_[0-9]+)","",line))))
    } else if (length(grep("number_of_missed",line)) > 0) {
      result = c(result,
        list(missed_1nodes = as.integer(gsub("([^0-9]|_[0-9]+)","",line))))
    } else if (length(grep("max_tree_depth",line)) > 0) {
      result = c(result,
        list(depth = as.integer(gsub("([^0-9]|_[0-9]+)","",line))))
    } else if (length(grep("number_of_table_enlargements",line)) > 0) {
      result = c(result,
        list(tab_enlarge = as.integer(gsub("([^0-9]|_[0-9]+)","",line))))
    } else if (length(grep("number_of_1-autarkies",line)) > 0) {
      result = c(result,
        list(aut1 = as.integer(gsub("([^0-9]|_[0-9]+)","",line))))
    } else if (length(grep("number_of_new_2-clauses",line)) > 0) {
      result = c(result,
        list(new_2c = as.integer(gsub("([^0-9]|_[0-9]+)","",line))))
    } else if (length(grep("maximal_number_of_added_2-clauses",line)) > 0) {
      result = c(result,
        list(max_added_2c = as.integer(gsub("([^0-9]|_[0-9]+)","",line))))
    } else if (length(grep("file_name",line)) > 0) {
      result = c(result, list(filename = gsub("file_name *","",line)))
    }
  
  }
  data.frame(result)
}
# For example, the following output from OKsolver_2002 (in test.result):
#
# s SATISFIABLE
# c sat_status                            1
# c initial_maximal_clause_length         10
# c initial_number_of_variables           10
# c initial_number_of_clauses             960
# c initial_number_of_literal_occurrences 9600
# c number_of_initial_unit-eliminations   2
# c reddiff_maximal_clause_length         1
# c reddiff_number_of_variables           2
# c reddiff_number_of_clauses             13
# c reddiff_number_of_literal_occurrences 11
# c number_of_2-clauses_after_reduction   6
# c running_time(sec)                     0.0
# c number_of_nodes                       10
# c number_of_single_nodes                
# c number_of_quasi_single_nodes          1
# c number_of_2-reductions                4
# c number_of_pure_literals               0
# c number_of_autarkies                   5
# c number_of_missed_single_nodes         7
# c max_tree_depth                        8
# c number_of_table_enlargements          0
# c number_of_1-autarkies                 0
# c number_of_new_2-clauses               0
# c maximal_number_of_added_2-clauses     0
# c file_name                             test1.cnf
#
# yields the following data.frame:
#
# > oklib_load_all()
# > E = read_oksolver_output("test.result")
# > E
#   sat init_max_cl  n   c    l init_unit diff_max_cl diff_n diff_c diff_l
# 1   1          10 10 960 9600         2           1      2     13     11
#   bin_c time nodes single_nodes quasi_single_nodes r2 pure_lits aut
# 1     6    0    10           NA                  1  4         0   5
#   missed_1nodes depth tab_enlarge aut1 new_2c max_added_2c
# 1             7     8           0    0      0            0
#    filename
# 1 test1.cnf
#

# Reading outputs of OKsolver_2002 computations from files in the list
# stats_filename_l and returning a data.frame containing the statistics on
# the computations.
# Inputs:
#   stats_filename_l
#     A list of filenames each containing the output of a run of the
#     OKsolver_2002.
# Output:
#   A data.frame with a row for each filename in stats_filename_l. Each row
#   contains the following fields in following order:
#     sat ({0,1,2}): SATISFIABLE (1), UNSATISFIABLE (0), UNKNOWN (2).
#     init_max_cl (pos int): Initial maximum clause-length.
#     n (pos int): Initial number of variables.
#     c (pos int): Initial number of clauses.
#     l (pos int): Initial number of literal occurrences.
#     init_unit (pos int): Number of unit-clause propagations.
#     diff_max_cl (pos int): Difference between maximum clause-length before
#       and after preprocessing.
#     diff_n (pos int): Diff. in variables before/after preprocessing.
#     diff_c (pos int): Diff. in clauses before/after preprocessing.
#     diff_l (pos int): Diff. in literal occurrences before/after preproc.
#     bin_c (pos int): Number of 2-clauses after preproc.
#     time (double): Total time in seconds to solve the problem.
#     nodes (pos int): Number of nodes in the search tree.
#     single_nodes (pos int): Number of single nodes in search tree.
#     quasi_single_nodes (pos int): Number of quasi-single nodes.
#     r2 (pos int): Number of r_2-reductions.
#     pure_lits (pos int): Pure literals found during search.
#     aut (pos int): Number of autarkies.
#     missed_1nodes (pos int): Nodes which would have been found as
#       single nodes if the the "other" branch had been chosen first.
#     depth (pos int): Maximum depth of the search tree.
#     tab_enlarge (pos int): Table enlargements during the search.
#     aut1 (pos int): Number of 1-autarkies. A 1-autarky satisfies all clauses
#       except one.
#     new_2c (pos int): New binary clauses learnt.
#     max_added_2c (pos int): Maximum number of new binary clauses added.
#     filename (string): Name of the DIMACS file input to OKsolver_2002.
#
read_oksolver_outputs = function(filenames) {
  result_df = NULL
  for(file in filenames) {
      result_df = rbind(result_df,read_oksolver_output(file))
 }
 result_df
}
# For example, the following outputs from OKsolver_2002
# (in testdir/test1.result and testdir/test2.result):
#
# s SATISFIABLE
# c sat_status                            1
# c initial_maximal_clause_length         10
# c initial_number_of_variables           10
# c initial_number_of_clauses             960
# c initial_number_of_literal_occurrences 9600
# c number_of_initial_unit-eliminations   2
# c reddiff_maximal_clause_length         1
# c reddiff_number_of_variables           2
# c reddiff_number_of_clauses             13
# c reddiff_number_of_literal_occurrences 11
# c number_of_2-clauses_after_reduction   6
# c running_time(sec)                     0.0
# c number_of_nodes                       10
# c number_of_single_nodes                
# c number_of_quasi_single_nodes          1
# c number_of_2-reductions                4
# c number_of_pure_literals               0
# c number_of_autarkies                   5
# c number_of_missed_single_nodes         7
# c max_tree_depth                        8
# c number_of_table_enlargements          0
# c number_of_1-autarkies                 0
# c number_of_new_2-clauses               0
# c maximal_number_of_added_2-clauses     0
# c file_name                             test1.cnf
#
# and
#
# s SATISFIABLE
# c sat_status                            1
# c initial_maximal_clause_length         10
# c initial_number_of_variables           12
# c initial_number_of_clauses             1060
# c initial_number_of_literal_occurrences 24500
# c number_of_initial_unit-eliminations   4
# c reddiff_maximal_clause_length         3
# c reddiff_number_of_variables           4
# c reddiff_number_of_clauses             42
# c reddiff_number_of_literal_occurrences 103
# c number_of_2-clauses_after_reduction   64
# c running_time(sec)                     0.1
# c number_of_nodes                       243
# c number_of_single_nodes                
# c number_of_quasi_single_nodes          0
# c number_of_2-reductions                23
# c number_of_pure_literals               1
# c number_of_autarkies                   5
# c number_of_missed_single_nodes         9
# c max_tree_depth                        10
# c number_of_table_enlargements          0
# c number_of_1-autarkies                 0
# c number_of_new_2-clauses               0
# c maximal_number_of_added_2-clauses     0
# c file_name                             test2.cnf
#
# yield the data.frame:
#
# > oklib_load_all()
# > E = read_oksolver_outputs(Sys.glob("testdir/*.result"))
# > E
#
#   sat init_max_cl  n    c     l init_unit diff_max_cl diff_n diff_c diff_l
# 1   1          10 10  960  9600         2           1      2     13     11
# 2   1          10 12 1060 24500         4           3      4     42    103
#   bin_c time nodes single_nodes quasi_single_nodes r2 pure_lits aut
# 1     6  0.0    10           NA                  1  4         0   5
# 2    64  0.1   243           NA                  0 23         1   5
#   missed_1nodes depth tab_enlarge aut1 new_2c max_added_2c  filename
# 1             7     8           0    0      0            0 test1.cnf
# 2             9    10           0    0      0            0 test2.cnf
#
# Note the use of "Sys.glob" to produce a list of files in a directory based
# on a glob pattern.
#
  
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

