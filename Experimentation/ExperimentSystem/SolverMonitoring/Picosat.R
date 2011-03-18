# Matthew Gwynne, 19.9.2009 (Swansea)
# Copyright 2009, 2010, 2011 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

# #######
# Input #
# #######

# Reading the output of a picosat913 computation from filename and returning a
# data.frame containing the statistics on the computation.
#
# Inputs:
#   filename
#     The filename containing the output of a run of the picosat913 solver.
#
# Output:
#   A data.frame with a single row with the following fields in the
#   following order:
#
#     filename (string)
#       The name of the file picosat913 was run on to produce the output.
#     n (positive integer)
#       The number of variables in the DIMACS file picosat913 was run on to
#       generate filename.
#     c (positive integer)
#       The number of variables in the DIMACS file picosat913 was run on to
#       generate filename.
#     sat ({0,1,2})
#       Whether picosat913 found filename to be SATISFIABLE (1),
#       UNSATISFIABLE (0) or it was unable to determine satisfiablity (2).
#     restarts (positive integer)
#       The number of restarts picosat913 has performed while solving filename.
#     failed_lits (positive integer)
#       Number of forced assignments found using partial failed literal
#       reductions.
#     conflicts (positive integer)
#       The number of conflicts picosat913 has found while solving filename.
#     decisions (positive integer)
#       The number of decisions picosat913 has performed while solving filename.
#     fixed_vars (positive integer)
#       The number of variables fixed to true or false by picosat913 during
#       it's search.
#     learned_lits (positive integer)
#       The number of literals which picosat913 determined by conflict clause
#       analysis.
#     deleted_lits (double)
#       The percentage of literals deleted from the problem by picosat913
#       during the search and inference steps.
#     prop (positive integer)
#       The number of unit clause propagations that picosat913 has performed
#       while solving filename.
#     simps (positive integer)
#       The number of simplications performed by picosat913 during it's
#       search.
#     mem (double)
#       The maximum amount of main memory in Megabytes used by picosat913 when
#       solving filename.
#     time (double)
#       The number of seconds it took picosat913 to solve filename.
#
# Note this function will not read the output of any of the precosat solvers,
# as their output format is considerably different to picosat.
#  
read_picosat_output = function(filename, ...) {
  S = system(paste("cat ", filename," | grep \"^c\\|s\""), intern=TRUE)
  result = list()
  for (line in S) {
    line = gsub("^[cs] *", "",line)
    if (length(grep("^parsing *",line)) > 0) {
      result = c(result,list(filename = gsub("^parsing *","",line)))
    } else if (length(grep("^initialized", line)) > 0) {
      result = c(result,
        list(n = as.integer(gsub("[^0-9]","",line))))
    } else if (length(grep("trivial clauses",line)) > 0) {
      result = c(result,
        list(c = as.integer(gsub("[^0-9]","",line))))
    } else if (length(grep("decisions",line)) > 0) {
      result = c(result,
        list(decisions = as.integer(gsub("[^0-9]","",line))))
    } else if (length(grep("conflicts *$",line)) > 0) {
      result = c(result,
        list(conflicts = as.integer(gsub("[^0-9]","",line))))
    } else if (length(grep("fixed variables",line)) > 0) {
      result = c(result,
        list(fixed_vars = as.integer(gsub("[^0-9]","",line))))
    } else if (length(grep("learned literals",line)) > 0) {
      result = c(result,
        list(learned_lits = as.integer(gsub("[^0-9]","",line))))
    } else if (length(grep("failed literals",line)) > 0) {
      result = c(result,
        list(failed_lits = as.integer(gsub("[^0-9]","",line))))
    } else if (length(grep("deleted literals",line)) > 0) {
      result = c(result,
        list(deleted_lits = as.double(gsub("[^0-9\\.]","",line))))
    } else if (length(grep("propagations",line)) > 0) {
      result = c(result,
        list(prop = as.integer(gsub("[^0-9]","",line))))
    } else if (length(grep("simplifications",line)) > 0) {
      result = c(result,
        list(simps = as.integer(gsub("[^0-9]","",line))))
    } else if (length(grep("iterations",line)) > 0) {
      result = c(result,
        list(iter = as.integer(gsub("[^0-9]","",line))))
    } else if (length(grep("seconds total run time",line)) > 0) {
      result = c(result,
        list(time = as.double(gsub("[^0-9\\.]","",line))))
    } else if (length(grep("MB maximally allocated",line)) > 0) {
      result = c(result,
        list(mem = as.double(gsub("[^0-9\\.]","",line))))
    } else if (length(grep("restarts",line)) > 0) {
      result = c(result,
        list(restarts = as.integer(gsub("[^0-9\\.]","",line))))
    } else if (length(grep("SATISFIABLE",line)) > 0) {
      result = c(result, list(sat = 1))
    } else if (length(grep("UNSATISFIABLE",line)) > 0) {
      result = c(result, list(sat = 0))
    }
  }
  data.frame(result)
}
# As an example, we can generate the CNF for the 4-bit AES Sbox in maxima:
#
# maxima> oklib_load_all()$
# maxima> output_ss_sbox_fullcnf_stdname(2,4,ss_polynomial_2_4)$
#
# and then running picosat913 on the file:
#
# shell> picosat913 AES_sbox_2_4_full.cnf > sbox.result 2>&1
#
# we get the following data.frame:
#
# R> oklib_load_all()
# R> E = read_picosat_output("sbox.result")
# R> E
#                filename n   c sat iter restarts failed_lits conflicts decisions
# 1 AES_sbox_2_4_full.cnf 8 240   1    0        0           0         2         8
#   fixed_vars learned_lits deleted_lits prop simps mem time
# 1          0           13            0   11     1   0    0
#

# Reading multiple picosat913 output files into a data.frame.
# See read_picosat913_output.
read_picosat_outputs = function(filenames) {
 result_df = NULL
 for(file in filenames) {
     result_df = rbind(result_df,read_picosat_output(file))
 }
 result_df
}
# As an example, we can generate the CNFs for unsatisfiable PHP formulas
# mapping m+1 pigeons to m holes:
#
# maxima> oklib_load_all()$
# maxima> for i : 1  thru 5 do output_weak_php_stdname(i+1,i);
#
# and then running picosat913 on these files:
#
# shell> for i in $(seq 1 5); do picosat913 PHP_weak_*_${i}.cnf > php_${i}.result 2>&1; done
#
# we get the following data.frame (using Sys.glob to generate the list of
# files):
#
# R> oklib_load_all()
# R> E = read_picosat_outputs(Sys.glob("php_*.result"))
# R> E
#           filename  n  c sat iter restarts failed_lits conflicts decisions
# 1 PHP_weak_2_1.cnf  2  3   1    0        0           0         1         0
# 2 PHP_weak_3_2.cnf  6  9   1    0        0           1         2         0
# 3 PHP_weak_4_3.cnf 12 22   1    1        0           0         7         9
# 4 PHP_weak_5_4.cnf 20 45   1    3        0           0        30        40
# 5 PHP_weak_6_5.cnf 30 81   1    4        0           0       139       161
#   fixed_vars learned_lits deleted_lits prop simps mem time
# 1          2            0          0.0    2     0   0    0
# 2          6            6          0.0   12     0   0    0
# 3         12           21          8.3  115     1   0    0
# 4         20          116          7.4  447     1   0    0
# 5         30          938         11.8 2038     1   0    0
#
# If one has parameters in the filename of the CNFs that picosat913 was run
# on, we can add an extra column to the data.frame, extracting the value from
# the filename:
#
# R> E$m = as.integer(gsub("(^PHP_weak_.*_|.cnf$)","",E$filename))
# R> plot(E$m, E$decisions)
# 
#
